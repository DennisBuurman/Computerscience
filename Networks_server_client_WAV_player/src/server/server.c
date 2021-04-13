/*
 * Skeleton-code behorende bij het college Netwerken, opleiding Informatica,
 * Universiteit Leiden.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include <netinet/in.h>

#include "../communication/asp/asp.h"
#include "../communication/communication.h"
#include "../util/util.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <errno.h>

#define BIND_PORT 1234
#define DEFAULT_QUALITY 5
#define DROP_CHANCE 100
#define DROP_RATIO 5

//commandline option struct
optionset opts;

struct wave_header {
	char riff_id[4];
	uint32_t size;
	char wave_id[4];
	char format_id[4];
	uint32_t format_size;
	uint16_t w_format_tag;
	uint16_t n_channels;
	uint32_t n_samples_per_sec;
	uint32_t n_avg_bytes_per_sec;
	uint16_t n_block_align;
	uint16_t w_bits_per_sample;
};

// wave file handle
struct wave_file {
	struct wave_header* wh;
	int fd;

	void *data;
	uint32_t data_size;

	uint8_t* samples;
	uint32_t payload_size;
};

static struct wave_file wf = {0,};

/*
Open a wave file
- wf: a struct in which to put properties of the WAVE
- filename: the filename of the WAVE file to open
*/
static bool open_wave_file(struct wave_file* wf, const char* filename) {
	// Open the file for read only access
	wf->fd = open(filename, O_RDONLY);
	if (wf->fd < 0) {
		fprintf(stderr, "couldn't open %s\n", filename);
		return false;
	}

	struct stat statbuf;
	// Get the size of the file
	if (fstat(wf->fd, &statbuf) < 0)
		return false;

	wf->data_size = statbuf.st_size; // Total size of the file

	// Map the file into memory
	wf->data = mmap(0x0, wf->data_size, PROT_READ, MAP_SHARED, wf->fd, 0);
	if (wf->data == MAP_FAILED) {
		fprintf(stderr, "mmap failed\n");
		return false;
	}

	wf->wh = wf->data;

	// Check whether the file is a wave file
	if (strncmp(wf->wh->riff_id, "RIFF", 4)
	    || strncmp(wf->wh->wave_id, "WAVE", 4)
	    || strncmp(wf->wh->format_id, "fmt", 3)) {
		fprintf(stderr, "%s is not a valid wave file\n", filename);
		return false;
	}

	// Skip to actual data fragment
	uint8_t* p = (uint8_t*) wf->data + wf->wh->format_size + 16 + 4;
	uint32_t* size = (uint32_t*) (p + 4);
	do {
		if (strncmp((char*) p, "data", 4))
			break;

		wf->samples = p;
		wf->payload_size = *size;
		p += 8 + *size;
	} while (strncmp((char*) p, "data", 4) && (uint32_t) (((uint8_t*) p) - (uint8_t*) wf->data) < statbuf.st_size);

	if (wf->wh->w_bits_per_sample != 16) {
		fprintf(stderr, "can't play sample with bitsize %d\n",
						wf->wh->w_bits_per_sample);
		return false;
	}

	float playlength = (float) *size / (wf->wh->n_channels * wf->wh->n_samples_per_sec * wf->wh->w_bits_per_sample / 8);

	printf("file %s, mode %s, samplerate %u, time %.1f sec\n", filename, wf->wh->n_channels == 2 ? "Stereo" : "Mono", wf->wh->n_samples_per_sec, playlength);
	return true;
} //open_wave_file

/*Close and clean up a WAVE file
- wf: the WAVE file to close
*/
static void close_wave_file(struct wave_file *wf) {
	munmap(wf->data, wf->data_size);
	close(wf->fd);
} //close_wave_file

static void compress_set_params(uint8_t quality, unsigned int* skipI, unsigned int* bitR){
	switch (quality) {
		case 1:
			*skipI = 2;
			*bitR = 1;
		break;
		case 2:
			*skipI = 4;
			*bitR = 1;
		break;
		case 3:
			*skipI = 2;
			*bitR = 0;
		break;
		case 4:
			*skipI = 4;
			*bitR = 0;
		break;
		case 5:
			*skipI = 0;
			*bitR = 0;
		break;
		default:
			pdie("unsupported quality level selected");
		break;
	}
}

