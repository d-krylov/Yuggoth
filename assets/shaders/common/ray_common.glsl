
#ifndef RAY_COMMON_GLSL
#define RAY_COMMON_GLSL

struct Hit {
  vec4 color;
  vec3 direction;
  vec3 origin;
};

struct Vertex {
	float px, py, pz;
	float nx, ny, nz;
	float uvx, uvy;
};

mat2 rotate(float a) {
  return mat2(cos(a), sin(a), -sin(a), cos(a));
}

#endif // RAY_COMMON_GLSL
