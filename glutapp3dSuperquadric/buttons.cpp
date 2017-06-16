
# include "buttons.h"

Buttons::Buttons()
{
	_numpoints = 0;
	vertScrollYPos = 0.7;
	
}

// init is called only once:
void Buttons::init()
{
	// Build program:
	_vsh.load_and_compile(GL_VERTEX_SHADER, "../vsh_mcol_flat.glsl");
	_fsh.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_flat.glsl");
	_prog.init_and_link(_vsh, _fsh);

	// Define buffers needed:
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	_prog.uniform_locations(2); // will send 2 variables: the 2 matrices below
	_prog.uniform_location(0, "vTransf");
	_prog.uniform_location(1, "vProj");

}

void Buttons::createBox(double x0, double y0, double length, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color)
{
	int i;

	P.push(GsVec(x0 - length / 2, y0 + height / 2, 0.0));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, 0.0));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, 0.0));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, 0.0));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, 0.0));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, 0.0));

	for (int i = 0; i < 6; i++) C.push(color);
}

// build may be called everytime the object changes (not the case for this axis object):
void Buttons::build()
{

	// Tree branch width slider
	float x = -0.12;
	createBox(x, -0.80, 0.05, 0.03, P, C, GsColor(92, 150, 69));
	// slider vert bar
	createBox(x, -0.80, 0.01, 0.19, P, C, GsColor(114, 110, 110));

	// Tree branch height slider
	x = -0.05;
	createBox(x, -0.80, 0.05, 0.03, P, C, GsColor(92, 150, 69));
	// slider vert bar
	createBox(x, -0.80, 0.01, 0.19, P, C, GsColor(114, 110, 110));




	// scrollbar grip lines
	/*GsColor gripLinesColor = GsColor(248, 97, 37);
	createBox(-0.5, -1.0, 0.01, 0.05, P, C, gripLinesColor);
	createBox(-0.5 + 0.02, -1.0, 0.01, 0.05, P, C, gripLinesColor);
	createBox(-0.5 - 0.02, -1.0, 0.01, 0.05, P, C, gripLinesColor);*/
	



	// scrollbar grip lines
	 x = 0.025;
	//createBox(x, 0.763, 0.02, 0.01, P, C, gripLinesColor);
	//createBox(x, 0.763 + 0.025, 0.02, 0.01, P, C, gripLinesColor);
	//createBox(x , 0.763 - 0.025, 0.02, 0.01, P, C, gripLinesColor);

	// Vertical scrollbar
	createBox(x, vertScrollYPos, 0.04, 0.3, P, C, GsColor(191, 191, 187));
	// Horizontal scrollbar
	createBox(-0.5, -0.97, 0.3, 0.06, P, C, GsColor(191, 191, 187));


	// scrollbar background
	createBox(x, -0.04, 0.04, 1.9, P, C, GsColor(231, 235, 229));
	createBox(-0.5, -0.965, 1.1, 0.06, P, C, GsColor(231, 235, 229));


//	createBox(0, 0, 2.308, 0.06, P, C, GsColor(0, 0, 0));
	//createBox(0, 0, 0.06, 2.3, P, C, GsColor(0, 0, 0));


	 x = 0.28;
	float y = -0.52;
	for (int i = 0; i < 3; i++)
	{
		createBox(x, y, 0.15, 0.1, P, C, GsColor(243, 235, 231));
		x += (0.15 + 0.01);
	}

	// T(x n)
	createBox(x + 0.08, y, 0.25, 0.1, P, C, GsColor(243, 235, 231));

	 x = 0.28;
	 y -= 0.12;
	for (int i = 0; i < 3; i++)
	{
		createBox(x, y, 0.15, 0.1, P, C, GsColor(243, 235, 231));
		x += (0.15 + 0.01);
	}

	// division
	createBox(x + 0.08, y, 0.25, 0.1, P, C, GsColor(243, 235, 231));

	 x = 0.28;
	 y -= 0.12;

	 // reset button
	 createBox(x + 0.56, y - 0.12, 0.25, 0.1, P, C, GsColor(139, 39, 39));

	for (int i = 0; i < 3; i++)
	{
		createBox(x, y, 0.15, 0.1, P, C, GsColor(243, 235, 231));
		if (i == 1) {
			createBox(x, y - 0.12, 0.15, 0.1, P, C, GsColor(243, 235, 231));
			createBox(x + (0.15 + 0.01), y - 0.12, 0.15, 0.1, P, C, GsColor(243, 235, 231));
			createBox(x - (0.15 + 0.01), y - 0.12, 0.15, 0.1, P, C, GsColor(68, 139, 39));
		}
			

		x += (0.15 + 0.01);
	}

	// +
	createBox(x + 0.08, y, 0.25, 0.1, P, C, GsColor(243, 235, 231));

	// calculator button background
	createBox(0.6, -0.7, 0.82, 0.52, P, C, GsColor(220, 208, 203));
	createBox(0.6, -0.7, 0.85, 0.55, P, C, GsColor(200, 188, 182));


	// Equation button background
	createBox(-0.55, -0.80, 0.73, 0.17, P, C, GsColor(220, 208, 203));
	createBox(-0.55, -0.80, 0.76, 0.20, P, C, GsColor(200, 188, 182));


	//change


	// Depth Arrows
	GsColor arrowColor = GsColor(246, 144, 9);
	 x = -0.98f;
	P.push(GsVec(x, -0.90f, 0.0f));
	P.push(GsVec(x - 0.04f ,-0.82f, 0.0f));
	P.push(GsVec(x + 0.04f, -0.82f, 0.0f));

	P.push(GsVec(x, -0.70f, 0.0f));
	P.push(GsVec(x - 0.04f, -0.80f, 0.0f));
	P.push(GsVec(x + 0.04f, -0.80f, 0.0f));

	for (int i = 0; i < 6; i++)
		C.push(arrowColor);


	


	

	
	// send data to OpenGL buffers:
	glBindVertexArray(va[0]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(gsbyte)*C.size(), C.pt(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glBindVertexArray(0); // break the existing vertex array object binding.

						  // save size so that we can free our buffers and later draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.capacity(0); C.capacity(0);
}

// draw will be called everytime we need to display this object:
void Buttons::draw(GsMat& tr, GsMat& pr)
{
	// Prepare program:
	glUseProgram(_prog.id);
	glUniformMatrix4fv(_prog.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_prog.uniloc[1], 1, GL_FALSE, pr.e);

	// Draw:
	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.
}

