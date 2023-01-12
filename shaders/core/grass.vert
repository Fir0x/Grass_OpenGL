#version 450

layout(location=0) in vec2 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;

out vec3 geoPosition;
out vec3 geoNormal;
out vec2 geoUV;

void main()
{
	geoPosition = vec3(position.x, 0.0, position.y);
	geoNormal = vec3(0.0, 1.0, 0.0);
	geoUV = uv;
}