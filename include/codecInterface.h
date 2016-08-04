#include <stdint.h>
#include <unordered_map>
#include <boost/any.hpp>



class codecInterface
{
	public :
		codecInterface();
		virtual ~codecInterface();

		virtual int createEncoder();
		virtual int createDecoder();

		virtual int destroyEncoder();
		virtual int destroyDecoder();


		virtual void initialize();
		virtual int getCodecType();

		virtual uint8_t code(uint8_t rawData);
		virtual uint8_t decode(uint8_t codedData);
		virtual int codecFunction(std::list<boost::any> arguments);


		std::unordered_map < std::string, int > settings;
		const int codecCode;


};