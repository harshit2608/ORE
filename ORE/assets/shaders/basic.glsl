#SHADER_TYPE VERTEX

#version 450 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);
}


#SHADER_TYPE FRAGMENT

#version 450 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0,0.5 ,0.2 ,1.0 );
}