/*  Sirikata
 *  AudioService.hpp
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

#ifndef _SIRIKATA_AUDIO_SERVICE_HPP_
#define _SIRIKATA_AUDIO_SERVICE_HPP_

#include <sirikata/space/SpaceContext.hpp>
#include <sirikata/space/ServerMessage.hpp>
#include <sirikata/core/network/SSTImpl.hpp>
#include <sirikata/space/Platform.hpp>
#include <sirikata/core/util/Factory.hpp>

namespace Sirikata {

class AudioService;

/** Interface for audio service.  This provides a way for the local object hosts 
 *  to get the mixed audio stream. 
 * 	This would be extended to support hierarchical audio mixing between space servers.
 */
class SIRIKATA_SPACE_EXPORT AudioService : public ObjectSessionListener {
public:

    typedef Stream<SpaceObjectReference> SSTStream;
    typedef SSTStream::Ptr SSTStreamPtr;

    AudioService(SpaceContext* ctx);
    ~AudioService();

    const SpaceContext* context() const {
        return mContext;
    }

    // ObjectSessionListener Interface
    void newSession(ObjectSession* session);

    /** Methods dealing with information requests. */
		// TODO: Add as needed

    /** Subscriptions for other servers. */
		// TODO: Need to define how to manage remote space server
		// 			 subscriptions for audio streams.

    /** Subscriptions for local objects. */
    void subscribe(const UUID& uuid);
    void unsubscribe(const UUID& uuid);

    /** MessageRecipient Interface. */
    void receiveAudioStream(UUID source, void* buffer, uint32 length);

		void handleAudioSubstream(int err, SSTStreamPtr s);

		void handleAudioSubstreamRead(SSTStreamPtr s, std::stringstream* prevdata, uint8* buffer, int length);

    /* PING REPLY TEST */
    void pingBackOH (UUID source, void* buffer, uint32 length);

    Stream<SpaceObjectReference>::Ptr getObjectStream(const UUID& uuid) {
        ObjectSession* session = mContext->sessionManager()->getSession(ObjectReference(uuid));
        if (session == NULL) return Stream<SpaceObjectReference>::Ptr();
        return session->getStream();
    }

protected:
    SpaceContext* mContext;

}; // class AudioService

} // namespace Sirikata

#endif //_SIRIKATA_AUDIO_SERVICE_HPP_
