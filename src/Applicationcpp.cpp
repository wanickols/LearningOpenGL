//docs.gl <---- Great documentation for learning openGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    //source must exist at this point, no temp strings
    const char* src = source.c_str();
    //specify shader source code
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id); //compile

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //returns length of shader log
        //allows to dynamically allocate length to char on stack because char[length] isn't allowed (because c++)
        char* message = (char*)alloca(length * sizeof(char));
        //uses maxLength to set length of string, by using length we can make it as long as it needs to be. Not sure why you wouldn't do this. THen it returns how long it was back to length, shouldn't change for us. 
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " 
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id; 
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    //getting indexes for new 
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    //TODO add an assert to check if compilation completed. 
    

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;




    /* Initialize the library */
    if (!glfwInit())
        return -1;

  

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Inits Glew;
    //Glew interacts finds Graphics API to get the openGL functions and assigns them the right name. This is not a library, more like a linker between the graphics and your IDE. 
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::APPLICATION::GLEWINIT NOT OK \n";
        return -1;
    }

    //prints GL version to console
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    float positions[6] =
    {
        -0.5f, -0.5f,
         0.f,  0.5f,
         0.5f,  -0.5f
    };
    
   
    unsigned int vbo; //ID for buffer
    //Allocates memeory on the VRAM of the graphics card for use   
    glGenBuffers(1, &vbo);  //generates a buffer and returns and id
    //binds it, draw will call whatever is bound
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //binds buffer as an array

    //sets data of buffer. 
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    //tells openGL the format of your attribute. See gldoc for each parameter  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    

    //Shader
    std::string vertexShader =
        "#version 330 core \n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        " gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core \n"
        "\n"
        "out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0); \n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glLinkProgram(shader);
    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
                    

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}