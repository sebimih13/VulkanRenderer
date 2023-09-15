#version 450

layout (location = 0) in vec2 fragOffset;

layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push
{
	vec4 position;
	vec4 color;
	float radius;
} push;

struct PointLight
{
	vec4 position;
	vec4 color;
};

layout (set = 0, binding = 0) uniform GlobalUBO
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	PointLight pointLights[10];		// TODO : specialization constants
	int numLights;
} ubo;

void main()
{
	float dis = sqrt(dot(fragOffset, fragOffset));
	if (dis >= 1.0)
	{
		discard;
	}

	outColor = vec4(push.color.xyz, 1.0);
}
