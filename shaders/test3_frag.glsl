#version 330 core

#define MATERIAL_SHINESS 16

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

/* shared variables */
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

/* directional light variables */
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

out vec4 FragColor;

vec3 CalcDirLight(vec3 dirLightDir, vec3 lightColor, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLightDir);

    /* diffuse */
    float diff = max(dot(normal, lightDir), 0.0);

    /* specular */
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHINESS);

    vec3 ambient  = lightColor  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = lightColor  * diff * 0.5 * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = lightColor * spec * 0.5 * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

void main()
{
    /* shared variables */
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLightDir, dirLightColor, norm, viewDir);

    FragColor = vec4(result, 1.0f);
}



