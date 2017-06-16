
# include "interface.h"


Interface::Interface()
{
	_numpoints = 0;
}

// init is called only once:
void Interface::init()
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

void Interface::createBox(double x0, double y0, double length, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color)
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
void Interface::build()
{
	
	// Arrows
	GsColor arrowColor = GsColor(246, 144, 9);
	createBox(-1.1, -0.037, 0.025, 1.83, P, C, arrowColor);
	P.push(GsVec(-1.1, -1.0, 0.0));
	P.push(GsVec(-1.14, -0.95, 0.0));
	P.push(GsVec(-1.06, -0.95, 0.0));

	P.push(GsVec(-1.1, 0.90, 0.0));
	P.push(GsVec(-1.14, 0.85, 0.0));
	P.push(GsVec(-1.06, 0.85, 0.0));

	for (int i = 0; i < 6; i++)
		C.push(arrowColor);


	GsColor shadow = GsColor(66, 0, 0);
	createBox(-1.09, -0.037, 0.025, 1.83, P, C, shadow);
	P.push(GsVec(-1.09, -1.01, 0.0));
	P.push(GsVec(-1.13, -0.96, 0.0));
	P.push(GsVec(-1.05, -0.96, 0.0));

	P.push(GsVec(-1.09, 0.89, 0.0));
	P.push(GsVec(-1.13, 0.84, 0.0));
	P.push(GsVec(-1.05, 0.84, 0.0));

	for (int i = 0; i < 6; i++)
		C.push(shadow);

	//createBox(-1.1, -0.037, 0.035, 1.83, P, C, GsColor::white);


	// Recursive tree title box
	createBox(-0.5, 1.0, 1.1, 0.15, P, C, GsColor(248, 97, 37));
	createBox(-0.5, 0.99, 1.1, 0.15, P, C, GsColor(213, 61, 1));

	// Box for recursive tree
	//createBox(-0.5, 0.0, 1.1, 2.0, P, C, GsColor::white);
	// Shadow
	//createBox(-0.47, -0.01, 1.1, 2.07, P, C, GsColor(66, 0, 0));


	// Table tree title box


	createBox(0.6, 1.0, 0.9, 0.15, P, C, GsColor(248, 97, 37));
	createBox(0.6, 0.99, 0.9, 0.15, P, C, GsColor(213, 61, 1));

	// Box for Table 
	createBox(0.6, 0.0, 0.9, 2.0, P, C, GsColor::white);
	// Shadow
	createBox(0.63, -0.01, 0.9, 2.07, P, C, GsColor(66, 0, 0));





	// Outer Boundaries, cover
	createBox(-0.4 + 1.1, 0.0, 1.24, 2.1, P, C, GsColor(139, 39, 39));
	createBox(-0.5 - 1.1, 0.0, 1.1, 2.0, P, C, GsColor(139, 39, 39));
	createBox(-0.522 - 1.1, -2.0, 1.2, 2.0, P, C, GsColor(139, 39, 39));

	// bottom
    createBox(0.0, -2.0, 2.5, 1.91, P, C, GsColor(139, 39, 39));
	createBox(0.0, 2.0, 2.5, 2.05, P, C, GsColor(139, 39, 39));


	// equation box cover boundary
	createBox(-0.5, -0.8, 1.1, 0.3, P, C, GsColor::white);
	
	//change
	// 

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
void Interface::draw(GsMat& tr, GsMat& pr)
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

