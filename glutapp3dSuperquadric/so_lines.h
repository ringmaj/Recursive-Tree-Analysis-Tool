
// Ensure the header file is included only once in multi-file projects
#ifndef SO_LINES_H
#define SO_LINES_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoLines : public GlObjects
 { private :
    GlShader _vsh, _fsh;
    GlProgram _prog;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsColor> C; // colors
    int _numelements;     // saves the number of points

   public :
    SoLines ();
    void init ();
    void build ( const GsArray<GsVec>& lines, const GsColor& c );
    void draw ( GsMat& tr, GsMat& pr );
 };

#endif // SO_LINES_H
