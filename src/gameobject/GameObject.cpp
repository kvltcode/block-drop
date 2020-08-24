#include "GameObject.h"


GameObject::GameObject(glm::vec3 position):
    mPosition(position)
{
    updateModel();
}

void GameObject::setPosition(glm::vec3 position) 
{ 
    mPosition = position; 
    updateModel();
}

void GameObject::setPosition(float x, float y, float z /*= 0.0f*/) 
{ 
    mPosition.x = x; 
    mPosition.y = y; 
    mPosition.z = z;
    updateModel();
}

void GameObject::setPositionX(float position) 
{ 
    mPosition.x = position; 
    updateModel();
}

void GameObject::setPositionY(float position) 
{ 
    mPosition.y = position; 
    updateModel();
}

void GameObject::setPositionZ(float position) 
{ 
    mPosition.z = position; 
    updateModel();
}

void GameObject::setScale(glm::vec3 scale) 
{ 
    mScale = scale; 
    updateModel();
}

void GameObject::setScale(float x, float y, float z /*= 1.0f*/) 
{ 
    mScale.x = x; 
    mScale.y = y; 
    mScale.z = z;    
    updateModel();
}

void GameObject::setRotation(glm::vec3 rotation)
{
    mRotation = rotation;
    updateModel();
}

void GameObject::setRotation(float x, float y, float z /*= 0.0f*/)
{
    mRotation.x = x;
    mRotation.y = y;
    mRotation.z = z;
    updateModel();
}

void GameObject::setRotationX(float rotation)
{
    mRotation.x = rotation;
    updateModel();
}

void GameObject::setRotationY(float rotation)
{
    mRotation.y = rotation;
    updateModel();
}

void GameObject::setRotationZ(float rotation)
{
    mRotation.z = rotation;
    updateModel();
}

void GameObject::setColour(float r, float g, float b, float a /*= 1.0f*/)
{
    mColour.r = r;
    mColour.g = g;
    mColour.b = b;
    mColour.a = a;
}

///@todo this stuff destroys performance
void GameObject::updateModel()
{
    //translate, rotate, scale
    glm::mat4 model = glm::mat4(1.0f);

    //translate
    model = glm::translate(model, mPosition);

    //rotate
    //usually move, then rotate but this doesn't need moved because the quad centre is already in the middle due to vertex positions and that's what we're rotating around
    model = glm::rotate(model, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); //z axis
    model = glm::rotate(model, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); //y axis
    model = glm::rotate(model, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); //x axis

    //scale
    model = glm::scale(model, mScale);

    mModel = model;
}

const glm::mat4& GameObject::getModel() const
{
    return mModel;
}
