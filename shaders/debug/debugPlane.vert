#version 450

layout(location=0) in vec2 position;

out vec3 geoPosition;

void main()
{
	geoPosition = vec3(position.x, 0.0, position.y);
}