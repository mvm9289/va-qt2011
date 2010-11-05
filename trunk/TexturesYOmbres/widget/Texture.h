
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <QImage>
#include <QtOpenGL/qgl.h>
#include <string>
#include <map>

using namespace std;

static class Textures
{
        friend class Texture;
    
    private:
        static map<string, unsigned int> loadedTextures;
    
    public:
        int getTextureID(string filename);
};

class Texture
{
    private:
        QImage *bitMap;
        string textureFilename;
        int textureID;
        bool doMipMap;
        int minificationFilter;
        int magnificationFilter;
        int wrappingS;
        int wrappingT;
    
    public:
        Texture();
        ~Texture();
    
        int getTextureID();
        string loadTexture(QString filename);
        void doMipMapping();
        void setMinMagFilter(int minFilter, int magFilter);
        void setWrapMode(int wrapS, int wrapT);
        void sendToGL();
};
#endif
