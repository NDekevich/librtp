#include "RTP.h"
#include <stdlib.h>
#include <iostream>

//#define STRICT




using namespace rtp;

Rtp::Rtp(bool padding, bool extHeader) {
	
	packet.firstOctet=0x80;			// 10000000 (v = 0b10 = 0d2)
	setPadding(padding);
	setExtension(extHeader);
	/*
	if (padding) {
		packet.firstOctet |= 0x20; //0b00100000
	}
	else {
		packet.firstOctet &= 0xDF; //0b11011111
	}
	if (extHeader) {
		packet.firstOctet += 16;
	}
	*/
	packet.secondOctet = 0;			// 00000000 ( m = 0, pt = 0(pcmu))
	std::vector<uint32_t> CSRC(0);
	extensionLength = 0;
	extensionNum = 0;
}

//template <typename rtpIO>
Rtp::Rtp(uint8_t* input)
{
	setRtpPacket(input);
}


//----------------SETTERS + GETTERS----------------


 void Rtp::setVersion(int version)
{
	packet.firstOctet = (packet.firstOctet & 0x3F) + (version << 6); //0b00111111 
}
 

int Rtp::getVersion() const
{
	return (packet.firstOctet&0xC0)>>6;  //0b11000000
}

void Rtp::setPadding(bool padding)
{
	if (padding) {
		packet.firstOctet |= 0x20 ; //0b00100000
	}
	else {
		packet.firstOctet &= 0xDF; //0b11011111
	}
}

bool Rtp::getPadding() const
{
	return ((packet.firstOctet&0x20)>>5)==1; //0b00100000
}

void Rtp::setExtension(bool extension)
{
	if (extension) {
		packet.firstOctet |= 10 ; //0b00010000
	}
	else {
		packet.firstOctet &=0xEF ; //0b11101111
	}
}

bool Rtp::getExtension() const
{
	return ((packet.firstOctet&10)>>4)==1;  //0b00010000
}

void Rtp::setCSRCcount(int cc)
{
	packet.firstOctet = (packet.firstOctet & 0xF0) + cc; //0b11110000
}

int Rtp::getCSRCcount() const
{
	return(packet.firstOctet & 0x0F); //0b00001111
}

void Rtp::setMarker(bool marker)
{
	if (marker) {
		packet.secondOctet |= 0x80; //0b10000000
	}
	else {
		packet.secondOctet &= 0x7F; //0b01111111
	}
}

bool Rtp::getMarker() const
{
	return ((packet.secondOctet & 0x80) >> 7)==1; //0b10000000
}

void Rtp::setPayloadType(Rtp::rtpPayloadTypes pt)
{
	packet.secondOctet = (packet.secondOctet & 0x80) + (int)pt; //0b10000000
}

Rtp::rtpPayloadTypes Rtp::getPayloadType() const
{
	return (rtpPayloadTypes)(packet.secondOctet & 0x7F); //0b01111111
}

//remove setters/getters below;

void Rtp::setSeqNum(uint16_t seqNum)
{
	packet.seqNum = seqNum;
}

void Rtp::setTimestamp(uint32_t time)
{
	packet.timeStamp = time;
}

void Rtp::setSSRC(uint32_t ssrc)
{
	packet.SSRC = ssrc;
}

void Rtp::setExtensionLength(uint16_t length)
{
	extensionLength = length;
}

void Rtp::setExtensionNum(uint16_t num)
{
	extensionNum = num;
}

template <typename extensionType>
void Rtp::setHeaderExtension(extensionType pointer) {
	headerExtention = *pointer;
}

bool Rtp::addCSRC(uint32_t SRC) {
	if (getCSRCcount()<15){
		setCSRCcount(getCSRCcount() + 1);
		CSRC.push_back(SRC);
		return true;
	}
	return false;
}

void Rtp::setSizeofPayload(uint32_t size)
{
	sizeofPayload = size;
}

void Rtp::setPayload(uint8_t * data)
{
	payload = data;
}



// ----------------PACKET POINTERS----------------
// todo add payload; --done
// todo add padding;
// todo add calcuation of payload size(depending on format of it)
//
// todo add Host to network order (endianess)
//
// try with char arrays;
// remove pointer arithmetic
// 
//


