#pragma once
#include "codecInterface.h"
#include "unordered_map"
//#include "../build/deps/include/opus.h"
#include <opus.h>
#define _Debug
#include <boost/any.hpp>

//TODO Opus decoding + interface + sender Audio playback + documentation

class opusInterface //: public codecInterface
{
public:
	opusInterface();
	opusInterface(std::unordered_map<std::string, int> sets);
	~opusInterface();

	int createEncoder(std::unordered_map<std::string, int> sets);

	int createDecoder(std::unordered_map<std::string, int> sets);

	int destroyEncoder();
	int destroyDecoder();

	int codecFunction(std::vector<boost::any> arguments);

	virtual int encoderFunction(std::vector<boost::any> arguments);

	virtual int decoderFunction(std::vector<boost::any> arguments);


	int code(std::vector<uint8_t>* data);

	int decode(std::vector<uint8_t>* data);

	std::unordered_map<std::string, int> settings;
	OpusEncoder* encoder;
	OpusDecoder* decoder;
	int* error_encoder;
	int* error_decoder;
};