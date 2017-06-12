
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CLOCK_H
#define SO_CLOCK_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoClock : public GlObjects
{
private:
	GlShader _vsh, _fsh;
	GlProgram _prog;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // colors
	int _numpoints;     // saves the number of points

public:
	SoClock();
	void init();
	void build(float t);
	void draw(GsMat& tr, GsMat& pr);
};

#endif // SO_CLOCK_H
