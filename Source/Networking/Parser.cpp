#include "Parser.h"

std::ostream &operator<<(std::ostream &stream, GameServerEnums e)
{
	switch (e)
	{
		case GSE_WAITING:
			stream << "Waiting for additional players.";
			break;
		case GSE_LOBBY_COUNTDOWN:
			stream << "Lobby countdown.";
			break;
		case GSE_TRANSITION:
			stream << "Transitioning into game.";
			break;
		case GSE_GAME_COUNTDOWN:
			stream << "Game countdown.";
			break;
		case GSE_GAME:
			stream << "In game.";
			break;
		default:
			stream << "Unknown";
	}
	return stream;
}

Parser::Parser()
{
}

bool Parser::Serialize(/* Something */)
{
	// string.WriteUChar(NCE_NEW_PLAYER);
	// string += Something.Serialize();

	return true;
}

bool Parser::SerializeInput(bool * in, int size)
{
	string.ClearBuffer();
	if (!string.WriteUChar(NCE_PLAYER_INPUT)) return false;
	
	for (int i = 0; i < size; i++)
		if (!string.WriteBool(in[i])) return false;

	return true;
}

bool Parser::DeserializeInput(NetString *string, bool *out, int size)
{
	for (int i = 0; i < size; i++)
		if (!string->ReadBool(out[i])) return false;

	return true;
}

void Parser::End()
{
	unsigned char end = 0;
	string.ReadUChar(end);
	if (end != NCE_END)
		string.WriteUChar(NCE_END);
}

unsigned int Parser::GetStreamLength()
{
	return string.BufferLength();
}

NetString *Parser::GetStream()
{
	return &string;
}