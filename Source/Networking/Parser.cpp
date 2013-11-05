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

bool Parser::SerializeInput(bool *in)
{
	string.ClearBuffer();
	if (!string.WriteUChar(NCE_PLAYER_INPUT)) return false;
	if (!string.WriteBool(in[0])) return false;
	if (!string.WriteBool(in[1])) return false;
	if (!string.WriteBool(in[2])) return false;
	if (!string.WriteBool(in[3])) return false;
	return true;
}

bool Parser::DeserializeInput(NetString *string, bool *out)
{
	unsigned char type;
	string->ReadUChar(type);
	if (type != NCE_PLAYER_INPUT)
		return false;
	if (!string->ReadBool(out[0])) return false;
	if (!string->ReadBool(out[1])) return false;
	if (!string->ReadBool(out[2])) return false;
	if (!string->ReadBool(out[3])) return false;
	return true;
}

void Parser::End()
{
	unsigned char end = 0;
	if (!string.ReadUChar(end))
		string.WriteUChar(NCE_END);
	else if (end != NCE_END)
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