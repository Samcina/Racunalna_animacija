#version 450 core

layout(location = 0) out vec4 o_Color;


in float p_Lifetime;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	texColor.a = texColor.a * p_Lifetime;
	o_Color = texColor;
}