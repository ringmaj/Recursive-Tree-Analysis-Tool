
// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODEL_H
#define SO_MODEL_H

// Include needed header files
# include <gsim/gs_mat.h>
# include <gsim/gs_light.h>
# include <gsim/gs_color.h>
# include <gsim/gs_material.h>
# include <gsim/gs_array.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoSuperquadric : public GlObjects
 { private :
    GlShader _vshgou, _fshgou, _vshphong, _fshphong;
    GlProgram _proggouraud, _progphong;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsColor> C; // diffuse colors per face
    GsArray<GsVec>   N; // normals
    GsMaterial _mtl;
    int _numpoints;     // just saves the number of points
    bool _phong;
    bool _flatn;
   public :
    GsArray<GsVec> NL; // normal lines computed in last build
   public :
    SoSuperquadric ();
    void init ();
    void flat ( bool b ) { _flatn=b; }
    void phong ( bool b ) { _phong=b; }
    void build ( float r, float s, float t, float a, float b, float c );
    void draw ( const GsMat& tr, const GsMat& pr, const GsLight& l );
 };

#endif // SO_MODEL_H
