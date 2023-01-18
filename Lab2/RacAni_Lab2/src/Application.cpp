#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include "FPSManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Random.h"

unsigned int width = 800;
unsigned int height = 800;
float aspectRatio = (float)width / height;
Camera camera(glm::vec3(0.0, 0.0, 0.8), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
double xpos, ypos;



// Test error function
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
    width = Width;
    height = Height;

    aspectRatio = ((float)width) / height;
    float xSpan = 1;
    float ySpan = 1;

    if (aspectRatio > 1) {
        xSpan *= aspectRatio;
    }
    else {
        ySpan = xSpan / aspectRatio;
    }

    camera.SetProjMatrix(glm::frustum(-1 * xSpan, xSpan, -1 * ySpan, ySpan, -0.5f, 0.5f));

    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) 
    {
        switch (key) 
        {
        case GLFW_KEY_A:
            camera.SetRotation(10.0f, 0.0f);
            break;
        case GLFW_KEY_D:
            camera.SetRotation(-10.0f, 0.0f);
            break;
        case GLFW_KEY_W:
            camera.SetRotation(0.0f, -10.0f);
            break;
        case GLFW_KEY_S:
            camera.SetRotation(0.0f, 10.0f);
            break;
        }
    }

}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
 
    window = glfwCreateWindow(width, height, "Lab2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    unsigned int activeCount = 0;



    FPSManager FPSManagerObject(window, 60, 1.0, "Lab2", &activeCount);
  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);
    

    if (glewInit() != GLEW_OK)
        std::cout << "Error: glewInit() failed!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    framebuffer_size_callback(window, width, height);
    glClearColor(0.0f, 0.f, 0.05f, 1.0f);

    Random rng;
    rng.Init();

    std::string texPath = "res/textures/NecoArcThumbsUp.png";

    Texture snow(texPath);

    ParticleSystem system(10000, snow);

    std::vector<ParticleSystem*> systems;
    systems.push_back(&system);

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float step = 0.5;
    float emitCount = 0;
    float generateSpeed = 400.0f;

    while (!glfwWindowShouldClose(window))
    {
        float timeStep = (float)FPSManagerObject.enforceFPS(true);
        glClear(GL_COLOR_BUFFER_BIT);

        
        system.Update(timeStep);

        position += glm::vec3(step * timeStep, 0.0f, 0.0f);
        if (position.x >= 1.0f || position.x <= -1.0f)
            step *= -1;;

        emitCount += generateSpeed * timeStep;
        for (auto& p : system.particlePool)
        {
            if (!p.active) continue;
            float lifeProgress = (p.lifeRemaining / p.lifetime);
            p.color.g = lifeProgress * 0.8f;
            p.velocity += glm::vec3(-p.velocity.x * (3 - lifeProgress), 0.0f, 0.0f) * timeStep;
        }
        while ((int)emitCount > 0)
        {
            glm::vec3 partPos = position + glm::vec3(rng.Float(-0.4f, 0.4f), rng.Float(-0.03f, 0.03f), (rng.Float(-0.4f, 0.4f)));
            glm::vec3 partVel = glm::vec3(rng.Float(-0.05f, 0.05f), rng.Float(0.1f, 0.05f), rng.Float(-0.05f, 0.05f));
            glm::vec3 partCol = glm::vec3(1.0f, 0.9f, 0.0f);
            system.Emit({ partPos, partVel, partCol, rng.Float(3.0f, 10.0f) });
            emitCount--;
        }
        system.Update(timeStep , glm::vec3(0.0f, 0.05f, 0.0f));

        activeCount = 0;
        activeCount += system.activeParticles;


        system.OnRender(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    

    glfwTerminate();

    return 0;
}