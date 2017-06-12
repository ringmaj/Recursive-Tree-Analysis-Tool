
# include "so_curve.h"
# include "curve_eval.h"

Curve::Curve()
{
	_numpoints = 0;
}

void Curve::init()
{

	_vshphong.load_and_compile(GL_VERTEX_SHADER, "../vsh_mcol_phong.glsl");
	_fshphong.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_mcol_phong.glsl");
	_progphong.init_and_link(_vshphong, _fshphong);

	// Define buffers needed:
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(3);       // will use 3 buffers

	_progphong.uniform_locations(9); // will send 9 variables
	_progphong.uniform_location(0, "vTransf");
	_progphong.uniform_location(1, "vProj");
	_progphong.uniform_location(2, "lPos");
	_progphong.uniform_location(3, "la");
	_progphong.uniform_location(4, "ld");
	_progphong.uniform_location(5, "ls");
	_progphong.uniform_location(6, "ka");
	_progphong.uniform_location(7, "ks");
	_progphong.uniform_location(8, "sh");
}

// build may be called everytime the object changes (not the case for this axis object):

bool Curve::isCtrlPt(float x, const GsArray<GsVec>& ctrlpnts)
{
	for (int i = 0; i < ctrlpnts.size(); i++)
	{
		if (x == ctrlpnts[i].x)
			return true;
	}

	std::cout << "FALSE ";
	return false;
}


GsVec Curve::findCenterPoint(GsVec A, GsVec B, GsVec C)
{
	float x;
	float y; 
	float z;

	x = (A.x + B.x + C.x) / 3;
	y = (A.y + B.y + C.y) / 3;
	z = (A.z + B.z + C.z) / 3;

	return GsVec(x, y, z);
}


GsVec Curve::findP(GsVec A, GsVec B, GsVec C)
{
	float x;
	float y;
	float z;

	x = (A.x + B.x + C.x) / 3;
	y = (A.y + B.y + C.y) / 3;
	z = (A.z + B.z + C.z) / 3;

	// Find highest point
	float highest = A.y;
	if (B.y >= A.y) highest = B.y;
	if (C.y >= B.y) highest = C.y;

	float delta = highest - y;



	return GsVec(x, y - (delta * 0.75f), z);
}


GsVec Curve::findQ(GsVec A, GsVec B, GsVec C)
{
	float x;
	float y;
	float z;

	x = (A.x + B.x + C.x) / 3;
	y = (A.y + B.y + C.y) / 3;
	z = (A.z + B.z + C.z) / 3;

	// find leftmost point
	float highest = A.x;
	if (B.x >= A.x) highest = B.x;
	if (C.x >= B.x) highest = C.x;

	float delta = highest - x;

	return GsVec(x + (highest * 0.75f), y, z);
}


//interpDiff - the difference in t for evaluating bezier segments
void Curve::build(float interpDiff, int curve, const GsArray<GsVec>& ctrlpnts)
{

	// Select curve, 0 = none, 1 = Lagrange, 2 = Bezier

	int n = ctrlpnts.size() - 1;


	if (curve == 2)
	{
		float t = 0.0;

		while (t <= 1.0f)
		{
			GsVec point = eval_bezier(t, ctrlpnts);
			P.push(point);

			point = eval_bezier(t + interpDiff, ctrlpnts);
			P.push(point);
			t = t + interpDiff;
		}

		t = 1.0;
		P.push(eval_bezier(t, ctrlpnts));

	}


	







		
		// Push normals

		// Arbitrary points P and Q
		GsVec pointP;
		GsVec pointQ;

		GsVec normals;

		


		// For each point, find vertex normal and push
		for (int i = 0; i < P.size(); i++)
		{
			
			for (int j = 0; j < P.size(); j++)
			{

				// Find all triangles that share same vertex, calculate normals for each and average
				if (P[i] == P[j])
				{
					// Select 3 points to create vectors
					GsVec center = findCenterPoint(P[i], P[i +1], P[i +2]);
					GsVec pt1 = normal(P[i], P[i + 1], P[i + 2]);
					GsVec pt2 = findP(P[i], P[i + 1], P[i + 2]);


				/*	pointP = pt1 - center;
					pointQ = pt2 - center;*/

					pointP = GsVec(center.x - 0.01f, center.y, center.z);
					pointQ = GsVec(center.x , center.y - 0.01f, center.z);

					// Set normal and push back
					normals = cross(pointP, pointQ);

					normals = normals * -1;
					N.push(normals);

				}
			}




		

	}


	C.size(P.size()); C.setall(GsColor::blue);
	_mtl.specular.set(255, 255, 255);
	_mtl.shininess = 20; // increase specular effect

						// send data to OpenGL buffers:
	glBindVertexArray(va[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*N.size(), N.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(gsbyte)*C.size(), C.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);


	glBindVertexArray(0); // break the existing vertex array object binding.
						  // save size so that we can free our buffers and later draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.capacity(0); C.capacity(0); N.capacity(0);
}

// draw will be called everytime we need to display this object:
void Curve::draw(const GsMat& tr, const GsMat& pr, const GsLight& l)
{
	float f[4];
	float sh = (float)_mtl.shininess;
	if (sh<0.001f) sh = 64;

	
		glUseProgram(_progphong.id);
		glUniformMatrix4fv(_progphong.uniloc[0], 1, GL_FALSE, tr.e);
		glUniformMatrix4fv(_progphong.uniloc[1], 1, GL_FALSE, pr.e);
		glUniform3fv(_progphong.uniloc[2], 1, l.pos.e);
		glUniform4fv(_progphong.uniloc[3], 1, l.amb.get(f));
		glUniform4fv(_progphong.uniloc[4], 1, l.dif.get(f));
		glUniform4fv(_progphong.uniloc[5], 1, l.spe.get(f));
		glUniform4fv(_progphong.uniloc[6], 1, _mtl.ambient.get(f));
		glUniform4fv(_progphong.uniloc[7], 1, _mtl.specular.get(f));
		glUniform1fv(_progphong.uniloc[8], 1, &sh);
	

	glBindVertexArray(va[0]);
	glDrawArrays(GL_LINES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.

}

