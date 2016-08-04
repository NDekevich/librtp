#include <stdint.h>
#include <unordered_map>

class codecInterface
{
	public :
		codecInterface();
		virtual ~codecInterface();


		virtual void initialize();
		virtual int getCodecCode();

		virtual uint8_t code(uint8_t rawData);
		virtual uint8_t decode(uint8_t codedData);
		 


		std::unordered_map < std::string, int > settings;
		const int codecCode;


};