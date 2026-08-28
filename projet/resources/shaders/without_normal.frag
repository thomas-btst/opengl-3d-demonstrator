#version 330 core
out vec4 FragColor;

in vec3 fragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float gaussian;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diffuse = light.diffuse * material.diffuse;
    // specular
    vec3 specular = light.specular * material.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}