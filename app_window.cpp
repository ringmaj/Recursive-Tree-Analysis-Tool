
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"
#include <string>

# define M_PI           3.14159265358979323846 

using namespace std;

void printbitmap(void* font, const string msg, double x, double y)
{
	/*glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);*/
	glUseProgram(0);

	glColor3f(0.0f, 0.0f, 0.0f);
	//glColor3f(1, 1, 1);
	glDisable(GL_LIGHTING);
	glRasterPos2d(x, y);
	for (string::const_iterator ii = msg.begin(); ii != msg.end(); ++ii)
	{
		glutBitmapCharacter(font, *ii);
	}
}


AppWindow::AppWindow(const char* label, int x, int y, int w, int h)
	:GlutWindow(label, x, y, w, h)
{
	initPrograms();
	addMenuEntry("Option 0", evOption0);
	addMenuEntry("Option 1", evOption1);
	_viewaxis = true;
	_fovy = GS_TORAD(60.0f);
	_rotx = _roty = 0;
	_w = w;
	_h = h;
	_pickprec = 0.035f;
	_pickprec = 0.035f;
	_poly1.select(-1, _pickprec);
	_oktodrag = false;

	_eye.set(0, 0, 2);
	_center.set(0, 0, 0);
	_up.set(0, 1, 0);
	_aspect = 1.0f; _znear = 0.1f; _zfar = 50.0f;

	
	depth = 0;
	recursion = 2;
	vertScroll = false;

	
	// Sample input, has two nodes for the equation
	recursiveNode input;
	input.recursions = 3;
	input.num1 = 2;
	input.num2 = 4;

	equationArray.push(input);


	input.recursions = 2;
	input.num1 = 1;
	input.num2 = 3;

	equationArray.push(input);

	input.recursions = 4;
	input.num1 = 1;
	input.num2 = 4;

	equationArray.push(input);

}

float degToRadian(float deg)
{
	return deg * (M_PI / 180);
}

void AppWindow::initPrograms()
{
	// Load your shaders and link your programs here:
	_flatvsh.load_and_compile(GL_VERTEX_SHADER, "../vsh_mcol_flat.glsl");
	_flatfsh.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_flat.glsl");
	_prog.init_and_link(_flatvsh, _flatfsh);


	// Init my scene objects:
	_axis.init();
	_interface.init();
	_lines.init();
	_recursiveTree.init();
	_buttonsInput.init();
	_scrollBox.init();

	GsVec pt1 = GsVec(0.425, 0.60, 0.0);
	GsVec pt2 = GsVec(0.75, 1.0, 0.0);
	GsVec pt3 = GsVec(1.075, 0.60, 0.0);

	_poly1.init( GsColor::darkblue, pt1, pt2, pt3, GsColor::darkred);


	 pt1 = GsVec(0.425, 0.10, 0.0);
	 pt2 = GsVec(0.75, 0.30, 0.0);
	 pt3 = GsVec(1.075, 0.50, 0.0);

	_poly2.init(GsColor::darkblue, pt1, pt2, pt3, GsColor::darkred);



	pt1 = GsVec(0.425, -0.40, 0.0);
	pt2 = GsVec(0.75, -0.20, 0.0);
	pt3 = GsVec(1.075, 0.0, 0.0);

	_poly3.init(GsColor::darkblue, pt1, pt2, pt3, GsColor::darkred);



	pt1 = GsVec(0.425, -0.90, 0.0);
	pt2 = GsVec(0.75, -0.70, 0.0);
	pt3 = GsVec(1.075, -0.50, 0.0);

	_poly4.init(GsColor::darkblue, pt1, pt2, pt3, GsColor::darkred);



	// set light :
	_light.set(GsVec(0, -10, 0), GsColor(90, 90, 90, 255), GsColor::white, GsColor::white);


	// Build some of my scene objects:
	_axis.build(1.0f); // axis has radius 1.0
	_buttonsInput.build();
	
	_interface.build();
	_lines.build();
	_scrollBox.build();

}

// mouse events are in window coordinates, but your 2D scene is in [-1,1]x[-1,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene(const GsVec2& v)
{

	GsVec2 out = GsVec2((2.0f*(v.x / float(_w))) - 1.0f,
		1.0f - (2.0f*(v.y / float(_h))));

	////std::cout << "x: " << out.x << " \t   y: " << out.y << std::endl;




	// GsVec2 is a lighteweight class suitable to return by value:
	return GsVec2((2.0f*(v.x / float(_w))) - 1.0f,
		1.0f - (2.0f*(v.y / float(_h))));
}