std::shared_ptr<uint8_t> Rtp::createRtpPacket() const 
{
	
	int length;
	length = MIN_HEADER_LENGTH + sizeof(CSRC.data())*CSRC.size() + sizeofPayload * sizeof(payload);
	length += getExtension() ? (EXTENTION_HEADER_PROFILE_LENGTH + extensionLength * sizeof(uint32_t)):0;
	//length += getPadding() ?() : ; //add  padding	
	std::shared_ptr<uint8_t> packetPointer(new uint8_t[length]); // does not work;
			
	uint8_t* memloc = packetPointer.get();
	memcpy(memloc, &packet, MIN_HEADER_LENGTH);
	memloc += MIN_HEADER_LENGTH; 
	memcpy(memloc, CSRC.data(), CSRC.size() * sizeof(uint32_t));
	memloc += CSRC.size() * sizeof(uint32_t);
	if (getExtension()) {
		memcpy(memloc, &extensionNum, sizeof(extensionNum));
		memloc += sizeof(extensionNum);
		memcpy(memloc, &extensionLength, sizeof(extensionLength));
		memloc += sizeof(extensionLength);
		memcpy(memloc, headerExtension, extensionLength * sizeof(headerExtension));
		memloc += extensionLength * sizeof(headerExtension);
	}
	memcpy(memloc, payload, sizeofPayload * sizeof(payload));
	memloc += sizeofPayload * sizeof(payload);
/*
	FOR TEST
		std::cout << std::endl << "LENGTH " << length << std::endl;
	std::cout << std::endl << "MEMLOC " << memloc - packetPointer.get() << std::endl;
	std::cout << std::endl << "VVPXCC  " << (int)*packetPointer.get() << std::endl;
*/
	
	return packetPointer;
}




/*//check for null when used + free memory after use.
uint8_t* Rtp::createRtpPacket() const				
{
	int packetLength = MIN_HEADER_LENGTH + sizeof(CSRC) + sizeofPayload*sizeof(payload);
	if (getExtension()) {
		int packetLength = MIN_HEADER_LENGTH + sizeof(CSRC) + EXTENTION_HEADER_PROFILE_LENGTH + extensionLength * sizeof(uint32_t) + sizeofPayload *sizeof(payload);
	}
	uint8_t *packetPointer = (uint8_t*)malloc(packetLength*sizeof(uint8_t));
	memcpy(packetPointer,&packet,MIN_HEADER_LENGTH);
	memcpy(packetPointer + MIN_HEADER_LENGTH, CSRC.data(), sizeof(CSRC));
	if (getExtension()) {
		memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC), &extensionNum, sizeof(extensionNum));
		memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC) + sizeof(extensionNum), &extensionLength, sizeof(extensionLength));
		memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC) + EXTENTION_HEADER_PROFILE_LENGTH, headerExtension, extensionLength * sizeof(uint32_t));
	}
	memcpy((packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC)), payload, sizeof(payload));// *sizeofPayload);
	return packetPointer;
	//(getExtension() ? (EXTENTION_HEADER_PROFILE_LENGTH + extensionLength * sizeof(uint32_t)) : 0))
}
*/

template <typename rtpIO>
void Rtp::setRtpPacket(rtpIO input)
{
	uint8_t* inpacket = input;
	
	packet = *(rtpPacket*)inpacket;
	inpacket += MIN_HEADER_LENGTH;
	int cc = getCSRCcount();
	for (int i = 0; i < cc; i++) {
		uint32_t temp = *(uint32_t*)inpacket;
		inpacket += 4;
		CSRC.push_back(temp);
	}
	if (getExtension()) {
		extensionNum = *(uint16_t*)inpacket;
		inpacket += 2;
		extensionLength = *(uint16_t*)inpacket;
		inpacket += 2;
		headerExtension = (void*)malloc(extensionLength * sizeof(uint32_t));
		memcpy((headerExtension), inpacket, extensionLength * sizeof(uint32_t));
		inpacket += extensionLength * 4;
	}
	std::cout << std::endl << " value " << *inpacket  << std::endl;
	payload = (uint8_t*)malloc(sizeofPayload * sizeof(payload));
											 
	memcpy(payload, inpacket, sizeofPayload * sizeof(payload));
	std::cout << std::endl << " value 2 " << *payload << std::endl;
	//std::cout << "part5";
}



//
// ----------------RTCP ----------------
//

// 0-1 version, 2 padding, 3-7 RECEPTION Report count

