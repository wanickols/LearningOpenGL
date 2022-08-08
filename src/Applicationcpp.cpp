//docs.gl <---- Great documentation for learning openGL
#include "pch.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"



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

    //framerate
    glfwSwapInterval(1); //1 takes whatever vsync has

    /* Inits Glew
       Glew interacts finds Graphics API to get the openGL functionsand assigns them the right name.
       This is not a library, more like a linker between the graphics and your IDE. 
    */
    if (glewInit() != GLEW_OK)
    {
        cout << "ERROR::APPLICATION::GLEWINIT NOT OK \n";
        return -1;
    }

    //prints GL version to console
    cout << glGetString(GL_VERSION) << endl;
    
    //Debug
    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(errorOccurredGL, NULL);
    {
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

    
        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        
        //make a layout
        VertexBufferLayout layout;
        layout.Push<float>(2);

        //binds buffer to a layout
        va.AddBuffer(vb, layout);

        //data, count
        IndexBuffer ib(indices, 6);


        Shader shader("res/shaders/Basic.Shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.5f, 1.0f, 1.0f);
        //SetColor

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        shader.Unbind();
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        float r = 0.0f;
        float increment = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();

            shader.SetUniform4f("u_Color", r, 0.5, 1.0, 1.0);

            va.Bind();
            ib.Bind();

            //can use nullptr because we bound it already.
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            if (r > 1.0 || r < 0.0f)
                increment = -increment;

            r += increment;
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}