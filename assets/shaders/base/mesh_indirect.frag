#version 460
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_ray_query : require

#include "../common/mesh.glsl"
#include "../common/phong.glsl"

layout (set = 0, binding = 2) readonly buffer Primitives { Primitive primitives[]; };
//layout (set = 1, binding = 0) uniform sampler2D textures[];

// INPUT
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in flat uint in_draw_id;

// OUTPUT
layout (location = 0) out vec4 out_color;

void main() {

  //Primitive primitive = primitives[nonuniformEXT(in_draw_id)];

  //vec4 ambient = texture(textures[primitive.color_texture_index], in_uv);

  vec3 light_direction = normalize(vec3(1.0));
  vec3 light_color = vec3(1.0, 1.0, 1.0);
  vec3 albedo = vec3(0.5, 0.5, 0.5);

  float NdotL = clamp(dot(light_direction, in_normal), 0.0, 1.0);

  vec3 color = get_phong(NdotL, light_color, albedo, 0.1);

  out_color = vec4(color, 1.0);
}