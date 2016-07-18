// testsRtp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include <fstream>

#include "libavcodec/avcodec.h"
#include "controlBlock.h"

using namespace rtp;

TEST(RtpTest, versionTest)
{
	Rtp testPacket;
	ASSERT_EQ(testPacket.getVersion(),2)<<"Starting";
	testPacket.setVersion(1);
	ASSERT_EQ(testPacket.getVersion(),1) << "Changed";
	;
}

TEST(RtpTest, paddingTest)
{
	Rtp testPacket;
	ASSERT_FALSE(testPacket.getPadding()) << "Initial";
	testPacket.setPadding(true);
	ASSERT_TRUE(testPacket.getPadding()) << "Changed";
}

TEST(RtpTest, extensionTest)
{
	Rtp testPacket;
	ASSERT_FALSE(testPacket.getExtension()) << "Initial";
	testPacket.setExtension(true);
	ASSERT_TRUE(testPacket.getVersion()) << "Changed";
}

TEST(RtpTest, markerTest)
{
	Rtp testPacket;
	ASSERT_FALSE(testPacket.getMarker()) << "Initial";
	testPacket.setMarker(true);
	ASSERT_TRUE(testPacket.getMarker()) << "Changed";
}

TEST(RtpTest, ccTest)
{
	Rtp testPacket;
	ASSERT_EQ(testPacket.getCSRCcount(), 0);
	testPacket.setCSRCcount(10);
	ASSERT_EQ(testPacket.getCSRCcount(), 10);
	testPacket.setCSRCcount(testPacket.getCSRCcount() + 1);
	ASSERT_EQ(testPacket.getCSRCcount(), 11);
}

TEST(RtpTest, payloadTypeTest)
{
	Rtp testPacket;
	ASSERT_EQ(testPacket.getPayloadType(),testPacket.PCMU);
	testPacket.setPayloadType(testPacket.PCMA);
	ASSERT_EQ(testPacket.getPayloadType(), testPacket.PCMA);
}

TEST(RtpTest, SeqNum)
{
	Rtp testPacket;
	ASSERT_EQ(testPacket.getSeqNum(), 0);
	testPacket.setSeqNum(123);
	ASSERT_EQ(testPacket.getSeqNum(), 123);
}

TEST(RtpTest, timeStamp)
{
	Rtp testPacket;
	ASSERT_EQ(testPacket.getTimestamp(), 0);
	testPacket.setTimestamp(123);
	ASSERT_EQ(testPacket.getTimestamp(), 123);

}

TEST(RtpTest, ssrc) {
	Rtp testPacket;
	ASSERT_EQ(testPacket.getSSRC(), 0);
	testPacket.setSSRC(123456789);
	ASSERT_EQ(testPacket.getSSRC(), 123456789);

}

TEST(RtpTest, CSRC) 
{
	Rtp testPacket;
	testPacket.addCSRC(123);
	std::vector<uint32_t> csrc = testPacket.getCSRC();
	ASSERT_EQ(csrc[0], 123);

}


