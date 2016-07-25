// testsRtp.cpp : Defines the entry point for the console application.
//

#include "controlBlock.h"

#include <gtest/gtest.h>
#include <fstream>

//RTP	
using namespace rtp;

TEST(RtpTest, versionTest)
	{
		Rtp testPacket;
		ASSERT_EQ(testPacket.getVersion(), 2) << "Starting";
		testPacket.setVersion(1);
		ASSERT_EQ(testPacket.getVersion(), 1) << "Changed";
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
		ASSERT_EQ(testPacket.getPayloadType(), testPacket.PCMU);
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
		firstPacket.setExtensionNum(123321);
		std::vector<uint8_t> headerExt = { 'a','b','c','d','e','f' };
		firstPacket.setExtensionLength(6);
		firstPacket.setHeaderExtension(headerExt);
		firstPacket.addCSRC(320013);
		firstPacket.addCSRC(124312);
		firstPacket.setSeqNum(980321);
		firstPacket.setPayloadType(firstPacket.PCMU);
		firstPacket.setTimestamp(121533);
		firstPacket.setSSRC(135315);
		std::vector<uint8_t> data = { '1','2','3','4','5','6','7','8','9' };
		firstPacket.setPayload(data);
		firstPacket.setSizeofPayload(9);



		std::shared_ptr<std::vector<uint8_t>> startpre = firstPacket.createRtpPacket();

		std::vector<uint8_t> start2 = *startpre;
		uint8_t* start = start2.data();



		/*

		ASSERT_EQ(*start, 146) << "vvpxcc";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 128) << "mpl";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 42) << "seqnum1";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "seqnum2";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 8) << "ts1";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "ts2";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "ts3";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "ts4";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 1) << "ssrc1";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "ssrc2";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "ssrc3";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "ssrc4";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 1) << "csrc11";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "csrc12";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "csrc13";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "csrc14";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 2) << "csrc21";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "csrc22";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "csrc23";
		std::cout << (int)(*start);// << std::endl;
		start++;
		ASSERT_EQ(*start, 0) << "csrc24";
		std::cout << (int)(*start) << std::endl;
		start++;
		ASSERT_EQ(*start, 3);
		start++;
		ASSERT_EQ(*start, 0);
		start++;
		ASSERT_EQ(*start, 2);
		start++;
		ASSERT_EQ(*start, 0);
		start++;

		start += 2;
		start += 4;
		//(*start, 'a') << "payload";
		*/
		Rtp secondPacket(start2);


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

//RTCP
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

TEST(RtcpTest, padding) {
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
	rtcpPacket1.setVersion(2);//0b10000000
	rtcpPacket1.setPayload(rtcpPacket1.SenderReport);
	rtcpPacket1.setHeaderSSRC(1);
	rtcpPacket1.setReportCount(0);
	rtcpPacket1.setHeaderLength(6);
	rtcpPacket1.senderReport.rtpTimestamp = 3;
	rtcpPacket1.senderReport.ntpFractionTimestamp = 1;
	rtcpPacket1.senderReport.ntpSecondsTimestamp = 2;
	rtcpPacket1.senderReport.octetCount = 31;
	rtcpPacket1.senderReport.packetCount = 4;


	Rtcp rtcpPacket2;
	std::shared_ptr<std::vector<uint8_t>> prestart = rtcpPacket1.createRtcpPacket();
	std::vector<uint8_t> start2 = *prestart;
	uint8_t* start = start2.data();
	rtcpPacket2.setRtcpPacket(start2);

	/*
	ASSERT_EQ(*start, 0b10000000) << "vvpxcc";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 200) << "pt";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 6) << "length1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "length2";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 1) << "ssrc1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ssrc2";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ssrc3";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ssrc4";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 2) << "ntp1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ntp2";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ntp3";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ntp4";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 1) << "sntp1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "sntp2";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "sntp13";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "sntp14";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 3) << "rtp1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "rtp2";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "rtp3";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "rtp4";
	std::cout << (int)(*start) << std::endl;
	start++;
	*/

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

	Rtcp rtcpPacket2;
	std::shared_ptr<std::vector<uint8_t>> prestart = rtcpPacket1.createRtcpPacket();
	std::vector<uint8_t> start2 = *prestart;
	uint8_t* start = start2.data();
	rtcpPacket2.setRtcpPacket(start2);










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
	rtcpPacket1.setHeaderSSRC(1);
	rtcpPacket1.setReportCount(0);
	rtcpPacket1.setHeaderLength(100);
	rtcpPacket1.resetSdesItems();

	rtcp::Rtcp::sdesItem i(1, "cnamecname");
	rtcpPacket1.addSdesItem(i);
	rtcp::Rtcp::sdesItem i2(2, "Nikita1234");
	rtcpPacket1.addSdesItem(i2);
	rtcp::Rtcp::sdesItem i3(3, "ehe@google");
	rtcpPacket1.addSdesItem(i3);
	rtcp::Rtcp::sdesItem i4(4, "0123456789");
	rtcpPacket1.addSdesItem(i4);
	rtcp::Rtcp::sdesItem i5(5, "qwertyqwertyqwerty");
	rtcpPacket1.addSdesItem(i5);
	rtcp::Rtcp::sdesItem i6(6, "qwertyqwertyqwerty");
	rtcpPacket1.addSdesItem(i6);





	Rtcp rtcpPacket2;
	std::shared_ptr<std::vector<uint8_t>> prestart = rtcpPacket1.createRtcpPacket();
	std::vector<uint8_t> start2 = *prestart;
	rtcpPacket2.setRtcpPacket(start2);

	uint8_t* start = start2.data();
/*
	ASSERT_EQ(*start, 0b10000000+6) << "vvpxcc";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 202) << "pt";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 17) << "length1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "length2";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 1) << "ssrc1";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ssrc2";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ssrc3";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 0) << "ssrc4";
	std::cout << (int)(*start) << std::endl;
	start++;


	ASSERT_EQ(*start, 1) << "type";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 10) << "iL";
	std::cout << (int)(*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 'c') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'n') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'a') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'm') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'e') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'c') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'n') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'a') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'm') << "data1";
	std::cout << (*start) << " ";// << std::endl;
	start++;
	ASSERT_EQ(*start, 'e') << "data1";
	std::cout << (*start) << " "<< std::endl;
	start++;

	ASSERT_EQ(*start, 2) << "type";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 10) << "iL";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 'N') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'i') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'k') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'i') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 't') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'a') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '1') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '2') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '3') << "data1";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '4') << "data1";
	std::cout << (*start) << std::endl;
	start++;

	//ASSERT_TRUE(false);
	ASSERT_EQ(*start, 3) << "type";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 10) << "iL";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, 'e') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'h') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'e') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '@') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'g') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'o') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'o') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'g') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'l') << "data2";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 'e') << "data2";
	std::cout << (*start) << std::endl;
	start++;

	ASSERT_EQ(*start, 4) << "type";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, 10) << "iL";
	std::cout << (int)(*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '0') << "data";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '1') << "data";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, '2') << "data";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '3') << "data";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, '4') << "data";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '5') << "data";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '6') << "data";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, '7') << "data";
	std::cout << (*start);// << std::endl;
	start++;
	ASSERT_EQ(*start, '8') << "data";
	std::cout << (*start) << std::endl;
	start++;
	ASSERT_EQ(*start, '9') << "data";
	std::cout << (*start);// << std::endl;
	start++;
	*/
	
	
	
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

	std::shared_ptr<std::vector<uint8_t>> prestart = rtcpPacket1.createRtcpPacket();
	std::vector<uint8_t> start = *prestart;
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
	controlBlock cBlock;
	std::string ip = "127.0.0.1";
	short port = 30000;
	std::shared_ptr<boost::asio::ip::udp::socket> outS = cBlock.createOutputSocket(ip, port);
	cBlock.createRtpVal(outS);


	std::shared_ptr<boost::asio::ip::udp::socket> inS = cBlock.createInputSocket(port);
	cBlock.createRtpVal(inS);

	(*cBlock.socketRtpMap[outS]).setMarker(true);
	(*cBlock.socketRtpMap[outS]).setExtension(true);
	(*cBlock.socketRtpMap[outS]).setExtensionNum(200);
	(*cBlock.socketRtpMap[outS]).setExtensionLength(5);
	std::vector<uint8_t> headerExt = { '1','2','3','4','5' };
	(*cBlock.socketRtpMap[outS]).setHeaderExtension(headerExt);
	(*cBlock.socketRtpMap[outS]).addCSRC(1);
	(*cBlock.socketRtpMap[outS]).addCSRC(2);
	(*cBlock.socketRtpMap[outS]).setSeqNum(42);
	(*cBlock.socketRtpMap[outS]).setPayloadType((*cBlock.socketRtpMap[outS]).PCMU);
	(*cBlock.socketRtpMap[outS]).setTimestamp(0);
	(*cBlock.socketRtpMap[outS]).setSSRC(1);
	std::vector<uint8_t> dataP = { 'a','b','c','d'};
	(*cBlock.socketRtpMap[outS]).setSizeofPayload(4);
	(*cBlock.socketRtpMap[outS]).setPayload(dataP);


	std::vector<uint8_t> v = *(*cBlock.socketRtpMap[outS]).createRtpPacket();
	std::vector<char> outArray (v.begin(),v.end());


	cBlock.sendRtpData(dataP, outS);
	//cBlock.sendRawData(&outArray, outS);

	std::vector<uint8_t> inArray;
	inArray.resize(2048);
	boost::asio::ip::udp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

	cBlock.receiveRtpData(inS);
	rtp:Rtp secondPacket(*cBlock.socketRtpMap[inS]);
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getVersion(), secondPacket.getVersion()) << "version";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getCSRCcount(), secondPacket.getCSRCcount()) << "Count";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getCSRC()[0], secondPacket.getCSRC()[0]) << "CSRC_0";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getCSRC()[1], secondPacket.getCSRC()[1]) << "CSRC_1";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getSeqNum(), secondPacket.getSeqNum()) << "SeqNum";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getPayloadType(), secondPacket.getPayloadType()) << "type";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getTimestamp(), secondPacket.getTimestamp()) << "Timestamp";
	ASSERT_EQ((*cBlock.socketRtpMap[outS]).getSSRC(), secondPacket.getSSRC()) << "SSRC";
	ASSERT_EQ(*(*cBlock.socketRtpMap[outS]).getHeaderExtension(), *secondPacket.getHeaderExtension()) << "Header Extension";
	ASSERT_EQ(*(*cBlock.socketRtpMap[outS]).getPayload(), *secondPacket.getPayload()) << "payload";


	/*
	int len = cBlock.receiveRawData(&inArray, inS);
	if (len > 0) {
		for (int i = 0; i < len; i++) {
			//ASSERT_EQ(outArray[i], inArray[i]);
			//std::cout << "wtf\n";
		}
		Rtp secondPacket;
		v.clear();
		v.insert(v.begin(), inArray.begin(), inArray.begin() + len);
		secondPacket.setRtpPacket(v);

		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getVersion(),	 secondPacket.getVersion()) << "version";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getCSRCcount(), secondPacket.getCSRCcount()) << "Count";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getCSRC()[0], secondPacket.getCSRC()[0]) << "CSRC_0";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getCSRC()[1], secondPacket.getCSRC()[1]) << "CSRC_1";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getSeqNum(), secondPacket.getSeqNum()) << "SeqNum";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getPayloadType(), secondPacket.getPayloadType()) << "type";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getTimestamp(), secondPacket.getTimestamp()) << "Timestamp";
		ASSERT_EQ((*cBlock.socketRtpMap[outS]).getSSRC(), secondPacket.getSSRC()) << "SSRC";
		ASSERT_EQ(*(*cBlock.socketRtpMap[outS]).getHeaderExtension(), *secondPacket.getHeaderExtension()) << "Header Extension";
		ASSERT_EQ(*(*cBlock.socketRtpMap[outS]).getPayload(), *secondPacket.getPayload()) << "payload";
			
	}
	*/

		
}










int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::cin.get();
}