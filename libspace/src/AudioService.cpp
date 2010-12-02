/*  Sirikata
 *  AudioService.cpp
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

#include <sirikata/space/AudioService.hpp>

namespace Sirikata {

AudioService::ObjectStreamMap AudioService::objStreamMap;

int AudioService::ActiveMixChannels() {
  int i;
  for (i=0;i<num_channels;i++)
    if (mix_channel[i].playing > 0)
      return 1;

  return 0;
}
 
void AudioService::MixChannels() {
  Uint8 *mix_input;
  int i, mixable, volume = SDL_MIX_MAXVOLUME;
  Uint8 *stream = (Uint8 *) malloc (BUFFER_SIZE);
  int len = BUFFER_SIZE;
  memset (stream, 0, BUFFER_SIZE);

  while (ActiveMixChannels()) {

    /* Mix any playing channels... */
    for ( i=0; i<num_channels; ++i ) {
      if ( mix_channel[i].playing > 0 ) {
        int index = 0;
        int remaining = len;
        while (mix_channel[i].playing > 0 && index < len) {
          remaining = len - index;
          volume = (mix_channel[i].volume*mix_channel[i].chunk->volume) / MIX_MAX_VOLUME;
          mixable = mix_channel[i].playing;
          if ( mixable > remaining ) {
            mixable = remaining;
          }

          mix_input = mix_channel[i].samples;
          SDL_MixAudio(stream+index,mix_input,mixable,volume);

          mix_channel[i].samples += mixable;
          mix_channel[i].playing -= mixable;
          index += mixable;
        }
      }
    }

    if ( mix_postmix ) {
      mix_postmix(NULL, stream, len);
    }

  }
}

int AudioService::MixChannelNew(int which, Mix_Chunk *chunk) {
  int i;

  /* Don't play null pointers :-) */
  if ( chunk == NULL ) {
    Mix_SetError("Tried to play a NULL chunk");
    return(-1);
  }

  /* Lock the mixer while modifying the playing channels */
  //SDL_LockAudio();
  {
    /* If which is -1, play on the first free channel */
    if ( which == -1 ) {
      for ( i=0; i<num_channels; ++i ) {
        if ( mix_channel[i].playing <= 0 )
          break;
      }
      if ( i == num_channels ) {
        Mix_SetError("No free channels available");
        which = -1;
      } else {
        which = i;
      }
    }

    /* Queue up the audio data for this channel */
    if ( which >= 0 ) {
      num_playing_channels++;
      mix_channel[which].samples = chunk->abuf;
      mix_channel[which].playing = chunk->alen;
      mix_channel[which].chunk = chunk;
      mix_channel[which].paused = 0;
    }
  }
  //SDL_UnlockAudio();

  /* Return the channel on which the sound is being played */
  return(which);
}
 
void AudioService::MixChannelDel(int which) {
  int i = which;
  if (which >= 0 && which < num_channels) {
    num_playing_channels--;
    mix_channel[i].chunk = NULL;
    mix_channel[i].playing = 0;
    mix_channel[i].volume = SDL_MIX_MAXVOLUME;
    mix_channel[i].paused = 0;
  }
}

void AudioService::MixChannelsInit () {
  int i;
	mix_postmix = NULL;
	num_channels = MAX_CHANNELS;
	mix_channel = (struct _Mix_Channel *) malloc(num_channels * sizeof(struct _Mix_Channel));
  for ( i=0; i<num_channels; ++i ) {
    mix_channel[i].chunk = NULL;
    mix_channel[i].playing = 0;
    mix_channel[i].volume = SDL_MIX_MAXVOLUME;
    mix_channel[i].paused = 0;
  }
}

