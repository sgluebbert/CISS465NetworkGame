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

bool Parser::DeserializeInput(NetString *string, bool *out)
{
	unsigned char type;
	string->ReadUChar(type);
	if (type != NCE_PLAYER_INPUT)
		return false;
	string->ReadBool(out[0]);
	string->ReadBool(out[1]);
	string->ReadBool(out[2]);
	string->ReadBool(out[3]);
	return true;
}

void Parser::End()
{
	unsigned char end;
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