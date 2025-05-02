#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color 
    // veryyyyyyy importanttttttt
    frag_color = tint * fs_in.color * texture(tex, fs_in.tex_coord);
    vec3 light_direction = normalize(vec3(1.0,1.0,1.0));
    vec3 normal = normalize(fs_in.normal);
    vec3 view=normalize(fs_in.view);
    vec3 light_ambient =  vec3(0.4,0.4,0.4);
    vec3 material_ambient =  vec3(0.5,0.5,0.5);
    vec3 ambient=light_ambient*material_ambient;

    vec3 light_diffuse=vec3(1.0,0.9,0.8);
    vec3 material_diffuse=vec3(1.0,0.9,0.8);
    vec3 diffuse= light_diffuse*material_diffuse*max(0.0,dot(light_direction,normal));
    
    vec3 reflected=reflect(-light_direction,normal);
    vec3 light_specular=vec3(1.0,0.9,0.8);
    vec3 material_specuar=vec3(1.0,0.9,0.8);
    float shinniness= 2.0;
    vec3 specular = light_specular*material_specuar*pow(max(0.0,dot(reflected,view)),shinniness);
    
    frag_color.rgb*=diffuse+ambient+specular;
    //frag_color=vec4(fs_in.normal,1.0);
}