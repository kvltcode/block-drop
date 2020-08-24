#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec4 v_colour;
out vec2 v_texCoord;
out float v_texIndex;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ProjectionView;

//uniform mat4 u_Transform;
void main()
{
    v_colour = colour;
    v_texCoord = texCoord;
    v_texIndex = texIndex;
    gl_Position = u_ProjectionView * vec4(position, 1.0);
    //gl_Position = u_Projection * u_View * u_Transform * vec4(position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_colour;
in vec2 v_texCoord;
in float v_texIndex;
uniform sampler2D u_texture;

void main()
{  
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, v_texCoord).r);
    colour = v_colour * sampled;
};