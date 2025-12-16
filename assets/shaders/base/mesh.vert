#version 460
#extension GL_ARB_shader_draw_parameters: require

// IN
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

// OUT
layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;

// PUSH CONSTANTS
layout (push_constant) uniform PushConstants {
	mat4 projection;
	mat4 view;
  mat4 model;
} pc;

void main() {
	out_position = vec3(pc.model * vec4(in_position, 1.0));
	out_normal = mat3(transpose(inverse(pc.model))) * in_normal;
	out_uv = in_uv;
  
  gl_Position = pc.projection * pc.view * pc.model * vec4(in_position, 1.0);
}