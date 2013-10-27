#ifndef PARSER_H
#define PARSER_H

#include "SDL_net.h"
#include <iostream>

static const int ParserBufferStartLength = 1;

struct Parser
{
	Parser();
	Parser(unsigned char *buf, int length);

	void ClearBuffer();

	// These functions return true if it was able to read or write
	bool WriteBool(bool);
	bool WriteUChar(unsigned char);
	bool WriteInt(int);
	bool WriteFloat(float);

	bool ReadBool(bool &);
	bool ReadUChar(unsigned char &);
	bool ReadInt(int &);
	bool ReadFloat(float &);

	int BufferLength() { return bufferLength; }
	unsigned char *Buffer() { return buffer; }


private:
	bool Expand(int);

	unsigned char * buffer;
	int bufferLength;
	int bufferSize;
	int bufferIndex;
};

std::ostream &operator<<(std::ostream &stream, Parser &parser);

#endif