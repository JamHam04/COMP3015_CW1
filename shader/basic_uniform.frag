#version 460

in vec3 Position;
in vec3 Normal;


layout (location = 0) out vec4 FragColor;

uniform struct LightInfo {
    vec4 Position;
    vec3 La; // ambient
    vec3 Ld; // diffuse
    vec3 L; // intensity
} Light;

uniform struct MaterialInfo {
    vec3 Ka; // ambient
    vec3 Kd; // diffuse
    vec3 Ks; // specular
    float Shininess;
} Material;


vec3 blinnPhong(vec3 position, vec3 normal) {
    vec3 diffuse = vec3(0), specular = vec3(0);
    vec3 ambient = Light.La * Material.Ka;
    vec3 s = normalize(Light.Position.xyz - position);

    float sDotN = max(dot(s, normal), 0.0);
    diffuse = Light.Ld * Material.Kd * sDotN;
    if (sDotN > 0.0) {
        vec3 v = normalize(-position);
        vec3 h = normalize(s + v);
        specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
    }
    return ambient + (diffuse + specular) * Light.L;
}

void main()
{
    FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
}
