#version 400 core

uniform  mat4  modelMatrix;
uniform  mat4  viewMatrix;
uniform  mat4  projMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec4 tangent;
in vec2 texCoord;

in vec4 jointWeights; 
in ivec4 jointIndices;

uniform mat4 joints [128];

out  Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 world_position;
} OUT;

void  main(void) {
	OUT.colour = colour;
	OUT.texCoord = texCoord;

	vec3 wNormal = normalize(mat3(modelMatrix) * normalize(normal));
	vec3 wTangent = normalize(mat3(modelMatrix) * normalize(tangent.xyz));

	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wNormal, wTangent) * tangent.w;

	vec4  localPos   = vec4(position , 1.0f);
	vec4  skelPos    = vec4 (0,0,0,0);

	for(int i = 0; i < 4; ++i) {
		int    jointIndex   = jointIndices[i];
		float  jointWeight = jointWeights[i];
		skelPos += joints[jointIndex] * localPos * jointWeight;
	}

	OUT.world_position = skelPos.xyz;

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(skelPos.xyz , 1.0);
}