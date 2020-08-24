#include "Renderer.h"
#include "RenderStats.h"

#include "Config.h"

#include <glad/glad.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <iostream>

bool Renderer::initialised = false;

void Renderer::init(unsigned int width, unsigned int height)
{
    if(!initialised)
    {
        mQuad = std::make_unique<Quad>();
        mTexturedQuad = std::make_unique<TexturedQuad>();
        mCube = std::make_unique<Cube>();
        mTexturedCube = std::make_unique<TexturedCube>();

        mDepthBuffer = std::make_unique<DepthBuffer>();        

        initialised = true;
    }    
}

void Renderer::setupBuffer(RenderBuffer renderBuffer /*= RenderBuffer::DEFAULT*/)
{
    switch(renderBuffer)
    {
        case RenderBuffer::DEPTH:        {
            
            mDepthBuffer->setUp();
        }
        break;
        default:
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, Config::screenWidth, Config::screenHeight);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }
}

const unsigned int Renderer::getBufferTexture(RenderBuffer renderBuffer  /*= RenderBuffer::DEFAULT*/) const 
{
    switch(renderBuffer)
    {
        case RenderBuffer::DEPTH:        
        {
            return mDepthBuffer->getDepthMapTexture();
        }
        break;
        default:
        {
            return -1;
        }
    }
}

void Renderer::drawQuad()
{
    if(initialised)
    {
        mQuad->bind();            
        glDrawElements(GL_TRIANGLES, mQuad->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr);
        renderstats::verticeCount += 4;
        renderstats::quadCount++;
        renderstats::drawCount++;
    }
}

void Renderer::drawTexturedQuad()
{
    if(initialised)
    {
        mTexturedQuad->bind();    
        glDrawElements(GL_TRIANGLES, mTexturedQuad->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr);
        renderstats::verticeCount += 4;
        renderstats::quadCount++;
        renderstats::drawCount++;
    }
}

void Renderer::drawCube()
{
    if(initialised)
    {   
        mCube->bind();    
        glDrawElements(GL_TRIANGLES, mCube->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr);
        renderstats::verticeCount += 24;
        renderstats::quadCount++;
        renderstats::drawCount++;
    }
}

void Renderer::drawTexturedCube()
{
    if(initialised)
    {
        mTexturedCube->bind();    
        glDrawElements(GL_TRIANGLES, mTexturedCube->getIndexBufferCount(), GL_UNSIGNED_INT, nullptr);
        renderstats::verticeCount += 24;
        renderstats::quadCount++;
        renderstats::drawCount++;
    }
}