#shader vertex
#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;

out vec3 fColor;
out float lightCos;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	fColor = color;
	lightCos = dot(normal, normalize(lightPos - position));
}

#shader fragment
#version 450

layout(location=0) out vec4 output_color;

in vec3 fColor;
in float lightCos;

void main()
{
	output_color = vec4(fColor * max(lightCos, 0.0), 1.0);
}