Mix_Chunk * loadMixChunk (char *file) {
	  Mix_Chunk *chunk;
	  SDL_AudioSpec wavespec;
	  SDL_AudioCVT wavecvt;
	  int samplesize;
	
	  /* Allocate the chunk memory */
	  chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
	  if ( chunk == NULL ) {
	    SDL_SetError("Out of memory");
			return NULL;
	  }
	
	  if ( SDL_LoadWAV(file, &wavespec, (Uint8 **)&chunk->abuf, &chunk->alen) == NULL ) {
			return NULL;
	  }
	
	  SDL_BuildAudioCVT(&wavecvt, wavespec.format, wavespec.channels, wavespec.freq,
	                            AUDIO_S16,   2,             22050);
	
	  samplesize = ((wavespec.format & 0xFF)/8)*wavespec.channels;
	  wavecvt.len = chunk->alen & ~(samplesize-1);
	  wavecvt.buf = (Uint8 *)malloc(wavecvt.len*wavecvt.len_mult);
	  if ( wavecvt.buf == NULL ) {
	    SDL_SetError("Out of memory");
	    SDL_FreeWAV(chunk->abuf);
	    free(chunk);
	    return(NULL);
	  }
	  memcpy(wavecvt.buf, chunk->abuf, chunk->alen);
	  SDL_FreeWAV(chunk->abuf);
	
	  /* Run the audio converter */
	  if ( SDL_ConvertAudio(&wavecvt) < 0 ) {
	    free(wavecvt.buf);
	    free(chunk);
	    return(NULL);
	  }
	  chunk->allocated = 1;
	  chunk->abuf = wavecvt.buf;
	  chunk->alen = wavecvt.len_cvt;
	  chunk->volume = MIX_MAX_VOLUME;
		return chunk;
}


void mixaudio(void *udata, Uint8 *_stream, int _len)
{
  AudioService::ObjectStreamMap::iterator it;

  SHA256 hash = SHA256::computeDigest (_stream, _len);
  SILOG(msg, warning, "mixed data hash : " << hash.convertToHexString());

	Uint8 *snd = (Uint8*) malloc (_len);
	memcpy (snd, _stream, _len);
	//Uint8 *snd = NULL;

  for ( it=AudioService::objStreamMap.begin() ; it != AudioService::objStreamMap.end(); it++ ) {
    std::string payload = "Hello from Space";
    Uint8 *txData = (Uint8 *) payload.data();
    //Uint32 dataLen = payload.size();
    Uint32 dataLen = _len;
    AudioService::registeredObject curObj = (*it).second;
    if (snd) {
			SILOG(msg, warning, "Sending copied buffer");
      curObj.objStream->write (snd, BUFFER_SIZE);
    } else
      curObj.objStream->write (_stream, _len);
    //curObj.objStream->write (txData, dataLen);
  }
/***** Playback test *****
      SDL_PauseAudio (0);
      Mix_Chunk *chunk = NULL;
      chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
      if ( chunk == NULL ) {
        SILOG(msg, warning, "Out of memory");
      }

      SILOG(msg, warning, "\n\n\n Copying received data to chunk. \n\n");

      chunk->allocated = 1;
      chunk->abuf = (Uint8 *) malloc (_len+1);
      memcpy(chunk->abuf, _stream, _len);
      chunk->alen = _len;
      chunk->volume = MIX_MAX_VOLUME;
      int channel;

      channel = Mix_PlayChannel(-1, chunk, 0);
      if(channel == -1) {
        SILOG(msg, warning, "Unable to play WAV file: " << Mix_GetError());
      } else
        while(Mix_Playing(channel) != 0);
      Mix_FreeChunk(chunk);
      SDL_PauseAudio (1);
***** Playback test *****/

  SILOG(msg, warning, "Got the mixed data of length : " << _len);
//  SILOG(msg, warning, "and hash : " << hash);
}



AudioService::AudioService(SpaceContext* ctx)
  : mContext(ctx)
{

/*******************************************************/
	  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
	    SILOG(msg, warning, "Unable to initialize SDL: " << SDL_GetError());
	  }
	
	  int audio_rate = 22050;
	  Uint16 audio_format = AUDIO_S16SYS;
	  int audio_channels = 2;
	  int audio_buffers = 4096;
	
	  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
	    SILOG(msg, warning, "Unable to initialize audio : " << Mix_GetError());
	  }
/*******************************************************/
/*
		Mix_SetPostMix(mixaudio, NULL);
		SDL_PauseAudio (1);
		Mix_AllocateChannels (8);
*/
		numActiveChannels = 0;
		for (short i=0; i<MAX_CHANNELS; i++)
			audioChannels[i] = -1;
		MixChannelsInit();
		mix_postmix = mixaudio;
}

AudioService::~AudioService() {
//    Mix_CloseAudio();
//    SDL_Quit();
}

void AudioService::newSession(ObjectSession* session) {
    using std::tr1::placeholders::_1;
    using std::tr1::placeholders::_2;

    Stream<SpaceObjectReference>::Ptr strm = session->getStream();

		SILOG(msg, warning, "Got new session !");

		strm->listenSubstream(OBJECT_PORT_AUDIO,
          std::tr1::bind(&AudioService::handleAudioSubstream, this, _1, _2)
      );
}

