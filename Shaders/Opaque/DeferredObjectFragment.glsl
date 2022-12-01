#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 world_position;
} IN;

layout(location = 0) out vec4 colorOutput;
layout(location = 1) out vec4 normalOutput;

void main() {
	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec3 bump_normal = texture(bumpTex, IN.texCoord).rgb * 2.0 -1.0;
	bump_normal = normalize(TBN * normalize(bump_normal));

	colorOutput = diffuse;
	normalOutput = vec4(bump_normal.xyz * 0.5 + 0.5, 1.0);
}