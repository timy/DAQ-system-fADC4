#pragma once

#include "RingBuffer.h"
#include "Device/ADC4/DataAdc4.h"

const size_t nElemOfRingBuffer = 256;
RingBuffer<const DataAdc4*, nElemOfRingBuffer> queuePacket; // ring buffer
const size_t maxDataLength = 2 << 14; // max number of data segments of type uint64_t, as specified by ndigo_packet.length