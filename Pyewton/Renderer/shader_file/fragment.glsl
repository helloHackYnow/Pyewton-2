#version 450 core
#define MAX_NUM_TOTAL_LIGHTS 100

struct PointLight
{
    vec3 position;
    vec3 color;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform bool isObjEmissive;
uniform vec3 objLightColor;

uniform int nb_lights;
uniform PointLight light_points[MAX_NUM_TOTAL_LIGHTS];

uniform vec3 ambient;

vec3 CalcPointLight(PointLight light){
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 result = diffuse * objectColor;
 
    return result;
}

void main()
{    
    vec3 final_color;
    if(!isObjEmissive)
    {
        final_color += ambient * objectColor;

        for(int i = 0; i < nb_lights; i++)
        {
            final_color += CalcPointLight(light_points[i]);
        }
    }
    else
    {
        final_color = objLightColor;
    }

    

    FragColor = vec4(final_color, 1.0);
}