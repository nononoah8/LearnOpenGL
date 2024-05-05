class IndexBuffer {
private:
  unsigned int m_rendererId;
  unsigned int m_count;
public:
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  inline unsigned int GetCount() {
    return m_count;
  }
};