TEST(RtpTest, packetFormation) {
	Rtp firstPacket;
	firstPacket.setMarker(true);
	firstPacket.setExtension(true);
	firstPacket.setExtensionNum(200);
	firstPacket.setExtensionLength(1);
	uint8_t headerExt = 'c';
	firstPacket.setHeaderExtension(&headerExt);
	firstPacket.addCSRC(1);
	firstPacket.addCSRC(2);
	firstPacket.setSeqNum(42);
	firstPacket.setPayloadType(firstPacket.PCMU);
	firstPacket.setTimestamp(0);
	firstPacket.setSSRC(1);
	uint8_t data = 'a';
	firstPacket.setPayload(&data);
	firstPacket.setSizeofPayload(1);
	



	std::shared_ptr<uint8_t> startpre = firstPacket.createRtpPacket();
	uint8_t* start = startpre.get();
	uint8_t* start2 = start;
	
	

	ASSERT_EQ(*start, 146) << "vvpxcc";
	start++;
	ASSERT_EQ(*start, 128) <<"mpl";
	start++;
	ASSERT_EQ(*start, 42) << "seqnum1";
	start++;
	ASSERT_EQ(*start, 0) << "seqnum2";
	start++;
	ASSERT_EQ(*start, 0) << "ts1";
	start++;
	ASSERT_EQ(*start, 0) << "ts2";
	start++;
	ASSERT_EQ(*start, 0) << "ts3";
	start++;
	ASSERT_EQ(*start, 0) << "ts4";
	start++;
	ASSERT_EQ(*start, 1) << "ssrc1";
	start++;
	ASSERT_EQ(*start, 0) << "ssrc2";
	start++;
	ASSERT_EQ(*start, 0) << "ssrc3";
	start++;
	ASSERT_EQ(*start, 0) << "ssrc4";
	start++;
	ASSERT_EQ(*start, 1) << "csrc11";
	start++;
	ASSERT_EQ(*start, 0) << "csrc12";
	start++;
	ASSERT_EQ(*start, 0) << "csrc13";
	start++;
	ASSERT_EQ(*start, 0) << "csrc14";
	start++;
	ASSERT_EQ(*start, 2) << "csrc21";
	start++;
	ASSERT_EQ(*start, 0) << "csrc22";
	start++;
	ASSERT_EQ(*start, 0) << "csrc23";
	start++;
	ASSERT_EQ(*start, 0) << "csrc24";
	start++;
	start += 2;
	start += 2;
	start += 4;
	ASSERT_EQ(*start, 'a')<<"payload"; //fails!!!!=/

	Rtp secondPacket(start2);
	
	
	
	ASSERT_EQ(firstPacket.getVersion(), secondPacket.getVersion())<<"version";
	ASSERT_EQ(firstPacket.getCSRCcount(), secondPacket.getCSRCcount())<<"Count";
	ASSERT_EQ(firstPacket.getCSRC()[0], secondPacket.getCSRC()[0]) << "CSRC_0";
	ASSERT_EQ(firstPacket.getCSRC()[1], secondPacket.getCSRC()[1]) << "CSRC_1";
	ASSERT_EQ(firstPacket.getSeqNum(), secondPacket.getSeqNum()) << "SeqNum";
	ASSERT_EQ(firstPacket.getPayloadType(), secondPacket.getPayloadType()) << "type";
	ASSERT_EQ(firstPacket.getTimestamp(), secondPacket.getTimestamp()) << "Timestamp";
	ASSERT_EQ(firstPacket.getSSRC(), secondPacket.getSSRC()) << "SSRC";
	ASSERT_EQ(*firstPacket.getHeaderExtension(), *secondPacket.getHeaderExtension()) << "Header Extension";
	ASSERT_EQ(*firstPacket.getPayload(), *secondPacket.getPayload()) << "payload";
	//*/
		
	
}

using namespace rtcp;

TEST(RtcpTest, version) {
	Rtcp rtcpPacket;
	ASSERT_EQ(rtcpPacket.getVersion(), 2);
	rtcpPacket.setVersion(3);
	ASSERT_EQ(rtcpPacket.getVersion(), 3);
}

TEST(RtcpTest, payload) {
	Rtcp rtcpPacket;
	ASSERT_EQ(rtcpPacket.getPayload(), 0);
	rtcpPacket.setPayload(rtcpPacket.SenderReport);
	ASSERT_EQ(rtcpPacket.getPayload(), rtcpPacket.SenderReport);
}

TEST(RtcpTest, padding ) {
	Rtcp rtcpPacket;
	rtcpPacket.setPadding(false);
	ASSERT_FALSE(rtcpPacket.getPadding());
	rtcpPacket.setPadding(true);
	ASSERT_TRUE(rtcpPacket.getPadding());
}


TEST(RtcpTest, reportCount) {
	Rtcp rtcpPacket;
	rtcpPacket.setReportCount(5);
	ASSERT_EQ(rtcpPacket.getReportCount(), 5);	
}


