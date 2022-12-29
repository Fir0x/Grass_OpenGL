#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

struct FrameContext
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

layout(binding = 0) uniform Data {
	FrameContext frame;
};

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragUV;

void main()
{
	fragPos = vec3(frame.viewMatrix * modelMatrix * vec4(position, 1.0));
	fragNormal = normalMatrix * normal;
	fragUV = uv;

	gl_Position = frame.projectionMatrix * vec4(fragPos, 1.0);
}