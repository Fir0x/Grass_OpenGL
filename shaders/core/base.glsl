#shader vertex
#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragUV;

void main()
{
	fragPos = vec3(viewMatrix * modelMatrix * vec4(position, 1.0));
	fragNormal = normalMatrix * normal;
	fragUV = uv;

	gl_Position = projectionMatrix * vec4(fragPos, 1.0);
}

#shader fragment
#version 450

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseTex;
	vec3 specular;
	sampler2D specularTex;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(location=0) out vec4 output_color;

uniform Material material;
uniform Light light;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform mat4 viewMatrix;

in vec2 fragUV;
in vec3 fragPos;
in vec3 fragNormal;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(vec3(viewMatrix * vec4(lightPos, 1.0)) - fragPos);
	float lightCos = dot(normal, lightDir);
	vec3 mtlDiffuse = vec3(texture(material.diffuseTex, fragUV)) * material.diffuse;
	vec3 diffuse = light.diffuse * (max(lightCos, 0.0) * mtlDiffuse);

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 mtlSpecular = vec3(texture(material.specularTex, fragUV)) * material.specular;
	vec3 specular = light.specular * (spec * mtlSpecular);

	vec3 color = ambient + diffuse + specular;
	output_color = vec4(color, 1.0);
}