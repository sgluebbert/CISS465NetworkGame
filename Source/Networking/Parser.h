#ifndef PARSER_H
#define PARSER_H

#include "NetString.h"

enum NetworkChunkEnums { NCE_END, NCE_NEW_CONNECTION, NCE_NEW_PLAYER, NCE_PLAYER, NCE_PLAYER_EXITED, NCE_REMOVE_PLAYER, NCE_TOO_MANY_PLAYERS, NCE_ALREADY_JOINED, NCE_LOBBY, NCE_PLAYER_INPUT, NCE_PLAYER_GREETING, NCE_LOOKUP_PLAYER, NCE_MAP, NCE_START_GAME_TIMER, NCE_LOBBY_PLAYER_SYNC, NCE_LOBBY_STATE_SYNC };
enum GameServerEnums { GSE_WAITING, GSE_LOBBY_COUNTDOWN, GSE_TRANSITION, GSE_GAME_COUNTDOWN, GSE_GAME, GSE_GAME_ENDED };

std::ostream &operator<<(std::ostream &stream, GameServerEnums e);

struct Parser
{
	Parser();

	// Insert Serialize Methods
	// bool Serialize(Entity &) for example
	bool Serialize(/* Something */);
	bool SerializeInput(bool *, int size);

	// Insert Deserialize Methods
	bool Deserialize();
	bool DeserializeInput(NetString *string, bool *out, int size);

	void End();
	unsigned int GetStreamLength();
	NetString *GetStream();

private:
	NetString string;
};

#endif