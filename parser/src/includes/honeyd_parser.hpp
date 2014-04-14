#pragma once
namespace hdparser
{
	/* parser backend */
	/** Someone implementing a log parser should never
	 * have to touch this.  The log statement is desigend
	 * around not having to touch this class.  Also, the
	 * searching it does to detect attacks is case insensitive.
	 * */
	 
	class honeyd_parser
	{
		private:
			/** if the first thing is an ip address, pulls it out
			 *  assigns it to out, and cuts it from in
			 *  returns success status */
			int getWord(std::string &inStr, std::string &outStr, char delim);
			bool pullIP(std::string &inStr, std::string &outStr, char delim);
			bool pullTS(std::string &inStr, std::string &outStr, char delim);
			std::string formatTS(std::string ts);
			int errorNumber;
		
		public:
			/**
			 * Honeyd Log fields:
			 * 		timeStamp
			 * 		packetType
			 * 		sourceIP
			 * 		sourceSocket
			 * 		targetIP
			 * 		targetSocket
			 * 		osVersion
			 * 
			 * Apache Log fields
			 * 		sourceIP
			 * 		timeStamp
			 * 		attackStr
			 */
			void errPrint();
			bool isGNumber(char c);
			bool isGAlpha(char c);
			bool isSymbol(char c, char symbol);
			bool isTimeStamp(std::string s);
			bool isPacketType(std::string s);
			bool isIP(std::string s);
			bool isSocket(std::string s);
			bool isOSVersion(std::string s);
			bool isTS(std::string s);
			std::string currString;
			std::string timeStamp;
			std::string packetType;
			std::string sourceIP;
			std::string sourceSocket;
			std::string targetIP;
			std::string targetSocket;
			std::string osVersion;
			std::string junkData;
			std::string attackStr;
			/** unused, currently */
			int index;
			/** internal */
			bool isGood;
			/** this is / will be set in the constructor */
			enum t_LOGTYPE {HONEY_D, APACHE, OTHER} mode;
			/** internal */
			enum t_ATTACK_TYPE {SQL_INJECTION, XSS, DUBIOUS_PHP, HTTP_ETC};
			
			/** internal -- to be used in implementing a more dynamic parser */
			enum WORDTYPE
			{
				TIMESTAMP,
				PACKETTYPE,
				IP,
				SOCKET,
				OSVERSION,
				JUNKDATA
			};
			WORDTYPE getWordType(std::string s);
			//TODO:: implement a smarter parser

			honeyd_parser();//done
			/** basic constructor, assumes honeyd log */
			honeyd_parser(std::string rawString);//done
			/** more explicit constructor, once it's improved */
			honeyd_parser(std::string rawString, t_LOGTYPE setting);
			~honeyd_parser();//nothing to do, so done by default
			
			/** wrapper function for more utility */
			void omniSetString(std::string &rawString, t_LOGTYPE which);
			
			/** called by honeyd_log_statement */
			void setString(std::string rawString);//done
			/** access.log parsing */
			void setStringApache(std::string rawString);
			std::string getTimeStamp();//done
			bool getValidTimeStamp();
			
			std::string getPacketType();//done
			bool getValidPacketType();
			
			std::string getSourceIP();//done
			bool getValidSourceIP();
			
			std::string getSourceSocket();
			bool getValidSourceSocket();
			
			std::string getTargetIP();//done
			bool getValidTargetIP();
			
			std::string getTargetSocket();//done
			bool getValidTargetSocket();
			
			std::string getOsVersion();//done
			bool getValidOsVersion();
			
			std::string getJunkData();
			t_ATTACK_TYPE getJunkDataAttackType();
			std::string toUpper(std::string strToBeUpper);
			
	};
}