TEST(RtcpTest, senderReportFormation) {
	Rtcp rtcpPacket1;
	rtcpPacket1.setVersion(2);
	rtcpPacket1.setPayload(rtcpPacket1.SenderReport);
	rtcpPacket1.setHeaderSSRC(1001);
	rtcpPacket1.setReportCount(0);
	rtcpPacket1.setHeaderLength(6);
	rtcpPacket1.senderReport.rtpTimestamp = 50;
	rtcpPacket1.senderReport.ntpFractionTimestamp = 100;
	rtcpPacket1.senderReport.ntpSecondsTimestamp = 150;
	rtcpPacket1.senderReport.octetCount = 32;
	rtcpPacket1.senderReport.packetCount = 1;


	Rtcp rtcpPacket2;
	std::shared_ptr<uint8_t> prestart = rtcpPacket1.createRtcpPacket();
	uint8_t* start = prestart.get();
	rtcpPacket2.setRtcpPacket(start);


	ASSERT_EQ(rtcpPacket1.getVersion(), rtcpPacket2.getVersion());
	ASSERT_EQ(rtcpPacket1.getPayload(), rtcpPacket2.getPayload());
	ASSERT_EQ(rtcpPacket1.getsHeaderSSRC(), rtcpPacket2.getsHeaderSSRC());
	ASSERT_EQ(rtcpPacket1.getHeaderLength(), rtcpPacket2.getHeaderLength());
	ASSERT_EQ(rtcpPacket1.getReportCount(), rtcpPacket2.getReportCount());
	ASSERT_EQ(rtcpPacket1.senderReport.rtpTimestamp, rtcpPacket2.senderReport.rtpTimestamp);
	ASSERT_EQ(rtcpPacket1.senderReport.ntpFractionTimestamp, rtcpPacket2.senderReport.ntpFractionTimestamp);
	ASSERT_EQ(rtcpPacket1.senderReport.ntpSecondsTimestamp, rtcpPacket2.senderReport.ntpSecondsTimestamp);
	ASSERT_EQ(rtcpPacket1.senderReport.octetCount, rtcpPacket2.senderReport.octetCount);
	ASSERT_EQ(rtcpPacket1.senderReport.ntpFractionTimestamp, rtcpPacket2.senderReport.ntpFractionTimestamp);
	ASSERT_EQ(rtcpPacket1.senderReport.packetCount, rtcpPacket2.senderReport.packetCount);

}

TEST(RtcpTest, recieverReportFormation) {
	Rtcp rtcpPacket1;
	rtcpPacket1.setVersion(2);
	rtcpPacket1.setPayload(rtcpPacket1.ReceiverReport);
	rtcpPacket1.setHeaderSSRC(1001);
	rtcpPacket1.setReportCount(0);
	rtcpPacket1.setHeaderLength(19);
	rtcpPacket1.resetReportBlock();
	
	rtcp::Rtcp::reportBlock rb;
	rb.delaySinceLSR = 100;
	rb.fractionLost = 0;
	rb.highestSeqNum = 10;
	rb.interarrivalJitter = 1000;
	rb.lastSR = 10000;
	rb.packetsLost = 100000;
	rb.ssrc = 1000000;
	rtcpPacket1.addReportBlock(rb);
	rtcp::Rtcp::reportBlock rb1;
	rb1.delaySinceLSR = 101;
	rb1.fractionLost = 1;
	rb1.highestSeqNum = 11;
	rb1.interarrivalJitter = 1001;
	rb1.lastSR = 10001;
	rb1.packetsLost = 100001;
	rb1.ssrc = 1000001;
	rtcpPacket1.addReportBlock(rb1);
	rtcp::Rtcp::reportBlock rb2;
	rb2.delaySinceLSR = 102;
	rb2.fractionLost = 2;
	rb2.highestSeqNum = 12;
	rb2.interarrivalJitter = 1002;
	rb2.lastSR = 10002;
	rb2.packetsLost = 100002;
	rb2.ssrc = 1000002;
	rtcpPacket1.addReportBlock(rb2);
	rtcp::Rtcp::reportBlock rb3;
	rb1.delaySinceLSR = 103;
	rb1.fractionLost = 3;
	rb1.highestSeqNum = 13;
	rb1.interarrivalJitter = 1003;
	rb1.lastSR = 10003;
	rb1.packetsLost = 100003;
	rb1.ssrc = 1000003;
	rtcpPacket1.addReportBlock(rb3);
	rtcp::Rtcp::reportBlock rb4;
	rb1.delaySinceLSR = 104;
	rb1.fractionLost = 4;
	rb1.highestSeqNum = 14;
	rb1.interarrivalJitter = 1004;
	rb1.lastSR = 10004;
	rb1.packetsLost = 100004;
	rb1.ssrc = 1000004;
	rtcpPacket1.addReportBlock(rb4);
	rtcp::Rtcp::reportBlock rb5;
	rb1.delaySinceLSR = 105;
	rb1.fractionLost = 5;
	rb1.highestSeqNum = 15;
	rb1.interarrivalJitter = 1005;
	rb1.lastSR = 10005;
	rb1.packetsLost = 100005;
	rb1.ssrc = 1000005;
	rtcpPacket1.addReportBlock(rb5);
	
	Rtcp rtcpPacket2;
	std::shared_ptr<uint8_t> prestart = rtcpPacket1.createRtcpPacket();
	uint8_t* start = prestart.get();
	rtcpPacket2.setRtcpPacket(start);

	ASSERT_EQ(rtcpPacket1.getVersion(), rtcpPacket2.getVersion());
	ASSERT_EQ(rtcpPacket1.getPayload(), rtcpPacket2.getPayload());
	ASSERT_EQ(rtcpPacket1.getsHeaderSSRC(), rtcpPacket2.getsHeaderSSRC());
	ASSERT_EQ(rtcpPacket1.getHeaderLength(), rtcpPacket2.getHeaderLength());
	ASSERT_EQ(rtcpPacket1.getReportCount(), rtcpPacket2.getReportCount());
	std::vector <rtcp::Rtcp::reportBlock> blocks1 = rtcpPacket1.getReportBlocks();
	std::vector <rtcp::Rtcp::reportBlock> blocks2 = rtcpPacket1.getReportBlocks();
	ASSERT_EQ(blocks1.size(), blocks2.size());
	for (int i = 0; i < blocks1.size(); i++) {
		ASSERT_EQ(blocks1[i].delaySinceLSR, blocks2[i].delaySinceLSR);
		ASSERT_EQ(blocks1[i].fractionLost, blocks2[i].fractionLost);
		ASSERT_EQ(blocks1[i].highestSeqNum, blocks2[i].highestSeqNum);
		ASSERT_EQ(blocks1[i].interarrivalJitter, blocks2[i].interarrivalJitter);
		ASSERT_EQ(blocks1[i].lastSR, blocks2[i].lastSR);
		ASSERT_EQ(blocks1[i].packetsLost, blocks2[i].packetsLost);
		ASSERT_EQ(blocks1[i].ssrc, blocks2[i].ssrc);
	}
}

