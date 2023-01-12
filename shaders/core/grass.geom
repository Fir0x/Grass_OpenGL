#version 450

layout(points) in;

#define SEGMENT_COUNT 7

layout(triangle_strip, max_vertices=(SEGMENT_COUNT * 6 + 3) * 2) out;

struct FrameContext
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
	float time;
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

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float randRange(vec2 uv, float left, float right)
{
	float t = rand(uv);
	return mix(left, right, t);
}
const float tipFactor = 0.1;

#define PI 3.1415926538

const float normalRotationAngle = 45.0 * PI / 180.0;
const float maxBladeRotation = 8.0 * PI / 180;

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

struct SegmentInfo
{
	vec3 bottomCenter;
	vec3 topCenter;

	vec3 leftSegmentNormal;
	vec3 rightSegmentNormal;

	float bottomGradientT;
	float topGradientT;
};

SegmentInfo generateSegmentInfo(vec3 bladeBase, vec3 bladeMiddleAnchor,
								vec3 bladeTopAnchor, float bladeHeight,
								float bottomHeight, float topHeight,
								vec3 right, mat4 normalLeftRotation,
								mat4 normalRightRotation)
{
	// Center calculus
	const float virtualBottomY = bladeBase.y + bottomHeight;
	const float virtualTopY = bladeBase.y + topHeight;
	const float bottomGradientT = (virtualBottomY - bladeBase.y) / bladeHeight;
	const float topGradientT = (virtualTopY - bladeBase.y) / bladeHeight;
	vec3 bottomCenter = quadraticBezier(bladeBase, bladeMiddleAnchor, bladeTopAnchor, bottomGradientT);
	vec3 topCenter = quadraticBezier(bladeBase, bladeMiddleAnchor, bladeTopAnchor, topGradientT);

	// Normals calculus
	const vec3 up = normalize(topCenter - bottomCenter);
	const vec3 segmentNormal = cross(right, up);

	const vec3 leftSegmentNormal = vec3(normalLeftRotation * vec4(segmentNormal, 1.0));
	const vec3 rightSegmentNormal = vec3(normalRightRotation * vec4(segmentNormal, 1.0));

	SegmentInfo infos = { bottomCenter, topCenter, leftSegmentNormal, rightSegmentNormal, bottomGradientT, topGradientT };

	return infos;
}