// Called every time there is a window event
void AppWindow::glutKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	// Increase/decrease branch width
	case 27: exit(1); break;
	case 'a': _recursiveTree.setBranchWidth(_recursiveTree.getBranchWidth() - 0.1); //std::cout << "Branch Width:  " << _recursiveTree.getBranchWidth() << std::endl;  redraw(); break;
	case 'q': _recursiveTree.setBranchWidth(_recursiveTree.getBranchWidth() + 0.1); //std::cout << "Branch Width:  " << _recursiveTree.getBranchWidth() << std::endl;  redraw(); break;


	case ' ': _buttonsInput.setVertScrollY(0.0); redraw();  break;
	}
}

void AppWindow::glutSpecial(int key, int x, int y)
{
	bool rd = true;
	const float incr = GS_TORAD(2.5f);
	const float inct = 0.05f;
	const float incf = 0.05f;
	bool ctrl = glutGetModifiers()&GLUT_ACTIVE_CTRL ? true : false;
	bool alt = glutGetModifiers()&GLUT_ACTIVE_ALT ? true : false;
	////std::cout<<ctrl<<gsnl;

	switch (key)
	{
	case GLUT_KEY_LEFT:      if (recursion > 2) recursion--; std::cout << "recursion: " << recursion << std::endl;  redraw(); if (ctrl)_roty -= incr; else if (alt)_trans.x -= inct; std::cout << "left" << std::endl; break;
	case GLUT_KEY_RIGHT:     recursion++; std::cout << "recursion: " << recursion << std::endl;  redraw(); if (ctrl)_roty += incr; else if (alt)_trans.x += inct; break;
	case GLUT_KEY_UP:        if (depth >= 1) depth--; std::cout << "depth: " << depth << std::endl;  if (ctrl)_rotx += incr; else if (alt)_trans.y += inct; break;
	case GLUT_KEY_DOWN:      depth++; std::cout << "depth: " << depth << std::endl; if (ctrl)_rotx -= incr; else if (alt)_trans.y -= inct; break;
	case GLUT_KEY_PAGE_UP:   if (ctrl)_fovy -= incf; else if (alt)_trans.z += inct; break;
	case GLUT_KEY_PAGE_DOWN: if (ctrl)_fovy += incf; else if (alt)_trans.z -= inct; break;
	default: return; // return without rendering
	}
	if (rd) redraw(); // ask the window to be rendered when possible
}

static GsVec plane_intersect(const GsVec& c, const GsVec& n, const GsVec& p1, const GsVec& p2)
{
	GsVec coords = n;
	float coordsw = -dot(n, c);
	float nl = n.norm();
	if (nl == 0.0) return GsVec::null;
	coords /= nl; coordsw /= nl;
	float fact = dot(coords, p1 - p2);
	if (fact == 0.0) return GsVec::null;
	float k = (coordsw + dot(coords, p1)) / fact;
	return mix(p1, p2, k);
}

GsVec AppWindow::rayXYintercept(const GsVec2& v)
{
	// Recall that a mouse click in the screen corresponds to a ray traversing the 3D scene
	// Here we intersect this ray with the XY plane:
	GsVec m(windowToScene(v));
	GsVec p1(m.x, m.y, _znear); // mouse click in the near plane
	GsMat M(_spr); M.invert();
	p1 = M * p1;          // mouse click in camera coordinates
	GsVec p2 = p1 - _eye;
	p2.normalize();
	p2 *= (_zfar + _znear); // zfar and znear are >0
	p2 += p1;             // p1,p2 ray in camera coordinates
	M = _str; M.invert();
	p1 = M * p1;
	p2 = M * p2;          // p1,p2 ray in scene coordinates
	return plane_intersect(GsVec::null, GsVec::k, p1, p2); // intersect p1,p2 ray with XY plane
}

void AppWindow::glutMouse(int button, int state, int x, int y)
{
	GsVec m = rayXYintercept(GsVec2(x, y));
	////std::cout<<m<<gsnl;

	////std::cout << "x: " << x << "\t y: " << y << std::endl;


	// Check if a vertex is being selected, ie, if m is very close to a vertex:
	int i;
	const GsArray<GsVec>& V = _poly1.vertexarray();
	for (i = 0; i<V.size(); i++)
		if (dist(V[i], m) <= _pickprec) break;

	if (i<V.size()) // vertex selected
	{
		_poly1.select(i, _pickprec);
		_oktodrag = true;
		redraw();
	}


	// Check if a vertex is being selected, ie, if m is very close to a vertex:
	
	const GsArray<GsVec>& V1 = _poly2.vertexarray();
	for (i = 0; i<V1.size(); i++)
		if (dist(V1[i], m) <= _pickprec) break;

	if (i<V1.size()) // vertex selected
	{
		_poly2.select(i, _pickprec);
		_oktodrag = true;
		redraw();
	}


	const GsArray<GsVec>& V2 = _poly3.vertexarray();
	for (i = 0; i<V2.size(); i++)
		if (dist(V2[i], m) <= _pickprec) break;

	if (i<V2.size()) // vertex selected
	{
		_poly3.select(i, _pickprec);
		_oktodrag = true;
		redraw();
	}

	const GsArray<GsVec>& V3 = _poly4.vertexarray();
	for (i = 0; i<V3.size(); i++)
		if (dist(V3[i], m) <= _pickprec) break;

	if (i<V3.size()) // vertex selected
	{
		_poly4.select(i, _pickprec);
		_oktodrag = true;
		redraw();
	}



	else if (state == GLUT_UP)
	{ // At this point no selection was detected and we will add a new vertex:
		//_poly1.add(m);
		_oktodrag = false;
		redraw();
	}




}

