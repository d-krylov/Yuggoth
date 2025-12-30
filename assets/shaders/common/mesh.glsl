#ifndef STRUCTURES_GLSL
#define STRUCTURES_GLSL

struct Light {
  float color_x, color_y, color_z;
  float position_x, position_y, position_z;
  float direction_x, direction_Y, direction_z;
  float attenuation_x, attenuation_y, attenuation_z;
};

struct Vertex {
  float px, py, pz;
  float nx, ny, nz;
  float uvx, uvy;
};

struct Primitive {
  int color_texture_index;
  int normal_texture_index;
  uint vertices_offset;
  uint indices_offset;
  uint indices_size;
};

struct Meshlet {
  uint index_offset;
  uint index_count;
};

#endif // STRUCTURES_GLSL