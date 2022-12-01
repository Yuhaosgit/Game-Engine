#version 400 core

uniform mat4 shadowMatrix = mat4(1.0f);

in vec3 position;
in vec4 colour;
in vec2 texCoord;

in vec4 jointWeights;
in ivec4 jointIndices;

uniform bool hasJoints = false;

uniform mat4 joints[128];

void main(void)
{
    if (hasJoints) {
	  vec4  localPos   = vec4(position , 1.0f);
	  vec4  skelPos    = vec4 (0,0,0,0);
	  for(int i = 0; i < 4; ++i) {
	    int    jointIndex   = jointIndices[i];
	    float  jointWeight = jointWeights[i];
		skelPos  +=  joints[jointIndex] * localPos * jointWeight;
	  }
	  gl_Position = shadowMatrix * vec4(skelPos.xyz, 1.0);
	} 
	
	else {
	  gl_Position = shadowMatrix * vec4(position, 1.0);
	}
}