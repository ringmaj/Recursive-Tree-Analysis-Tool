// Ensure the header file is included only once in multi-file projects
#ifndef SO_GRAPH_H
#define SO_GRAPH_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoGraph : public GlObjects
{
private:
	GlShader _vsh, _fsh;
	GlProgram _prog;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // colors
	int _numpoints;     // saves the number of points

public:
	SoGraph();
	void init();
	void createBox(double x0, double y0, double length, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color);
	void build();
	void draw(GsMat& tr, GsMat& pr);
};

#endif // SO_GRAPH_H
