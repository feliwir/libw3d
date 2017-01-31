R"(
#version 330
in vec4 out_normal;
in vec2 out_txcoord;

uniform bool has_diffuse;
uniform sampler2D diffuse;

out vec4 color;

void main()
{
	if(has_diffuse)
		color = texture(diffuse,out_txcoord);
	else
		color = vec4(1.0,0,1.0,1.0);
})";