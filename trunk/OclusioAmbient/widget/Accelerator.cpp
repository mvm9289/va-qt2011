
#include "Accelerator.h"
#include "Object.h"

Accelerator::Accelerator(Object* obj, vector<int> f):subNode1(0), subNode2(0)
{
    owner = obj;
    faces = f;
    
    createBox();
    box.updateFaces();
    
    if (faces.size() > 10) createSubnodes();
}

Accelerator::~Accelerator()
{
    if (subNode1) delete subNode1;
    if (subNode2) delete subNode2;
}

void Accelerator::createSubnodes()
{
    vector<int> f1;
    vector<int> f2;
    axisPartition(f1, f2);

    if (f1.size() == 0 || f2.size() == 0) return;
    
    subNode1 = new Accelerator(owner, f1);
    subNode2 = new Accelerator(owner, f2);
    faces.clear();
}

void Accelerator::createBox()
{
    if (faces.size() > 0)
    {
        vector<Face> f = owner->faces;
        vector<Vertex> v = owner->vertices;
        box.init(v[f[faces[0]].vertices[0]].coord);
        for (vector<int>::iterator i = faces.begin(); i != faces.end(); i++)
        {
            vector<int> vertices = f[(*i)].vertices;
            for (vector<int>::iterator j = vertices.begin(); j != vertices.end(); j++)
                box.update(v[(*j)].coord);
        }
    }
}

Axis Accelerator::findAxis(float& limit)
{
    float incX = box.maxb.x - box.minb.x;
    float incY = box.maxb.y - box.minb.y;
    float incZ = box.maxb.z - box.minb.z;
    Point c = box.center();
    
    if (incX > incY)
    {
        if (incX > incZ)
        {
            limit = c.x;
            return X;
        }
        else
        {
            limit = c.z;
            return Z;
        }
    }
    else if (incY > incZ)
    {
        limit = c.y;
        return Y;
    }
    else
    {
        limit = c.z;
        return Z;
    }
}

void Accelerator::axisPartition(vector<int>& f1, vector<int>& f2)
{
    if (faces.size() > 0)
    {
        f1.clear(); f2.clear();
        float limit;
        Axis axis = findAxis(limit);

        vector<Face> f = owner->faces;
        vector<Vertex> v = owner->vertices;
        for (vector<int>::iterator i = faces.begin(); i != faces.end(); i++)
        {
            vector<int> vertices = f[(*i)].vertices;
            Point p1 = v[vertices[0]].coord;
            Point p2 = v[vertices[1]].coord;
            Point p3 = v[vertices[2]].coord;
            float xValue = (p1.x + p2.x + p3.x)/3;
            float yValue = (p1.y + p2.y + p3.y)/3;
            float zValue = (p1.z + p2.z + p3.z)/3;

            switch (axis)
            {
                case X:
                    if (xValue <= limit) f1.push_back((*i));
                    else f2.push_back((*i));
                    break;
                case Y:
                    if (yValue <= limit) f1.push_back((*i));
                    else f2.push_back((*i));
                    break;
                case Z:
                    if (zValue <= limit) f1.push_back((*i));
                    else f2.push_back((*i));
                    break;
                default:
                    break;
            }
        }
    }
}

bool Accelerator::hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec)
{
    bool ret = false, ret1 = false, ret2 = false;
    SurfaceHitRecord rec1, rec2, rec3;

    if (box.hit(r, tmin, tmax, rec))
    {
        if (subNode1 || subNode2)
        {
            if (subNode1) ret1 = subNode1->hit(r, tmin, tmax, rec1);
            if (subNode2) ret2 = subNode2->hit(r, tmin, tmax, rec2);

            if (ret1 && ret2)
            {

                if (rec1.t < rec2.t) rec = rec1;
                else rec = rec2;
            }
            else
            {
                if (ret1) rec = rec1;
                else  rec = rec2;
            }

            ret = (ret1 || ret2);
        }
        else if (faces.size() != 0)
        {
            int idx;
            bool hit = false, inter = false;

            Face f(owner);
            rec3.t = 99999999;

            for (vector<int>::const_iterator itF = faces.begin(); itF != faces.end(); ++itF)
            {
                idx = (*itF);
                f = (owner->faces[idx]);
                hit = f.hit(r, tmin, tmax, rec);

                if (hit)
                {
                    inter = true;
                    if (rec.t < rec3.t) rec3 = rec;
                }
            }
            rec = rec3;
            ret = inter;
        }
    }

    return ret;
}

bool Accelerator::shadowHit(const Ray& r, float tmin, float tmax)
{
    bool ret = false, ret1 = false, ret2 = false;
    SurfaceHitRecord rec1;

    if (box.hit(r, tmin, tmax, rec1))
    {
        if (subNode1 || subNode2)
        {
            if (subNode1) ret1 = subNode1->shadowHit(r, tmin, tmax); 
            if (subNode2 && !ret1) ret2 = subNode2->shadowHit(r, tmin, tmax);
            ret = (ret1 || ret2);
        }
        else if (faces.size() != 0)
        {
            int idx;
            bool hit = false;
            
            Face f(owner);

            for (vector<int>::const_iterator itF = faces.begin();  itF != faces.end() && !hit; ++itF)
            {
                idx = (*itF);
                f = (owner->faces[idx]);
                hit = f.hit(r, tmin, tmax, rec1);
            }
            ret = hit;
        }
    }
    
    return ret;
}
