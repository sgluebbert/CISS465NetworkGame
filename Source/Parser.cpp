#include "Parser.h"

Parser::Parser()
	: buffer(NULL), bufferSize(0), bufferLength(0), bufferIndex(0)
{
	ClearBuffer();
}

Parser::Parser(unsigned char *buf, int length)
	: buffer(buf), bufferSize(length), bufferLength(length), bufferIndex(0)
{
}

void Parser::ClearBuffer()
{
	if (buffer != NULL)
		delete [] buffer;

	bufferSize = ParserBufferStartLength;
	buffer = new unsigned char[bufferSize];

	bufferLength = 0;
	bufferIndex = 0;
}

bool Parser::WriteBool(bool value)
{
	return WriteUChar(value);
}

bool Parser::WriteUChar(unsigned char value)
{
	if (bufferIndex >= bufferSize - 2)
	{
		if (!Expand(bufferSize * 2))
			return false;
	}
	
	buffer[bufferIndex++] = value;
	bufferLength++;
	return true;
}

bool Parser::WriteInt(int value)
{
	if (bufferIndex >= bufferSize - 5)
	{
		if (!Expand(bufferSize * 2))
			return false;
	}
	
	SDLNet_Write32(value, buffer + bufferIndex);
	bufferIndex += 4;
	bufferLength += 4;
	return true;
}

bool Parser::WriteFloat(float value)
{
	if (bufferIndex >= bufferSize - 5)
	{
		if (!Expand(bufferSize * 2))
			return false;
	}
	
	unsigned int temp;
	memcpy(&temp, &value, sizeof(float) < sizeof(unsigned int) ? sizeof(float) : sizeof(unsigned int));

	SDLNet_Write32(temp, buffer + bufferIndex);
	bufferIndex += 4;
	bufferLength += 4;
	return true;
}

bool Parser::ReadBool(bool &out)
{
	if (bufferIndex >= bufferLength - 1)
		return false;

	out = buffer[bufferIndex++];
	return true;
}

bool Parser::ReadUChar(unsigned char &out)
{
	if (bufferIndex >= bufferLength - 1)
		return false;

	out = buffer[bufferIndex++];
	return true;
}

bool Parser::ReadInt(int &out)
{
	if (bufferIndex >= bufferLength - 4)
		return false;

	out = SDLNet_Read32(buffer + bufferIndex);
	bufferIndex += 4;
	return true;
}

bool Parser::ReadFloat(float &out)
{
	if (bufferIndex >= bufferLength - 4)
		return false;

    unsigned int temp = SDLNet_Read32(buffer + bufferIndex);
    float t;
	bufferIndex += 4;
    memcpy(&t, &temp, sizeof(float));
    out = t;
	return true;
}

bool Parser::Expand(int newSize)
{
	if (newSize <= 0)
		newSize = 1;

	unsigned char *temp = new unsigned char[newSize];
	if (temp == NULL)
		return false;

	for (int i = 0; i < bufferSize; i++)
		temp[i] = buffer[i];

	delete [] buffer;
	buffer = temp;
	bufferSize = newSize;

	return true;
}

std::ostream &operator<<(std::ostream &stream, Parser &parser)
{
	int size = parser.BufferLength();
	unsigned char *b = parser.Buffer();

	for (int i = 0; i < size; i++)
	{
		stream << (int)b[i];
	}
	return stream;
}