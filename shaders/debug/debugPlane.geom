#version 450

layout(points) in;

layout(triangle_strip, max_vertices=36) out;

struct FrameContext
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

layout(binding = 0) uniform Data {
	FrameContext frame;
};

uniform mat4 modelMatrix;

in vec3 geoPosition[1];

float radius = 0.05;

void main() {
	mat4 transfertMatrix = frame.projectionMatrix * frame.viewMatrix * modelMatrix;

	float centerX = geoPosition[0].x;
	float centerY = geoPosition[0].y;
	float centerZ = geoPosition[0].z;

	// Top face
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	EndPrimitive();

	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	EndPrimitive();


	// Bottom face
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	EndPrimitive();

	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	EndPrimitive();


	// Front face
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	EndPrimitive();


	// Right face
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	EndPrimitive();

	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	EndPrimitive();


	// Back face
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	EndPrimitive();


	// Left face
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	EndPrimitive();

	gl_Position = transfertMatrix * vec4(centerX - radius, centerY + radius, centerZ - radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ + radius, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX - radius, centerY - radius, centerZ - radius, 1.0);
	EmitVertex();
	EndPrimitive();
}