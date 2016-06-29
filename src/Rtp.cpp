#include "RTP.h"
#include <stdlib.h>
//#define STRICT




using namespace rtp;

Rtp::Rtp(bool padding, bool extHeader) {
	
	packet.firstOctet=0x80;			// 10000000 (v = 0b10 = 0d2)
	if (padding) {
		packet.firstOctet += 32; 
	}
	if (extHeader) {
		packet.firstOctet += 16;
	}
	packet.secondOctet = 0;			// 00000000 ( m = 0, pt = 0(pcmu))
	std::vector<uint32_t> CSRC(0);
	extensionLength = 0;
	extensionNum = 0;
}

template <typename rtpIO>
Rtp::Rtp(rtpIO input)
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
		packet.firstOctet |= 0x80; //0b10000000
	}
	else {
		packet.firstOctet &= 0x7F; //0b01111111
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

void Rtp::setPayload(char * data)
{
	payload = data;
}


//----------------PACKET POINTERS----------------
//todo add extension to passed data;--done
//todo add payload;
//todo add padding;


//check for null when used + free memory after use.
uint8_t* Rtp::createRtpPacket() const				
{
	int packetLength = MIN_HEADER_LENGTH + sizeof(CSRC)+sizeofPayload;//+sizeof(payload);
	if (getExtension()) {
		int packetLength = MIN_HEADER_LENGTH + sizeof(CSRC) + EXTENTION_HEADER_PROFILE_LENGTH + extensionLength * sizeof(uint32_t)+sizeofPayload; //+sizeof(payload);
	}
	uint8_t *packetPointer = (uint8_t*)malloc(packetLength);
	memcpy(packetPointer,&packet,MIN_HEADER_LENGTH);
	memcpy(packetPointer + MIN_HEADER_LENGTH, CSRC.data(), sizeof(CSRC));
	if (getExtension()) {
		memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC), &extensionNum, sizeof(extensionNum));
		memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC)+sizeof(extensionNum), &extensionLength, sizeof(extensionLength));
		memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC) + EXTENTION_HEADER_PROFILE_LENGTH, headerExtension, extensionLength * sizeof(uint32_t));
	}
	memcpy(packetPointer + MIN_HEADER_LENGTH + sizeof(CSRC) + EXTENTION_HEADER_PROFILE_LENGTH + extensionLength * sizeof(uint32_t),payload,(sizeofPayload)*sizeofPayload);
	return packetPointer;
}


template <typename rtpIO>
void Rtp::setRtpPacket(rtpIO input)
{
	uint8_t* inpacket = input;
	//uint8_t* start = inpacket;
	packet = *(rtpPacket*)inpacket;
	inpacket += MIN_HEADER_LENGTH;
	/*
	packet.firstOctet = *inpacket++;
	packet.secondOctet = *inpacket++;
	//uint16_t* inpacket = (uint16_t*)inpacket;
	packet.seqNum = (uint16_t)*inpacket;
	inpacket += 2;
	//uint32_t* inpacket = (uint32_t*)inpacket;
	packet.timeStamp = (uint32_t)*inpacket++;
	inpacket += 4;
	packet.SSRC = (uint32_t)*inpacket++;
	inpacket += 4;
	*/
	int cc = getCSRCcount();
	memcpy((CSRC.data()), inpacket, cc * sizeof(uint32_t));//logic error??
	if (getExtension()) {

		extensionNum = *(uint16_t*)inpacket;
		inpacket += 2;
		extensionLength = *(uint16_t*)inpacket;
		inpacket += 2;
		headerExtension = (void*)malloc(extensionLength * sizeof(uint32_t));
		memcpy((headerExtension), inpacket, extensionLength * sizeof(uint32_t));

	}
	payload = (char*)malloc(sizeofPayload * sizeof(payload));
	payload = memcpy(payload, inpacket, sizeofPayload * sizeof(payload));
	//free(input);									// --!!!!!!! Memory leak?
}



//
// ----------------RTCP ----------------
//

// 0-1 version, 2 padding, 3-7 RECEPTION Report count


Rtcp::Rtcp()
{
	header.firstOctet = 0x80;
}

void Rtcp::setVersion(int version)
{
	header.firstOctet = (header.firstOctet & 0x3F) + (version << 6);
}

int Rtcp::getVersion() const
{
	return (header.firstOctet & 0xC0)>>6 ;
}

void Rtcp::setPadding(bool padding)
{
	if (padding) {
		header.firstOctet |= 0x20; //0b00100000
	}
	else {
		header.firstOctet &= 0xDF; //0b11011111
	}
}

bool Rtcp::getPadding() const
{
	return ((header.firstOctet & 0x20)>>5)==1;
}

void Rtcp::setReportCount(int rc)
{
	if (rc < 31)
		header.firstOctet = (header.firstOctet & 0xE0) + rc;
}

int Rtcp::getReportCount()
{
	return header.firstOctet&0x1F;
}

void Rtcp::setPayload(rtcpPayloadTypes pc)
{
	header.payloadType = pc;
}

void Rtcp::setHeaderLength(uint16_t length)
{
	header.length = length;
}

void Rtcp::setHeaderSSRC(uint32_t ssrc)
{
	SSRC = ssrc;
}

Rtcp::reportBlock::reportBlock(uint32_t ssr, uint8_t fl, uint32_t pl, uint32_t hSeqNum, uint32_t jitter, uint32_t lsr, uint32_t dslsr)
{
	ssrc = ssr;
	fractionLost = fl;
	packetsLost = (pl&0x00FFFFFF);
	highestSeqNum = hSeqNum;
	interarrivalJitter = jitter;
	lastSR = lsr;
	delaySinceLSR = dslsr;
}

Rtcp::senderInfo::senderInfo(uint64_t ntpTs, uint32_t rtpTs, uint32_t pc, uint32_t oc)
{
	ntpTimestamp = ntpTs;
	rtpTimestamp = rtpTs;
	packetCount = pc;
	octetCount = oc;
}

Rtcp::sdesItem::sdesItem(rtpSdesTypes type, uint32_t length, char * data)
{
	rtpSdesType = type;
	itemLength = length;
	item = (char*)malloc(length * sizeof(char));
	memcpy(item, data, length * sizeof(char));
}
