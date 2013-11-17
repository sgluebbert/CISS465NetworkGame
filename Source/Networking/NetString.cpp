#include "NetString.h"

NetString::NetString()
	: buffer(NULL), bufferSize(0), bufferLength(0), bufferIndex(0), fullCount1(0), fullCount2(0), count(false), whole(false)
{
	ClearBuffer();
}

NetString::NetString(unsigned char *buf, int length)
	: buffer(buf), bufferSize(length), bufferLength(length), bufferIndex(0), fullCount1(0), fullCount2(0), count(false), whole(false)
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

	fullCount1 = 0;
	fullCount2 = 0;
	count = false;
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

	return true;
}

bool NetString::AddNetworkBuffer(unsigned char *buf, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (buf[i] == 0)
			count = true;
		else if (count && (buf[i] != 255 || buf[i] != 254))
		{
			count = false;
			fullCount1 = 0;
			fullCount2 = 0;
		}

		if (buf[i] != 255)
			fullCount1 = 0;
		else
			fullCount1++;

		if (fullCount1 == 2 && i != length - 1)
		{
			fullCount1 = 0;
			fullCount2 = 0;
			if (buf[i + 1] == 0)
			{
				// the 0 is faked; ignore it
				WriteUChar(buf[i]);
				i++;
				continue;
			}
			else if (buf[i + 1] == 255)
			{
				// frame start
				ClearBuffer();
				i++;
				continue;
			}
		}

		if (buf[i] != 254)
			fullCount2 = 0;
		else
			fullCount2++;

		if (fullCount2 == 2 && i != length - 1)
		{
			fullCount1 = 0;
			fullCount2 = 0;
			if (buf[i + 1] == 0)
			{
				// the 0 is faked; ignore it
				WriteUChar(buf[i]);
				i++;
				continue;
			}
			else if (buf[i + 1] == 254)
			{
				// frame end
				bufferLength -= 2;
				whole = true;
				return true;
			}
		}

		WriteUChar(buf[i]);
	}

	return false;
}

NetString *NetString::ToNetworkBuffer()
{
	NetString *stream = new NetString();

	// Encode Start Sequence
	stream->WriteUChar(0);
	stream->WriteUChar(255);
	stream->WriteUChar(255);
	stream->WriteUChar(255);

	bool _count = false;
	int _fullCount1 = 0, _fullCount2 = 0;
	for (int i = 0; i < bufferLength; i++)
	{
		stream->WriteUChar(buffer[i]);

		if (buffer[i] == 0)
			_count = true;
		else if (_count && (buffer[i] != 255 || buffer[i] != 254))
		{
			_count = false;
			_fullCount1 = 0;
			_fullCount2 = 0;
		}

		if (buffer[i] != 255)
			_fullCount1 = 0;
		else
			_fullCount1++;

		if (_fullCount1 >= 2)
		{
			stream->WriteUChar(0);
			_fullCount1 = 0;
		}

		if (buffer[i] != 254)
			_fullCount2 = 0;
		else
			_fullCount2++;

		if (_fullCount2 >= 2)
		{
			stream->WriteUChar(0);
			_fullCount2 = 0;
		}
	}

	// Encode End Sequence
	stream->WriteUChar(0);
	stream->WriteUChar(254);
	stream->WriteUChar(254);
	stream->WriteUChar(254);

	return stream;
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

bool NetString::WriteString(std::string &text)
{
	if (bufferIndex >= bufferSize - text.length() - 1)
	{
		if (!Expand(std::max(bufferSize * 2, (int)text.length() * 2)))
			return false;
	}

	for (int i = 0; i < text.length(); i++)
	{
		if (!WriteUChar(text[i]))
			return false;
	}

	if (!WriteUChar(0))
		return false;

	return true;
}

bool NetString::ReadBool(bool &out)
{
	if (bufferIndex >= bufferLength)
		return false;

	out = buffer[bufferIndex++];
	return true;
}

bool NetString::ReadUChar(unsigned char &out)
{
	if (bufferIndex >= bufferLength)
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

bool NetString::ReadString(std::string &text)
{
	unsigned char temp;
	int i = 0, bufferSize = 20;
	char *buffer = new char[bufferSize];
	while (true)
	{
		if (!ReadUChar(temp))
			return false;
		buffer[i++] = (char)temp;
		if (temp == 0)
			break;

		if (i == bufferSize)
		{
			char *t = new char[bufferSize * 2];
			memcpy(t, buffer, bufferSize);
			delete [] buffer;
			buffer = t;
		}
	}

	text = std::string(buffer);
	delete [] buffer;
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