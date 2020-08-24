#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;
uniform float u_NearPlane;
uniform float u_FarPlane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));	
}

void main()
{             
    float depthValue = texture(u_Texture, v_TexCoord).r;
    colour = vec4(vec3(LinearizeDepth(depthValue) / u_FarPlane), 1.0); // perspective
    //colour = vec4(vec3(depthValue), 1.0); // orthographic
}