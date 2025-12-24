#ifndef STRUCTURES_GLSL
#define STRUCTURES_GLSL

struct Light {
  float px, py, pz;
  float cr, cg, cb;
  
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