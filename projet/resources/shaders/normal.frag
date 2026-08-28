#version 330 core

in vec3 geomNormal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(geomNormal, 1.0);
}