#version 330 core

#define MATERIAL_SHINESS 16
#define DIR_LIGHT_STRENGTH 0.5
#define SPECULAR_STRENGTH 0.5
#define AMBIENT_STRENGTH 0.1

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

/* config variables */
uniform bool day;

/* shared variables */
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

/* directional light variables */
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

/* point1 light */
uniform vec3 p1Lpos;
uniform vec3 p1Lcol;
uniform float p1Lconst;
uniform float p1Llinear;
uniform float p1Lquadratic;

/* point2 light */
uniform vec3 p2Lpos;
uniform vec3 p2Lcol;
uniform float p2Lconst;
uniform float p2Llinear;
uniform float p2Lquadratic;

/* point3 light */
uniform vec3 p3Lpos;
uniform vec3 p3Lcol;
uniform float p3Lconst;
uniform float p3Llinear;
uniform float p3Lquadratic;

out vec4 FragColor;

vec3 CalcDirLight(vec3 dirLightDir, vec3 lightColor, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLightDir);

    /* diffuse */
    float diff = max(dot(normal, lightDir), 0.0);

    /* specular */
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHINESS);

    vec3 ambient  = lightColor * vec3(texture(texture_diffuse1, TexCoords)) * AMBIENT_STRENGTH;
    vec3 diffuse  = lightColor * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = lightColor * spec *  vec3(texture(texture_specular1, TexCoords)) * SPECULAR_STRENGTH;
    return (ambient + diffuse + specular) * DIR_LIGHT_STRENGTH;
}

vec3 CalcPointLight(vec3 LightPos, vec3 LightColor,
                    float LightConst, float LightLinear, float LightQuadratic,
                    vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(LightPos - fragPos);

    /* diffuse shading */
    float diff = max(dot(normal, lightDir), 0.0);

    /* specular shading */
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHINESS);

    /* attenuation */
    float distance    = length(LightPos - fragPos);
    float attenuation = 1.0 / (LightConst + LightLinear * distance +
                        LightQuadratic * (distance * distance));

    /* combine results */
    vec3 ambient  = LightColor * vec3(texture(texture_diffuse1, TexCoords)) * AMBIENT_STRENGTH;
    vec3 diffuse  = LightColor  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = LightColor * spec * vec3(texture(texture_specular1, TexCoords)) * SPECULAR_STRENGTH;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    /* shared variables */
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f);
    /* directional light */
    if (day)
    {
        result = CalcDirLight(dirLightDir, dirLightColor, norm, viewDir);
    }

    /* point */
    result += CalcPointLight(p1Lpos, p1Lcol, p1Lconst, p1Llinear, p1Lquadratic, norm, FragPos, viewDir);
    result += CalcPointLight(p2Lpos, p2Lcol, p2Lconst, p2Llinear, p2Lquadratic, norm, FragPos, viewDir);
    result += CalcPointLight(p3Lpos, p3Lcol, p3Lconst, p3Llinear, p3Lquadratic, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}



