
#include "Accelerator.h"
#include "Object.h"
#include "Vertex.h"
#include "Point.h"

Accelerator::Accelerator(Object* obj, vector<int> f):subNode1(0), subNode2(0)
{
    owner = obj;
    faces = f;
    if (faces.size() > 0)
    {
        createBox();
        if (faces.size() > 10) createSubnodes();
    }
}

Accelerator::~Accelerator()
{
    if (subNode1) delete(subNode1);
    if (subNode2) delete(subNode2);
}

void Accelerator::createBox()
{
    vector<Vertex> v = owner->vertices;
    vector<Face> f = owner->faces;
    box.init(v[f[faces[0]].vertices[0]].coord);
    int n =  faces.size();
    for (int i = 0; i < n; i++)
    {
        int m = f[faces[i]].vertices.size();
        for (int j = 0; j < m; j++)
            box.update(v[f[faces[i]].vertices[j]].coord);
    }
    box.updateFaces();
}

void Accelerator::createSubnodes()
{
    float incX = box.maxb.x - box.minb.x;
    float incY = box.maxb.y - box.minb.y;
    float incZ = box.maxb.z - box.minb.z;
    
    if (incX >= incY && incX >= incZ)
    {
        float midX = (box.maxb.x + box.minb.x)/2.0;
        if (createSubnodes(X, midX)) faces.clear();
    }
    else if (incY >= incX && incY >= incZ)
    {
        float midY = (box.maxb.y + box.minb.y)/2.0;
        if (createSubnodes(Y, midY)) faces.clear();
    }
    else
    {
        float midZ = (box.maxb.z + box.minb.z)/2.0;
        if (createSubnodes(Z, midZ)) faces.clear();
    }
}

bool Accelerator::createSubnodes(Axis axis, float limit)
{
    vector<Face> f = owner->faces;
    vector<int> f1;
    vector<int> f2;
    
    int n = faces.size();
    for (int i = 0; i < n; i++)
    {
        if (atLeft(axis, limit, f[faces[i]])) f1.push_back(faces[i]);
        else f2.push_back(faces[i]);
    }
    
    if (f1.size() == 0 || f2.size() == 0) return false;
    
    subNode1 = new Accelerator(owner, f1);
    subNode2 = new Accelerator(owner, f2);
    
    return true;
}

bool Accelerator::atLeft(Axis axis, float limit, Face f)
{
    vector<Vertex> v = f.owner->vertices;
    int n = f.vertices.size();
    float xValue, yValue, zValue;
    switch(axis)
    {
        case X:
            xValue = 0;
            for (int i = 0; i < n; i++) xValue += v[f.vertices[i]].coord.x;
            xValue /= n;
            if (xValue <= limit) return true;
            return false;
            break;
        case Y:
            yValue = 0;
            for (int i = 0; i < n; i++) yValue += v[f.vertices[i]].coord.y;
            yValue /= n;
            if (yValue <= limit) return true;
            return false;
            break;
        case Z:
            zValue = 0;
            for (int i = 0; i < n; i++) zValue += v[f.vertices[i]].coord.z;
            zValue /= n;
            if (zValue <= limit) return true;
            return false;
            break;
        default:
            break;
    }
    
    return false;
}

void Accelerator::render()
{
    if (subNode1 || subNode2) 
    {
        if (subNode1) subNode1->render();
        if (subNode2) subNode2->render();
    }
    else box.Render();
}

bool Accelerator::hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const
{
    if (box.hit(r, tmin, tmax, rec))
    {
        if (subNode1 || subNode2)
        {
            if (subNode1 && subNode1->hit(r, tmin, tmax, rec)) return true;
            if (subNode2 && subNode2->hit(r, tmin, tmax, rec)) return true;
        }
        else if (faces.size() > 0)
        {
            vector<Face> f = owner->faces;
            int m = faces.size();
            for (int i = 0; i < m; i++)
                if (f[faces[i]].hit(r, tmin, tmax, rec)) return true;
        }
    }
    
    return false;
}

bool Accelerator::hitMinDist(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const
{
    vector<Face> f = owner->faces;
    int n = faces.size();
    SurfaceHitRecord surfaceMin(rec);
    surfaceMin.t = -1.0;
    bool intersect = false;
    for (int i = 0; i < n; i++)
    {
        if (f[faces[i]].hit(r, tmin, tmax, rec) &&  (surfaceMin.t == -1.0 || rec.t < surfaceMin.t))
        {
            intersect = true;
            surfaceMin.surface = &f[faces[i]];
            surfaceMin.t = rec.t;
        }
    }
    rec = SurfaceHitRecord(surfaceMin);
    
    return intersect;
}

