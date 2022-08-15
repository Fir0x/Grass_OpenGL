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
out vec3 worldPos;
out vec3 fragNormal;

void main()
{
	worldPos = vec3(modelMatrix * vec4(position, 1.0));
	gl_Position = projectionMatrix * viewMatrix * vec4(worldPos, 1.0);
	fragColor = color;
	fragNormal = normalMatrix * normal;
}

#shader fragment
#version 450

layout(location=0) out vec4 output_color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 viewPos;

in vec3 fragColor;
in vec3 worldPos;
in vec3 fragNormal;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - worldPos);
	float lightCos = dot(normal, lightDir);
	vec3 diffuse = max(lightCos, 0.0) * lightColor;

	vec3 viewDir = normalize(viewPos - worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * spec * lightColor; 

	vec3 color = clamp((ambient + diffuse + specular), 0.0, 1.0) * fragColor;
	output_color = vec4((ambient + diffuse + specular) * fragColor, 1.0);
}