#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec4 tangent;
in vec2 texCoord;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 world_position;
} OUT;

void main() {
	OUT.colour = colour;
	OUT.texCoord = texCoord;

	vec3 wNormal = normalize(mat3(modelMatrix) * normalize(normal));
	vec3 wTangent = normalize(mat3(modelMatrix) * normalize(tangent.xyz));

	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wNormal , wTangent) * tangent.w;

	vec4 world_pos = modelMatrix * vec4(position,1);
	OUT.world_position = world_pos.xyz;

	gl_Position = (projMatrix * viewMatrix) * world_pos;
}