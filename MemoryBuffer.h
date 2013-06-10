#ifndef ___MEMORY_BUFFER_H___
#define ___MEMORY_BUFFER_H___

class MemoryBuffer
{
private:
	char* buffer;
	int size;
	int realSize;

public:
	MemoryBuffer(int size = 1) : size(size), realSize(size)
	{
		buffer = new char[size];
	}

	MemoryBuffer(const MemoryBuffer& memoryBuffer) : size(memoryBuffer.size), realSize(size)
	{
		buffer = new char[size];
		memcpy(buffer, memoryBuffer.buffer, size);
	}

	~MemoryBuffer()
	{
		delete [] buffer;
	}

	MemoryBuffer& operator=(const MemoryBuffer& memoryBuffer)
	{
		delete [] buffer;
		size = realSize = memoryBuffer.size;
		buffer = new char[size];
		memcpy(buffer, memoryBuffer.buffer, size);
		return *this;
	}

	template <typename T>
	T* GetPointer()
	{
		return (T*)buffer;
	}

	void Resize(int size)
	{
		if(realSize < size)
		{
			delete [] buffer;
			realSize = size;
			buffer = new char[realSize];
		}
		this->size = size;
	}

	template <typename T>
	void Resize(int count)
	{
		Resize(count * sizeof(T));
	}

	int GetSize() const
	{
		return realSize;
	}
};

#endif
