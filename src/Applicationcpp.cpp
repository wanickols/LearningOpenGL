#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//docs.gl <---- Great documentation for learning openGL

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
         0.5f, -0.5f,
         0.5f,  0.5f
    };
    
    unsigned int buffer; //ID for buffer
    //Allocates memeory on the VRAM of the graphics card for use   
    glGenBuffers(1, &buffer);  //generates a buffer and returns and id
    //binds it, draw will call whatever is bound
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //binds buffer as an array
    //             type         , size. data,   usage case
    //                                          static means drawn once updated often, draw means were gonna draw it
    glBufferData(GL_ARRAY_BUFFER, 6, positions, GL_STATIC_DRAW);
    

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