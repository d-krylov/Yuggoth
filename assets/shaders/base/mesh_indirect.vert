#version 460
#extension GL_GOOGLE_include_directive : require
#include "../common/mesh.glsl"

// INPUTS
layout (set = 0, binding = 0) readonly buffer Vertices { Vertex vertices[]; };
layout (set = 0, binding = 1) readonly buffer Models { mat4 models[]; };

// PUSH CONSTANTS
layout (push_constant) uniform PushConstants {
	mat4 projection;
	mat4 view;
	mat4 model;
} pc;

// OUTPUT
layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;
layout (location = 3) out flat uint out_drawid;

void main() {
	Vertex vertex = vertices[gl_VertexIndex];

	vec3 position = vec3(vertex.px, vertex.py, vertex.pz);
	vec3 normal = vec3(vertex.nx, vertex.ny, vertex.nz);
	mat4 model = models[gl_DrawID];

	mat3 normal_matrix = transpose(inverse(mat3(model)));

	out_position = vec3(model * vec4(position, 1.0));
	out_normal = normalize(normal_matrix * normal);
	out_uv = vec2(vertex.uvx, vertex.uvy);
	out_drawid = gl_DrawID;
  
  gl_Position = pc.projection * pc.view * model * vec4(position, 1.0);
}

