struct VSInput {
  float3 pos : LOCATION0;
  float2 uv  : LOCATION1;
};

struct VSOutput {
  float4 pos : POSITION;
  float2 uv  : LOCATION0;
};

struct PushConstants {
  float time;
  float2 offset;
};

[[vk::push_constant]]
PushConstants pc;


[[vk::binding(0, 0)]]
cbuffer UBO {
  float4x4 mvp;
};

[shader("vertex")]
VSOutput main(VSInput input) {
  VSOutput o;
  o.uv  = input.uv;
  o.pos = mul(mvp, float4(input.pos, 1.0));
  return o;
}