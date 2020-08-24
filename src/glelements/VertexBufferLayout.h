#pragma once
#include <glad/glad.h> 

#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalised;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> mElements;
	unsigned int mStride;
public:
	VertexBufferLayout()
		:mStride(0) {};

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		mElements.push_back({ GL_FLOAT, count, GL_FALSE });
		mStride += (VertexBufferElement::getSizeOfType(GL_FLOAT) * count);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		mStride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		mStride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;;
	}

	inline const std::vector<VertexBufferElement> getElements() const { return mElements; }
	inline unsigned int getStride() const { return mStride; };

};
