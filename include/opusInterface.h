#pragma once
#include "codecInterface.h"
#include "unordered_map"
#include <../build/deps/include/opus.h>
#include <boost/any.hpp>

class opusInterface //: public codecInterface
{
public:
	opusInterface(std::unordered_map<std::string, int> sets);
	~opusInterface();

	int createEncoder(std::unordered_map<std::string, int> sets);

	int createDecoder(std::unordered_map<std::string, int> sets);

	int destroyEncoder();
	int destroyDecoder();

	int codecFunction(std::vector<boost::any> arguments);

	virtual int encoderFunction(std::vector<boost::any> arguments);

	virtual int decoderFunction(std::vector<boost::any> arguments);


	std::vector<uint8_t> code(std::vector<uint8_t>);

	std::vector<uint8_t> decode(std::vector<uint8_t>);

	std::unordered_map<std::string, int> settings;
	OpusEncoder* encoder;
	OpusDecoder* decoder;
	int* error_encoder;
	int* error_decoder;
};