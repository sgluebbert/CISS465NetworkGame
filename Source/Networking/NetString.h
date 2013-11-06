#ifndef NETSTRING_H
#define NETSTRING_H

#include "SDL_net.h"
#include <iostream>

static const int NetStringBufferStartLength = 1;

struct NetString
{
	NetString();
	NetString(unsigned char *buf, int length);
	~NetString();

	bool Seek(int);

	void ClearBuffer();
	bool AddUChars(unsigned char *buf, int length);

	// These functions return true if it was able to read or write
	bool WriteBool(bool);
	bool WriteUChar(unsigned char);
	bool WriteInt(int);
	bool WriteFloat(float);

	bool ReadBool(bool &);
	bool ReadUChar(unsigned char &);
	bool ReadInt(int &);
	bool ReadFloat(float &);

	int BufferLength() const { return bufferLength; }
	unsigned char *Buffer() const { return buffer; }

	bool operator+=(const NetString &other);

private:
	bool Expand(int);

	unsigned char * buffer;
	int bufferLength;
	int bufferSize;
	int bufferIndex;
};

std::ostream &operator<<(std::ostream &stream, NetString &);

#endif