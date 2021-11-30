#SHADER_TYPE VERTEX

#version 450 core
layout (location = 0) in vec3 a_Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(a_Position, 1.0);
}

#SHADER_TYPE FRAGMENT

#version 450 core
out vec4 FragColor;
uniform vec4 lightColor;

void main()
{
    FragColor = vec4(lightColor); // set alle 4 vector values to 1.0
}
