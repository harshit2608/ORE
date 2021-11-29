#SHADER_TYPE VERTEX

#version 450 core
layout (location = 0) in vec3 a_Position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = a_Position;
    vec4 pos = projection * view * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  

#SHADER_TYPE FRAGMENT

#version 450 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}