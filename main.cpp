#include <iostream>

#include <glad/glad.h>
#define GLFW_DLL
#include <GL/glfw3.h>

#include <numeric>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "gl_image.hpp"

namespace GL
{
    // ------------------
    float delta_time, last_time;
    //float mouse_sensitivity = 0.05f;
    
    float zoom = 1, cx, cy;
    int itr = 100;
    
    class GLFWResourceGuard
    {
    public:
        GLFWResourceGuard()
        {
            glfwInit();
        }
        ~GLFWResourceGuard()
        {
            glfwTerminate();
        }
    };
    // -----------------
    // IO callbacks
    void framebufferSizeCallback(GLFWwindow * const window, const int width, const int height);
    void scrollCallback(GLFWwindow* window, const double xoffset, const double yoffset);
    void processInput(GLFWwindow *window);
};

int main()
{
    using namespace GL;
    // ------------
    // glfw : init and config
    GLFWResourceGuard rg;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    auto window = glfwCreateWindow(1000, 800,
                                   "Mandelbrot", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create glfw window\n";
        return 0;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GL::framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // ------------------------------
    // setting up mouse input mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    
    unsigned mandelbrot_texture;
    try {
        mandelbrot_texture = GLTextureGenerator::generateTexture1D("resources/pal.png");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, mandelbrot_texture);
    }
    catch (std::runtime_error &re) {
        std::cout << "Texture wasn't loaded because:\n" << re.what();
    }
    
    mandelbrot_shader.use();
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    
    glBindVertexArray(VAO);
    
    int w, h;
    
    cx = -0.5, cy = 0; /* Camera position and zoom value */
    
    while(!glfwWindowShouldClose(window)) {
        
        GL::processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwGetWindowSize(window, &w, &h);
        
        mandelbrot_shader.setVec2("screen_size", static_cast <double>(w),
                                                  static_cast <double>(h));
        mandelbrot_shader.setFloat("screen_ratio", static_cast <double>(w) /
                                                   static_cast <double>(h));
        mandelbrot_shader.setVec2("center", cx, cy);
        mandelbrot_shader.setFloat("zoom", zoom);
        mandelbrot_shader.setInt("itr", itr);
        
        glm::mat4 model, view, projection;
        view = glm::scale(view, {0.5f, 0.5f, 0.5f});
        
        mandelbrot_shader.setMVP(model, view, projection);
        
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

void GL::scrollCallback(GLFWwindow *window, const double xoffset, const double yoffset)
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
        cy += 0.01f / zoom;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cx -= 0.01f / zoom;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cy -= 0.01f / zoom;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cx += 0.01f / zoom;
    }
    
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
        itr < std::numeric_limits <int>::max()) {
        ++itr;
    }
    
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
        itr > 50) {
        --itr;
    }
}