#version 450

layout(location=0) in vec3 position;

out vec3 geoPosition;

void main()
{
	geoPosition = position;
}