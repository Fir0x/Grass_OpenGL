#version 450

layout(location=0) out vec4 output_color;

uniform vec3 sunPosition;

in float gradientT;
in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragUV;

#define COLOR_COUNT 4

vec3 gradColors[COLOR_COUNT] = vec3[](
    vec3(0.0, 0.0, 0.0),
    vec3(0.17, 0.74, 0.0),
    vec3(0.17, 0.74, 0.0),
    vec3(0.66, 1.0, 0.55)
);

float gradAnchors[COLOR_COUNT-2] = float[](0.15,0.8);

vec3 colorGradient(vec3 colors[COLOR_COUNT], float anchors[COLOR_COUNT-2], float t)
{
    int lIndex = 0;
    for (int i = 0; i < COLOR_COUNT - 2; i++)
    {
        lIndex += int(t >= anchors[i]);
    }
    
    
    float lAnchor = lIndex == 0 ? 0.0 : anchors[lIndex-1];
    float rAnchor = lIndex == COLOR_COUNT - 2 ? 1.0 : anchors[lIndex];
    
    vec3 lColor = colors[lIndex];
    vec3 rColor = colors[lIndex+1];
    
    float remapedT = (t - lAnchor) / (rAnchor - lAnchor);
    
    return mix(lColor, rColor, remapedT);
}

void main()
{
    vec3 lightDir = normalize(sunPosition - fragPosition);
    vec3 normal = normalize(fragNormal);
    float factor = max(0.0, dot(lightDir, normal));

    vec3 color = factor * colorGradient(gradColors, gradAnchors, gradientT);
	output_color = vec4(color, 1.0);
}