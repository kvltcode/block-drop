#pragma once

class IndexBuffer
{
private:
	unsigned int mIndexBufferID;
	unsigned int mCount;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	unsigned int getCount() const { return mCount; }
};