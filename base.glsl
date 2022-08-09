#shader vertex
#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec3 color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 fColor;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	fColor = vec4(color, 1.0);
}

#shader fragment
#version 450

layout(location=0) out vec4 output_color;

in vec4 fColor;

void main()
{
	output_color = vec4(1.0,0.0,0.0,1.0);
}