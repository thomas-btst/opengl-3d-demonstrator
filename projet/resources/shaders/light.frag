#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor; 

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform sampler2D baseTexture;

void main() {
    FragColor = vec4(texture(baseTexture, texCoord).xyz, 1.0);
    float brightness = dot(FragColor.rgb, vec3(1.0, 1.0, 1.0));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}