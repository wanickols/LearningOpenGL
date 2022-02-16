//docs.gl <---- Great documentation for learning openGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath)
{

    //types of shaders
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
  
    ShaderType type = ShaderType::NONE;

    std::fstream fs;
    fs.open(filepath, std::ios::in);
 
    if (fs.is_open()) {
        std::string line;
        std::stringstream ss[2];

        while (std::getline(fs, line))
        {
 

            //looking for #shader tag at top of each shader
            if (line.find("#shader") != std::string::npos)
            {
                //changes mode based on type of shader
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                //prints to a ss based on current mode
                ss[int(type)] << line << "\n";
            }
        }
        fs.close();
        return { ss[0].str(), ss[1].str() };

    }else
    {
        std::cout << "ERROR::parseShader::File Unable to Open." << "\n";
    }

    return ShaderProgramSource();

   
}

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
    
    float positions[] =
    {
        -0.5f, -0.5f,
         0.5f,  -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };
    
   
    //index buffer
    unsigned int indices[] = {
        0 ,1, 2,
        2 ,3, 0
    };

    unsigned int vbo; //ID for buffer
    //Allocates memeory on the VRAM of the graphics card for use   
    glGenBuffers(1, &vbo);  //generates a buffer and returns and id
    //binds it, draw will call whatever is bound
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //binds buffer as an array
    //sets data of buffer. 
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    //tells openGL the format of your attribute. See gldoc for each parameter  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    
    //index buffer object
    unsigned int ibo;
    glGenBuffers(1, &ibo);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //uses gl_element instead of just normal
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    //gets source shaders from file and parses them
    ShaderProgramSource source = parseShader("res/shaders/Basic.Shader");

    //Creates shader
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glLinkProgram(shader);
    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
                    
                                                        //can use nullptr because we bound it already.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

   // glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}