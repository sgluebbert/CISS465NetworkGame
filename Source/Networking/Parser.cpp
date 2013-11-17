#include "Parser.h"

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