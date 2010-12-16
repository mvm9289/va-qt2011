
#ifndef _MATERIAL_LIB_H_
#define _MATERIAL_LIB_H_

#include "Material.h"

#include <vector>
#include <map>

using namespace std;

class MaterialLib
{
    private:
        vector<Material> materials;
        mutable map<string, int> dict;

    private:
        friend ostream& operator<< (ostream &os, const MaterialLib &m);
        
    public:
        MaterialLib();
        void readMtl(const char* filename);
        const Material& material(int index) const;
        int index(const string& name) const; 
};

ostream& operator<< (ostream &os, const MaterialLib &m);

#endif
