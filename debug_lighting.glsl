#shader vertex
#version 450

layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;

out float lightCos;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	lightCos = dot(normal, normalize(lightPos - position));
}

#shader fragment
#version 450

layout(location=0) out vec4 output_color;

uniform bool bw_mode = false;

in float lightCos;

void main()
{
	vec3 color;
	if (bw_mode)
		color = vec3(1.0,1.0,1.0) * max(lightCos, 0.0);
	else
		color = mix(vec3(0.0,0.0,1.0), vec3(1.0,0.0,0.0), (lightCos + 1) / 2);

	output_color = vec4(color, 1.0);
}