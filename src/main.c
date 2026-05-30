#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>

#include "world.h"
#include "worldEditor.h"
#include "materials.h"

// Unified Build Includes
#include "player.c"
#include "utils.c"
#include "materials.c"
#include "world.c"
#include "worldEditor.c"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    printf("%s\n", message);
}

int main(void)
{
    if (!glfwInit())
    {
        printf("GLFW failed to initialize\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 640, "Cave Miner", NULL, NULL);
    if (window == NULL)
    {
        printf("GLFW failed to create a window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 640, 640);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);

    INIT_REACTIONS();

    utils_init();
    world_init();
    


    Chunk *c = world_genChunk(0, -1);
    world_linkChunks(world_rootChunk, c);

    Player *player = player_init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        player_updateCamera(window, player, deltaTime);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            worldEditor_paint(window, player->camera_transform, world_rootChunk, &MATERIAL_LOOKUP[FIRE]);
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            worldEditor_paint(window, player->camera_transform, c, &MATERIAL_LOOKUP[LAVA]);

        world_simulateChunk(world_rootChunk);
        world_simulateChunk(c);

        world_drawChunk(world_rootChunk, (vec4 *)(player->camera_transform));
        world_drawChunk(c, (vec4 *)(player->camera_transform));

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    free(world_rootChunk);
    free(c);
    free(player);
    utils_freeAll();

    world_freeAll();

    glfwTerminate();

    return 0;
}