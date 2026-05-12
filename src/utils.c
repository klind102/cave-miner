#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

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

