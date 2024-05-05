#include "VertexBuffer.h"

#include "renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
  //* Open GL will make a singular buffer
  GLCall(glGenBuffers(1, &m_rendererId));
  //* creates a buffer of memory which says that it's an array and pass in the id of the buffer as well
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
  //* This will be us resizing the buffer and starting to use it
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer(){
  GLCall(glDeleteBuffers(1, &m_rendererId));
}


void VertexBuffer::Bind() const{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
}

void VertexBuffer::Unbind() const{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}