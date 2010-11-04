
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{
    private:
        static map<string, unsigned int> loadedTextures;
    
        QImage *bitMap;
        bool doMipMap;
        int minificationFilter;
        int magnificationFilter;
        int wrappingS;
        int wrappingT;
    
    public:
        Texture();
        ~Texture();
    
        static int getTextureID(string filename);
    
        string loadTexture(QString filename);
        void doMipMapping();
        void setMinMagFilter(int minFilter, int magFilter);
        void setWrapMode(int wrapS, int wrapT);
        void sendToGL();
}
#endif
