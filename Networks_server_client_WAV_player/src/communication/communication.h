#ifndef communication_h
#define communication_h

//header definitions
#define CHECKSUM_SIZE 2
#define ID_SIZE 1
#define QUALITY_SIZE 1
#define HEADER_SIZE (CHECKSUM_SIZE + ID_SIZE + QUALITY_SIZE)

//data definitions
#define DATA_SIZE 1024

//buffer definitions
#define MESSAGE_SIZE 1
#define BUFFER_SIZE (HEADER_SIZE + DATA_SIZE)

//Bitflip rate definitions
#define MAX_BITFLIP 10000000
#define MIN_BITFLIP 5

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>

#include "../util/util.h"
#include "asp/asp.h"

typedef struct{
	//Structure of a packet: CHECKSUM,ID,Quality,DATA
	uint16_t checksum;
	uint8_t id;
	uint8_t quality;
	uint8_t* data;
}packet_t;

typedef struct{
	//variables required for sockets
	packet_t packet;
	asp_socket_info socket_info;
	//bit flipping argument option
	bool bitflips;
}com_t;

void com_create(com_t* com, const short port, bool server);
bool com_send_p(com_t* const com);
bool com_send_m(com_t* const com, const uint8_t* const buf);
bool com_recv_p(com_t* const com);
bool com_recv_m(com_t* const com, uint8_t* buf);
uint16_t com_checksum(com_t* com);
uint32_t com_checksum_add(unsigned int len, uint8_t* buf);
void com_destroy(com_t* com);
void set_bitflip();
void bitflip(unsigned int len, uint8_t* buf);

#endif