TEST(RtcpTest, sourceDescriptionFormation) {
	Rtcp rtcpPacket1;
	rtcpPacket1.setVersion(2);
	rtcpPacket1.setPayload(rtcpPacket1.SourceDescription);
	rtcpPacket1.setHeaderSSRC(1001);
	rtcpPacket1.setReportCount(0);
	rtcpPacket1.setHeaderLength(100);
	rtcpPacket1.resetSdesItems();
	
	rtcp::Rtcp::sdesItem i(1, "cnamecname");
	rtcpPacket1.addSdesItem(i);
	rtcp::Rtcp::sdesItem i2(2, "Nikita1234");
	rtcpPacket1.addSdesItem(i2);
	rtcp::Rtcp::sdesItem i3(3, "ehe@google");
	rtcpPacket1.addSdesItem(i3);
	rtcp::Rtcp::sdesItem i4(4, "012345679");
	rtcpPacket1.addSdesItem(i4);
	rtcp::Rtcp::sdesItem i5(5, "qwertyqwertyqwerty");
	rtcpPacket1.addSdesItem(i5);
	rtcp::Rtcp::sdesItem i6(6, "qwertyqwertyqwerty");
	rtcpPacket1.addSdesItem(i6);


	std::shared_ptr<uint8_t> prestart = rtcpPacket1.createRtcpPacket();
	uint8_t* start = prestart.get();

	
	Rtcp rtcpPacket2;
	rtcpPacket2.setRtcpPacket(start);

	ASSERT_EQ(rtcpPacket1.getVersion(), rtcpPacket2.getVersion());
	ASSERT_EQ(rtcpPacket1.getPayload(), rtcpPacket2.getPayload());
	ASSERT_EQ(rtcpPacket1.getsHeaderSSRC(), rtcpPacket2.getsHeaderSSRC()) << "HeadSSRC";
	ASSERT_EQ(rtcpPacket1.getHeaderLength(), rtcpPacket2.getHeaderLength());
	ASSERT_EQ(rtcpPacket1.getReportCount(), rtcpPacket2.getReportCount());

	std::vector <rtcp::Rtcp::sdesItem> blocks1 = rtcpPacket1.getSdesItems();
	std::vector <rtcp::Rtcp::sdesItem> blocks2 = rtcpPacket1.getSdesItems();
	ASSERT_EQ(blocks1.size(), blocks2.size());
	for (int i = 0; i < blocks1.size(); i++) {
		ASSERT_EQ(blocks1[i].item, blocks2[i].item);
		ASSERT_EQ(blocks1[i].rtpSdesType, blocks2[i].rtpSdesType);
		ASSERT_EQ(blocks1[i].itemLength, blocks2[i].itemLength);
	}
	

}

