#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragUV;

void main()
{
	fragPos = vec3(viewMatrix * modelMatrix * vec4(position, 1.0));
	fragNormal = normalMatrix * normal;
	fragUV = uv;

	gl_Position = projectionMatrix * vec4(fragPos, 1.0);
}