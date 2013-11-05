#ifndef PARSER_H
#define PARSER_H

#include "NetString.h"

enum NetworkChunkEnums { NCE_END, NCE_NEW_PLAYER, NCE_PLAYER, NCE_REMOVE_PLAYER, NCE_TOO_MANY_PLAYERS, NCE_PLAYER_INPUT };

struct Parser
{
	Parser();

	// Insert Serialize Methods
	// bool Serialize(Entity &) for example
	bool Serialize(/* Something */);
	bool SerializeInput(bool *, int);

	// Insert Deserialize Methods
	bool Deserialize();
	bool DeserializeInput(NetString *string, bool *out);

	void End();
	unsigned int GetStreamLength();
	NetString *GetStream();

private:
	NetString string;
};

#endif