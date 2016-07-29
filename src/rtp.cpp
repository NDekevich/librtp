

#include "rtp.h"
#include <stdlib.h>
#include <iostream>
#include <boost/endian/conversion.hpp>


#define IS_BIG_ENDIAN 0

//#define STRICT


//REDO
//HEADER EXTENSION


// ----------------PACKET POINTERS----------------
// todo add payload; --done
// todo add padding;  
// todo add calcuation of payload size(depending on format of it)
//
// todo add Host to network order (endianess)         <------------ important
//
// try with char arrays; -- done with vectors
// 
//


using namespace rtp;

Rtp::Rtp() {
	
	if (IS_BIG_ENDIAN) {
		packet.firstOctet = 0x80;			// 10000000 (v = 0b10 = 0d2)
	}
	else
	{
		packet.firstOctet = 0b00000010;
	}
	packet.secondOctet = 0;			// 00000000 ( m = 0, pt = 0(pcmu))
	packet.seqNum = 0;
	packet.timeStamp = 0;
	packet.SSRC = 0;
	std::vector<uint32_t> CSRC(0);
	extensionLength = 0;
	extensionNum = 0;
}

//template <typename rtpIO>
Rtp::Rtp(std::vector<uint8_t> input)
{
	setRtpPacket(input);
}


//----------------SETTERS + GETTERS----------------

#if IS_BIG_ENDIAN

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
		packet.firstOctet |= 0x10 ; //0b00010000
	}
	else {
		packet.firstOctet &=0xEF ; //0b11101111
	}
}

