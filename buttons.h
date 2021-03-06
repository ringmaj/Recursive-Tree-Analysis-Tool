
// Ensure the header file is included only once in multi-file projects
#ifndef BUTTONS_H
#define BUTTONS_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"



// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class Buttons : public GlObjects
{
private:
	GlShader _vsh, _fsh;
	GlProgram _prog;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // colors
	int _numpoints;     // saves the number of points

	float vertScrollYPos;



public:
	Buttons();
	void init();
	float getVertScrollY() { return vertScrollYPos; }
	void setVertScrollY(float y) {  vertScrollYPos = y; }
	void createBox(double x0, double y0, double length, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color);
	void build();
	void draw(GsMat& tr, GsMat& pr);
};

#endif // BUTTONS_H
