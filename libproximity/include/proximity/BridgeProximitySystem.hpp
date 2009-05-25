/*  Sirikata Proximity Management -- Introduction Services
 *  BridgeProximitySystem.hpp
 *
 *  Copyright (c) 2009, Daniel Reiter Horn
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _PROXIMITY_BRIDGEPROXIMITYSYSTEM_HPP_
#define _PROXIMITY_BRIDGEPROXIMITYSYSTEM_HPP_
#include "ObjectSpaceBridgeProximitySystem.hpp"
namespace Sirikata { namespace Proximity {
template <class SpacePtr> 
class BridgeProximitySystem : public ObjectSpaceBridgeProximitySystem<SpacePtr> {
protected:
    virtual bool forwardThisName(const std::string&name) {
        return name=="ObjLoc"||this->ObjectSpaceBridgeProximitySystem<SpacePtr>::forwardThisName(name)||name=="RetObj"||name=="DelObj";
    }
    virtual typename ObjectSpaceBridgeProximitySystem<SpacePtr>::DidAlterMessage addressMessage(RoutableMessageHeader&output,
                                const ObjectReference*source,
                                const ObjectReference*destination) {
        //if (source) //proximity management will have source through 
        //    output.set_source_object(source->getObjectUUID());
        if (destination) {
            output.set_destination_object(*destination);
            return ObjectSpaceBridgeProximitySystem<SpacePtr>::ALTERED_MESSAGE;
        }
        return ObjectSpaceBridgeProximitySystem<SpacePtr>::UNSPOILED_MESSAGE;
    }
    virtual void sendMessage(const ObjectReference&source,
                             const RoutableMessage&opaqueMessage,
                             const void *optionalSerializedMessageBody,
                             size_t optionalSerializedMessageBodySize) {
        mProximityConnection->send(source,opaqueMessage.body(),optionalSerializedMessageBody,optionalSerializedMessageBodySize);
    }
    virtual void readProximityMessage(const ObjectReference&objectConnection,Network::Chunk&msg) {
        if (msg.size()) {
            RoutableMessage message;
            MemoryReference ref=message.RoutableMessageHeader::ParseFromArray(&msg[0],msg.size());
            addressMessage(message,NULL,&objectConnection);
            //ObjectSpaceBridgeProximitySystem<SpacePtr>::UNSPOILED_MESSAGE) {
            this->deliverMessage(objectConnection,message,ref.first,ref.second);//FIXME: this deliver happens in a thread---do we need a distinction
        }
    }
public:
    ProximityConnection *mProximityConnection;
    BridgeProximitySystem(SpacePtr obj,ProximityConnection*connection) : ObjectSpaceBridgeProximitySystem<SpacePtr> (obj),mProximityConnection(connection) {
        this->mImportantNames.insert("ObjLoc");
        this->mImportantNames.insert("DelObj");
        mProximityConnection->setParent(this);
    }
    enum MessageBundle{
        DELIVER_TO_UNKNOWN,
        DELIVER_TO_OBJECT,
        DELIVER_TO_PROX,
        DELIVER_TO_BOTH
        
    };

    /**
     * Pass the ReturnedObjectConnection info, 
     * containing an Object UUID to the proximity manager, 
     * so the proximity system knows about a new object
     */
    virtual ObjectReference newObj(const Sirikata::Protocol::IRetObj& newObjMsg,
                        const void *optionalSerializedReturnObjectConnection=NULL,
                        size_t optionalSerializedReturnObjectConnectionSize=0){
        ObjectReference source(newObjMsg.object_reference());
        mProximityConnection->constructObjectStream(source);
        RoutableMessage toSend;
        this->constructMessage(toSend,&source,NULL,"RetObj",newObjMsg,optionalSerializedReturnObjectConnection,optionalSerializedReturnObjectConnectionSize);
        this->sendMessage(source,toSend,NULL,0);
        return source;
    }
    /**
     * The proximity management system must be informed of all position updates
     * Pass an objects position updates to this function
     */
    virtual void objLoc(const ObjectReference&source, const Sirikata::Protocol::IObjLoc&objLocMsg, const void *optionalSerializedObjLoc=NULL,size_t optionalSerializedObjLocSize=0) {
        RoutableMessage toSend;
        this->constructMessage(toSend,&source,NULL,"ObjLoc",objLocMsg,optionalSerializedObjLoc,optionalSerializedObjLocSize);
        this->sendMessage(source,toSend,NULL,0);
    }
    /**
     * Objects may be destroyed: indicate loss of interest here
     */
    virtual void delObj(const ObjectReference&source, const Sirikata::Protocol::IDelObj&delObjMsg, const void *optionalSerializedDelObj=NULL,size_t optionalSerializedDelObjSize=0) {
        RoutableMessage toSend;
        this->constructMessage(toSend,&source,NULL,"DelObj",delObjMsg,optionalSerializedDelObj,optionalSerializedDelObjSize);
        this->sendMessage(source,toSend,NULL,0);
        this->mProximityConnection->deleteObjectStream(source);
    }
    /**
     * Process a message that may be meant for the proximity system
     * \returns true if object was deleted
     */
    virtual void processOpaqueSpaceMessage(const ObjectReference*object,
                                               const void *serializedMessage,
                                               size_t serializedMessageSize) {
        RoutableMessageHeader mesg;
        MemoryReference remainder=mesg.ParseFromArray(serializedMessage,serializedMessageSize);
        if (this->internalProcessOpaqueProximityMessage(object,
                                                        mesg,
                                                        remainder.first,
                                                        remainder.second)==ProximitySystem::OBJECT_DELETED) {
            if (object)
                this->mProximityConnection->deleteObjectStream(*object);
        }
    }
    /**
     * Process a message that may be meant for the proximity system
     * \returns true if object was deleted
     */
    virtual void processOpaqueSpaceMessage(const ObjectReference*object,
                                               const RoutableMessageHeader& mesg,
                                               const void *serializedMessageBody,
                                               size_t serializedMessageBodySize) {
        if (this->internalProcessOpaqueProximityMessage(object,
                                                        mesg,
                                                        serializedMessageBody,
                                                        serializedMessageBodySize)==ProximitySystem::OBJECT_DELETED) {
            if (object)
                this->mProximityConnection->deleteObjectStream(*object);
        }
    }
   
};

} }
#endif
