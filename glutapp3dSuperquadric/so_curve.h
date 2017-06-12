// Ensure the header file is included only once in multi-file projects
#ifndef SO_CURVE_H
#define SO_CURVE_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_material.h>
# include <gsim/gs_light.h>
# include "ogl_tools.h"


// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class Curve : public GlObjects
{
private:

	GlShader _vshgou, _fshgou, _vshphong, _fshphong;
	GlProgram _proggouraud, _progphong;

	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // color
	GsArray<GsVec> N; // normal

	GsMaterial _mtl;
	int _numpoints;     // just saves the number of points


public:
	Curve();
	void init();
	GsVec findCenterPoint(GsVec A, GsVec B, GsVec C);
	GsVec findP(GsVec A, GsVec B, GsVec C);
	GsVec findQ(GsVec A, GsVec B, GsVec C);
	bool isCtrlPt(float x, const GsArray<GsVec>& ctrlpnts);
	void build(float interpDiff, int curve, const GsArray<GsVec>& ctrlpnts);
	void draw(const GsMat& tr, const GsMat& pr, const GsLight& l);
};

#endif // SO_AXIS_H