/*
Change the quality of the audio to a desired quality/size.
- wf: the WAVE file from where the samples should be read
- offset: from where in the WAVE file should the samples be read
- buffer: the buffer in which to put the output sound data_size
- quality: determines how agressively the audio is compressed
*/
static uint32_t compress(struct wave_file* wf, uint32_t offset, uint8_t* buffer, uint8_t quality){
	uint8_t* samples = wf->samples;
	unsigned int skipInterval = 0;
	unsigned int bitReduce = 0;

	//Determine sizes of input data
	uint16_t orig_bit_depth = wf->wh->w_bits_per_sample;
	uint16_t orig_sample_size = (orig_bit_depth / 8);
	uint16_t orig_num_channels = wf->wh->n_channels;
	unsigned int orig_frame_size = ((orig_bit_depth * orig_num_channels) / 8);
	uint8_t* orig_sample = malloc(orig_frame_size);

	//Set parameters according to quality level
	compress_set_params(quality, &skipInterval, &bitReduce);

	//Determine sizes of output data
	uint16_t new_bit_depth = orig_bit_depth;
	if (bitReduce) {
		new_bit_depth = orig_bit_depth/2;
	}
	unsigned int new_num_channels = orig_num_channels;
	if (((new_bit_depth * new_num_channels) % 8) != 0) {
		pdie("New frame size not byte-aligned");
	}
	if ((new_bit_depth % 8) != 0) {
		pdie("New bit depth not byte-aligned");
	}
	unsigned int new_frame_size = ((new_bit_depth * new_num_channels) / 8);
	unsigned int new_sample_size = (new_bit_depth / 8);
	uint8_t* new_sample = malloc(new_frame_size);

	//Resample the audio
	unsigned int j;
	unsigned int channel;
	unsigned int bytes_read = 0;
	unsigned int bytes_written = 0;
	while (bytes_written < DATA_SIZE) {
		if (skipInterval > 1) {
			j = 0;
			while (j < skipInterval-1) {
				channel = 0;
				while (channel < new_num_channels) {
					memcpy(orig_sample, samples+offset+bytes_read+channel*orig_sample_size, orig_sample_size);
					memcpy(new_sample, &orig_sample[orig_sample_size-new_sample_size], new_sample_size);
					if (bitReduce) {
						*new_sample = *new_sample + 128;
					}
					memcpy(buffer+bytes_written+channel*new_sample_size, new_sample, new_sample_size);
					++channel;
				}
				bytes_read += orig_frame_size;
				bytes_written += new_frame_size;
				++j;
			}
			bytes_read += orig_frame_size;
		} else {
			channel = 0;
			while (channel < new_num_channels) {
				memcpy(orig_sample, samples+offset+bytes_read+channel*orig_sample_size, orig_sample_size);
				memcpy(new_sample, &orig_sample[orig_sample_size-new_sample_size], new_sample_size);
				if (bitReduce) {
					*new_sample = *new_sample + 128;
				}
				memcpy(buffer+bytes_written+channel*new_sample_size, new_sample, new_sample_size);
				++channel;
			}
			bytes_read += orig_frame_size;
			bytes_written += new_frame_size;
		}
	}
	return bytes_read;
} //compress

/*
Determine the current packet ID based on the previous packet ID_SIZE
- new: The new ID to be determined
- prev: the ID of the previous packet
*/
static void setID(uint8_t* new, uint8_t prev){
	uint8_t number = prev & 127;
	uint8_t sign = (prev >> 7) & 1;
	if(number < 127){
		number += 1;
	} else {
		number = 0;
		sign = ~sign;
	}
	sign <<= 7;
	*new = number + sign;
} //setID

/*
Send audio data over com to a client
- com: contains the parameters used to send the audio data
*/
static void send_audio(com_t* const com) {
	uint8_t* buf = malloc(DATA_SIZE);
	uint8_t* recvbuf = malloc(MESSAGE_SIZE);
	uint8_t id = 0;
	uint8_t prev_id = 0;
	uint8_t quality = DEFAULT_QUALITY;
	uint32_t it = 0;
	bool sent = false;

	//Check if samples can be read from the file
	if(wf.samples == NULL){
		pdie("In send_audio: error reading samples");
	}

	//Read and send audio samples
	while(it < wf.payload_size - DATA_SIZE){
		//Receive a message from the client with the desired quality
		com_recv_m(com, recvbuf);
		
		//Adjust quality
		if(opts.fixed_quality == -1){
			quality = *recvbuf;
		} else {
			quality = opts.fixed_quality;
		}

		//Create and send audio data
		while(!sent){
			int drop = rand() % DROP_CHANCE;
			setID(&id, prev_id);
			it += compress(&wf, it, buf, quality);
			com->packet.id = id;
			com->packet.quality = quality;
			memcpy(com->packet.data, buf, DATA_SIZE);
			if(drop > DROP_RATIO || !opts.drop_packets){
				com_send_p(com);
				sent = true;
			} 
			//Update ID information
			prev_id = id;
		}
		sent = false;
	}

	//free allocated memory
	free(buf);
	free(recvbuf);
} //send_audio

inline static void init_opts(){
	opts.drop_packets = false;
	opts.bit_flips = false;
	opts.fixed_quality = -1; 
}

/*
Open a WAVE file and stream the contents over a network connection to a client
*/
int main(int argc, char** argv) {
	//setup network stuff
	com_t* com = malloc(sizeof(com_t));
	com_create(com, BIND_PORT, true);

	srand(time(0));

	//Parse command-line options
	init_opts();
	char* filename;
	char const* extension = ".wav";
	char const* Extension = ".WAV";
	int opt;
	bool fflag = false;

	while((opt = getopt(argc, argv, ":f:hd:b:q:")) != -1){
		switch(opt){
			case 'f':
				fflag = true;
				if(strstr(optarg, (char*) extension) != NULL && strstr(optarg, (char*) Extension) != NULL){
					pdie("No .wav extension");
				}
				filename = (char*) optarg;
				printf("%s\n", filename);
				break;
			case 'h':
				printf("Commandline arguments:\n-f filename.wav\nRequired. Opens filename.wav and streams content to client.\n\n-h\nPrints the available commandline options and their arguments\n\n-d\nEnables packet drop simulation\n\n-b\nEnables bit flip simulation\n\n-q integer\nSets fixed quality level from 1 to 5\n\n");
				break;
			case 'd':
				opts.drop_packets = true;
				break;
			case 'b':
				opts.bit_flips = true;
				com->bitflips = true;
				break;
			case 'q':
				if(optarg == NULL){
					pdie("-q option requires an integer from 1 to 5");
				}
				opts.fixed_quality = atoi(optarg);
				if(opts.fixed_quality < 1 || opts.fixed_quality > 5){
					pdie("-q option requires an integer from 1 to 5");
				}
				break;
		}
	}

	if(!fflag){
		pdie("No filename argument -f");
	}

	//Open the WAVE file
	if (!open_wave_file(&wf, filename))
		return -1;

	//Read and send the WAVE filename
	srand(time(NULL)); // for random drop of package
	send_audio(com);

	// Clean up
	close_wave_file(&wf);

	return 0;
} //main
