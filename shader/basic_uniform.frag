#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// Textures
//layout (binding = 0) uniform sampler2D floorTexture;
//layout (binding = 1) uniform sampler2D damageTexture;
//layout (binding = 2) uniform sampler2D normalTexture;




layout (location = 0) out vec4 FragColor;
uniform int NumLights;

uniform struct LightInfo {
    vec4 Position;
    vec3 La; // ambient
    vec3 Ld; // diffuse
    vec3 L; // intensity
} Lights[2];

uniform struct MaterialInfo {
    vec3 Ka; // ambient
    vec3 Kd; // diffuse
    vec3 Ks; // specular
    float Shininess;
} Material;

uniform struct TextureInfo {
    sampler2D diffuseTexture;
    sampler2D normalTexture;
    sampler2D mixDiffuseTexture;
    sampler2D mixNormalTexture;
} Textures;



vec3 blinnPhong(LightInfo light, vec3 position, vec3 normal, vec3 texture) {
    vec3 diffuse = vec3(0), specular = vec3(0);

    //vec4 floorTextureColor = texture(floorTexture, TexCoord);
    //vec4 damageTextureColor = texture(damageTexture, TexCoord);
    //vec4 wallTextureColor = texture(wallTexture, TexCoord);


    //vec3 texColor = mix(floorTextureColor.rgb, damageTextureColor.rgb, damageTextureColor.a);
    //vec3 texColor = texture(floorTexture, TexCoord).rgb;
          


    vec3 ambient = light.La * texture;

    vec3 s = normalize(light.Position.xyz - position);

    float sDotN = max(dot(s, normal), 0.0);
    diffuse = light.Ld * texture * sDotN;
    if (sDotN > 0.0) {
        vec3 v = normalize(-position);
        vec3 h = normalize(s + v);
        specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
    }
    return ambient + (diffuse + specular) * light.L;
}

void main()
{
    vec3 color = vec3(0);

    // Normal mapping

    vec3 diffuseTex = texture(Textures.diffuseTexture, TexCoord).rgb;
    vec3 normalTex = texture(Textures.normalTexture, TexCoord).rgb;

    vec4 mixDiffuseTex = texture(Textures.mixDiffuseTexture, TexCoord);
    vec3 mixNormalTex = texture(Textures.mixNormalTexture, TexCoord).rgb;

    normalTex = 2.0 * normalTex - 1.0;
    mixNormalTex = 2.0 * mixNormalTex - 1.0;

    float mixFactor = mixDiffuseTex.a;
    vec3 finalDiffuseTex = mix(diffuseTex, mixDiffuseTex.rgb, mixFactor);
    vec3 finalNormalTex = mix(normalTex, mixNormalTex, mixFactor);


    


    // Lighting
    for (int i = 0; i < NumLights; i++) {
        color += blinnPhong(Lights[i], Position, normalize(finalNormalTex), finalDiffuseTex);
    }
    FragColor = vec4(color, 1.0); 

}
