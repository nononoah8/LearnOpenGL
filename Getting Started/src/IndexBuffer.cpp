#include "IndexBuffer.h"

#include "renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): m_count(count) {
  //* Open GL will make a singular buffer
  GLCall(glGenBuffers(1, &m_rendererId));
  //* creates a buffer of memory which says that it's an array and pass in the id of the buffer as well
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
  //* This will be us resizing the buffer and starting to use it
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer(){
  GLCall(glDeleteBuffers(1, &m_rendererId));
}


void IndexBuffer::Bind() const{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::Unbind() const{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}