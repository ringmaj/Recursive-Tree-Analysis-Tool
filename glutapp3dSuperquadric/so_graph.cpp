
# include "so_graph.h"

SoGraph::SoGraph()
{
	_numpoints = 0;
}

// init is called only once:
void SoGraph::init()
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

void SoGraph::createBox(double x0, double y0, double length,double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color) {

	int i;


	// Side 1
	P.push(GsVec(x0 - length / 2, y0 + height / 2, 0.0));
	P.push(GsVec(x0 - length / 2, y0 - height / 2, 0.0));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, 0.0));
	P.push(GsVec(x0 + length / 2, y0 - height / 2, 0.0));
	P.push(GsVec(x0 + length / 2, y0 + height / 2, 0.0));
	P.push(GsVec(x0 - length / 2, y0 + height / 2, 0.0));



	for (i = 0; i < 6; i++) C.push(color);



}

// build may be called everytime the object changes (not the case for this axis object):
void SoGraph::build()
{
	//createBox(double x0, double y0, double z0, double length, double width, double height, std::vector<GsVec> &P, std::vector<GsColor> &C, GsColor color)

	createBox(0.75, 0.8, 0.65, 0.40, P, C, GsColor::white);
	createBox(0.75, 0.3, 0.65, 0.40, P, C, GsColor::white);
	createBox(0.75, -0.2, 0.65, 0.40, P, C, GsColor::white);
	createBox(0.75, -0.7, 0.65, 0.40, P, C, GsColor::white);

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
void SoGraph::draw(GsMat& tr, GsMat& pr)
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

