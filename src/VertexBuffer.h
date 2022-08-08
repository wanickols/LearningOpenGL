#pragma once
class VertexBuffer
{
private:
	unsigned int m_RendererID; //keeps track for every type of object we create.  
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	
};

