#shader vertex
#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 fragColor;
out vec3 fragPos;
out vec3 fragNormal;

void main()
{
	fragPos = vec3(viewMatrix * modelMatrix * vec4(position, 1.0));
	fragColor = color;
	fragNormal = normalMatrix * normal;

	gl_Position = projectionMatrix * vec4(fragPos, 1.0);
}

#shader fragment
#version 450

layout(location=0) out vec4 output_color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform mat4 viewMatrix;

in vec3 fragColor;
in vec3 fragPos;
in vec3 fragNormal;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(vec3(viewMatrix * vec4(lightPos, 1.0)) - fragPos);
	float lightCos = dot(normal, lightDir);
	vec3 diffuse = max(lightCos, 0.0) * lightColor;

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * spec * lightColor; 

	vec3 color = (ambient + diffuse + specular) * fragColor;
	output_color = vec4(color, 1.0);
}