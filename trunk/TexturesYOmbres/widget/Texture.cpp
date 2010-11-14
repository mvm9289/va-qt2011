
#include "Texture.h"

Texture::Texture():bitMap(NULL), textureFilename(""), textureID(-1), doMipMap(false), minificationFilter(-1), magnificationFilter(-1), wrappingS(-1), wrappingT(-1) {}

Texture::~Texture()
{
    if (bitMap != NULL) delete bitMap;
}

int Texture::getTextureID(string filename, bool projector)
{
    if (projector)
    {
        map<string, unsigned int>::iterator it = loadedProjectorTextures.find(filename);
        if (it != loadedProjectorTextures.end())
        {
            textureID = (int)(it->second);
            return textureID;
        }
    }
    else
    {
        map<string, unsigned int>::iterator it = loadedTextures.find(filename);
        if (it != loadedTextures.end())
        {
            textureID = (int)(it->second);
            return textureID;
        }
    }
        
    return -1;
}

int Texture::getTextureID()
{
    return textureID;
}

string Texture::loadTexture(QString filename)
{
    bitMap = new QImage();
    
    if (bitMap->load(filename))
    {
        textureFilename = string(filename.toLatin1().data());
        *bitMap = QGLWidget::convertToGLFormat(*bitMap);
        return textureFilename;
    }
    else
    {
        delete bitMap;
        bitMap = NULL;
        return "";
    }
}

void Texture::doMipMapping()
{
    doMipMap = true;
}

void Texture::setMinMagFilter(int minFilter, int magFilter)
{
    minificationFilter = minFilter;
    magnificationFilter = magFilter;
}

void Texture::setWrapMode(int wrapS, int wrapT)
{
    wrappingS = wrapS;
    wrappingT = wrapT;
}

void Texture::sendToGL(bool projector)
{
    glGenTextures(1, (GLuint *)(&textureID));
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitMap->width(), bitMap->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitMap->bits());
    
    resendToGL(projector);
    if (projector) loadedProjectorTextures[textureFilename] = textureID;
    else loadedTextures[textureFilename] = textureID;
    
    delete bitMap;
    bitMap = NULL;
}

void Texture::resendToGL(bool projector)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingT);
    if (projector)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        float borderColor[4] = {1.0, 1.0, 1.0, 0.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        float borderColor[4] = {0.0, 0.0, 0.0, 0.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
}
