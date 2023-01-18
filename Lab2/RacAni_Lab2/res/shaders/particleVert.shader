#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in float a_Lifetime;

uniform mat4 u_View;

uniform mat4 u_Transform;

out float v_Lifetime;

void main()
{

	gl_Position = u_View * u_Transform *  vec4(a_Pos,1.0);

	v_Lifetime = a_Lifetime;
}