TEST(RtcpTest, goodbyeFormation) {
	Rtcp rtcpPacket1;
	rtcpPacket1.setVersion(2);
	rtcpPacket1.setPayload(rtcpPacket1.Goodbye);
	rtcpPacket1.setHeaderSSRC(1001);
	rtcpPacket1.setReportCount(0);
	rtcpPacket1.setHeaderLength(100);
	rtcpPacket1.resetLeavers();
	rtcpPacket1.addLeaver(10001);
	rtcpPacket1.addLeaver(10002);
	rtcpPacket1.addLeaver(10003);
	rtcpPacket1.addLeaver(10004);

	std::shared_ptr<uint8_t> prestart = rtcpPacket1.createRtcpPacket();
	uint8_t* start = prestart.get();

	Rtcp rtcpPacket2;
	rtcpPacket2.setRtcpPacket(start);


	ASSERT_EQ(rtcpPacket1.getVersion(), rtcpPacket2.getVersion());
	ASSERT_EQ(rtcpPacket1.getPayload(), rtcpPacket2.getPayload());
	ASSERT_EQ(rtcpPacket1.getsHeaderSSRC(), rtcpPacket2.getsHeaderSSRC()) << "HeadSSRC";
	ASSERT_EQ(rtcpPacket1.getHeaderLength(), rtcpPacket2.getHeaderLength());
	ASSERT_EQ(rtcpPacket1.getReportCount(), rtcpPacket2.getReportCount());


	ASSERT_EQ(rtcpPacket1.getLeavers(), rtcpPacket2.getLeavers());
}


//control block;


TEST(control_block, outSocket) {
	Rtp firstPacket;
	firstPacket.setMarker(true);
	firstPacket.setExtension(true);
	firstPacket.setExtensionNum(200);
	firstPacket.setExtensionLength(1);
	uint8_t headerExt = 'c';
	firstPacket.setHeaderExtension(&headerExt);
	firstPacket.addCSRC(1);
	firstPacket.addCSRC(2);
	firstPacket.setSeqNum(42);
	firstPacket.setPayloadType(firstPacket.PCMU);
	firstPacket.setTimestamp(0);
	firstPacket.setSSRC(1);
	uint8_t data = 'a';
	firstPacket.setPayload(&data);
	firstPacket.setSizeofPayload(1);
	

	controlBlock cBlock;
	std::string ip = "127.0.0.1";
	short port = 30000;
	std::shared_ptr<boost::asio::ip::udp::socket> outS = cBlock.createOutputSocket(ip, port);
	std::shared_ptr<boost::asio::ip::udp::socket> inS = cBlock.createInputSocket(port);
	std::vector<char> outArray(1024);
	std::shared_ptr<uint8_t> prestart = firstPacket.createRtpPacket();
	memcpy(outArray.data(), prestart.get(), 32);


	boost::array<char, 1024> inArray;
	boost::asio::ip::udp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
	cBlock.sendRawData(&outArray, outS);
	size_t len = cBlock.receiveRawData(&inArray, inS);
	if (len > 0) {
		for (int i = 0; i < inArray.size(); i++) {
			ASSERT_EQ(outArray[i], inArray[i]);
		}
	}
	ASSERT_EQ(100, 100);
	Rtp secondPacket;
	secondPacket.setRtpPacket((uint8_t*)inArray.data());

	ASSERT_EQ(firstPacket.getVersion(), secondPacket.getVersion()) << "version";
	ASSERT_EQ(firstPacket.getCSRCcount(), secondPacket.getCSRCcount()) << "Count";
	ASSERT_EQ(firstPacket.getCSRC()[0], secondPacket.getCSRC()[0]) << "CSRC_0";
	ASSERT_EQ(firstPacket.getCSRC()[1], secondPacket.getCSRC()[1]) << "CSRC_1";
	ASSERT_EQ(firstPacket.getSeqNum(), secondPacket.getSeqNum()) << "SeqNum";
	ASSERT_EQ(firstPacket.getPayloadType(), secondPacket.getPayloadType()) << "type";
	ASSERT_EQ(firstPacket.getTimestamp(), secondPacket.getTimestamp()) << "Timestamp";
	ASSERT_EQ(firstPacket.getSSRC(), secondPacket.getSSRC()) << "SSRC";
	ASSERT_EQ(*firstPacket.getHeaderExtension(), *secondPacket.getHeaderExtension()) << "Header Extension";
	ASSERT_EQ(*firstPacket.getPayload(), *secondPacket.getPayload()) << "payload";
	
}









int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}