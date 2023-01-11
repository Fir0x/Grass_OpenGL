#version 450

layout(points) in;

#define SEGMENT_COUNT 4

layout(triangle_strip, max_vertices=SEGMENT_COUNT * 6 + 3) out;

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

float segmentWidth = 0.02;
float segmentHeight = 0.1;

void main() {
	mat4 transfertMatrix = frame.projectionMatrix * frame.viewMatrix * modelMatrix;

	float centerX = geoPosition[0].x;
	float centerY = geoPosition[0].y;
	float centerZ = geoPosition[0].z;

	for (int i = 0; i < SEGMENT_COUNT; i++)
	{
		// Left triangle
		gl_Position = transfertMatrix * vec4(centerX - segmentWidth, centerY + (i+1) * segmentHeight, centerZ, 1.0);
		EmitVertex();
		gl_Position = transfertMatrix * vec4(centerX - segmentWidth, centerY + i * segmentHeight, centerZ, 1.0);
		EmitVertex();
		gl_Position = transfertMatrix * vec4(centerX + segmentWidth, centerY + (i+1) * segmentHeight, centerZ, 1.0);
		EmitVertex();
		EndPrimitive();

		// Right triangle
		gl_Position = transfertMatrix * vec4(centerX + segmentWidth, centerY + (i+1) * segmentHeight, centerZ, 1.0);
		EmitVertex();
		gl_Position = transfertMatrix * vec4(centerX - segmentWidth, centerY + i * segmentHeight, centerZ, 1.0);
		EmitVertex();
		gl_Position = transfertMatrix * vec4(centerX + segmentWidth, centerY + i * segmentHeight, centerZ, 1.0);
		EmitVertex();
		EndPrimitive();
	}

	// Top triangle
	gl_Position = transfertMatrix * vec4(centerX - segmentWidth, centerY + SEGMENT_COUNT * segmentHeight, centerZ, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX + segmentWidth, centerY + SEGMENT_COUNT * segmentHeight, centerZ, 1.0);
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX, centerY + (SEGMENT_COUNT + 0.3) * segmentHeight, centerZ, 1.0);
	EmitVertex();
	EndPrimitive();
}