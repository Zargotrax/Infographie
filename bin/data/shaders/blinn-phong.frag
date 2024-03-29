#version 330

struct Material {
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float brightness;
    vec3 color;
};

struct DirectionalLight {
    vec3 direction;
    float brightness;
    vec3 color;
};

struct ProjectorLight {
    vec3 position;
    vec3 direction;
    float brightness;
    float cutoffAngle; // in degrees
    float outerCutoffAngle; // in degrees
    vec3 color;
};

in vec3 vNormal;
in vec3 vPosition;

uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform ProjectorLight projectorLight;

uniform vec3 ambiant;
uniform vec3 camera;

out vec4 fragColor;

void main() {
    vec3 n = normalize(vNormal);
    vec3 v = normalize(vPosition);
    fragColor = vec4(ambiant + material.ambiant, 1);

    vec3 l_pointLight = normalize(pointLight.position - vPosition);
    float reflection_diffuse_pointLight = max(dot(n, l_pointLight), 0.0);
    vec3 h_pointLight = normalize(v + l_pointLight);
    float reflection_specular_pointLight = pow(max(dot(n, h_pointLight), 0.0), pointLight.brightness);
    fragColor += vec4(material.diffuse * reflection_diffuse_pointLight * pointLight.color +
        material.specular * reflection_specular_pointLight * pointLight.color, 1.0);


    vec3 l_directionalLight = normalize(directionalLight.direction);
    float reflection_diffuse_directionalLight = max(dot(n, l_directionalLight), 0.0);
    vec3 h_directionalLight = normalize(v + l_directionalLight);
    float reflection_specular_directionalLight = pow(max(dot(n, h_directionalLight), 0.0), directionalLight.brightness);
    fragColor += vec4(material.diffuse * reflection_diffuse_directionalLight * directionalLight.color +
        material.specular * reflection_specular_directionalLight * directionalLight.color, 1.0);


    vec3 l_projectorLight = normalize(projectorLight.position - vPosition);
    float reflection_diffuse_projectorLight = max(dot(n, l_projectorLight), 0.0);
    float angle = degrees(acos(dot(-l_projectorLight, normalize(-projectorLight.direction))));
    float attenuation = smoothstep(projectorLight.outerCutoffAngle, projectorLight.cutoffAngle, angle);
    vec3 projectedColor = projectorLight.color * attenuation;
    vec3 h_projectorLight = normalize(l_projectorLight + v);
    float reflection_specular_projectorLight = pow(max(dot(n, h_projectorLight), 0.0), projectorLight.brightness);
    fragColor += vec4(material.diffuse * reflection_diffuse_projectorLight * projectedColor +
        material.specular * reflection_specular_projectorLight * projectedColor, 1.0);
}