void main()
{

	mat4 transfertMatrix = frame.projectionMatrix * frame.viewMatrix * modelMatrix;

	vec3 bladeBase = geoPosition[0];
	float centerX = bladeBase.x;
	float centerY = bladeBase.y;
	float centerZ = bladeBase.z;

	vec3 bladeDirection = geoNormal[0];

	vec3 bladeOrientation = vec3(0.0, 0.0, -1.0);
	const mat4 orientationRotation = rotationMatrix(bladeDirection, randRange(geoUV[0], -maxBladeRotation, maxBladeRotation));
	bladeOrientation = vec3(orientationRotation * vec4(bladeOrientation, 1.0));

	vec3 right = normalize(cross(bladeOrientation, bladeDirection));
	vec2 uv = geoUV[0];

	const float bladeHeight = randRange(geoUV[0], 0.35, 0.47);
	const float segmentWidth = randRange(geoUV[0], 0.013, 0.024);
	const float segmentHeight = ((1.0 - tipFactor) * bladeHeight) / SEGMENT_COUNT;
	
	vec3 bladeMiddleAnchor = bladeBase + bladeDirection * bladeHeight / 2.0;
	vec3 bladeTopAnchor = bladeBase + bladeDirection * bladeHeight;

	float shiftFactor = 0.3 * sin(0.001 * frame.time);
	bladeMiddleAnchor += bladeOrientation * shiftFactor / 10.0;
	bladeTopAnchor += bladeOrientation * shiftFactor;

	//float lengthOffset = length(bladeMiddleAnchor - bladeBase) - bladeHeight / 2.0;
	//bladeMiddleAnchor += int(lengthOffset != 0) * normalize(bladeBase - bladeMiddleAnchor) * lengthOffset;

	//lengthOffset = length(bladeTopAnchor - bladeMiddleAnchor) - bladeHeight / 2.0;
	//bladeTopAnchor += int(lengthOffset != 0) * normalize(bladeMiddleAnchor - bladeTopAnchor) * lengthOffset;

	const mat4 normalLeftRotation = rotationMatrix(bladeDirection, normalRotationAngle);
	const mat4 normalRightRotation = rotationMatrix(bladeDirection, -normalRotationAngle);

	for (int i = 0; i < SEGMENT_COUNT; i++)
	{
		float bottomHeight = i * segmentHeight;
		float topHeight = (i+1) * segmentHeight;
		SegmentInfo segmentInfos = generateSegmentInfo(bladeBase, bladeMiddleAnchor, bladeTopAnchor, bladeHeight,
													  bottomHeight, topHeight, right, normalLeftRotation,
													  normalRightRotation);

		vec3 topLeft = segmentInfos.topCenter - segmentWidth * right;
		vec3 topRight = segmentInfos.topCenter + segmentWidth * right;
		vec3 bottomLeft = segmentInfos.bottomCenter - segmentWidth * right;
		vec3 bottomRight = segmentInfos.bottomCenter + segmentWidth * right;

		// Normals calculus
		const vec3 up = normalize(segmentInfos.topCenter - segmentInfos.bottomCenter);
		const vec3 segmentNormal = cross(up, right);

		// Front face
		// Left triangle
		gl_Position = transfertMatrix * vec4(topLeft, 1.0);
		gradientT = segmentInfos.topGradientT;
		fragPosition = topLeft;
		fragNormal = segmentInfos.leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(bottomLeft, 1.0);
		gradientT = segmentInfos.bottomGradientT;
		fragPosition = bottomLeft;
		fragNormal = segmentInfos.leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(topRight, 1.0);
		gradientT = segmentInfos.topGradientT;
		fragPosition = topRight;
		fragNormal = segmentInfos.rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		EndPrimitive();

		// Right triangle
		gl_Position = transfertMatrix * vec4(topRight, 1.0);
		gradientT = segmentInfos.topGradientT;
		fragPosition = topRight;
		fragNormal = segmentInfos.rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(bottomLeft, 1.0);
		gradientT = segmentInfos.bottomGradientT;
		fragPosition = bottomLeft;
		fragNormal = segmentInfos.leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(bottomRight, 1.0);
		gradientT = segmentInfos.bottomGradientT;
		fragPosition = bottomRight;
		fragNormal = segmentInfos.rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		EndPrimitive();

		// Back face
		vec3 correction =  -bladeOrientation * 0.001;
		// Left triangle
		gl_Position = transfertMatrix * vec4(topRight + correction, 1.0);
		gradientT = segmentInfos.topGradientT;
		fragPosition = topRight;
		fragNormal = -segmentInfos.leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(bottomRight + correction, 1.0);
		gradientT = segmentInfos.bottomGradientT;
		fragPosition = bottomRight;
		fragNormal = -segmentInfos.leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(topLeft + correction, 1.0);
		gradientT = segmentInfos.topGradientT;
		fragPosition = topLeft;
		fragNormal = -segmentInfos.rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		EndPrimitive();

		// Right triangle
		gl_Position = transfertMatrix * vec4(topLeft + correction, 1.0);
		gradientT = segmentInfos.topGradientT;
		fragPosition = topLeft;
		fragNormal = -segmentInfos.rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(bottomRight + correction, 1.0);
		gradientT = segmentInfos.bottomGradientT;
		fragPosition = bottomRight;
		fragNormal = -segmentInfos.leftSegmentNormal;
		fragUV = uv;
		EmitVertex();
		gl_Position = transfertMatrix * vec4(bottomLeft + correction, 1.0);
		gradientT = segmentInfos.bottomGradientT;
		fragPosition = bottomLeft;
		fragNormal = -segmentInfos.rightSegmentNormal;
		fragUV = uv;
		EmitVertex();
		EndPrimitive();
	}

	// Top triangle
	float bottomHeight = SEGMENT_COUNT * segmentHeight;
	float topHeight = bladeHeight;
	SegmentInfo tipInfos = generateSegmentInfo(bladeBase, bladeMiddleAnchor, bladeTopAnchor, bladeHeight,
													bottomHeight, topHeight, right, normalLeftRotation,
													normalRightRotation);
	
	const vec3 up = normalize(tipInfos.topCenter - tipInfos.bottomCenter);
	const vec3 tipNormal = cross(right, up);

	vec3 bottomLeft = tipInfos.bottomCenter - segmentWidth * right;
	vec3 bottomRight = tipInfos.bottomCenter + segmentWidth * right;

	gl_Position = transfertMatrix * vec4(bottomLeft, 1.0);
	gradientT = tipInfos.bottomGradientT;
	fragPosition = bottomLeft;
	fragNormal = tipInfos.leftSegmentNormal;
	fragUV = uv;
	EmitVertex();
	gl_Position = transfertMatrix * vec4(bottomRight, 1.0);
	gradientT = tipInfos.bottomGradientT;
	fragPosition = bottomRight;
	fragNormal = tipInfos.rightSegmentNormal;
	fragUV = uv;
	EmitVertex();
	gl_Position = transfertMatrix * vec4(tipInfos.topCenter, 1.0);
	gradientT = 1.0;
	fragPosition = tipInfos.topCenter;
	fragNormal = tipNormal;
	fragUV = uv;
	EmitVertex();
	EndPrimitive();

	gl_Position = transfertMatrix * vec4(bottomRight, 1.0);
	gradientT = tipInfos.bottomGradientT;
	fragPosition = bottomRight;
	fragNormal = -tipInfos.leftSegmentNormal;
	fragUV = uv;
	EmitVertex();
	gl_Position = transfertMatrix * vec4(bottomLeft, 1.0);
	gradientT = tipInfos.bottomGradientT;
	fragPosition = bottomLeft;
	fragNormal = -tipInfos.rightSegmentNormal;
	fragUV = uv;
	EmitVertex();
	gl_Position = transfertMatrix * vec4(tipInfos.topCenter, 1.0);
	gradientT = 1.0;
	fragPosition = tipInfos.topCenter;
	fragNormal = -tipNormal;
	fragUV = uv;
	EmitVertex();
	EndPrimitive();
}