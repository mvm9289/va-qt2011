
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
        box.init((owner->vertices[(owner->faces[faces[0]]).vertices[0]]).coord);
        for (vector<int>::iterator i = faces.begin(); i != faces.end(); i++)
        {
            vector<int> vertices = (owner->faces[(*i)]).vertices;
            for (vector<int>::iterator j = vertices.begin(); j != vertices.end(); j++)
                box.update((owner->vertices[(*j)]).coord);
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

        for (vector<int>::iterator i = faces.begin(); i != faces.end(); i++)
        {
            vector<int> vertices = (owner->faces[(*i)]).vertices;
            Point p1 = (owner->vertices[vertices[0]]).coord;
            Point p2 = (owner->vertices[vertices[1]]).coord;
            Point p3 = (owner->vertices[vertices[2]]).coord;
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
    if (box.shadowHit(r, tmin, tmax))
    {
        if (subNode1 || subNode2)
        {
            bool hit = false;
            if (subNode1 && subNode1->hit(r, tmin, tmax, rec)) hit = true;
            if (subNode2 && subNode2->hit(r, tmin, tmax, rec)) hit = true;
            return hit;
        }
        else if (faces.size() > 0)
        {
            bool hit = false;
            for (vector<int>::iterator i = faces.begin(); i != faces.end(); i++)
            {
                SurfaceHitRecord recAux;
                if ((owner->faces[(*i)]).hit(r, tmin, tmax, recAux))
                {
                    hit = true;
                    if (recAux.t < rec.t)  rec = SurfaceHitRecord(recAux);
                }
            }
            return hit;
        }
    }

    return false;
}

bool Accelerator::shadowHit(const Ray& r, float tmin, float tmax)
{
    if (box.shadowHit(r, tmin, tmax))
    {
        if (subNode1 || subNode2)
        {
            if (subNode1 && subNode1->shadowHit(r, tmin, tmax)) return true; 
            if (subNode2 && subNode2->shadowHit(r, tmin, tmax)) return true;
        }
        else if (faces.size() > 0)
            for (vector<int>::iterator i = faces.begin();  i != faces.end(); i++)
                if ((owner->faces[(*i)]).shadowHit(r, tmin, tmax)) return true;
    }
    
    return false;
}
