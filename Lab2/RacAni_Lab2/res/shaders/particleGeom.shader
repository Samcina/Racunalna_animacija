#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
in float v_Lifetime[];

uniform mat4 u_Projection;

out float p_Lifetime;
out vec2 v_TexCoord;

void build_quad(vec4 position)
{
    float size = 0.08f;
    gl_Position = position + vec4(-size * v_Lifetime[0], -size * v_Lifetime[0], 0.0, 0.0);    // 1:bottom-left
    p_Lifetime = v_Lifetime[0];
    gl_Position = u_Projection * gl_Position;
    v_TexCoord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = position + vec4(size * v_Lifetime[0], -size * v_Lifetime[0], 0.0, 0.0);    // 2:bottom-right
    p_Lifetime = v_Lifetime[0];
    v_TexCoord = vec2(0.0, 1.0);
    gl_Position = u_Projection * gl_Position;
    EmitVertex();
    gl_Position = position + vec4(-size * v_Lifetime[0], size * v_Lifetime[0], 0.0, 0.0);    // 3:top-left
    p_Lifetime = v_Lifetime[0];
    v_TexCoord = vec2(1.0, 0.0);
    gl_Position = u_Projection * gl_Position;
    EmitVertex();
    gl_Position = position + vec4(size * v_Lifetime[0], size * v_Lifetime[0], 0.0, 0.0);    // 4:top-right
    p_Lifetime = v_Lifetime[0];
    v_TexCoord = vec2(0.0, 0.0);
    gl_Position = u_Projection * gl_Position;
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_quad(gl_in[0].gl_Position);
}