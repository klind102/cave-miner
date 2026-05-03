#version 460 core

#define AIR 0
#define SAND 1
#define STONE 2

in vec2 uv;
out vec4 fragColor;

uniform sampler2D tex;

float hash21(uvec2 p) {
  uint n = p.x + 1920U * p.y + (1920U * 1080U);
  n = (n << 13U) ^ n;
  n = n * (n * n * 15731U + 789221U) + 1376312589U;
  return float(n & uint(0x7fffffffU)) / float(0x7fffffff);
}

void main() {
  int material = int(texture(tex, uv).r * 255.0);

  switch(material) {
    case AIR:
      fragColor = vec4(0.0, 0.0, 0.0, 1.0);
      return;
    case SAND:
      fragColor = mix(vec4(1.0, 1.0, 0.0, 1.0), vec4(0.5, 0.5, 0.0, 1.0), hash21(uvec2(uv * textureSize(tex, 0))));
      return;
    case STONE:
      fragColor = mix(vec4(0.75, 0.75, 0.75, 1.0), vec4(0.5, 0.5, 0.5, 1.0), hash21(uvec2(uv * textureSize(tex, 0))));
      return;
    default:
      fragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
