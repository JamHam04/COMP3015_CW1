#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// Textures
layout (binding = 0) uniform sampler2D floorTexture;


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


vec3 blinnPhong(LightInfo light, vec3 position, vec3 normal) {
    vec3 diffuse = vec3(0), specular = vec3(0);

    vec3 texColor = texture(floorTexture, TexCoord).rgb;

    vec3 ambient = light.La * texColor;
    vec3 s = normalize(light.Position.xyz - position);

    float sDotN = max(dot(s, normal), 0.0);
    diffuse = light.Ld * texColor * sDotN;
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
    for (int i = 0; i < NumLights; i++) {
        color += blinnPhong(Lights[i], Position, normalize(Normal));
    }
    FragColor = vec4(color, 1.0);

}
