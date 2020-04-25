#include <cstdint>
#include <memory>
#include "Buffer.h"
#include <type_traits>
#include <string>

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

