#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 p1Lpos;
uniform vec3 p1Lcol;
uniform float p1Lconst;
uniform float p1Llinear;
uniform float p1Lquadratic;


out vec4 FragColor;


void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * p1Lcol;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(p1Lpos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * p1Lcol;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * p1Lcol;

    float distance    = length(p1Lpos - FragPos);
    float attenuation = 1.0 / (p1Lconst + p1Llinear * distance +
    p1Lquadratic * (distance * distance));

    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    vec4 tmp1 = vec4(diffuse, 1.0f) * texture(texture_diffuse1, TexCoords) * 1.444;
    vec4 tmp2 = vec4(specular, 1.0f) * texture(texture_specular1, TexCoords);
    vec4 tmp3 = vec4(ambient, 1.0f) * texture(texture_diffuse1, TexCoords) * 1.444;
    vec4 result = tmp1 + tmp2 + tmp3;
    FragColor = result;
}

