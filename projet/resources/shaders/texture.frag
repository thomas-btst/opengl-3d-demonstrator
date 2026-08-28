#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

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

uniform sampler2D baseTexture;

void main() {
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse * texture(baseTexture, texCoord).xyz;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Gaussian
    float cosAngIncidence = dot(norm, lightDir);
    cosAngIncidence = clamp(cosAngIncidence, 0.0, 1.0);
    float angleNormalHalf = acos(dot(halfwayDir, norm));
    float exponent = angleNormalHalf / material.gaussian;
    exponent = -(exponent * exponent);
    float gaussianTerm = exp(exponent);

    gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;

    vec3 specular = light.specular * gaussianTerm * material.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}