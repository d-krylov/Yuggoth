#version 460 core
#extension GL_GOOGLE_include_directive : require

#include "../common/phong.glsl"

// IN
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

// OUT
layout (location = 0) out vec4 out_color;

void main() {

  vec3 ambient = vec3(0.5, 0.5, 0.5); 

  vec3 light_direction = normalize(vec3(1.0));
  vec3 light_color = vec3(1.0, 1.0, 1.0);

  float NdotL = clamp(dot(light_direction, in_normal), 0.0, 1.0);

  vec3 color = get_phong(NdotL, light_color, ambient, 0.1);

  out_color = vec4(color, 1.0);
}