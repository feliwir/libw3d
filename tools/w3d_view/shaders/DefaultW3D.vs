R"(
#version 330
layout (location=0)in vec3 in_pos;
layout (location=1)in vec3 in_normal;
layout (location=2)in vec2 in_txcoord;
layout (location=3)in ivec2 in_bones;

uniform mat4 vp;
uniform mat4 m;
uniform bool has_skinning;
uniform mat4 bones[128];

out vec4 out_normal;
out vec2 out_txcoord;

void main()
{
	vec4 localPos = vec4(in_pos,1);
	
	if(has_skinning)
	{
		localPos = bones[in_bones[0]] * localPos;
	}

	gl_Position = vp*m*localPos;
	out_txcoord = in_txcoord;
	out_normal = vec4(in_normal,0);
}
)"