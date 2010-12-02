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

#include "SDL.h"
#include "SDL_mixer.h"
#include <map>
#include <sirikata/space/SpaceContext.hpp>
#include <sirikata/space/ServerMessage.hpp>
#include <sirikata/core/network/SSTImpl.hpp>
#include <sirikata/space/Platform.hpp>
#include <sirikata/core/util/Factory.hpp>
#include <sirikata/core/util/Sha256.hpp>

using namespace std;

namespace Sirikata {

# define BUFFER_SIZE 65536
# define MAX_CHANNELS 8

class AudioService;

#define NUM_SOUNDS 2
struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

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

//		Mix_Chunk * loadMixChunk (char *file);

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

    typedef struct {
      SSTStreamPtr objStream;
      short channelID;
			short valid;
    } registeredObject;

		typedef map<UUID, registeredObject> ObjectStreamMap;
    static ObjectStreamMap objStreamMap;

		UUID getUUID (SSTStreamPtr s);
		registeredObject memObjectStreamMap (SSTStreamPtr s);
		short getFreeChannel();
		void freeChannel(short channelNum);

		// Extracted SDL_mixer code
		int ActiveMixChannels(); 
		void MixChannels();
		int MixChannelNew(int which, Mix_Chunk *chunk);
		void MixChannelDel(int which);
		void MixChannelsInit ();

protected:
    SpaceContext* mContext;
		Uint8 recAudio[BUFFER_SIZE];
		Uint32 recAudioLen;

		Uint8 numActiveChannels;
		short audioChannels[MAX_CHANNELS];
	
		struct _Mix_Channel {
		  Mix_Chunk *chunk;
		  int playing;
		  int paused;
		  Uint8 *samples;
		  int volume;
		} *mix_channel;
		
		int num_channels;
		int num_playing_channels;
		void (*mix_postmix)(void *udata, Uint8 *stream, int len);

}; // class AudioService

} // namespace Sirikata

#endif //_SIRIKATA_AUDIO_SERVICE_HPP_
