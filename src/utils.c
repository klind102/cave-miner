#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"




void utils_init()
{
   const float vertices[] = {-1, -1, -1, 1, 1, 1, 1, -1};
   const unsigned int indices[] = {0, 1, 2, 0, 2, 3};

   unsigned int vertexBuffer, indexBuffer;

   glGenVertexArrays(1, &utils_spriteVertexVAO);
   glBindVertexArray(utils_spriteVertexVAO);

   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glGenBuffers(1, &indexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

const char *LoadShaderSource(const char *filePath)
{
   FILE *file = fopen(filePath, "rb");
   if (!file)
   {
      printf("ERROR: could not locate shader source file: %s\n", filePath);
      return NULL;
   }

   fseek(file, 0, SEEK_END);
   long fileSize = ftell(file);
   fseek(file, 0, SEEK_SET);

   char *buffer = (char *)malloc(fileSize + 1);
   if (!buffer)
   {
      printf("ERROR: could not allocate memory for shader source: %s\n", filePath);
      fclose(file);
      return NULL;
   }

   size_t bytesRead = fread(buffer, 1, fileSize, file);
   fclose(file);

   buffer[bytesRead] = '\0';
   return buffer;
}

void utils_freeAll(){
   glDeleteBuffers(1, &utils_spriteVertexVAO);
}