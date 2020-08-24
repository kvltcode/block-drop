#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;


out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix;

void main()
{
    vs_out.FragPos = vec3(u_Model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(u_Model))) * normal;
    vs_out.TexCoords = texCoord;
    vs_out.FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * u_Model * vec4(position, 1.0);
}


#shader fragment
#version 330 core

out vec4 Fragcolour;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.0001 * (1.0 - dot(normal, lightDir)), 0.00001);
    // check whether current frag pos is in shadow
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}

uniform vec4 u_Diffuse_Colour; //todo - rename to texture colour
uniform vec3 u_Ambient_Colour;
uniform vec3 u_Light_Colour;
uniform float u_Shininess;

//todo - add in attenuation
void main()
{           
    vec4 colour = u_Diffuse_Colour * texture(diffuseTexture, fs_in.TexCoords);
    vec3 normal = normalize(fs_in.Normal);

    // ambient
    vec3 ambient = u_Ambient_Colour * colour.rgb;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * u_Light_Colour * colour.rgb;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    vec3 specular = spec * u_Light_Colour * colour.rgb; 

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = ambient + ((1.0 - shadow) * (diffuse + specular)); //shadow only applied to the diffuse and specular

    // add in alpha 
    vec4 result = vec4(lighting, colour.a);

    //Fragcolour = u_Diffuse_Colour * texture(diffuseTexture, fs_in.TexCoords);

    // Gamma correction
    Fragcolour = pow(result, vec4(1.0/2.2));
}