void AppWindow::glutMotion(int x, int y)
{

	GsVec m = rayXYintercept(GsVec2(x, y));


	//std::cout << "x: " << m.x << " \t   y: " << m.y << std::endl;

	if (  ((m.x >= 0.005 && m.x <= 0.045) || vertScroll == true ) && m.y <= (_buttonsInput.getVertScrollY() + 0.15) && m.y >= (_buttonsInput.getVertScrollY() - 0.15))
	{
		if (m.y > (0.7 + 0.15))
		{
			_buttonsInput.setVertScrollY(0.7);
			redraw();
		}
		else
		{
			vertScroll = true;
			float yMove = abs(0.7 - _buttonsInput.getVertScrollY());
			//std::cout << "ymove: " << yMove << std::endl;
			_recursiveTree.setRootYPos(0.8 + yMove);
			//std::cout << "HIT" << std::endl;
			_buttonsInput.setVertScrollY(m.y);

			if (m.y > (_buttonsInput.getVertScrollY() + 0.15) || m.y < (_buttonsInput.getVertScrollY() - 0.15))
				vertScroll = false;

			redraw();
		}

		
		redraw();

	}



	
}

void AppWindow::glutMenu(int m)
{
	////std::cout << "Menu Event: " << m << std::endl;
}

void AppWindow::glutReshape(int w, int h)
{
	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);
	_w = w; _h = h;
}

void AppWindow::drawMenus()

{
//;

	glUseProgram(0);


	
	glColor3f(1.0f, 1.0f, 1.0f);

	// Recursive Tree title text
	glRasterPos2f(-0.55, 0.85);
	string output = "Recursive Tree";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());


	glColor3f(0.462f, 0.435f, 0.435f);
	glRasterPos2f(-0.75,-0.70);
	 output = "T(n) = ";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

















	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(0.185, 0.85);
	output = "Depth";
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)output.c_str());

	glRasterPos2f(0.32, 0.85);
	output = "S.P Size";
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)output.c_str());

	glRasterPos2f(0.475, 0.85);
	output = "# Nodes";
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)output.c_str());

	glRasterPos2f(0.62, 0.85);
	output = "WL/Node";
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)output.c_str());

	glRasterPos2f(0.765, 0.85);
	output = "Work Load";
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)output.c_str());


	// Depth table values

	glColor3f(0.0f, 0.0f, 0.0f);

	if (depth >= 0)
	{
		glRasterPos2f(0.190, 0.70);
		output = "0";
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

		glRasterPos2f(0.190, 0.45);
		output = "1";
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());
	}

	if (depth >= 1)
	{
		glRasterPos2f(0.190, 0.2);
		output = "2";
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());
	}

	if (depth >= 2)
	{
		glRasterPos2f(0.190, -0.05);
		output = "d";
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());
	}

	if (depth >= 3)
	{
		glRasterPos2f(0.190, -0.3);
		output = "D";
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());
	}

	if (depth >= 4)
	{
		glRasterPos2f(0.190, -0.3);
		output = "D";
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());
	}




	// Calculator values




	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(0.235, -0.78);
	output = "=";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(0.37, -0.78);
	 output = "0";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glColor3f(0.5f, 0.0f, 0.0f);
	glRasterPos2f(0.505, -0.78);
	output = "n";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());


	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(0.235, -0.675);
	output = "1";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.370, -0.675);
	output = "2";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.505, -0.675);
	output = "3";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.235, -0.570);
	output = "4";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.370, -0.570);
	output = "5";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.505, -0.570);
	output = "6";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.235, -0.465);
	output = "7";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.370, -0.465);
	output = "8";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.505, -0.465);
	output = "9";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());


	// Reset button
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(0.680, -0.78);
	output = "Reset";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	// Divide button
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(0.720, -0.67);
	output = "÷";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	// + button
	glRasterPos2f(0.720, -0.57);
	output = "+";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());

	glRasterPos2f(0.70, -0.47);
	output = "T(x)";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)output.c_str());


	// T(xn)

	//redraw();
}

