#ifndef CURVE_EVAL
#define CURVE_EVAL

#include "gsim/gs.h"
#include "gsim/gs_array.h"
#include "gsim/gs_color.h"
#include "gsim/gs_vec.h"

GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts);


#endif CURVE_EVAL
