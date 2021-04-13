/*
 * Skeleton-code behorende bij het college Netwerken, opleiding Informatica,
 * Universiteit Leiden.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>

#include <alsa/asoundlib.h>

#include "../communication/asp/asp.h"
#include "../communication/communication.h"
#include "../util/util.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BIND_PORT 1234

#define NUM_CHANNELS 2
#define SAMPLE_RATE 44100
#define BLOCK_SIZE 1024
// 1 Frame = Stereo 16 bit = 32 bit = 4 byte
#define FRAME_SIZE 4
#define DEFAULT_QUALITY 5 // Try the highest quality first
#define QUALITY_INCREASE 5 // Percentage threshold required to increase quality
#define QUALITY_DECREASE 10 // Percentage threshold required to decrease quality

struct sound_format {
	unsigned int sample_rate;
	unsigned short num_channels;
	unsigned short frame_size;
};

static int checkID(uint8_t id, uint8_t prev_id){
	uint8_t diff_sign = (id >> 7)^(prev_id >> 7);
	uint8_t num = id & 127;
	uint8_t prev_num = prev_id & 127;
	if(diff_sign){
		if(prev_num <= num){
			return -1;
		} else {
			return ((127 - prev_num) + (num));
		}
	} else {
		if(prev_num >= num){
			return -1;
		} else {
			return (num - prev_num - 1);
		}
	}
}

/*
Configure an audio device on which the streamed audio will be played.
- snd_handle: handle of the audio device to configure_audio
- sf: struct with audio parameters
*/
static bool configure_audio(snd_pcm_t** snd_handle_ref, struct sound_format* sf) {
	//Open audio device
	int err = snd_pcm_open(snd_handle_ref, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (err < 0) {
		fprintf(stderr, "couldnt open audio device: %s\n", snd_strerror(err));
		return false;
	}
	snd_pcm_t* snd_handle = *snd_handle_ref;

	// Configure parameters of PCM output
	err = snd_pcm_set_params(
		snd_handle,
		SND_PCM_FORMAT_S16,
		SND_PCM_ACCESS_RW_INTERLEAVED,
		NUM_CHANNELS,
		SAMPLE_RATE,
		0,              // Allow software resampling
		500000          // 0.5 seconds latency
	);
	if (err < 0) {
		printf("couldnt configure audio device: %s\n", snd_strerror(err));
		return false;
	}

	//Set audio parameters
	sf->sample_rate = 44100;
	sf->num_channels = 2;
	sf->frame_size = 4;

	return true;
} //configure_audio

/*
Reconfigure the audio device according to a new quality level
- snd_handle: The audio device to reconfigure
- quality: The quality that dictates the new audio parameters
- sf: needs to be updated with the new audio parameters
*/
static bool set_quality(snd_pcm_t *snd_handle, uint8_t quality, struct sound_format *sf) {
	int err = 0; //store any error codes
	int samp_rate = SAMPLE_RATE;
	unsigned int format = 2; //SND_PCM_FORMAT_S16 enum value
	bool s16 = true;
	snd_pcm_drain(snd_handle);
	switch (quality) {
		case 1:
			s16 = false;
			samp_rate = SAMPLE_RATE*0.5; //22050
		break;
		case 2:
			s16 = false;
			samp_rate = SAMPLE_RATE*0.75; //33075
		break;
		case 3:
			samp_rate = SAMPLE_RATE*0.5; //22050
		break;
		case 4:
			samp_rate = SAMPLE_RATE*0.75; //33075
		break;
		case 5:
			//nothing to change
		break;
		default:
			return false;
		break;
	}
	sf->sample_rate = samp_rate;
	sf->num_channels = 2;
	sf->frame_size = 4;
	if(!s16){
		format = 1; //SND_PCM_FORMAT_U8
		sf->frame_size = 2;
	}
	err = snd_pcm_set_params(
				snd_handle,
				format,
				SND_PCM_ACCESS_RW_INTERLEAVED,
				NUM_CHANNELS,
				samp_rate,
				0,              // Allow software resampling
				500000	       // 0.5 seconds latency
	);
	if (err < 0) {
		printf("couldnt configure audio device: %s\n", snd_strerror(err));
		return false;
	}
	snd_pcm_start(snd_handle);
	return true;
} //set_quality

// Adjusts quality based on missing/corrupted packets from the last 100 packets
// Return 1: increase quality; Return -1: decrease quality; Return 0: no quality change 
static int adjust_quality(asp_socket_info* si){
	if(!si->packets_missing){
		return 1;
	}
	double div = ((double)si->packets_missing / (double)si->packets_received)*100;
	si->packets_received = 0;
	si->packets_missing = 0;
	if(div > QUALITY_DECREASE){
		return -1;
	} else if(div < QUALITY_INCREASE){
		return 1;
	}
	return 0;
}

// Send a message and receive a packet
static inline bool send_receive(com_t* com, uint8_t* quality){
	com_send_m(com, quality);
	com->socket_info.remote_addrlen = sizeof(com->socket_info.remote_addr);
	return com_recv_p(com);
}

// update the asp variables for packets
static inline void asp_update(asp_socket_info* asp, int missing_packets){
	asp->packets_received++;
	asp->packets_missing += missing_packets;
}

static void stream_audio(snd_pcm_t *snd_handle, struct sound_format *sf,
                 unsigned int buffer_size, com_t* com) {
	uint8_t prev_id;
	int check_status = 0;
	int recvlen;
	unsigned blocksize = 1024; /*was 0*/

	// set up buffers/qualities
	uint8_t current_quality = DEFAULT_QUALITY;
	uint8_t new_quality = DEFAULT_QUALITY;
	uint8_t* recvbuffer = malloc(buffer_size); //Message without header
	uint8_t* playbuffer = malloc(BLOCK_SIZE);

	//Initial fill of the buffer
	prev_id = 0;
	for (unsigned int i = 0; i < buffer_size; i += (DATA_SIZE + QUALITY_SIZE)) {
		if(send_receive(com, &new_quality)){
			// check the id and prev_id to determine packet loss
			if (!((check_status = checkID(com->packet.id, prev_id)) < 0)) {
				asp_update(&com->socket_info, check_status);
				prev_id = com->packet.id;
				memcpy(recvbuffer+i, &com->packet.quality, QUALITY_SIZE);
				memcpy(recvbuffer+i+QUALITY_SIZE, com->packet.data, DATA_SIZE);
			}
		} else { // skip corrupted packet
			com->socket_info.packets_missing++;
			i -= (DATA_SIZE + QUALITY_SIZE);
		}
	}

	// Play
	printf("playing...\n");

	int i = 0;
	uint8_t* recv_read_ptr = recvbuffer;
	uint8_t* recv_write_ptr = recvbuffer;
	uint8_t* play_ptr;

	while (true) {
		if (i <= 0) {
			//Get a sample block
			memcpy(&new_quality, recv_read_ptr, QUALITY_SIZE);
			memcpy(playbuffer, recv_read_ptr+QUALITY_SIZE, BLOCK_SIZE);
			recv_read_ptr += BLOCK_SIZE+QUALITY_SIZE;
			if (recv_read_ptr >= recvbuffer + buffer_size) {
				recv_read_ptr = recvbuffer;
			}
			//Check quality of the sample block and adjust settings if needed
			if (new_quality != current_quality) {
				current_quality = new_quality;
				if (!set_quality(snd_handle, new_quality, sf)) {
					pdie("Error changing quality level");
				}
			}
			play_ptr = playbuffer;
			i = blocksize;
		}

		// write frames to ALSA
		snd_pcm_sframes_t frames = snd_pcm_writei(snd_handle, play_ptr, (blocksize - (*play_ptr - *playbuffer)) / sf->frame_size);

		// Check for errors
		int ret = 0;
		if (frames < 0)
			ret = snd_pcm_recover(snd_handle, frames, 0);
		if (ret < 0) {
			fprintf(stderr, "ERROR: Failed writing audio with snd_pcm_writei(): %i\n", ret);
			exit(EXIT_FAILURE);
		}
		if (frames > 0 && frames < (blocksize - (*play_ptr - *playbuffer)) / sf->frame_size)
			printf("Short write (expected %i, wrote %li)\n", (blocksize - (*play_ptr - *playbuffer)) / sf->frame_size, frames);

		// advance pointers accordingly
		if (frames > 0) {
			play_ptr += frames * sf->frame_size;
			i -= frames * sf->frame_size;
		}

		if ((unsigned) (*play_ptr - *playbuffer) == blocksize)
			i = 0;

		for (unsigned int j = 0; j < 3; ++j) {
			if ((recv_write_ptr == recv_read_ptr) && j > 0) {
				break;
			}

			//Adjust quality if needed
			if(com->socket_info.packets_received >= 100){
				new_quality += adjust_quality(&com->socket_info);
				if(new_quality < 1){
					new_quality = 1;
				}
				if(new_quality > 5){
					new_quality = 5;
				}
			}
			
			//Receive new messages
			if(send_receive(com, &new_quality)){
				// check the id and prev_id to determine packet loss
				if (!((check_status = checkID(com->packet.id, prev_id)) < 0)) {
					asp_update(&com->socket_info, check_status);
					prev_id = com->packet.id;
					memcpy(recv_write_ptr, &com->packet.quality, QUALITY_SIZE);
					memcpy(recv_write_ptr+QUALITY_SIZE, com->packet.data, DATA_SIZE);
				}
				recv_write_ptr += BLOCK_SIZE+QUALITY_SIZE;
				if (recv_write_ptr >= recvbuffer + buffer_size) {
					recv_write_ptr = recvbuffer;
				}
			} else {
				com->socket_info.packets_missing++;
				j--;
			}
		}
	}

	// clean up
	free(recvbuffer);
	free(playbuffer);
}

int main(int argc, char **argv) {
	int buffer_size = BLOCK_SIZE;
	struct sound_format* sf = malloc(sizeof(struct sound_format));

	//Parse command-line options
	int opt;
	while((opt = getopt(argc, argv, ":b:h")) != -1){
		switch(opt){
			case 'b':
				if((buffer_size = atoi(optarg)) < 1){
					printf("-b requires positive integer, default 1Kb used\n");
					buffer_size = 1;
				}
				buffer_size *= (BLOCK_SIZE+QUALITY_SIZE); // convert to Kbyte
				break;
			case 'h':
				printf("-b integer\nbuffer size in KB; standard 1\n\n-h\nPrints the available commandline options and their arguments\n\n");
				break;
		}
	}

	//Set up network stuff
	com_t* com = malloc(sizeof(com_t));
	com_create(com, BIND_PORT, false);

	//Open and configure audio device
	snd_pcm_t* snd_handle;
	if (!configure_audio(&snd_handle, sf)) {
		return -1;
	}

	//Stream the audio
	stream_audio(snd_handle, sf, buffer_size, com);

	//clean up
	free(sf);
	snd_pcm_drain(snd_handle);
	snd_pcm_hw_free(snd_handle);
	snd_pcm_close(snd_handle);

	return 0;
}
