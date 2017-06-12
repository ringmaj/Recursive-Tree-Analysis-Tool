#include "curve_eval.h"
#include <cmath>
# include <vector>




// Function to interpolate points for bezier curve

GsVec interpolate(float t, std::vector<GsVec>& ctrlpnts, float &x0, float &y0) {
	int numCtrlPts = ctrlpnts.size();

	std::vector<GsVec> secondaryPts;


	// Setup first trace
	if (numCtrlPts == 0)
	{
		// No points to interoplate
	}
	else if (numCtrlPts == 1)
	{
		// if only one point, return point
		return ctrlpnts[0];
	}
	else
	{

		// Find the distance between one point and the next. The interpolated point will the a ratio of that distance. x + (dist * t), y + (ydist * t)
		for (int i = 0; i < numCtrlPts - 1; i++)
		{
			float xDistance = ctrlpnts[i + 1].x - ctrlpnts[i].x;
			float yDistance = ctrlpnts[i + 1].y - ctrlpnts[i].y;

			float newX = ctrlpnts[i].x + (xDistance * t);
			float newY = ctrlpnts[i].y + (yDistance * t);
			float z = ctrlpnts[i].z;

			// Push interpolated points back to vector
			secondaryPts.push_back(GsVec(newX, newY, z));

		}

	}

	ctrlpnts.clear();

	for (int i = 0; i < secondaryPts.size(); i++)
		ctrlpnts.push_back(secondaryPts[i]);

	x0 = ctrlpnts[0].x;
	y0 = ctrlpnts[0].y;


	// Return interpolated point
	GsVec result = GsVec(ctrlpnts[0].x, ctrlpnts[0].x, ctrlpnts[0].z + 0.5f);



}



GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts)
{


	// Size
	int numCtrlPts = ctrlpnts.size();

	// Vector to store interpolated points
	std::vector<GsVec> secondaryPts;


	// Setup first trace

	if (numCtrlPts == 0)
	{
		GsVec empty = GsVec(0.0, 0.0, 0.0);
		return empty;
		// No points to interpolate
	}
	else if (numCtrlPts == 1)
	{
		// if only one point, return point
		return ctrlpnts[0];
	}
	else
	{


		// Find the distance between one point and the next. The interpolated point will the a ratio of that distance. x + (dist * t), y + (ydist * t)
		for (int i = 0; i < numCtrlPts - 1; i++)
		{
			float xDistance = ctrlpnts[i + 1].x - ctrlpnts[i].x;
			float yDistance = ctrlpnts[i + 1].y - ctrlpnts[i].y;

			float newX = ctrlpnts[i].x + (xDistance * t);
			float newY = ctrlpnts[i].y + (yDistance * t);
			float z = ctrlpnts[i].z;


			// Push interpolated points back to vector
			secondaryPts.push_back(GsVec(newX, newY, z));


		}


	}



	GsVec result;

	float x = 0.0;
	float y = 0.0;


	// SecondaryPts holds our interpolated points. Interpolate until only one point left. That is the interpolated point found.
	while (secondaryPts.size() != 1)
		result = interpolate(t, secondaryPts, x, y);


	// Return interpolated point
	result = GsVec(x, y, 0.0f);
	return result;


}

