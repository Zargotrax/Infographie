#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec3 position;
in vec3 normal;

out vec3 vNormal;
out vec3 vPosition;

void main() {
    vNormal = normal;
    vPosition = position;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1);
}