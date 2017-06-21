
# include "recursive_tree.h"
# include <string>


RecursiveTree::RecursiveTree()
{
	_numpoints = 0;
	rootXPos = -0.5;
	rootYPos = 0.8;
	branchWidth = 1.2;


}

// init is called only once:
void RecursiveTree::init()
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


// build may be called everytime the object changes (not the case for this axis object):
void RecursiveTree::build(int recursions, int searchDepth, GsArray<recursiveNode> equation)
{

	// GsArray<recursiveNode> equation holds all of our nodes for the equation: eg 4T(3/2n) is  node

	//test
	glLineWidth(0.8);
	orig.capacity(0);
	int depth = 0;

	// Holds the size for the boundaries between recursions
	float boundary = branchWidth;

	// Holds the increment for in between recursions
	float incr = boundary / (equation.size() + 1);




	//Push starting point
	orig.push(GsVec(rootXPos, rootYPos, 0.0f));


	// Create first recursive iteration for depth 1, Base case:  Root adds main nodes below, then main nodes recurse further
	float x = rootXPos - (boundary / 2);
	
	std::cout << "equation array size: " << equation.size() << std::endl;
	for (int i = 0; i < equation.size(); i++)
	{
		x += incr;
		P.push(GsVec(rootXPos, rootYPos, 0.0f));
		P.push(GsVec(x, rootYPos - 0.2f, 0.0f));

		// Push new point to orig, save for later
		orig.push(GsVec(x, (rootYPos)-0.2f, 0.0f));

	}

	//// Depth is now 1
	depth = 1;



	// Print tree for each node in equation
	for (int i = 0; i < equation.size(); i++)
	{

	}


	//int startIndex = pow(recursions, depth) - (recursions - 1);

	//while (depth <= searchDepth)
	//{

	//	//std::cout << "size: " << orig.size() << std::endl;

	//	int endIndex = (startIndex + pow(recursions, depth));

	//	// Boundary shrinks for each depth
	//	boundary = boundary / recursions;
	//	incr = boundary / (recursions + 1);
	//	//std::cout << "incr: " << incr << std::endl;


	//	//	std::cout << "start: " << startIndex << "\tend: " << endIndex << std::endl;
	//	// Does recursion for each subtree

	//	for (int i = startIndex; i < endIndex; i++)
	//	{


	//		x = orig[i].x - (boundary / 2);
	//		//std::cout << "x: " << x << std::endl;
	//		for (int j = 0; j < recursions; j++)
	//		{
	//			x += incr;
	//			GsVec start = orig[i];
	//			start.y -= 0.08;
	//			P.push(start);
	//			P.push(GsVec(x, orig[i].y - 0.2f, 0.0f));

	//			// Push new point to orig, save for later
	//			orig.push(GsVec(x, orig[i].y - 0.2f, 0.0f));
	//			//std::cout << "push point: " << i + j + 1 << std::endl;
	//		}
	//	}

	//	//std::cout << "size: " << orig.size() << std::endl;
	//	startIndex = endIndex;
	//	depth++;
	//}


	C.size(P.size());
	C.setall(GsColor::black);

	int last = pow(recursions, searchDepth + 1) * 2;

	for (int i = C.size() - last; i < C.size(); i++)
		C.set(i, GsColor::red);






















	//glLineWidth(0.8);
	//orig.capacity(0);
	//int depth = 0;

	//// Holds the size for the boundaries between recursions
	//float boundary = branchWidth;

	//// Holds the increment for in between recursions
	//float incr = boundary / (recursions + 1);




	////Push starting point
	//orig.push(GsVec(rootXPos,rootYPos, 0.0f));


	//// Create first recursive iteration for depth 1, Base case
	//float x = rootXPos - (boundary / 2);
	////std::cout << "x: " << x << std::endl;
	//for (int i = 0; i < recursions; i++)
	//{
	//	x += incr;
	//	P.push(GsVec(rootXPos, rootYPos, 0.0f));
	//	P.push(GsVec(x, rootYPos - 0.2f, 0.0f));

	//	// Push new point to orig, save for later
	//	orig.push(GsVec(x, (rootYPos) - 0.2f, 0.0f));

	//}



	//// Depth is now 1
	//depth = 1;


	//int startIndex = pow(recursions, depth) - (recursions - 1);

	//while (depth <= searchDepth)
	//{

	//	//std::cout << "size: " << orig.size() << std::endl;

	//	int endIndex = (startIndex + pow(recursions, depth));

	//	// Boundary shrinks for each depth
	//	boundary = boundary / recursions;
	//	incr = boundary / (recursions + 1);
	//	//std::cout << "incr: " << incr << std::endl;


	////	std::cout << "start: " << startIndex << "\tend: " << endIndex << std::endl;
	//	// Does recursion for each subtree

	//	for (int i = startIndex; i < endIndex; i++)
	//	{


	//		x = orig[i].x - (boundary / 2);
	//		//std::cout << "x: " << x << std::endl;
	//		for (int j = 0; j < recursions; j++)
	//		{
	//			x += incr;
	//			GsVec start = orig[i];
	//			start.y -= 0.08;
	//			P.push(start);
	//			P.push(GsVec(x, orig[i].y - 0.2f, 0.0f));

	//			// Push new point to orig, save for later
	//			orig.push(GsVec(x, orig[i].y - 0.2f, 0.0f));
	//			//std::cout << "push point: " << i + j + 1 << std::endl;
	//		}
	//	}

	//	//std::cout << "size: " << orig.size() << std::endl;
	//	startIndex = endIndex;
	//	depth++;
	//}

	////	for (int i = startIndex; i < endIndex; i++)
	////	{


	////		x = orig[i].x - (boundary / 2);
	////		//std::cout << "x: " << x << std::endl;
	////		for (int j = 0; j < recursions; j++)
	////		{
	////			x += incr;
	////			P.push(orig[i]);
	////			P.push(GsVec(x, orig[i].y - 0.2f, 0.0f));

	////			// Push new point to orig, save for later
	////			orig.push(GsVec(x, orig[i].y - 0.2f - 0.13f, 0.0f));
	////			//std::cout << "push point: " << i + j + 1 << std::endl;
	////		}
	////	}

	////	//std::cout << "size: " << orig.size() << std::endl;
	////	startIndex = endIndex;
	////	depth++;
	////}

	//C.size(P.size());
	//C.setall(GsColor::black);

	//int last = pow(recursions, searchDepth + 1) * 2;

	//for (int i = C.size() - last; i < C.size(); i++)
	//	C.set(i, GsColor::red);




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
void RecursiveTree::draw(GsMat& tr, GsMat& pr)
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

