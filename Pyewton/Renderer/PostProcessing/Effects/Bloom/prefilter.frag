#version 450 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;
uniform float knee;
uniform float threshold;

//https://github.com/Unity-Technologies/Graphics/blob/master/Packages/com.unity.render-pipelines.high-definition/Runtime/PostProcessing/Shaders/BloomCommon.hlsl 05 august 2023
// Quadratic color thresholding
// curve = (threshold - knee, knee * 2, 0.25 / knee)
//vec4 threshold = x: threshold value (linear), y: threshold - knee, z: knee * 2, w: 0.25 / knee
vec3 QuadraticThreshold(vec3 color, float threshold_, vec3 curve)
{
    // Pixel brightness
    float br = max(color.r, max(color.g, color.b));

    // Under-threshold part
    float rq = clamp(br - curve.x, 0.0, curve.y);
    rq = curve.z * rq * rq;

    // Combine and apply the brightness response curve
    color *= max(rq, br - threshold_) / max(br, 1e-4);

    return color;
}

vec4 Prefilter(vec4 color, vec4 threshold)
{
    color = vec4(QuadraticThreshold(color.xyz,  threshold.x, threshold.yzw), 1);
    return color;
}

void main()
{
    vec4 threshold_ = vec4(threshold, threshold - knee, knee * 2, 0.25 / knee);
    vec4 color = texture(screenTexture, TexCoords);
    FragColor =  Prefilter(color, threshold_);
}