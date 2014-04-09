#pragma once

//sets strings that if examined will give instructions to call
//populateFields(string someString) at some point
HoneyDLogStatement::HoneyDLogStatement()
{
	hdparser::honeyd_parser hdp;
	timeStamp = hdp.getTimeStamp();
	packetType = hdp.getPacketType();
	sourceIP = hdp.getSourceIP();
	targetIP = hdp.getTargetIP();
	targetSocket = hdp.getTargetSocket();
	osVersion = hdp.getOsVersion();
}

//nothing to free
HoneyDLogStatement::~HoneyDLogStatement()
{
}


//sets fields
HoneyDLogStatement::HoneyDLogStatement(std::string rawLine)
{
	hdparser::honeyd_parser hdp(rawLine);
	timeStamp = hdp.getTimeStamp();
	packetType = hdp.getPacketType();
	sourceIP = hdp.getSourceIP();
	sourceSocket = hdp.getSourceSocket();
	targetIP = hdp.getTargetIP();
	targetSocket = hdp.getTargetSocket();
	osVersion = hdp.getOsVersion();
}

//also sets fields
void HoneyDLogStatement::populateFields(std::string rawLine)
{
	hdparser::honeyd_parser hdp(rawLine);
	timeStamp = hdp.getTimeStamp();
	validtimeStamp = hdp.getValidTimeStamp();
	if (!validtimeStamp)
		timeStamp = "E";
	packetType = hdp.getPacketType();
	validpacketType = hdp.getValidPacketType();
	if (!validpacketType)
		packetType = "E";
	sourceIP = hdp.getSourceIP();
	validsourceIP = hdp.getValidSourceIP();
	if (!validsourceIP)
		sourceIP = "E";
	sourceSocket = hdp.getSourceSocket();
	validsourceSocket = hdp.getValidSourceSocket();
	if(!validsourceSocket)
		sourceSocket = "E";
	targetIP = hdp.getTargetIP();
	validtargetIP = hdp.getValidTargetIP();
	if (!validtargetIP)
		targetIP = "E";
	targetSocket = hdp.getTargetSocket();
	validtargetSocket = hdp.getValidTargetSocket();
	if (!validtargetSocket)
		targetSocket = "E";
	osVersion = hdp.getOsVersion();
	validosVersion = hdp.getValidOsVersion();
}

void HoneyDLogStatement::populateFields(std::string rawLine, t_LOGTYPE logType)
{
	if (logType == HONEY_D)
	{
		populateFields(rawLine);
	}
	else
	{
		
	}
}