void AppWindow::printTreeText()
{
	// Holds all nodes for each branch
	GsArray<node> nodeArr = _recursiveTree.getNodes();


	for (int i = 0; i < nodeArr.size(); i++)
	{

		string nodeString = "";

		if (nodeArr[i].y <= 0.88)
		{
			//nodeString += to_string(nodeArr[i].recursions);
			nodeString += "T(";
			nodeString += to_string(nodeArr[i].num1);
			nodeString += "/";
			nodeString += to_string(nodeArr[i].num2);
			nodeString += "n)";

			cout << "node: " << nodeString << endl;


			printbitmap(GLUT_BITMAP_HELVETICA_18, nodeString, nodeArr[i].x + 0.035f, nodeArr[i].y - 0.10f);
		}
	}


	//GsArray<GsVec> points = _recursiveTree.getPoints();

	////std::cout << points[0].y << std::endl;
	//if (points[0].y <= 0.81)
	//printbitmap(GLUT_BITMAP_HELVETICA_18,"T(n)", points[0].x + 0.035f, points[0].y -0.08f );






	//for (int i = 1; i < points.size(); i++)
	//{
	//	if (points[i].y < 0.75 && points[i].y > -0.5)
	//	{
	//		////std::cout << "space: " << abs((points[i].x + 0.06) - (points[i+1].x + 0.06)) << std::endl;
	//		if( abs((points[i].x + 0.06) - (points[i+1].x + 0.05)) > 0.05)
	//		printbitmap(GLUT_BITMAP_HELVETICA_18, "T(x)", points[i].x + 0.06f, points[i].y - 0.12f);
	//		else
	//			printbitmap(GLUT_BITMAP_HELVETICA_18, ".", points[i].x + 0.06f, points[i].y - 0.06f);
	//	}
	//	
	//}
	
}

// Prints the equation the user selects at the bottom of the screen
void AppWindow::printEquation()
{


	


	float x = -0.62;


	//Print all nodes in equation
	for (int i = 0; i < equationArray.size(); i++)
	{
		// Sample string for the node
		string nodeString = "4(3/2n)";

		// Number of recursions
		string s = std::to_string(equationArray[i].recursions);
		char const *pchar = s.c_str();
		nodeString[0] = pchar[0];

		// First Number
		 s = std::to_string(equationArray[i].num1);
		 pchar = s.c_str();
		nodeString[2] = pchar[0];

		// First Number
		s = std::to_string(equationArray[i].num2);
		pchar = s.c_str();
		nodeString[4] = pchar[0];





	//	cout << "string:" << nodeString << std::endl;
		

		printbitmap(GLUT_BITMAP_HELVETICA_18, nodeString, x, -0.7f);
		printbitmap(GLUT_BITMAP_HELVETICA_18, "+", x + 0.12, -0.7f);
		x = x + 0.15;


	}





	//Print all nodes in equation
	//for (int i = 0; i < equationArray.size(); i++)
	//{
		/*printbitmap(GLUT_BITMAP_HELVETICA_18, "4(3/2n)", x, -0.7f);
		printbitmap(GLUT_BITMAP_HELVETICA_18, "+", x + 0.12, -0.7f);
		x = x + 0.15;
		printbitmap(GLUT_BITMAP_HELVETICA_18, "4(3/2n)", x, -0.7f);*/

	//}

		// testing equation print
		


}


// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Define our scene transformation:
	GsMat rx, ry;
	rx.rotx(_rotx);
	ry.roty(_roty);
	_str = rx*ry;
	_str.setrans(_trans);

	// Define our projection transformation:
	// (see gltutors example projection.exe, we are replicating the same behavior here)


	_cam.lookat(_eye, _center, _up);

	_proj.perspective(_fovy, _aspect, _znear, _zfar);
	_spr = _proj * _cam; // final scene projection


	
	_recursiveTree.build(recursion, depth, equationArray);

	printTreeText();
	printEquation();
	drawMenus();

	_buttonsInput.build();
	_buttonsInput.draw(_str, _spr);



	_lines.draw(_str, _spr);
	_interface.draw(_str, _spr);

	
	_recursiveTree.draw(_str, _spr);
	_scrollBox.draw(_str, _spr);




		



	


	
	//if (viewTrickMode == true)
	//{

	//	//_deck.build();
	//	GsMat rotX, rotY, rotZ, T, translation, transY, rotation, init, scale;
	//	_deck.draw(T, _spr, _light);

	//}
	












	


	// Swap buffers and draw:
	glFlush();         // flush the pipeline (usually not necessary)
	glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

