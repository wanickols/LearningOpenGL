#pragma once
class IndexBuffer
{
private:
	unsigned int m_RendererID; //keeps track for every type of object we create.  
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	//accessors
	inline unsigned int getCount() const { return m_Count; };

};