bool Rtp::getExtension() const
{
	return ((packet.firstOctet&0x10)>>4)==1;  //0b00010000
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
#else
void Rtp::setCSRCcount(int cc)
{
	packet.firstOctet = (packet.firstOctet & 0x0F) + (cc << 4);
}

int Rtp::getCSRCcount() const
{
	return (packet.firstOctet & 0b11110000) >> 4;
}

void Rtp::setExtension(bool extension)
{
	if (extension) {
		packet.firstOctet |= 0b00001000 ; //0b00010000
	}
	else {
		packet.firstOctet &=0b11110111 ; //0b11101111
	}
}

bool Rtp::getExtension() const
{
	return ((packet.firstOctet&0b00001000)>>3)==1;  //0b00010000
}


void Rtp::setPadding(bool padding)
{
	if (padding) {
		packet.firstOctet |= 0b00000100; //0b00100000
	}
	else {
		packet.firstOctet &= 0b11111011; //0b11011111
	}
}

bool Rtp::getPadding() const
{
	return ((packet.firstOctet & 0b00000100) >> 2) == 1; //0b00100000
}

void Rtp::setVersion(int version)
{
	packet.firstOctet = (packet.firstOctet & 0x11111100) + (version); //0b00111111 
}

int Rtp::getVersion() const
{
	return (packet.firstOctet & 0b00000011);  //0b11000000
}

void Rtp::setPayloadType(Rtp::rtpPayloadTypes pt)
{
	packet.secondOctet = (packet.secondOctet & 0b00000001) + ((int)pt<<1); //0b10000000
}

Rtp::rtpPayloadTypes Rtp::getPayloadType() const
{
	return (rtpPayloadTypes)((packet.secondOctet & 0b11111110)>>1); //0b01111111
}

void Rtp::setMarker(bool marker)
{
	if (marker) {
		packet.secondOctet |= 0b00000001; //0b10000000
	}
	else {
		packet.secondOctet &= 0b11111110; //0b01111111
	}
}

bool Rtp::getMarker() const
{
	return ((packet.secondOctet & 0b00000001)) == 1; //0b10000000
}

#endif
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

void Rtp::setPayload(std::vector<uint8_t> data)
{
	payload = data;
}


void Rtp::setHeaderExtension(std::vector<uint8_t> ext)
{
	headerExtension = ext;
}


#if IS_BIG_ENDIAN

std::shared_ptr<std::vector<uint8_t>> Rtp::createRtpPacket() const
{
	std::shared_ptr<std::vector<uint8_t>> outPacket(new std::vector<uint8_t>);
	try {
	
		uint8_t* ptr = (uint8_t*)&packet;
		(*outPacket).insert((*outPacket).begin(), ptr, ptr + sizeof(packet));
	
		ptr = (uint8_t*)CSRC.data();
		for (int i = 0; i < CSRC.size()*4; i++) {
			(*outPacket).push_back(*ptr);
			ptr++;
		}
			
		if (getExtension()) {
			ptr = (uint8_t*)&extensionNum;
			(*outPacket).insert((*outPacket).end(), ptr, ptr + 2);
			ptr = (uint8_t*)&extensionLength;
			(*outPacket).insert((*outPacket).end(), ptr, ptr + 2);
			(*outPacket).insert((*outPacket).end(), headerExtension.begin(), headerExtension.end());
		}
		
		(*outPacket).insert((*outPacket).end(), payload.begin(), payload.end());
		int length = (*outPacket).size();
		
		if ((getPadding()) && (length % 4 != 0)) {
			uint8_t i = 0;
			for (i = 0; i < (3 - length % 4); i++) {
				(*outPacket).push_back(0);
			}
			(*outPacket).push_back(i + 1);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return outPacket;
}


void Rtp::setRtpPacket(std::vector<uint8_t> input)
{
	int position = 0;
	packet = *(rtpPacket*)input.data();
	position += MIN_HEADER_LENGTH;
	int cc = getCSRCcount();
	CSRC.clear();
	uint32_t* ptr;
	for (int i = 0; i < cc; i++) {
		ptr = (uint32_t*)(input.data() + position);
		CSRC.push_back(*ptr);
		position+=4;
	}
	if (getExtension()) {
		extensionNum = *(uint16_t*)(input.data()+position);
		position += 2;
		extensionLength = *(uint16_t*)(input.data()+position);
		position += 2;
		headerExtension.clear();
		headerExtension.insert(headerExtension.begin(), (input.begin() + position), (input.begin() + position + extensionLength));
		position += extensionLength;
	}
	payload.clear();
	if(getPadding()){
		int paddingSize = *(input.end() - 1);
		if ((input.begin() + position) > (input.end() - paddingSize)) {
			payload.insert(payload.begin(), input.begin() + position, input.end() - paddingSize);
		}
	}
	else
	{
		payload.insert(payload.begin(), input.begin() + position, input.end());
	}
}
#else 


std::shared_ptr<std::vector<uint8_t>> Rtp::createRtpPacket() const
{
	std::shared_ptr<std::vector<uint8_t>> outPacket(new std::vector<uint8_t>);
	try {
		uint32_t temp32;
		uint16_t temp16;
		rtp::Rtp::rtpPacket oPack = packet;
		boost::endian::endian_reverse_inplace(oPack.seqNum);
		boost::endian::endian_reverse_inplace(oPack.SSRC);
		boost::endian::endian_reverse_inplace(oPack.timeStamp);
		uint8_t* ptr = (uint8_t*)&oPack;
		(*outPacket).insert((*outPacket).begin(), ptr, ptr + sizeof(oPack));
		
		for (int i = 0; i < getCSRCcount(); i++) {
			temp32 = boost::endian::endian_reverse(CSRC[i]);
			ptr = (uint8_t*)&temp32;
			for (int j = 0; j < 4; j++) {
				(*outPacket).push_back(*ptr);
				ptr++;
			}
		}
		
		if (getExtension()) {
			temp16 = boost::endian::endian_reverse(extensionNum);
			ptr = (uint8_t*)&temp16;
			(*outPacket).insert((*outPacket).end(), ptr, ptr + 2);
			temp16 = boost::endian::endian_reverse(extensionLength);
			ptr = (uint8_t*)&temp16;
			(*outPacket).insert((*outPacket).end(), ptr, ptr + 2);
			(*outPacket).insert((*outPacket).end(), headerExtension.begin(), headerExtension.end());
		}
		
		(*outPacket).insert((*outPacket).end(), payload.begin(), payload.end());
		int length = (*outPacket).size();

		if ((getPadding()) && (length % 4 != 0)) {
			uint8_t i = 0;
			for (i = 0; i < (3 - length % 4); i++) {
				(*outPacket).push_back(0);
			}
			(*outPacket).push_back(boost::endian::endian_reverse(i + 1));
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Error rtp 1: " << e.what() << std::endl;
	}
	return outPacket;
}

void Rtp::setRtpPacket(std::vector<uint8_t> input)
{
	try {

		int position = 0;
		packet.firstOctet = input[position];
		position++;
		packet.secondOctet = input[position];
		position++;
		uint16_t* ptr16 = (uint16_t*)(input.data() + position);
		packet.seqNum = boost::endian::endian_reverse(*ptr16);
		position += 2;
		uint32_t* ptr32 = (uint32_t*)(input.data() + position);
		packet.timeStamp = boost::endian::endian_reverse(*ptr32);
		position += 4;
		ptr32 = (uint32_t*)(input.data() + position);
		packet.SSRC = boost::endian::endian_reverse(*ptr32);

		int cc = getCSRCcount();
		CSRC.clear();

		for (int i = 0; i < cc; i++) {
			ptr32 = (uint32_t*)(input.data() + position);
			CSRC.push_back(boost::endian::endian_reverse(*ptr32));
			position += 4;
		}
		if (getExtension()) {
			extensionNum = boost::endian::endian_reverse(*(uint16_t*)(input.data() + position));
			position += 2;
			extensionLength = boost::endian::endian_reverse(*(uint16_t*)(input.data() + position));
			position += 2;
			headerExtension.clear();
			headerExtension.insert(headerExtension.begin(), (input.begin() + position), (input.begin() + position + extensionLength));
			position += extensionLength;
		}
		payload.clear();
		if (getPadding()) {
			int paddingSize = *(input.end() - 1);
			if ((input.begin() + position) > (input.end() - paddingSize)) {
				payload.insert(payload.begin(), input.begin() + position, input.end() - paddingSize);
			}
		}
		else
		{
			payload.insert(payload.begin(), input.begin() + position, input.end());
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Error rtp.cpp 1:" << e.what() << std::endl;
	}
}


#endif




rtp::Rtp::rtpPacket::rtpPacket()
{
#if IS_BIG_ENDIAN
	firstOctet = 128;
#else
	firstOctet = 0b00000010;
#endif
	secondOctet = 0;
	seqNum = 0;
	timeStamp = 0;
	SSRC = 0;
}
