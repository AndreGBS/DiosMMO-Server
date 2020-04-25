#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <memory>
#include <string.h>

using namespace std;

class Buffer
{
public:
	Buffer(const size_t size);
	Buffer(const char* buff);
	~Buffer();

	inline void reset(const char* buff);
	template<typename T> inline void write(T input);
	template<typename T> inline void writePtr(T input, size_t size);
	template<typename T> inline T read();
	template<typename T> inline void readPtr(T ptr, size_t size);
	inline void writeString(const string& str);
	inline string readString();

	inline const char* get();
	inline size_t tell();
	inline void seek(const size_t position);

private:
	bool readOnly;
	size_t size;
	size_t position = 0;
	char* buff = nullptr;
};

template<typename T> void Buffer::write(T input)
{
	if(readOnly)
		return;

	memcpy(buff+position, &input, sizeof(T));
	position += sizeof(input);
}

template<typename T> void Buffer::writePtr(T input, const size_t)
{
	if(readOnly)
		return;
	
	memcpy(buff+position, input, size);
	position += size;
}

template<typename T> T Buffer::read()
{
	T ret;
	memcpy(&ret, buff+position, sizeof(T));
	position += sizeof(T);
	return ret;
}

template<typename T> void Buffer::readPtr(T ptr, const size_t size)
{
	memcpy(ptr, buff+position, size);
	position += size;
}

void Buffer::reset(const char* buff)
{
	if(!readOnly && buff != nullptr)
		delete [] buff;
		
	buff = const_cast<char*>(buff);
	position = 0;
}
const char * Buffer::get()
{
	return buff;
}

void Buffer::seek(const size_t position)
{
	this->position = position;
}

void Buffer::writeString(const string& str)
{
	if(readOnly)
		return;

	memcpy(buff+position, str.c_str(), str.size());
	position += str.size();
}

string Buffer::readString()
{
	string output;
	while(buff[position] != 0)
	{
		output += buff[position];
		++position;
	}
	++position;

	return output;
}

size_t Buffer::tell()
{
	return position;
}

#endif