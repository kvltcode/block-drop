#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_texCoord;

out vec3 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
//uniform mat4 mvp;

void main()
{
    v_texCoord = texCoord;
    TexCoords = position;
    vec4 pos = u_Projection * u_View * u_Model * vec4(position, 1.0);
    gl_Position = pos.xyww; 
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_texCoord;
in vec3 TexCoords;
uniform samplerCube u_Texture;

void main()
{
    colour = texture(u_Texture, TexCoords);
    //colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};