#version 450

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 nearPoint;
out vec3 farPoint;

out mat4 fragView;
out mat4 fragProj;

// Grid position are in clipped space
vec3 gridPlane[6] = vec3[] (
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    fragProj = projection;
    fragView = view;
    
    vec3 p = gridPlane[gl_VertexID];
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection); // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection); // unprojecting on the far plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}
