
#include "Accelerator.h"
#include "Object.h"
#include "Vertex.h"
#include "Point.h"

Accelerator::Accelerator(Object* obj, vector<int> f)
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
    int n = subnodes.size();
    for (int i = 0; i < n; i++) delete(subnodes[i]);
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
    
    float midX = (box.maxb.x + box.minb.x)/2.0;
    float midY = (box.maxb.y + box.minb.y)/2.0;
    float midZ = (box.maxb.z + box.minb.z)/2.0;
    
    vector<Box> boxes;
    boxes.push_back(Box(Point(midX, box.minb.y, box.minb.z), box.maxb));
    boxes.push_back(Box(Point(box.minb.x, midY, box.minb.z), box.maxb));
    boxes.push_back(Box(Point(box.minb.x, box.minb.y, midZ), box.maxb));
    
    if (incX >= incY && incX >= incZ)
    {
        bool valid = createSubnodes(boxes[0]);
        if (!valid && incY >= incZ) valid = createSubnodes(boxes[1]);
        if (!valid)  valid = createSubnodes(boxes[2]);
        if (valid) faces.clear();
    }
    else if (incY >= incX && incY >= incZ)
    {
        bool valid = createSubnodes(boxes[1]);
        if (!valid && incX >= incZ) valid = createSubnodes(boxes[0]);
        if (!valid)  valid = createSubnodes(boxes[2]);
        if (valid) faces.clear();
    }
    else
    {
        bool valid = createSubnodes(boxes[2]);
        if (!valid && incX >= incY) valid = createSubnodes(boxes[0]);
        if (!valid)  valid = createSubnodes(boxes[1]);
        if (valid) faces.clear();
    }
}

bool Accelerator::createSubnodes(Box b1)
{
    vector<Face> f = owner->faces;
    vector<int> f1;
    vector<int> f2;
    f1.clear();
    f2.clear();
    
    int n = faces.size();
    for (int i = 0; i < n; i++)
    {
        if (isInterior(b1, f[faces[i]])) f1.push_back(faces[i]);
        else f2.push_back(faces[i]);
    }
    
    if (f1.size() == 0 || f2.size() == 0) return false;
    
    Accelerator* n1 = new Accelerator(owner, f1);
    Accelerator* n2 = new Accelerator(owner, f2);
    subnodes.push_back(n1);
    subnodes.push_back(n2);
    
    return true;
}

bool Accelerator::isInterior(Box b, Face f)
{
    vector<Vertex> v = f.owner->vertices;
    int n = f.vertices.size();
    for (int i = 0; i < n; i++)
    {
        Point p = v[f.vertices[i]].coord;
        if (p.x >= b.minb.x && p.y >= b.minb.y && p.z >= b.minb.z &&
            p.x <= b.maxb.x && p.y <= b.maxb.y && p.z <= b.maxb.z)
                return true;
    }
    
    return false;
}

void Accelerator::render()
{
    int n = subnodes.size();
    if (n == 0) box.Render();
    else for (int i = 0; i < n; i++) subnodes[i]->render();
}

bool Accelerator::hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const
{
    if (box.hit(r, tmin, tmax, rec))
    {
        int n = subnodes.size();
        if (n == 0)
        {
            vector<Face> f = owner->faces;
            int m = faces.size();
            for (int i = 0; i < m; i++)
            {
                rec.surface = &f[faces[i]];
                if (f[faces[i]].hit(r, tmin, tmax, rec)) return true;
            }
        }
        else
            for (int i = 0; i < n; i++)
                if (subnodes[i]->hit(r, tmin, tmax, rec)) return true;
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

