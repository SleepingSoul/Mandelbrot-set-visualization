/* Copiright 2018 Tihran Katolikian*/

#include <glad/glad.h>
#define GLFW_DLL
#include <GL/glfw3.h>

#include <cstdlib>

#include "shader.hpp"

namespace GL
{
    // ------------------
    float delta_time, last_time;
    float mouse_sensitivity = 0.5f;
    
    // ------------------
    // uniforms required for user interaction with
    // rendering process
    float zoom = 1,
          cx = -0.5,
          cy = 0;
    
    // ------------------
    // number of Mandelbrot iterations.
    // default value is 100, but user can change it
    int iterations = 100;
    
    // -----------------
    // IO callbacks
    void framebufferSizeCallback(GLFWwindow * const window, const int width, const int height);
    void scrollCallback(GLFWwindow * const window, const double xoffset, const double yoffset);
    void processInput(GLFWwindow *window);
};

int main()
{
    using namespace GL;

    // ------------
    // Greet a user! :)
    std::cout << "Hello there!\nHope you enjoy the spectacular view "
                 "of Mandelbrot set :) \nRegards, Tihran Katolikian.\n";

    // ------------
    // glfw : init and config
    glfwInit();
    
    // ------------
    // we ensure that glfwTerminate function
    // will be called before the program will end
    std::atexit(glfwTerminate);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    auto window = glfwCreateWindow(1000, 800,
                                   "Mandelbrot", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create glfw window\n";
        return 0;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // ------------------------------
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast <GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD\n";
        return 0;
    }
    
    Shader mandelbrot_shader("mandelbrot.vs", "mandelbrot.fs");
    
    const float square[] = {
         1,  1, 0,
         1, -1, 0,
        -1,  1, 0,
        -1,  1, 0,
         1, -1, 0,
        -1, -1, 0
    };
    
    unsigned VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          reinterpret_cast <void *>(0));
    glEnableVertexAttribArray(0);
    
    mandelbrot_shader.use();
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    
    glBindVertexArray(VAO);
    
    // ---------------
    // thoose variables are needed to get the sizes of
    // the window in the main loop
    int w, h;
    
    while(!glfwWindowShouldClose(window)) {
        auto time = glfwGetTime();
        delta_time = time - last_time;
        last_time = time;
        
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwGetWindowSize(window, &w, &h);
        
        mandelbrot_shader.setVec2("screen_size", static_cast <float>(w),
                                                 static_cast <float>(h));
        mandelbrot_shader.setFloat("screen_ratio", static_cast <float>(w) /
                                                   static_cast <float>(h));
        mandelbrot_shader.setVec2("center", cx, cy);
        mandelbrot_shader.setFloat("zoom", zoom);
        mandelbrot_shader.setInt("iterations", iterations);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

void GL::framebufferSizeCallback(GLFWwindow * const window, const int width,
                                 const int height)
{
    glViewport(0, 0, width, height);
}

void GL::scrollCallback(GLFWwindow * const window, const double xoffset,
                        const double yoffset)
{
    zoom += yoffset * 0.1 * zoom;
	if(zoom < 0.1) {
		zoom = 0.1;
	}
}

void GL::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cy += mouse_sensitivity * delta_time / zoom;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cx -= mouse_sensitivity * delta_time / zoom;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cy -= mouse_sensitivity * delta_time / zoom;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cx += mouse_sensitivity * delta_time / zoom;
    }
    
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
        iterations < std::numeric_limits <int>::max()) {
        ++iterations;
    }
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
        iterations > 50) {
        --iterations;
    }
}