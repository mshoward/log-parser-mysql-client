#pragma once
/** wrapper for the honeyd_parser so the parser can be improved at will
 * without messing up the interface */
	class HoneyDLogStatement
	{
		public:
			enum t_LOGTYPE {HONEY_D, OTHER_LOG};
			enum t_ATTACK_TYPE {SQL_INJECTION, XSS, DUBIOUS_PHP, HTTP_ETC};
			t_LOGTYPE mode;
			std::string timeStamp; //DateTime
			bool validtimeStamp;
			
			std::string packetType; //packetType
			bool validpacketType;
			
			std::string sourceIP; //sourceIP -000.000.000.000
			bool validsourceIP;
			
			std::string sourceSocket; //sourceSocket
			bool validsourceSocket;
			
			std::string targetIP; //targetIP
			bool validtargetIP;
			
			std::string targetSocket;//targetSocket
			bool validtargetSocket;
			
			std::string osVersion; //Version
			bool validosVersion;
			
			std::string urlData;
			bool validurlData;
			
			
			HoneyDLogStatement();
			~HoneyDLogStatement();
			HoneyDLogStatement(std::string rawLine);
			void populateFields(std::string rawLine);
			void populateFields(std::string rawLine, t_LOGTYPE logType);
	};
