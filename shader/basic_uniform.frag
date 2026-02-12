#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


layout (binding = 0) uniform sampler2D HDRTex;

uniform int Pass;
uniform float AvgLum;
uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform bool DoToneMap;



layout (location = 0) out vec4 FragColor;



uniform mat3 rgb2xyz = mat3(
    0.4124564, 0.2126729, 0.0193339,
    0.3572769, 0.7151522, 0.1191920,
    0.1804375, 0.0721750, 0.9503041
);

uniform mat3 xyz2rgb = mat3(
    3.2404542, -0.9692660, 0.0556434,
    -1.5371385, 1.8760108, -0.2040259,
    -0.4985314, 0.0415560, 1.0572252
);

uniform int NumLights;
uniform bool useMixTexture;

uniform struct LightInfo {
    vec4 Position;
    vec3 La; // ambient
    vec3 Ld; // diffuse
    vec3 L; // intensity
} Lights[3];

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

uniform struct FogInfo {
    vec3 Color;
    float minDist;
    float maxDist;
} Fog;

vec3 blinnPhong(LightInfo light, vec3 position, vec3 normal, vec3 texture) {
    vec3 diffuse = vec3(0), specular = vec3(0);


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

// HDR
void Pass1() {
    vec3 color = vec3(0);

    // Fog
    float distance = abs(Position.z);
    float fogFactor = clamp((Fog.maxDist - distance) / (Fog.maxDist - Fog.minDist), 0.0, 1.0);

    // Normal mapping
    vec3 diffuseTex = texture(Textures.diffuseTexture, TexCoord).rgb;
    vec3 normalTex = texture(Textures.normalTexture, TexCoord).rgb;
    normalTex = 2.0 * normalTex - 1.0;

    vec3 finalDiffuseTex = diffuseTex;
    vec3 finalNormalTex = normalTex;


    // Mixing Textures
    if (useMixTexture) {
        vec4 mixDiffuseTex = texture(Textures.mixDiffuseTexture, TexCoord);
        vec3 mixNormalTex = texture(Textures.mixNormalTexture, TexCoord).rgb;
  
        mixNormalTex = 2.0 * mixNormalTex - 1.0;

        float mixFactor = mixDiffuseTex.a;
        finalDiffuseTex = mix(diffuseTex, mixDiffuseTex.rgb, mixFactor);
        finalNormalTex = mix(normalTex, mixNormalTex, mixFactor);
        
    }


    // Lighting
    for (int i = 0; i < NumLights; i++) {
        color += blinnPhong(Lights[i], Position, normalize(finalNormalTex), finalDiffuseTex);
    }

    color = mix(Fog.Color, color, fogFactor);
    FragColor = vec4(color, 1.0);

}

// Tonemapping
void Pass2() {
    vec4 color = texture(HDRTex, TexCoord);

    vec3 xyzCol = rgb2xyz * color.rgb;
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;

    vec3 xyYCol = vec3(xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

    float L = (Exposure * xyYCol.z) / AvgLum;
    L = (L * (1.0 + L / (White * White))) / (1.0 + L);


    xyzCol.x = (L * xyYCol.x) / xyYCol.y;
    xyzCol.y = L;
    xyzCol.z = (L * (1.0 - xyYCol.x - xyYCol.y)) / xyYCol.y;

    //FragColor = color;
    FragColor = vec4(xyz2rgb * xyzCol, 1.0);
}

void main()
{

    if (Pass == 1) Pass1();
    else if (Pass == 2) Pass2();

}
