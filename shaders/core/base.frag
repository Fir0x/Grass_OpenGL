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
	float inCutOff;
	float outCutOff;

	float constant;
	float linear;
	float quadratic;
};

layout(location=0) out vec4 output_color;

const int MAX_DIR_LIGHT = 4;
const int MAX_POINT_LIGHT = 4;
const int MAX_SPOT_LIGHT = 4;

uniform Material material;
uniform DirLight dirLight[MAX_DIR_LIGHT];
uniform PointLight pointLight[MAX_POINT_LIGHT];
uniform SpotLight spotLight[MAX_SPOT_LIGHT];

struct FrameContext
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
	float time;
};

layout(binding = 0) uniform Data {
	FrameContext frame;
};

uniform bool useDiffuseTex;
uniform bool useSpecularTex;

in vec2 fragUV;
in vec3 fragPos;
in vec3 fragNormal;

vec3 processDirectionalLight(DirLight light)
{
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = vec3(frame.viewMatrix * vec4(-light.direction, 0.0));
	float lightCos = dot(normal, lightDir);
	vec3 mtlDiffuse = material.diffuse;
	if (useDiffuseTex)
		mtlDiffuse *= vec3(texture(material.diffuseTex, fragUV));
	vec3 diffuse = light.color * (max(lightCos, 0.0) * mtlDiffuse);

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 mtlSpecular =  material.specular;
	if (useSpecularTex)
		mtlSpecular *= vec3(texture(material.specularTex, fragUV));
	vec3 specular = light.color * (spec * mtlSpecular);

	return max(diffuse + specular, 0.0);
}

vec3 processPointLight(PointLight light)
{
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(vec3(frame.viewMatrix * vec4(light.position, 1.0)) - fragPos);
	float lightCos = dot(normal, lightDir);
	vec3 mtlDiffuse = material.diffuse;
	if (useDiffuseTex)
		mtlDiffuse *= vec3(texture(material.diffuseTex, fragUV));
	vec3 diffuse = light.color * (max(lightCos, 0.0) * mtlDiffuse);

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 mtlSpecular =  material.specular;
	if (useSpecularTex)
		mtlSpecular *= vec3(texture(material.specularTex, fragUV));
	vec3 specular = light.color * (spec * mtlSpecular);

	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse *= attenuation;
    specular *= attenuation;  

	return max(diffuse + specular, 0.0);
}

vec3 processSpotLight(SpotLight light)
{
	vec3 lightDir = normalize(vec3(frame.viewMatrix * vec4(light.position, 1.0)) - fragPos);
	float theta = dot(lightDir, vec3(frame.viewMatrix * vec4(-light.direction, 0.0)));
	float epsilon = light.inCutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);
	
	vec3 normal = normalize(fragNormal);
	float diff = max(dot(normal, lightDir), 0.0) * intensity;
	vec3 mtlDiffuse = material.diffuse;
	if (useDiffuseTex)
		mtlDiffuse *= vec3(texture(material.diffuseTex, fragUV));
	vec3 diffuse = light.color * diff * mtlDiffuse;
	
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * intensity;
	vec3 mtlSpecular =  material.specular;
	if (useSpecularTex)
		mtlSpecular *= vec3(texture(material.specularTex, fragUV));
	vec3 specular = light.color * spec * mtlSpecular;

	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse *= attenuation;
    specular *= attenuation; 

	return max(diffuse + specular, 0.0);
}

void main()
{
	vec3 color = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < MAX_DIR_LIGHT; i++)
		color += processDirectionalLight(dirLight[i]);

	for (int i = 0; i < MAX_POINT_LIGHT; i++)
		color += processPointLight(pointLight[i]);
		
	for (int i = 0; i < MAX_SPOT_LIGHT; i++)
		color += processSpotLight(spotLight[i]);

	output_color = vec4(color, 1.0);
}