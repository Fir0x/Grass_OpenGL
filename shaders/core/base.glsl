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
	vec3 diffuse;
	sampler2D diffuseTex;
	vec3 specular;
	sampler2D specularTex;
	float shininess;
};

struct DirLight
{
	vec3 color;
	vec3 direction;
};

struct PointLight
{
	vec3 color;
	vec3 position;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float cutoff;
};

layout(location=0) out vec4 output_color;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform mat4 viewMatrix;

in vec2 fragUV;
in vec3 fragPos;
in vec3 fragNormal;

vec3 processDirectionalLight(DirLight light)
{
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = vec3(viewMatrix * vec4(-light.direction, 0.0));
	float lightCos = dot(normal, lightDir);
	vec3 mtlDiffuse = vec3(texture(material.diffuseTex, fragUV)) * material.diffuse;
	vec3 diffuse = light.color * (max(lightCos, 0.0) * mtlDiffuse);

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 mtlSpecular = vec3(texture(material.specularTex, fragUV)) * material.specular;
	vec3 specular = light.color * (spec * mtlSpecular);

	return max(diffuse + specular, 0.0);
}

vec3 processPointLight(PointLight light)
{
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(vec3(viewMatrix * vec4(light.position, 1.0)) - fragPos);
	float lightCos = dot(normal, lightDir);
	vec3 mtlDiffuse = vec3(texture(material.diffuseTex, fragUV)) * material.diffuse;
	vec3 diffuse = light.color * (max(lightCos, 0.0) * mtlDiffuse);

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 mtlSpecular = vec3(texture(material.specularTex, fragUV)) * material.specular;
	vec3 specular = light.color * (spec * mtlSpecular);

	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse *= attenuation;
    specular *= attenuation;  

	return max(diffuse + specular, 0.0);
}

vec3 processSpotLight(SpotLight light)
{
	vec3 lightDir = normalize(vec3(viewMatrix * vec4(light.position, 1.0)) - fragPos);
	float theta = dot(lightDir, vec3(viewMatrix * vec4(-light.direction, 0.0)));
	float isLighted = max(sign(theta - light.cutoff), 0.0);
	
	vec3 normal = normalize(fragNormal);
	float diff = max(dot(normal, lightDir), 0.0) * isLighted;
	vec3 mtlDiffuse = vec3(texture(material.diffuseTex, fragUV)) * material.diffuse;
	vec3 diffuse = light.color * diff * mtlDiffuse;
	
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * isLighted;
	vec3 mtlSpecular = vec3(texture(material.specularTex, fragUV)) * material.specular;
	vec3 specular = light.color * spec * mtlSpecular;

	return max(diffuse + specular, 0.0);
}

void main()
{
	vec3 color = processDirectionalLight(dirLight) + processPointLight(pointLight) + processSpotLight(spotLight);
	output_color = vec4(color, 1.0);
}