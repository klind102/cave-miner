#ifndef MATERIALS_H
#define MATERIALS_H
// material types
#define AIR (unsigned char)0
#define SAND (unsigned char)1
#define STONE (unsigned char)2

typedef struct Material
{
  unsigned char type;
  int moveOrder[5];
} Material;

#endif