#shader vertex
#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragColor;
out vec3 worldPos;
out vec3 fragNormal;

void main()
{
	worldPos = position;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	fragColor = color;
	fragNormal = normal;
}

#shader fragment
#version 450

layout(location=0) out vec4 output_color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;

in vec3 fragColor;
in vec3 worldPos;
in vec3 fragNormal;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	float lightCos = dot(fragNormal, normalize(lightPos - worldPos));
	vec3 diffuse = max(lightCos, 0.0) * lightColor;

	vec3 color = (ambient + diffuse) * fragColor;
	output_color = vec4(color, 1.0);
}