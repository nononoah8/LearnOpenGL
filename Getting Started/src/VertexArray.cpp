#include "VertexArray.h"
#include "renderer.h"


VertexArray::VertexArray(){
  GLCall(glGenVertexArrays(1, &m_renderer_id)); 
}
VertexArray::~VertexArray(){
  GLCall(glDeleteVertexArrays(1, &m_renderer_id)); 
}

void VertexArray::Bind() const{
  GLCall(glBindVertexArray(m_renderer_id));
}

void VertexArray::Unbind() const{
  GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
  Bind();
  vb.Bind();

  const auto& elements = layout.GetElements();

  unsigned int offset = 0;

  for(unsigned int i = 0; i < elements.size(); ++i){
    const auto& element = elements[i];
    
    //* Now we need to enable the vertex attribute
    //* Need to call glEnableVertexArray... to enable it so we can use it and so it can show
    GLCall(glEnableVertexAttribArray(i));

    //* This gets the attirbute of the buffer. It will take in the index(where to start the modified values)
    //* Then it will take the size or the number of things per vertex, then the type, then if we want it normalized
    //* normalized is basically if we have a 0-255 value and it needs to be turned into a 0-1f.
    //* Then the offset between each vertex which is 2 * sizeof(float) in this case.
    //? Then finally, the pointer is the offset to the other coordinates not posistion
    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
    offset += element.count * element.GetSize(element.type);
  }
}