#version 330 core

uniform sampler2D diffuseTex_ground;
uniform sampler2D bumpTex_ground;

uniform sampler2D diffuseTex_weed;
uniform sampler2D bumpTex_weed;
//

in Vertex {
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
	mat3 TBN = mat3(normalize(IN.tangent),normalize(IN.binormal),normalize(IN.normal));

	vec4 diffuse = vec4(0,0,0,0);
	vec3 bump_normal = vec3(0,0,0);

	float ratio = 1 - (IN.world_position.y - 120) / 50;

	if (IN.world_position.y < 120.0f) {
		diffuse = texture(diffuseTex_ground, IN.texCoord);
		bump_normal = texture(bumpTex_ground, IN.texCoord).rgb;
	}
	else if (IN.world_position.y >= 120.0f && IN.world_position.y <= 170.0f) {
		diffuse = mix(texture(diffuseTex_weed, IN.texCoord),
			texture(diffuseTex_ground, IN.texCoord), ratio);

		bump_normal = mix(texture(bumpTex_weed, IN.texCoord),
			texture(bumpTex_ground, IN.texCoord), ratio).rgb;
	}
	else if (IN.world_position.y > 170.0f) {
		diffuse = texture(diffuseTex_weed, IN.texCoord);
		bump_normal = texture(bumpTex_weed, IN.texCoord).rgb;
	}

	bump_normal = bump_normal * 2.0 - 1.0;
	bump_normal = normalize(TBN * normalize(bump_normal));

	colorOutput = diffuse;
	normalOutput = vec4(bump_normal.xyz * 0.5 + 0.5, 1.0);
}