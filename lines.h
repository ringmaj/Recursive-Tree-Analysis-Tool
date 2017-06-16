
// Ensure the header file is included only once in multi-file projects
#ifndef LINES_H
#define LINES_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class Lines : public GlObjects
{
private:
	GlShader _vsh, _fsh;
	GlProgram _prog;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // colors
	int _numpoints;     // saves the number of points

public:
	Lines();
	void init();
	void createBox(double x0, double y0, double length, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color);
	void build();
	void draw(GsMat& tr, GsMat& pr);
};

#endif // LINES_H
