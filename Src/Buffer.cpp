#include <cstdint>
#include <memory>
#include "Buffer.h"
#include <type_traits>

using namespace std;

Buffer::Buffer(const size_t size)
	: readOnly(false)
	, size(size)
	, buff(new char[size])

{
}
Buffer::Buffer(const char* buff)
	: readOnly(true)
	, buff(const_cast<char*>(buff))
{	
}


Buffer::~Buffer()
{
	if(!readOnly && buff != nullptr)
		delete [] buff;
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
