#include "Light.h"
#include <gtc/matrix_transform.hpp>

Light::Light(glm::vec3 position): GameObject(position)
{
    //Diffuse colour used by child class instead of colour in parent
    //setting colour to diffuse so they match anyway
  	mColour = { mDiffuseColour.r, mDiffuseColour.g, mDiffuseColour.b, 1.0f};
    setScale(50.0f, 50.0f, 50.0f);

    //orthographic projection
    //mProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, mNearClip, mFarClip);
    //perspective projection
    mProjection = glm::perspective(glm::radians(90.0f), 
                                    static_cast<float>(Config::screenWidth) / static_cast<float>(Config::screenHeight), 
                                    mNearClip, 
                                    mFarClip);
}

const glm::mat4 Light::getLightSpaceMatrix() const
{
    glm::vec3 lightFront = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::mat4 lightView = glm::lookAt(mPosition, mPosition + lightFront, glm::vec3(0.0f, 0.0f, 1.0f));
    return mProjection * lightView; 
}