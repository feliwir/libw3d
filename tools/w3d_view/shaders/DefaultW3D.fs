R"(
#version 330
in vec4 out_normal;
in vec2 out_txcoord;

uniform bool has_diffuse;
uniform sampler2D diffuse;

out vec4 color;

void main()
{
	vec2 uv = vec2(out_txcoord.x,-out_txcoord.y);
	if(has_diffuse)
		color = texture(diffuse,uv);
	else
		color = vec4(1.0,0,1.0,1.0);
})";