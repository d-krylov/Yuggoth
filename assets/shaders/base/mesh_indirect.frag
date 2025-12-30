#version 460
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_ray_query : require

#include "../common/mesh.glsl"
#include "../common/phong.glsl"

layout (set = 0, binding = 2) readonly buffer Primitives { Primitive primitives[]; };
layout (set = 0, binding = 3) uniform Lights { Light lights[1000]; };
//layout (set = 1, binding = 0) uniform sampler2D textures[];

layout (set = 0, binding = 4) uniform accelerationStructureEXT TLAS;

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

  vec3 color = vec3(0.0);
  vec3 albedo = vec3(0.2, 0.2, 0.2);

  for (int i = 0; i < 10; i++) {

    Light light = lights[i];

    vec3 light_color = vec3(light.color_x, light.color_y, light.color_z);
    vec3 light_origin = vec3(light.position_x, light.position_y, light.position_z);
    vec3 light_attenuation = vec3(light.attenuation_x, light.attenuation_y, light.attenuation_z);
    vec3 light_direction = normalize(light_origin - in_position);

    float steps = distance(in_position, light_origin);

    rayQueryEXT ray_query;
	  rayQueryInitializeEXT(ray_query, TLAS, gl_RayFlagsTerminateOnFirstHitEXT, 0xff, in_position, 0.1f, light_direction, steps + 1.0);
	  rayQueryProceedEXT(ray_query);

    bool hit = rayQueryGetIntersectionTypeEXT(ray_query, true) == gl_RayQueryCommittedIntersectionNoneEXT;

    float NdotL = clamp(dot(light_direction, in_normal), 0.0, 1.0);
    NdotL *= hit ? 1.0 : 0.01;

    float k = 1.0 / (0.1 + dot(light_attenuation, vec3(steps * steps, steps, 1.0)));
    
    color += k * get_phong(NdotL, light_color, albedo, 0.1);
  }

  out_color = vec4(color, 1.0);
}