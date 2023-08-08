#version 450 core 

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 blur_dir;

const int SAMPLE_COUNT = 11;

const float OFFSETS[11] = float[11](
    -9.260003189282239,
    -7.304547036499911,
    -5.353083811756559,
    -3.4048471718931532,
    -1.4588111840004858,
    0.48624268466894843,
    2.431625915613778,
    4.378621204796657,
    6.328357272092126,
    8.281739853232981,
    10
);

const float WEIGHTS[11] = float[11](
    0.002071619848105582,
    0.012832728894200915,
    0.0517012035286156,
    0.1355841921107385,
    0.23159560769543552,
    0.2577662485651885,
    0.18695197035734282,
    0.08833722224378082,
    0.027179417353550506,
    0.005441161635553416,
    0.0005386277674878371
);

// blurDirection is:
//     vec2(1,0) for horizontal pass
//     vec2(0,1) for vertical pass
// The sourceTexture to be blurred MUST use linear filtering!
// pixelCoord is in [0..1]
vec4 blur(in sampler2D sourceTexture, vec2 blurDirection, vec2 pixelCoord)
{
    vec4 result = vec4(0.0);
    vec2 size = textureSize(sourceTexture, 0);
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        vec2 offset = blurDirection * OFFSETS[i] / size;
        float weight = WEIGHTS[i];
        result += texture(sourceTexture, pixelCoord + offset) * weight;
    }
    return result;
}

void main()
{ 
    FragColor = blur(screenTexture, blur_dir, TexCoords);
}