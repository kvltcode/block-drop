#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;

out vec4 v_colour;
out vec3 v_normal;
out vec2 v_texCoord;
out float v_texIndex;

uniform mat4 u_View;
uniform mat4 u_Projection;

//uniform mat4 u_Transform;
void main()
{
    v_colour = colour;
    v_texCoord = texCoord;
    v_texIndex = texIndex;
    gl_Position = u_Projection * u_View * vec4(position, 1.0);
    //gl_Position = u_Projection * u_View * u_Transform * vec4(position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_colour;
in vec3 v_normal;
in vec2 v_texCoord;
in float v_texIndex;
uniform sampler2D u_textures[32];

void main()
{
    //weird (known) bug where if you select an array value using a value that isn't a context, the textures get artifacts
    switch(int(v_texIndex))
	{
		case 0: colour = texture(u_textures[0], v_texCoord) * v_colour; break;
   		case 1: colour = texture(u_textures[1], v_texCoord) * v_colour; break;
   		case 2: colour = texture(u_textures[2], v_texCoord) * v_colour; break;
        case 3: colour = texture(u_textures[3], v_texCoord) * v_colour; break;
        case 4: colour = texture(u_textures[4], v_texCoord) * v_colour; break;
        case 5: colour = texture(u_textures[5], v_texCoord) * v_colour; break;
        case 6: colour = texture(u_textures[6], v_texCoord) * v_colour; break;
        case 7: colour = texture(u_textures[7], v_texCoord) * v_colour; break;
        case 8: colour = texture(u_textures[8], v_texCoord) * v_colour; break;
        case 9: colour = texture(u_textures[9], v_texCoord) * v_colour; break;
        case 10: colour = texture(u_textures[10], v_texCoord) * v_colour; break;
        case 11: colour = texture(u_textures[11], v_texCoord) * v_colour; break;
        case 12: colour = texture(u_textures[12], v_texCoord) * v_colour; break;
        case 13: colour = texture(u_textures[13], v_texCoord) * v_colour; break;
        case 14: colour = texture(u_textures[14], v_texCoord) * v_colour; break;
        case 15: colour = texture(u_textures[15], v_texCoord) * v_colour; break;
        case 16: colour = texture(u_textures[16], v_texCoord) * v_colour; break;
        case 17: colour = texture(u_textures[17], v_texCoord) * v_colour; break;
        case 18: colour = texture(u_textures[18], v_texCoord) * v_colour; break;
        case 19: colour = texture(u_textures[19], v_texCoord) * v_colour; break;
        case 20: colour = texture(u_textures[20], v_texCoord) * v_colour; break;
        case 21: colour = texture(u_textures[21], v_texCoord) * v_colour; break;
        case 22: colour = texture(u_textures[22], v_texCoord) * v_colour; break;
        case 23: colour = texture(u_textures[23], v_texCoord) * v_colour; break;
        case 24: colour = texture(u_textures[24], v_texCoord) * v_colour; break;
        case 25: colour = texture(u_textures[25], v_texCoord) * v_colour; break;
        case 26: colour = texture(u_textures[26], v_texCoord) * v_colour; break;
        case 27: colour = texture(u_textures[27], v_texCoord) * v_colour; break;
        case 28: colour = texture(u_textures[28], v_texCoord) * v_colour; break;
        case 29: colour = texture(u_textures[29], v_texCoord) * v_colour; break;
        case 30: colour = texture(u_textures[30], v_texCoord) * v_colour; break;        
        case 31: colour = texture(u_textures[31], v_texCoord) * v_colour; break;        
	}

    //int index = int(v_texIndex);
    //colour = texture(u_textures[index], v_texCoord);
    //colour = texture(u_textures[index], v_texCoord) * v_colour;
    //colour = vec4(v_texIndex, v_texIndex, v_texIndex, 1.0f);
    //colour = vec4(1.0, 0.0, 0.0, 1.0f);
    //colour = v_colour;
};