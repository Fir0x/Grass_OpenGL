#shader vertex
#version 450

layout(location=0) in vec3 position;

void main()
{
	gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 450

uniform vec4 unifcolor;

layout(location=0) out vec4 color;

void main()
{
	color = unifcolor;
}