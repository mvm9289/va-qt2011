
#include "Texture.h"

Texture::Texture():bitMap(NULL), doMipMap(false), minificationFilter(-1), magnificationFilter(-1), wrappingS(-1), wrappingT(-1) {}

Texture::~Texture()
{
    if (bitMap != NULL) delete bitMap;
}

static int Texture::getTextureID(string filename)
{
    map<string, unsigned int>::iterator it = loadedTextures.find(filename);
    if (it != NULL) return it->second;
    
    return -1;
}

string Texture::loadTexture(QString filename)
{
}

void Texture::doMipMapping()
{
}

void Texture::setMinMagFilter(int minFilter, int magFilter)
{
}

void Texture::setWrapMode(int wrapS, int wrapT)
{
}

void Texture::sendToGL()
{
}

