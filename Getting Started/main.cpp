#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  //* This will be the positions of the vertices of the triangle
  float positions[6] = {
    -0.5f, -0.5f,
     0.0f, 0.5f,
     0.5f,-0.5f
  };

  //* This creates a vertex array object and it encapsulates the state of all the attributes.
  //* This includes the buffers they are sourced from, the attribute configurations such as data
  //* type and number of components and the associated array buffer bindings.
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);  

  //* Open GL will make a singular buffer and writes an id to the uint buffer.
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  //* creates a buffer of memory which says that it's an array and pass in the id of the buffer as well
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  //* This will be us resizing the buffer and starting to use it
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  
  //* Now we need to enable the vertex attribute
  //* Need to call glEnableVertexArray... to enable it so we can use it and so it can show
  glEnableVertexAttribArray(0);

  //* This gets the attirbute of the buffer. It will take in the index(where to start the modified values)
  //* Then it will take the size or the number of things per vertex, then the type, then if we want it normalized
  //* normalized is basically if we have a 0-255 value and it needs to be turned into a 0-1f.
  //* Then the offset between each vertex which is 2 * sizeof(float) in this case.
  //? Then finally, the pointer is the offset to the other coordinates not posistion
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);

  //* This means that we're using the shader language and using version 330 and we don't need the new features so we're using 330
  std::string vertexShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "\n"
    "void main(){\n"
    "  gl_Position = position;\n"
    "}\n";

  std::string fragmentShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;\n"
    "\n"
    "void main(){\n"
    "  color = vec4(.7, 0.1, 0.2, 1.0);\n"
    "}\n";
  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  //* This checks at the start of each loop if GLFW has instructed the window to close
  while(!glfwWindowShouldClose(window))
  {
    /* render heare */
    glClear(GL_COLOR_BUFFER_BIT);

    //* Draws the triangle, but don't have an index buffer, also need shaders to see it 
    //* This will draw the current bound buffer using glBindBuffer.
    glDrawArrays(GL_TRIANGLES, 0, 3);

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