#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <csignal>

#include "renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main()
{
  //* Used to initialize glfw
  glfwInit();

  //* Tell GLFW that we're using major version of openGL 3: 3.--
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //* Tell GLFW that we're using minor version of openGL 3: -.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //* Load functions into openGL from GLFW
  //* Core contains modern functions and compatability contains both the modern and outdated functions
  // And since we only care about the modern ones, we use the core profile
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //* the compiler will know if it's apple and if it is, it does the following:
  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  //* This creates a window object, and the first two arguments are width and height while the third is the name
  //* and the fourth is if we want it to be fullscreen while the last we don't care about.
  GLFWwindow* window = glfwCreateWindow(500,500,"New Window",NULL,NULL);
  //* error checking for if the window doesn't load
  if(window == NULL){
    std::cerr << "Window failed to create\n";
    glfwTerminate();
    return -1;
  }
  //* Adds the window to the current context, I assume it's like a context in scheme type of thing.
  glfwMakeContextCurrent(window);

  gladLoadGL();

  //* The area of the window that we want OpenGL to render in.
  //* bottom left corner of our window, coordinates 0,0 to the top right corner of our window: 500,500
  glViewport(0,0,500,500);

  glClearColor(0.4f,0.2f,0.7f,1.0f);

  //* does buffer stuff, will learn more about later.
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);

  //* These are the vertices in the positions of the triangle(s)
  float positions[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f,
  };

  //* This is now an index buffer
  //* Specifies the indices of the vertices in the positions array that make up each triangle. The first
  //* Triangle is made up of vertices 0, 1, and 2 and the second is of 3, 2, 0 and we don't need to re-define a vertex
  //! ALL INDEX BUFFERS MUST BE UNSIGNED INTS NOT REGULAR INTS
  unsigned int indices[]{
    0, 1, 2,
    3, 2, 0
  };

  //* This creates a vertex array object and it encapsulates the state of all the attributes.
  //* This includes the buffers they are sourced from, the attribute configurations such as data
  //* type and number of components and the associated array buffer bindings.
  //? Also don't know if I only need this for apple or if I need this for my desktop too.
  unsigned int vao;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  VertexArray va;

  //* Make a vertex buffer and don't need to bind it because it's automatically bound in the constructor.
  VertexBuffer vb(positions, 4 * 2 * sizeof(float));  

  VertexBufferLayout layout;
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  //* Generates an index buffer
  IndexBuffer ib(indices, 6);

  Shader shader("res/shaders/vertex.vert", "res/shaders/fragment.frag");
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.8f, 0.2f, 0.5f, 1.0f);

  float r = 0.0;
  float increment = 0.01f; 

  //* We just need to bind these things below
  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  shader.Unbind();

  //* This checks at the start of each loop if GLFW has instructed the window to close
  while(!glfwWindowShouldClose(window))
  {
    /* render heare */
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    shader.Bind();
    shader.SetUniform4f("u_Color", r, 0.9f,1-r,1.0f);
    
    va.Bind();
    ib.Bind();

    //* We change it to glDrawElements to use an index buffer
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if(r > 1.0f){
      increment = -0.01f;
    }else if(r < 0.0f){
      increment = 0.01f;
    }

    r += increment;

    //? Swap front and back bufers
    GLCall(glfwSwapBuffers(window));

    //* Process events to the window and shi
    GLCall(glfwPollEvents());
  }

  //* Destroy the window
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}