#include "base_plate.h"


BasePlate::BasePlate()
{
	_numpoints = 0;
}

void BasePlate::init()
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


void BasePlate::createBox(double x0, double y0, double z0, double length, double width, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color) {

	//int i;


	// Side 1
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 + width / 2));

	// Side 2
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 - width / 2));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 - width / 2));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 - width / 2));


	// Side 3
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 - width / 2));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 - width / 2));

	// Side 4
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 - width / 2));

	// Side 5
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 - width / 2));

	P.push(GsVec(x0 + length / 2, y0 + height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, z0 + width / 2));

	// Side 6
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 + width / 2));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 + width / 2));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 - width / 2));

	P.push(GsVec(x0 - length / 2, y0 + height / 2, z0 - width / 2));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 - width / 2));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, z0 + width / 2));



	//for (i = 0; i < 36; i++) C.push(color);

}

void BasePlate::build()
{

	//createBox(double x0, double y0, double z0, double length, double width, double height, std::vector<GsVec> &P, std::vector<GsColor> &C, GsColor color)


	// Truck image
	//https://www.windwardboardshop.com/wp/wp-content/uploads/2013/06/xunderstanding-skateboard-trucks-feature.jpg.pagespeed.ic.M_16T9B0gm.webp

	// Make Baseplate
	createBox(0.0, 0.0, 0.0, 0.15, 0.17, 0.010, P, C, GsColor::green);
	createBox(0.0, 0.020, 0.0, 0.075, 0.17, 0.06, P, C, GsColor::green);


	createBox(0.0, 0.0, 0.5, 0.15, 0.17, 0.010, P, C, GsColor::green);
	createBox(0.0, 0.020, 0.5, 0.075, 0.17, 0.06, P, C, GsColor::green);


	// int i = 0;
	//while(i < P.size())
	//{
	//	int numNormals = 0;
	//	GsVec allNormals;

	//	for (int j = 0; j < P.size(); j = j + 3)
	//	{
	//		float pt1 = (P[i] - P[j]).norm();
	//		float pt2 = (P[i] - P[j+1]).norm();
	//		float pt3 = (P[i] - P[j+2]).norm();

	//		if (pt1 == 0 || pt2 == 0 || pt3 == 0)
	//		{
	//			allNormals += normal(P[j], P[j + 1], P[j + 2]);
	//			numNormals++;

	//		}
	//			
	//	}

	//	//cout << "num normals: " << numNormals << endl;

	//	// Push normal for P[i]
	//	GsVec vertexNormal = allNormals / numNormals;
	//	N.push(vertexNormal);


	//	i++;
	//}

	GsVec norm;
	for (int i = 0; i <= P.size(); i = i + 6)
	{

		//	GsVec n = cross(P[i + 1] - P[i], P[i + 2] - P[i]);
		GsVec n = normal(P[i], P[i + 1], P[i + 2]);

		N.push(n);
		N.push(n);
		N.push(n);
		N.push(n);
		N.push(n);
		N.push(n);

	}


	C.size(P.size()); C.setall(GsColor::gray);
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

void BasePlate::draw(const GsMat& tr, const GsMat& pr, const GsLight& l)
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
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.
}

