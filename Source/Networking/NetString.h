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
	bool AddNetworkBuffer(unsigned char *buf, int length);
	NetString *ToNetworkBuffer();

	// These functions return true if it was able to read or write
	bool WriteBool(bool);
	bool WriteUChar(unsigned char);
	bool WriteInt(int);
	bool WriteFloat(float);
	bool WriteString(std::string &);

	bool ReadBool(bool &);
	bool ReadUChar(unsigned char &);
	bool ReadInt(int &);
	bool ReadFloat(float &);
	bool ReadString(std::string &);

	int BufferLength() const { return bufferLength; }
	unsigned char *Buffer() const { return buffer; }
	bool IsWhole() const { return whole; }

	bool operator+=(const NetString &other);

private:
	bool Expand(int);

	NetString(const NetString &other);
	NetString &operator=(const NetString &other);
	
	unsigned char * buffer;
	int bufferLength;
	int bufferSize;
	int bufferIndex;
	bool whole;

	// These Attributes are used for the byte stuffing (keeping a memory between AddNetworkBuffers)
	int fullCount1, fullCount2;
	bool count;
};

std::ostream &operator<<(std::ostream &stream, NetString &);

#endif