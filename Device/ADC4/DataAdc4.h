#pragma once

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#pragma pack(1)
struct packet_adc_header_t {
	const uint8_t adc4[4] = { 'A', 'D', 'C', '4' };
	uint8_t type;
	uint8_t card;
	uint8_t channel;
	uint8_t flags;
	uint32_t length;
	const uint8_t extra[4] = { 0, 0, 0, 0 };
};
#pragma pack()

struct DataAdc4 {
	uint8_t type;
	uint8_t card;
	uint8_t channel;
	uint8_t flags;
	uint32_t length;
	uint64_t timestamp;
	uint64_t* data;
};