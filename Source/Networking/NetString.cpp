#include "NetString.h"

NetString::NetString()
	: buffer(NULL), bufferSize(0), bufferLength(0), bufferIndex(0)
{
	ClearBuffer();
}

NetString::NetString(unsigned char *buf, int length)
	: buffer(buf), bufferSize(length), bufferLength(length), bufferIndex(0)
{
}

NetString::~NetString()
{
	if (buffer != NULL)
		delete [] buffer;	
}



bool NetString::Seek(int i)
{
	if (i < 0 || i >= bufferLength)
		return false;

	bufferIndex = i;
	return true;
}



void NetString::ClearBuffer()
{
	if (buffer != NULL)
		delete [] buffer;

	bufferSize = NetStringBufferStartLength;
	buffer = new unsigned char[bufferSize];

	bufferLength = 0;
	bufferIndex = 0;
}

bool NetString::AddUChars(unsigned char *buf, int length)
{
	int temp = bufferIndex;

	for (int i = 0; i < length; ++i)
	{
		if (!WriteUChar(buf[i]))
		{
			bufferIndex = temp;
			return false;
		}
	}

	bufferIndex = temp;
	return true;
}

bool NetString::WriteBool(bool value)
{
	return WriteUChar(value);
}

bool NetString::WriteUChar(unsigned char value)
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

bool NetString::WriteInt(int value)
{
	if (bufferIndex >= bufferSize - 5)
	{
		if (!Expand(std::max(bufferSize * 2, 8)))
			return false;
	}
	
	SDLNet_Write32(value, buffer + bufferIndex);
	bufferIndex += 4;
	bufferLength += 4;
	return true;
}

bool NetString::WriteFloat(float value)
{
	if (bufferIndex >= bufferSize - 5)
	{
		if (!Expand(std::max(bufferSize * 2, 8)))
			return false;
	}

	unsigned int temp;
	memcpy(&temp, &value, std::min(sizeof(float), sizeof(unsigned int)));

	SDLNet_Write32(temp, buffer + bufferIndex);
	bufferIndex += 4;
	bufferLength += 4;

	return true;
}

bool NetString::ReadBool(bool &out)
{
	if (bufferIndex >= bufferLength - 1)
		return false;

	out = buffer[bufferIndex++];
	return true;
}

bool NetString::ReadUChar(unsigned char &out)
{
	if (bufferIndex >= bufferLength - 1)
		return false;

	out = buffer[bufferIndex++];
	return true;
}

bool NetString::ReadInt(int &out)
{
	if (bufferIndex >= bufferLength - 4)
		return false;

	out = SDLNet_Read32(buffer + bufferIndex);
	bufferIndex += 4;
	return true;
}

bool NetString::ReadFloat(float &out)
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

bool NetString::Expand(int newSize)
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

bool NetString::operator+=(const NetString &other)
{
	if (other.BufferLength() + bufferLength > bufferSize)
	{
		if (!Expand((other.BufferLength() + bufferLength) * 1.2))
			return false;
	}

	unsigned char *otherBuffer = other.Buffer();
	for (int i = 0; i < other.BufferLength(); i++)
		buffer[bufferIndex++] = otherBuffer[i];

	bufferLength += other.BufferLength();

	return true;
}

std::ostream &operator<<(std::ostream &stream, NetString &netstring)
{
	int size = netstring.BufferLength();
	unsigned char *b = netstring.Buffer();

	for (int i = 0; i < size; i++)
	{
		stream << (int)b[i];
	}
	return stream;
}