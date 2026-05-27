#version 460 core

#define AIR 0
#define SAND 1
#define STONE 2
#define WATER 3

const vec4 COL_AIR = vec4(0.0, 0.0, 0.0, 1.0);

const vec4 COL_SAND_DARK = vec4(0.5, 0.5, 0.0, 1.0);
const vec4 COL_SAND_LIGHT = vec4(1.0, 1.0, 0.0, 1.0);

const vec4 COL_STONE_DARK = vec4(0.5, 0.5, 0.5, 1.0);
const vec4 COL_STONE_LIGHT = vec4(0.75, 0.75, 0.75, 1.0);

const vec4 COL_WATER = vec4(0.1, 0.4, 0.6, 1.0);

in vec3 texCoords;
out vec4 fragColor;

uniform sampler3D tex;

float hash11(uint n) {
    // integer hash copied from Hugo Elias
  n = (n << 13U) ^ n;
  n = n * (n * n * 15731U + 789221U) + 1376312589U;
  return float(n & uint(0x7fffffffU)) / float(0x7fffffff);
}
void main() {

  uvec2 texData = uvec2(texture(tex, texCoords) * 255.0);
  uint material = texData.r;
  uint variant = texData.g;

  switch(material) {
    case AIR:
      fragColor = COL_AIR;
      return;
    case SAND:
      fragColor = mix(COL_SAND_DARK, COL_SAND_LIGHT, hash11(variant));
      return;
    case STONE:
      fragColor = mix(COL_STONE_DARK, COL_STONE_LIGHT, hash11(variant));
      return;
      case WATER:
      fragColor = COL_WATER;
      return;
    default:
      fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
