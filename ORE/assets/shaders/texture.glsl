#SHADER_TYPE VERTEX

#version 450 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_TexCoords;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
    ourColor = a_Color;
    TexCoord = a_TexCoords;
}


#SHADER_TYPE FRAGMENT

#version 450 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
