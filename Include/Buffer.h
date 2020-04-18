#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <memory>
#include <string>

using namespace std;

class Buffer
{
public:
	Buffer(const size_t size);
	Buffer(const char* buff);
	~Buffer();

	template<typename T> void write(T input);
	template<typename T> void writePtr(T input, size_t size);
	template<typename T> T read();
	template<typename T> void readPtr(T ptr, size_t size);
	void writeString(const string& str);
	string readString();

	const char* get();
	size_t tell();
	void seek(const size_t position);

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

#endif