#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <csignal>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
//* This funciton is used to check for an error in any of our functions and then print the line, the file, and the 
//* It will breakpoint whenever there is an error and quit the code and tell me where and stuff.
#define GLCall(x) do { \
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); \
} while (false)

static void GLClearError(){
  while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* fn, const char* file, int line){
  while(GLenum error = glGetError()){
    std::cout << "OpenGL error: " << error << " [" << fn << "] [" << file << "] [" << line << "]\n";
    return false;
  }
  return true;
}

static std::string ParseShader(const std::string& file){
  std::ifstream stream(file);
  std::string line;
  std::stringstream shader;

  while(getline(stream, line)){
    shader << line << '\n';
  }

  return shader.str();
}

/*
Function to Compile a shader that takes in the type of shader and the source name as a string.
Also source string MUST be alive.
If you have a function that returns a string, should make it an l-value.
*/
static unsigned int CompileShader(unsigned int type, const std::string& source){
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  
  /*
    glShaderSource sets the source code in shader to the source code in the array of strings specified by string. 
    This function takes in the id or the shader type, then the number of shaders, then a double pointer to the source
    It also takes in a double pointer to length which specifies an array of string lengths.
  */
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  GLint success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if(!success) {
    GLint length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << std::endl;
    std::cout << message << std::endl;

    glDeleteShader(id);
    return 0;
  }

  return id;
}

/*
Creates the shader using the CompileShader function.
*/
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  //* Need to provide shader source code w our shader text and need to return something for that shader so we can bind it and then use it.
  //* Need to create our program
  unsigned int program = glCreateProgram();
  //* Create vertex shader
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  //* Create fragment shader
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  //* We now need to attach these 2 shaders into our program.
  //* Can think of it like having 2 different files and attaching them to our main program.
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  //* Since the shaders have been attached to the program, we don't need the memory overhead anymore and can just delete them.
  glDeleteShader(vs);
  glDeleteShader(fs);

  //* Return the program.
  return program;
}

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
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //* Open GL will make a singular buffer and writes an id to the uint buffer.
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  //* creates a buffer of memory which says that it's an array and pass in the id of the buffer as well
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  //* This will be us resizing the buffer and starting to use it
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
  
  //* Now we need to enable the vertex attribute
  //* Need to call glEnableVertexArray... to enable it so we can use it and so it can show
  glEnableVertexAttribArray(0);

  //* This gets the attirbute of the buffer. It will take in the index(where to start the modified values)
  //* Then it will take the size or the number of things per vertex, then the type, then if we want it normalized
  //* normalized is basically if we have a 0-255 value and it needs to be turned into a 0-1f.
  //* Then the offset between each vertex which is 2 * sizeof(float) in this case.
  //? Then finally, the pointer is the offset to the other coordinates not posistion
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);

  //* Generates an index buffer
  unsigned int ibo;
  glGenBuffers(1, &ibo);
  //* creates a buffer of memory which says that it's an array and pass in the id of the buffer as well
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  //* This will be us resizing the buffer and starting to use it
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int ), indices, GL_STATIC_DRAW);

  //* This means that we're using the shader language and using version 330 and we don't need the new features so we're using 330

  std::string vertexShader = ParseShader("res/shaders/vertex.vert");
  std::string fragmentShader = ParseShader("res/shaders/fragment.frag");

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  int location = glGetUniformLocation(shader, "u_Color");
  ASSERT(location != -1); //* If we get a locaiton of -1, then the program can't find the location.
  GLCall(glUniform4f(location, 0.7f, 0.9f,0.2f,1.0f));

  float r = 0.0;
  float increment = 0.01f; 

  //* We just need to bind these things below
  // GLCall(glBindVertexArray(0));
  // GLCall(glUseProgram(0));
  // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  //* This checks at the start of each loop if GLFW has instructed the window to close
  while(!glfwWindowShouldClose(window))
  {
    /* render heare */
    glClear(GL_COLOR_BUFFER_BIT);

    GLCall(glUniform4f(location, r, 0.9f,0.2f,1.0f));

    //* Draws the triangle, but don't have an index buffer, also need shaders to see it 
    //* This will draw the current bound buffer using glBindBuffer.
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    //* We change it to glDrawElements to use an index buffer
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if(r > 1.0f){
      increment = -0.01f;
    }else if(r < 0.0f){
      increment = 0.01f;
    }

    r += increment;

    //? Swap front and back bufers
    glfwSwapBuffers(window);

    //* Process events to the window and shi
    glfwPollEvents();
  }

  //* delete the shader
  glDeleteProgram(shader);
  //* Destroy the window
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

/*
* more shader notes I think



*/