
# include "lines.h"
# include <string>
# include <freeglut\freeglut.h>

#   define  GLUT_STROKE_ROMAN               ((void *)0x0000)
#   define  GLUT_STROKE_MONO_ROMAN          ((void *)0x0001)
#   define  GLUT_BITMAP_9_BY_15             ((void *)0x0002)
#   define  GLUT_BITMAP_8_BY_13             ((void *)0x0003)
#   define  GLUT_BITMAP_TIMES_ROMAN_10      ((void *)0x0004)
#   define  GLUT_BITMAP_TIMES_ROMAN_24      ((void *)0x0005)
#   define  GLUT_BITMAP_HELVETICA_10        ((void *)0x0006)
#   define  GLUT_BITMAP_HELVETICA_12        ((void *)0x0007)
#   define  GLUT_BITMAP_HELVETICA_18        ((void *)0x0008)

Lines::Lines()
{
	_numpoints = 0;
}

// init is called only once:
void Lines::init()
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

void Lines::createBox(double x0, double y0, double length, double height, GsArray<GsVec> &P, GsArray<GsColor> &C, GsColor color)
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

//void drawText(const char *text, int length, int x, int y) {
//	glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
//	double matrix[16]; // 16 doubles in stack memory
//	glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
//	glLoadIdentity(); // reset PROJECTION matrix to identity matrix
//	glOrtho(0, 800, 0, 600, -5, 5); // orthographic perspective
//	glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
//	glLoadIdentity(); // reset it to identity matrix
//	glPushMatrix(); // push current state of MODELVIEW matrix to stack
//	glLoadIdentity(); // reset it again. (may not be required, but it my convention)
//	glRasterPos2i(x, y); // raster position in 2D
//	for (int i = 0; i<length; i++) {
//		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]); // generation of characters in our text with 9 by 15 GLU font
//	}
//	glPopMatrix(); // get MODELVIEW matrix value from stack
//	glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
//	glLoadMatrixd(matrix); // reset
//	glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
//}


// build may be called everytime the object changes (not the case for this axis object):
void Lines::build()
{



	float line_incr = 0.9 / 5;
	float startPos = 0.15 + line_incr;

	GsColor tableLineColor = GsColor(119, 34, 34);
	GsColor tableTitleColor = GsColor(66, 0, 0);

	while (startPos <= 0.95)
	{
		P.push(GsVec(startPos, -0.85f, 0.0f));
		P.push(GsVec(startPos, 0.913f, 0.0f));
		C.push(tableLineColor);
		C.push(tableLineColor);

		P.push(GsVec(startPos, 1.075f, 0.0f));
		P.push(GsVec(startPos, 0.5f, 0.0f));
		C.push(GsColor::white);
		C.push(GsColor::white);





		startPos += line_incr;

		std::cout << "startPos: " << startPos << std::endl;
	}


	//for(int i = 0; i < )

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
void Lines::draw(GsMat& tr, GsMat& pr)
{
	// Prepare program:
	glUseProgram(_prog.id);
	glUniformMatrix4fv(_prog.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_prog.uniloc[1], 1, GL_FALSE, pr.e);

	// Draw:
	glBindVertexArray(va[0]);
	glDrawArrays(GL_LINES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.
}

