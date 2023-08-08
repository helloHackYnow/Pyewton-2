#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D bloomTex;
uniform sampler2D mainTex;

float bias = 0.04;

// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{ 
    vec4 color = vec4(texture(mainTex, TexCoords) + texture(bloomTex, TexCoords) * bias);
    FragColor = vec4(aces(color.xyz), 1);
}