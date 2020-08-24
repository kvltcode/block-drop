#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

uniform vec3 u_AmbientColour;
uniform vec3 u_DiffuseColour;

void main()
{
    colour = vec4(u_AmbientColour + u_DiffuseColour, 1.0);
};