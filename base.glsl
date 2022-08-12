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

uniform vec3 lightColor;
uniform float ambientStrength;

in vec3 fColor;
in float lightCos;

void main()
{
	vec3 ambient = ambientStrength * lightColor * fColor;
	vec3 diffuse = fColor * max(lightCos, 0.0);
	vec3 color = clamp(ambient + diffuse, 0.0, 1.0);
	output_color = vec4(color, 1.0);
}