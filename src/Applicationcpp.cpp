//docs.gl <---- Great documentation for learning openGL
#include "pch.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"
#include "Texture.h"



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
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f,  -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };


        //index buffer
        unsigned int indices[] = {
            0 ,1, 2,
            2 ,3, 0
        };

        //Determines how transparency works
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        
        //make a layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        //binds buffer to a layout
        va.AddBuffer(vb, layout);

        //data, count
        IndexBuffer ib(indices, 6);


        Shader shader("res/shaders/Basic.Shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 0.8f, 0.5f, 1.0f, 1.0f);
        //SetColor
        
        //Texturs
        Texture texture("res/textures/block.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //Unbinding
        ib.Unbind();
        va.Unbind();
        shader.Unbind();
        vb.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            /* Render here */
            renderer.Clear();

            shader.Bind();

            //shader.SetUniform4f("u_Color", r, 0.5, 1.0, 1.0);
            shader.SetUniform1i("u_Texture", 0);

            renderer.Draw(va, ib, shader);

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