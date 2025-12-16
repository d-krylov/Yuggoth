#ifndef PHONG_GLSL
#define PHONG_GLSL

vec3 get_phong(float NdotL, vec3 light_color, vec3 albedo, float ambient_strength) {
  vec3 ambient_color = ambient_strength * light_color;
  vec3 diffuse_color = albedo * NdotL;
  return ambient_color + light_color * diffuse_color;
}

#endif // PHONG_GLSL