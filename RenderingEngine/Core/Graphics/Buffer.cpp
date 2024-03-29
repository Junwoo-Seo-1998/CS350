/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Buffer.cpp
Purpose: Abstracted classes for vertex buffer object and index buffer with helper classes
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include"Buffer.h"


DataAndLayoutLocation::DataAndLayoutLocation(unsigned layout_location, DataType data, bool normalize)
	:m_DataType(data), m_LayoutLocation(layout_location), m_Size(0), m_ElementCount(0), m_Offset(0), m_Normalize(normalize)
{
	//compute size of data 
	switch (data)
	{
	case DataType::Bool:	m_Size = 4;						 break;//for padding
	case DataType::Int:		m_Size = sizeof(int);			 break;
	case DataType::Int2:	m_Size = sizeof(int) * 2;		 break;
	case DataType::Int3:	m_Size = sizeof(int) * 3;		 break;
	case DataType::Int4:	m_Size = sizeof(int) * 4;		 break;
	case DataType::Float:   m_Size = sizeof(float);			 break;
	case DataType::Float2:	m_Size = sizeof(float) * 2;		 break;
	case DataType::Float3:	m_Size = sizeof(float) * 3;		 break;
	case DataType::Float4:	m_Size = sizeof(float) * 4;		 break;
	case DataType::Mat3:	m_Size = sizeof(float) * 3 * 3;	 break;
	case DataType::Mat4:	m_Size = sizeof(float) * 4 * 4;	 break;
	}

	//get element count 
	switch (data)
	{
	case DataType::Bool:	m_ElementCount = 1; 	break;
	case DataType::Int:		m_ElementCount = 1;		break;
	case DataType::Int2:	m_ElementCount = 2;		break;
	case DataType::Int3:	m_ElementCount = 3;		break;
	case DataType::Int4:	m_ElementCount = 4;		break;
	case DataType::Float:	m_ElementCount = 1;		break;
	case DataType::Float2:	m_ElementCount = 2;		break;
	case DataType::Float3:	m_ElementCount = 3;		break;
	case DataType::Float4:	m_ElementCount = 4;		break;
	case DataType::Mat3:	m_ElementCount = 3 * 3;	break;
	case DataType::Mat4:	m_ElementCount = 4 * 4;	break;
	}
}

unsigned DataAndLayoutLocation::ShaderDataTypeToOpenGLBaseType() const
{
	switch (m_DataType)
	{
	case DataType::Bool:		return GL_BOOL;
	case DataType::Int:			return GL_INT;
	case DataType::Int2:		return GL_INT;
	case DataType::Int3:		return GL_INT;
	case DataType::Int4:		return GL_INT;
	case DataType::Float:		return GL_FLOAT;
	case DataType::Float2:		return GL_FLOAT;
	case DataType::Float3:		return GL_FLOAT;
	case DataType::Float4:		return GL_FLOAT;
	case DataType::Mat3:		return GL_FLOAT;
	case DataType::Mat4:		return GL_FLOAT;
	}
	return 0;
}

DescribedData::DescribedData(const std::initializer_list<DataAndLayoutLocation>& described_data)
	:m_DescribedData(described_data)
{
	//compute offset and stride
	unsigned int offset = 0;
	m_Stride = 0;
	for (auto& data : m_DescribedData)
	{
		data.m_Offset = offset;
		offset += data.m_Size;
		m_Stride += data.m_Size;
	}
}


VertexBuffer::~VertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_Buffer);
}


void VertexBuffer::SetData(int size, const void* data, unsigned offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::SetDataTypes(const DescribedData& data)
{
	m_DescribedData = data;
}

VertexBuffer::VertexBuffer(int byte_size)
	:m_Buffer(0), m_DescribedData()
{
	CreateBuffer(byte_size, nullptr);
}

void VertexBuffer::CreateBuffer(int size, const void* data)
{
	glCreateBuffers(1, &m_Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(int byte_size)
{
	return std::shared_ptr<VertexBuffer>(new VertexBuffer{ byte_size });
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
}

void VertexBuffer::BindToVertexArray() const
{
	//ENGINE_ASSERT(m_DescribedData.GetSize() > 0, "There should be at least one described data. \
	//\nExample:\nvertex_buffer->SetDataTypes({ {0, DataType::Float3} });//layout location, data type");
	Bind();
	for (const auto& description : m_DescribedData)
	{
		glEnableVertexAttribArray(description.m_LayoutLocation);
		const void* pointer = static_cast<char*>(nullptr) + description.m_Offset;
		glVertexAttribPointer(description.m_LayoutLocation, static_cast<GLint>(description.m_ElementCount),
			description.ShaderDataTypeToOpenGLBaseType(),
			description.m_Normalize ? GL_TRUE : GL_FALSE,
			static_cast<GLsizei>(m_DescribedData.GetStride()),
			pointer);
	}
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
}

std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(int byte_size)
{
	return std::shared_ptr<IndexBuffer>(new IndexBuffer{ byte_size });
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
}

void IndexBuffer::BindToVertexArray() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(int size, const void* data, unsigned offset)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

IndexBuffer::IndexBuffer(int byte_size)
	:m_Buffer(0)
{
	CreateBuffer(byte_size, nullptr);
}

void IndexBuffer::CreateBuffer(unsigned size, const void* data)
{
	glGenBuffers(1, &m_Buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

