#include "communication.h"

/*
Allocate and initialize the members of a com_t struct.
- com: the com_t struct to be initialized
- port: the port to bind the socket to
*/
void com_create(com_t* com, const short port, bool server){
	struct sockaddr_in* p = &com->socket_info.local_addr;
	com->packet.data = malloc(DATA_SIZE);
	
	if((com->socket_info.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		pdie("opening socket is wack");
	}
	if(!server){
		p = &com->socket_info.remote_addr;
	}
	
	bzero((char*) &(*p), sizeof(*p));
	(*p).sin_family = AF_INET;
	(*p).sin_addr.s_addr = INADDR_ANY;
	(*p).sin_port = htons(port);
	
	if(server){
		if(bind(com->socket_info.sockfd, (struct sockaddr *) &(*p), sizeof(*p))){
			pdie("Binding socket");
		}
	}
}

/*
Send data to the other party over com
- com: the socket parameters to use to send the data_size
- packet: the data to be sent
*/
bool com_send_p(com_t* const com){
	struct sockaddr_in* p = &com->socket_info.remote_addr;
	uint8_t* buf = malloc(BUFFER_SIZE);
	uint16_t checksum = com_checksum(com);
	
	memcpy(buf, &checksum, CHECKSUM_SIZE);
	memcpy(buf+CHECKSUM_SIZE, &com->packet.id, ID_SIZE);
	memcpy(buf+CHECKSUM_SIZE+ID_SIZE, &com->packet.quality, QUALITY_SIZE);
	memcpy(buf+HEADER_SIZE, com->packet.data, DATA_SIZE);
	
	if(com->bitflips){
		printf("BNVSHGDCSVHG\n");
		bitflip(BUFFER_SIZE-CHECKSUM_SIZE, buf+CHECKSUM_SIZE);
	}

	sendto(com->socket_info.sockfd, buf, BUFFER_SIZE, 0, (struct sockaddr*) &(*p), sizeof(*p));
	free(buf);
	return true;
} //com_send_p

bool com_send_m(com_t* const com, const uint8_t* const message) {
	struct sockaddr_in* p = &com->socket_info.remote_addr;
	sendto(com->socket_info.sockfd, message, MESSAGE_SIZE, 0, (struct sockaddr*)&(*p), sizeof(*p));
	return true;
} //com_send_m

/*
Receive data from the other party over com
- com: the socket parameters to use to receive the data
- packet: the packet in which to store the received data
*/
bool com_recv_p(com_t* const com){
	asp_socket_info* p = &com->socket_info;
	uint8_t* buf = malloc(BUFFER_SIZE);
	int recvlen;
	(*p).remote_addrlen = sizeof((*p).remote_addrlen);
	if((recvlen = recvfrom((*p).sockfd, buf, BUFFER_SIZE, 0,
		 (struct sockaddr *) &(*p).remote_addr, &(*p).remote_addrlen)) < 0){
		free(buf);
		pdie("recvfrom");
		return false;
	}
	memcpy(&com->packet.checksum, buf, CHECKSUM_SIZE);
	memcpy(&com->packet.id, buf+CHECKSUM_SIZE, ID_SIZE);
	memcpy(&com->packet.quality, buf+CHECKSUM_SIZE+ID_SIZE, QUALITY_SIZE);
	memcpy(com->packet.data, buf+HEADER_SIZE, DATA_SIZE);
	
	uint16_t check = com_checksum(com);
	if(check != com->packet.checksum){
		return false;
	}
	
	free(buf);
	return true;
} //com_recv_p

bool com_recv_m(com_t* const com, uint8_t* buf){
	asp_socket_info* p = &com->socket_info;
	int recvlen;
	(*p).remote_addrlen = sizeof((*p).remote_addrlen);
	if((recvlen = recvfrom((*p).sockfd, buf, BUFFER_SIZE, 0,
		 (struct sockaddr *) &(*p).remote_addr, &(*p).remote_addrlen)) < 0){
		pdie("recvfrom");
		return false;
	}
	return true;
} //com_recv_m

/*
Calculate the checksum of the packet's data within the com_t struct
*/
uint16_t com_checksum(com_t* com){
	uint16_t sum = 0;
	sum += com_checksum_add(ID_SIZE, &com->packet.id);
	sum += com_checksum_add(QUALITY_SIZE, &com->packet.quality);
	sum += com_checksum_add(DATA_SIZE, com->packet.data);
	return ~((sum & 0xffff)+(sum >> 16));
}

uint32_t com_checksum_add(unsigned int len, uint8_t* buf){
	uint32_t sum = 0;
	
	for(unsigned int i = 0; i < len; i++){
		if(i % 2 == 0){
			//shift
			sum += (uint32_t)buf[i] << 8;
		} else {
			//not shift
			sum += (uint32_t)buf[i];
		}
	}
	
	return sum;
}

/*
Free a com_t struct's members and finally free the com_t struct itself.
- com: the com_t struct instance to deallocate
*/
void com_destroy(com_t* com){
	free(com->packet.data);
	free(com);
} //com_destroy

/*
Randomly flips the 5th bit from the bytes in buf 
*/
void bitflip(unsigned int len, uint8_t* buf){
	for(unsigned int i = 0; i < len; i++){
		if(rand()%MAX_BITFLIP < MIN_BITFLIP){
			printf("Flip\n");
			*buf = *buf ^ (1 << 4);
		}
	}
}