void AudioService::subscribe(const UUID& uuid) {

}

void AudioService::unsubscribe(const UUID& uuid) {

}

UUID AudioService::getUUID (SSTStreamPtr s) {
		if (s) {
			Sirikata::EndPoint <SpaceObjectReference> ep = s->remoteEndPoint();
			return ep.endPoint.object().getObjectUUID();
		}
		return Sirikata::UUID::null();
}

AudioService::registeredObject AudioService::memObjectStreamMap (SSTStreamPtr s) {
		UUID objUuid = getUUID(s);
		ObjectStreamMap::iterator it;
		AudioService::registeredObject obj;
		obj.channelID = -1;
		obj.valid = -1;
		
		it = AudioService::objStreamMap.find(objUuid);
		if (it != AudioService::objStreamMap.end())
			return AudioService::objStreamMap.find(objUuid)->second;
		return obj;
}

short AudioService::getFreeChannel() {
    for (short i=0; i<MAX_CHANNELS; i++) {
			if (audioChannels[i] == -1) {
				audioChannels[i] = 1;
				return i;
			}
		}
		return -1;
}

void AudioService::freeChannel(short channelNum) {
		audioChannels[channelNum] = -1;
}


void AudioService::handleAudioSubstream(int err, SSTStreamPtr s) {
    s->registerReadCallback( std::tr1::bind(&AudioService::handleAudioSubstreamRead, this, s, new std::stringstream(), _1, _2) );
		recAudioLen = 0;
		AudioService::registeredObject newObj;
		UUID objUuid = getUUID(s);
		if (objUuid != Sirikata::UUID::null()) {
			newObj.objStream = s;
			newObj.channelID = -1;
			newObj.valid = 1;
			AudioService::objStreamMap[objUuid] = newObj;
		} else 
			SILOG(msg,warning," NULL uuid!");
}

void AudioService::handleAudioSubstreamRead(SSTStreamPtr s, std::stringstream* prevdata, uint8* buffer, int length) {
		std::string payload = "Hello back Object";
//		SILOG(msg,warning," Content = " << buffer);
//		s->write ((uint8 *)payload.data(), payload.size());
//    prevdata->write((const char*)buffer, length);

		if (recAudioLen + length <= BUFFER_SIZE) {
			memcpy (recAudio + recAudioLen, buffer, length);
			recAudioLen += length;
		} else {
			SILOG(msg, warning, "Playing to channel . total = " << recAudioLen + length);
			Mix_Chunk *chunk = NULL;
	    /* Allocate the chunk memory */
	    chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
	    if ( chunk == NULL ) {
	      SILOG(msg, warning, "Out of memory");
				return;
	    }

			chunk->allocated = 1;
			chunk->abuf = (Uint8 *) malloc (recAudioLen+1);
			memcpy(chunk->abuf, recAudio, recAudioLen);
			chunk->alen = recAudioLen;
			chunk->volume = MIX_MAX_VOLUME;
			//chunk->volume = 0;

			// Send to mixer
	    int channel;
			AudioService::registeredObject curObj = memObjectStreamMap (s);
/*			AudioService::registeredObject curObj;
			curObj.valid = 1;
			curObj.channelID = -1;
*/	
		
			if (curObj.valid != -1) {
				if (curObj.channelID == -1)
					curObj.channelID = getFreeChannel();
				if (curObj.channelID == -1) {
					SILOG(msg,error," Free audio channel not available.");
					return;
				}
				AudioService::objStreamMap[getUUID(s)] = curObj;

/*				numActiveChannels++;
				if (numActiveChannels > 0) {
					SDL_PauseAudio (0);
					Mix_SetPostMix(mixaudio, NULL);
				}
*/
//		    channel = Mix_PlayChannel(curObj.channelID, chunk, 0);
		    channel = MixChannelNew (curObj.channelID, chunk);
		    if(channel == -1) {
		      SILOG(msg, warning, "Unable to play WAV file: " << Mix_GetError());
		    }

//		    while(Mix_Playing(channel) != 0);

				MixChannels();

				Mix_FreeChunk(chunk);
/*				numActiveChannels--;
				if (numActiveChannels == 0)
					SDL_PauseAudio (1);
*/
			}
			recAudioLen = 0;
			memcpy (recAudio, buffer, length);
      recAudioLen += length;
		}
}

void AudioService::pingBackOH (UUID source, void* buffer, uint32 length) {
    SILOG(msg,error,"Received ping from OH.");
}

} // namespace Sirikata
