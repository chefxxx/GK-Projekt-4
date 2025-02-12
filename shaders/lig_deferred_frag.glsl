#version 330 core

#define AMBIENT_STRENGTH 0.1
#define DIR_LIGHT_STRENGTH 0.5
#define SPECULAR_STRENGTH 1.0
#define MATERIAL_SHINESS 16

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform vec3 viewPos;

/* config variables */
uniform float maxFog;
uniform float minFog;
uniform vec3 FogCol;
uniform bool day;

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

/* point4 light */
uniform vec3 p4Lpos;
uniform vec3 p4Lcol;
uniform float p4Lconst;
uniform float p4Llinear;
uniform float p4Lquadratic;

/* spotlight */
uniform vec3 spotPos;
uniform vec3 spotCol;
uniform vec3 spotDir;
uniform float spotInnerCutoff;
uniform float spotOuterCutoff;

vec3 CalcDirLight(vec3 dirLightDir, vec3 lightColor, vec3 normal, vec3 viewDir, vec3 Diff, float Spec)
{
    vec3 lightDir = normalize(-dirLightDir);

    /* diffuse */
    float diff = max(dot(normal, lightDir), 0.0);

    /* specular */
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHINESS);

    vec3 diffuse  = lightColor * diff * Diff;
    vec3 specular = lightColor * spec * Spec;
    return  (diffuse + specular) * DIR_LIGHT_STRENGTH;
}

vec3 CalcPointLight(vec3 LightPos, vec3 LightColor,
                    float LightConst, float LightLinear, float LightQuadratic,
                    vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Diff, float Spec)
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
    vec3 diffuse  = LightColor * diff * Diff;
    vec3 specular = LightColor * spec * Spec * SPECULAR_STRENGTH;

    diffuse  *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

vec3 CalcSpotlight(vec3 lightPos, vec3 lightColor, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 lightDirection, vec3 Diff, float Spec)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float theta = dot(lightDir, normalize(-lightDirection));

    if(theta > spotOuterCutoff)
    {
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHINESS);

        vec3 diffuse  = lightColor  * diff * Diff;
        vec3 specular = lightColor * spec * Spec * SPECULAR_STRENGTH;

        float epsilon   = spotInnerCutoff - spotOuterCutoff;
        float intensity = clamp((theta - spotOuterCutoff) / epsilon, 0.0, 1.0);

        diffuse  *= intensity;
        specular *= intensity;

        return (diffuse + specular);
    }

    return vec3(0.0f);
}

void main()
{
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 lighting  = Diffuse * AMBIENT_STRENGTH;
    vec3 viewDir  = normalize(viewPos - FragPos);

    if (day)
    {
        lighting += CalcDirLight(dirLightDir, dirLightColor, Normal, viewDir, Diffuse, Specular);
    }

    /* point */
    lighting += CalcPointLight(p1Lpos, p1Lcol, p1Lconst, p1Llinear, p1Lquadratic, Normal, FragPos, viewDir, Diffuse, Specular);
    lighting += CalcPointLight(p2Lpos, p2Lcol, p2Lconst, p2Llinear, p2Lquadratic, Normal, FragPos, viewDir, Diffuse, Specular);
    lighting += CalcPointLight(p3Lpos, p3Lcol, p3Lconst, p3Llinear, p3Lquadratic, Normal, FragPos, viewDir, Diffuse, Specular);
    lighting += CalcPointLight(p4Lpos, p4Lcol, p4Lconst, p4Llinear, p4Lquadratic, Normal, FragPos, viewDir, Diffuse, Specular);

    /* spotlight */
    lighting += CalcSpotlight(spotPos, spotCol, Normal, FragPos, viewDir, spotDir, Diffuse, Specular);

    float dist = length(FragPos - viewPos);
    float fog_factor = (maxFog - dist) / (maxFog - minFog);
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    lighting = mix(FogCol, lighting, fog_factor);

    FragColor = vec4(lighting, 1.0);
}