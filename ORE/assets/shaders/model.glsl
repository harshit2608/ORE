#SHADER_TYPE VERTEX

#version 450 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = a_TexCoords;    
    // TexCoords.y = 1 - TexCoords.y;
    gl_Position = projection * view * model * vec4(a_Position, 1.0);
}

#SHADER_TYPE FRAGMENT

#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}