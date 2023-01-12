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
in vec3 geoNormal[1];
in vec2 geoUV[1];

out float gradientT;
out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragUV;

const float bladeHeight = 0.4;
const float segmentWidth = 0.02;
const float tipFactor = 0.1;
const float segmentHeight = ((1.0 - tipFactor) * bladeHeight) / SEGMENT_COUNT;

const float normalRotationAngle = 45.0;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 quadraticBezier(vec3 p0, vec3 p1, vec3 p2, float t)
{
	vec3 a = mix(p0, p1, t);
	vec3 b = mix(p1, p2, t);
	return mix(a, b, t);
}

void main()
{
	mat4 transfertMatrix = frame.projectionMatrix * frame.viewMatrix * modelMatrix;

	float centerX = geoPosition[0].x;
	float centerY = geoPosition[0].y;
	float centerZ = geoPosition[0].z;

	vec3 grassDirection = geoNormal[0];
	vec2 uv = geoUV[0];

	float leftX = centerX - segmentWidth;
	float rightX = centerX + segmentWidth;

	vec3 bladeMiddleAnchor;
	vec3 bladeTopAnchor;

	const mat4 normalLeftRotation = rotationMatrix(grassDirection, normalRotationAngle);
	const mat4 normalRightRotation = rotationMatrix(grassDirection, -normalRotationAngle);

	for (int i = 0; i < SEGMENT_COUNT; i++)
	{
		const float bottomY = centerY + i * segmentHeight;
		const float topY = centerY + (i+1) * segmentHeight;

		// Normals calculus
		const vec3 right = normalize(vec3(rightX - centerX, 0.0, 0.0));
		const vec3 up = normalize(vec3(0.0, topY - centerY, 0.0));
		const vec3 segmentNormal = vec3(0.0, 0.0, -1.0);//cross(up, right);

		const vec3 leftSegmentNormal = vec3(normalLeftRotation * vec4(segmentNormal, 1.0));
		const vec3 rightSegmentNormal = vec3(normalRightRotation * vec4(segmentNormal, 1.0));

		// Left triangle
		gl_Position = transfertMatrix * vec4(leftX, topY, centerZ, 1.0);
		gradientT = (topY - centerY) / bladeHeight;
		fragPosition = vec3(leftX, topY, centerZ);
		fragNormal = leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(leftX, bottomY, centerZ, 1.0);
		gradientT = (bottomY - centerY) / bladeHeight;
		fragPosition = vec3(leftX, bottomY, centerZ);
		fragNormal = leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(rightX, topY, centerZ, 1.0);
		gradientT = (topY - centerY) / bladeHeight;
		fragPosition = vec3(rightX, topY, centerZ);
		fragNormal = rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		EndPrimitive();

		// Right triangle
		gl_Position = transfertMatrix * vec4(rightX, topY, centerZ, 1.0);
		gradientT = (topY - centerY) / bladeHeight;
		fragPosition = vec3(rightX, topY, centerZ);
		fragNormal = rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(leftX, bottomY, centerZ, 1.0);
		gradientT = (bottomY - centerY) / bladeHeight;
		fragPosition = vec3(leftX, bottomY, centerZ);
		fragNormal = leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(rightX, bottomY, centerZ, 1.0);
		gradientT = (bottomY - centerY) / bladeHeight;
		fragPosition = vec3(rightX, bottomY, centerZ);
		fragNormal = rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		EndPrimitive();
	}

	// Top triangle
	float bottomY = centerY + SEGMENT_COUNT * segmentHeight;
	float topY = centerY + bladeHeight;
	
	const vec3 right = normalize(vec3(rightX - centerX, 0.0, 0.0));
	const vec3 up = normalize(vec3(0.0, topY - centerY, 0.0));
	const vec3 tipNormal = cross(up, right);

	const vec3 leftSegmentNormal = vec3(normalLeftRotation * vec4(tipNormal, 1.0));
	const vec3 rightSegmentNormal = vec3(normalRightRotation * vec4(tipNormal, 1.0));

	gl_Position = transfertMatrix * vec4(leftX, bottomY, centerZ, 1.0);
	gradientT = (bottomY - centerY) / bladeHeight;
	fragPosition = vec3(leftX, bottomY, centerZ);
	fragNormal = leftSegmentNormal;
	fragUV = uv;
	EmitVertex();
	gl_Position = transfertMatrix * vec4(rightX, bottomY, centerZ, 1.0);
	gradientT = (bottomY - centerY) / bladeHeight;
	fragPosition = vec3(rightX, bottomY, centerZ);
	fragNormal = rightSegmentNormal;
	fragUV = uv;
	EmitVertex();
	gl_Position = transfertMatrix * vec4(centerX, topY, centerZ, 1.0);
	gradientT = 1.0;
	fragPosition = vec3(centerX, topY, centerZ);
	fragNormal = tipNormal;
	fragUV = uv;
	EmitVertex();
	EndPrimitive();
}