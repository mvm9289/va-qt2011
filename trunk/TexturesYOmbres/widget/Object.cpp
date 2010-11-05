
#include <QtOpenGL/qgl.h>
#include <cmath>

#include "Object.h"
#include "Box.h"
#include "Scene.h"

Object::Object(std::string n):name(n), triangles(0), quads(0), texture(-1)
{
    vertexTriangles = NULL;
    vertexQuads = NULL;
    vertices2 = NULL;
    normals = NULL;
    colors = NULL;
}

Object::~Object() {}

Box Object::boundingBox() const
{
    return _boundingBox;
}

void Object::computeBoundingBox()
{
    if (vertices.size())
    {
        Point p = vertices[0].coord;
        _boundingBox=Box(p, p);
        for (unsigned int i=1; i<vertices.size(); i++)
            _boundingBox.update(vertices[i].coord);
    }
}

void Object::initGL()
{
    createDisplayList();
    createVertexArrays();

		pos = center = boundingBox().center();
}

void Object::createDisplayList()
{
    DLindex = glGenLists(1);
    glNewList(DLindex, GL_COMPILE_AND_EXECUTE);
        immediateRender();
    glEndList();
}

void Object::createVertexArrays()
{
    triangles = 0;
    quads = 0;
    unsigned int triIdx;
    unsigned int quadIdx;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        if (faces[i].vertices.size() == 3) triangles++;
        else quads++;
    }

    if (vertexTriangles != NULL) free(vertexTriangles);
    if (vertexQuads != NULL) free(vertexQuads);
    if (vertices2 != NULL) free(vertices2);
    if (normals != NULL) free(normals);
    if (colors != NULL) free(colors);
    vertexTriangles = (GLuint *)malloc(sizeof(GLuint)*triangles*3);
    vertexQuads = (GLuint *)malloc(sizeof(GLuint)*quads*4);

    triIdx = 0;
    quadIdx = 0;
    colors = (GLfloat *)malloc(sizeof(GLfloat)*vertices.size()*3);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        Material material = Scene::matlib.material(faces[i].material);
        if (faces[i].vertices.size() == 3)
        {
            unsigned int v = faces[i].vertices[0];
            vertexTriangles[triIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;

            v = faces[i].vertices[1];
            vertexTriangles[triIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;

            v = faces[i].vertices[2];
            vertexTriangles[triIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;
        }
        else
        {
            unsigned int v = faces[i].vertices[0];
            vertexQuads[quadIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;

            v = faces[i].vertices[1];
            vertexQuads[quadIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;

            v = faces[i].vertices[2];
            vertexQuads[quadIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;

            v = faces[i].vertices[3];
            vertexQuads[quadIdx++] = v;
            colors[3*v] = material.kd.r;
            colors[3*v + 1] = material.kd.g;
            colors[3*v + 2] = material.kd.b;
        }
    }

    vertices2 = (GLfloat *)malloc(sizeof(GLfloat)*vertices.size()*3);
    normals = (GLfloat *)malloc(sizeof(GLfloat)*vertices.size()*3);
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        vertices2[i*3] = vertices[i].coord.x;
        vertices2[i*3 + 1] = vertices[i].coord.y;
        vertices2[i*3 + 2] = vertices[i].coord.z;

        normals[i*3] = vertices[i].normal.x;
        normals[i*3 + 1] = vertices[i].normal.y;
        normals[i*3 + 2] = vertices[i].normal.z;
    }

    glVertexPointer(3, GL_FLOAT, 0, vertices2);
    glNormalPointer(GL_FLOAT, 0 , normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
}

void Object::render(int mode)
{

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(pos.x,pos.y,pos.z);
    glTranslatef(-center.x,-center.y,-center.z);

    switch (mode)
    {
        case IMMEDIATE:
            immediateRender();
            break;
        case DISPLAY_LIST:
            glCallList(DLindex);
            break;
        case VERTEX_ARRAYS:
            vertexArraysRender();
            break;
        default:
            break;
    }

		glPopMatrix();
}

inline void Object::immediateRender()
{
    if (texture != -1)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
    }
    center = boundingBox().center();
    for(unsigned int i=0; i<faces.size(); i++)
    {
        glBegin (GL_POLYGON);
                Material material = Scene::matlib.material(faces[i].material);
                glColor3f(material.kd.r, material.kd.g, material.kd.b);
                for(unsigned int j=0; j<faces[i].vertices.size(); j++)
                { 
                    glNormal3f(vertices[faces[i].vertices[j]].normal.x,
                    vertices[faces[i].vertices[j]].normal.y,
                    vertices[faces[i].vertices[j]].normal.z);

                    Vector P = vertices[faces[i].vertices[j]].coord - center;
                    P.normalize();
                    glTexCoord2f((atan2(P.x, P.z)/(2*M_PI)), ((asin(P.y)/M_PI) + 0.5));

                    glVertex3f(vertices[faces[i].vertices[j]].coord.x,
                    vertices[faces[i].vertices[j]].coord.y,
                    vertices[faces[i].vertices[j]].coord.z);
                }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

inline void Object::vertexArraysRender()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawElements(GL_TRIANGLES, triangles*3, GL_UNSIGNED_INT, vertexTriangles);
    glDrawElements(GL_QUADS, quads*4, GL_UNSIGNED_INT, vertexQuads);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Object::updateNormals()
{
    for(unsigned int i=0; i<faces.size(); ++i)
        faces[i].computeNormal(vertices);
}

vector<int> Object::numTrianglesQuads()
{
    vector<int> info(2);
    info[0] = triangles;
    info[1] = quads;

    return info;
}

void Object::setTexture(int textureID)
{
    texture = textureID;
}


void Object::setPos(Point p)
{
    pos = p;
}

Point Object::getPos()
{
    return pos;
}





/*
Lectura d'un fitxer OBJ

Conté fragments de codi de obj_2_ply.c (C) Greg Turk

-----------------------------------------------------------------------
Copyright (c) 1998 Georgia Institute of Technology.  All rights reserved.   

Permission to use, copy, modify and distribute this software and its   
documentation for any purpose is hereby granted without fee, provided   
that the above copyright notice and this permission notice appear in   
all copies of this software and that you do not sell the software.   
*/


/* for file reading */
static char **words;
static int max_words = 0;
static int num_words = 0;
#define BIG_STRING 4096
static char str[BIG_STRING];
static char str_orig[BIG_STRING];

void get_indices ( char *word, int *vindex, int *tindex, int *nindex )

/******************************************************************************

GET_INDICES breaks up a word that may have slash-separated numbers into one or more
numbers.

Entry:
word - word to break up

Exit:
vindex - first number (vertex index)
tindex - second number (texture index)
nindex - third number (normal vector index)
******************************************************************************/
{
    char null[] = {" "};
    char *ptr;
    char *tp;
    char *np;

    /* by default, the texture and normal pointers are set to the null string */

    tp = null;
    np = null;

    /* replace slashes with null characters and cause tp and np to point */
    /* to character immediately following the first or second slash */

    for (ptr = word; *ptr != '\0'; ptr++) {
        if (*ptr == '/') {
            if (tp == null)
                tp = ptr + 1;
            else
                np = ptr + 1;

            *ptr = '\0';
        }
    }

    *vindex = atoi (word);
    *tindex = atoi (tp);
    *nindex = atoi (np);
}

void Object::make_face ( char **words, int nwords, int currentMaterial )
{
    Face face;
    for (int i = 0; i < nwords; i++) 
    {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9'))
        {
            get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.vertices.push_back(vindex - 1);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.vertices.push_back(vertices.size() + vindex);
            else 
            {
                fprintf (stderr, "Zero indices not allowed: '%s'\n", str_orig);
                exit (-1);
            }

            /*
            if ((tindex != 0 || nindex != 0) && warning == 0) {
            fprintf (stderr, "\n");
            fprintf (stderr, "Warning: textures and normals currently ignored.\n");
            fprintf (stderr, "\n");
            warning = 1;
            }
            */
        }
    }
    face.material = currentMaterial;

    faces.push_back(face);
}

char *fetch_line ( FILE *fp )
{
    //int i,j;
    char *ptr;
    char *ptr2;
    char *result;
    //char *comment_ptr;

    /* read in a line */
    result = fgets (str, BIG_STRING, fp);

    /* return NULL if we're at the end-of-file */
    if (result == NULL)
        return ((char *) -1);

    /* convert line-feed and tabs into spaces */
    /* (this guarentees that there will be a space before the */
    /*  null character at the end of the string) */

    str[BIG_STRING-2] = ' ';
    str[BIG_STRING-1] = '\0';

    for (ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == '\t') {
            *ptr = ' ';
        }
        else if (*ptr == '\n') {
            *ptr = ' ';
            break;
        }
    }

    /* copy the line */
    for (ptr = str, ptr2 = str_orig; *ptr != '\0'; ptr++, ptr2++)
        *ptr2 = *ptr;
    *ptr2 = '\0';

    /* look to see if this is a comment line (first non-space is '#') */

    for (ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == '#') {
            ptr++;
            while (*ptr == ' ')
                ptr++;
            return (ptr);
        }
        else if (*ptr != ' ') {
            break;
        }
    }

    /* if we get here, we've got a non-comment line */

    /* strip off trailing comments */

    while (*ptr != '\0') {
        if (*ptr == '#') {
            *ptr++ = ' ';
            *ptr = '\0';
            break;
        }
        ptr++;
    }

    return (NULL);
}

int fetch_words ( void )
{
    char *ptr;

    /* allocate room for words if necessary */
    if (max_words == 0) {
        max_words = 20;
        words = (char **) malloc (sizeof (char *) * max_words);
    }

    /* find the words in the line */

    ptr = str;
    num_words = 0;

    while (*ptr != '\0') {
        /* jump over leading spaces */
        while (*ptr == ' ')
            ptr++;

        /* break if we reach the end */
        if (*ptr == '\0')
            break;

        /* allocate more room for words if necessary */
        if (num_words >= max_words) {
            max_words += 10;
            words = (char **) realloc (words, sizeof (char *) * max_words);
        }

        /* save pointer to beginning of word */
        words[num_words++] = ptr;

        /* jump over non-spaces */
        while (*ptr != ' ')
            ptr++;

        /* place a null character here to mark the end of the word */
        *ptr++ = '\0';
    }

    /* return the number of words */
    return (num_words);
}

string getPath(const string& filename)
{
    int i=filename.length()-1;
    bool found = false;
    while (i>=0 && !found)
    {
        if (filename[i]=='/' || filename[i]=='\\') found=true;
        else --i;
    }
    return filename.substr(0, i+1);
}

// llegeix tots els fitxers .mtl i els afegeix a matlib
void read_mtllib( char **words, int nwords, MaterialLib& matlib, 
                 const string& filename )
{
    string path = getPath(filename);
    for (int i = 0; i < nwords; i++) 
    {
        int size = strlen(words[i])-1;
        while (size && (words[i][size]=='\n' || words[i][size]=='\r') ) words[i][size--]=0;
        matlib.readMtl((path+words[i]).c_str());
    }
}

// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), tambe es llegeixen. 
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
//  

void Object::readObj(const char* filename, MaterialLib& matlib)
{
    char* locale = setlocale(LC_ALL,NULL);
    setlocale(LC_NUMERIC, "C");
    int currentMaterial = -1;  

    FILE *fp = fopen(filename,"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << filename << endl;
    }
    else {
        clear();
        while (true) 
        {
            char *comment_ptr = fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = words[0];

            if (!strcmp (first_word, "v")) 
            {
                if (nwords < 4) 
                {
                    fprintf (stderr, "Too few coordinates: '%s'", str_orig);
                    exit (-1);
                }
                float x = atof (words[1]);
                float y = atof (words[2]);
                float z = atof (words[3]);

                if (nwords == 5) 
                {
                    float w = atof (words[4]);
                    x/=w;
                    y/=w;
                    z/=w;
                }

                //addVertex(Vertex(Point(x,y,z)));
                vertices.push_back(Vertex(Point(x,y,z)));
            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                make_face (&words[1], nwords-1, currentMaterial);
            }
            // added 
            else if (!strcmp (first_word, "mtllib")) {
                read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                int size = strlen(words[1])-1;
                while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                currentMaterial = matlib.index(words[1]);
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }  
        }
    }
    updateNormals();
    computeBoundingBox();
    
    setlocale(LC_ALL, locale);
}

// Netela les dades de l'objecte per a poder carregar un nou model
void Object::clear()
{
    vertices.clear();
    faces.clear();
    _boundingBox.init(Point());
}
