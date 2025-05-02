#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
} vs_out;

uniform mat4 transform;
uniform mat4 modelInverseTranspose; 
uniform mat4 model;
uniform vec3 camera_position;
void main(){
    //TODO: (Req 7) Change the next line to apply the transformation matrix
    vec3 world = (model* vec4(position, 1.0)).xyz;
    gl_Position = transform *vec4(world,1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    vs_out.normal = normalize((modelInverseTranspose * vec4(normal,0.0)).xyz);
    vs_out.view=camera_position-world;
}
