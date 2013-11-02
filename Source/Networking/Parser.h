#ifndef PARSER_H
#define PARSER_H

#include "NetString.h"

enum NetworkChunkEnums { NCE_END, NCE_NEW_PLAYER, NCE_PLAYER, NCE_REMOVE_PLAYER };

struct Parser
{
	Parser();

	// Insert Serialize Methods
	// bool Serialize(Entity &) for example
	bool Serialize(/* Something */);

	// Insert Deserialize Methods
	bool Deserialize();

private:
	NetString string;
};

#endif