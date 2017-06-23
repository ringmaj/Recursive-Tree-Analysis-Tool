
# include "recursive_tree.h"
# include <string>


RecursiveTree::RecursiveTree()
{
	_numpoints = 0;
	rootXPos = -0.5;
	rootYPos = 0.8;
	branchWidth = 1.2;
	branchHeight = 0.2;


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
	nodeArray.capacity(0);
	int depth = 0;

	// Holds the size for the boundaries between recursions
	float boundary = branchWidth;

	// Holds the increment for in between recursions
	float incr = boundary / (equation.size() + 1);



	// Create first recursive iteration for depth 1, Base case:  Root adds main nodes below, then main nodes recurse further
	float x = rootXPos - (boundary / 2);
	
	std::cout << "equation array size: " << equation.size() << std::endl;

	// stores the x/y position of the nodes at depth 1
	float nodeXPos = x + incr;
	float nodeYPs = rootYPos - 0.2f;


	// Push points at depth 1
	node input;
	for (int i = 0; i < equation.size(); i++)
	{
		x += incr;
		P.push(GsVec(rootXPos, rootYPos, 0.0f));
		P.push(GsVec(x, rootYPos - 0.15f, 0.0f));
		C.push(GsColor::black);
		C.push(GsColor::black);

		input.recursions = equation[i].recursions;

		input.orig_num1 = equation[i].num1;
		input.orig_num2 = equation[i].num2;
		input.num1 = equation[i].num1;
		input.num2 = equation[i].num2;

		input.x = x;
		input.y = rootYPos - 0.2f;

		input.numChildren = 0;
		input.expanded = false;

		input.boundary = branchWidth * 0.35;

		nodeArray.push(input);
	}

	

	


	// Add nodes for the lower depths
	depth = 1;
	float newBoundary;
	

	while (depth <= searchDepth)
	{

		
		float size = nodeArray.size();

		for (int i = 0; i < size; i++)
		{
			// Check if node is already fully expanded
			if (nodeArray[i].recursions == nodeArray[i].numChildren)
				nodeArray[i].expanded = true;

			

			// Node still needs to be expanded
			if (nodeArray[i].expanded == false)
			{

				 x = nodeArray[i].x - (nodeArray[i].boundary / 2);
				 incr = nodeArray[i].boundary / (nodeArray[i].recursions + 1);

				
				// std::cout << "recursions: " << nodeArray[i].recursions  << "\t numChildren: " << nodeArray[i].numChildren << std::endl;
				while (nodeArray[i].recursions != nodeArray[i].numChildren)
				{

					input.recursions = nodeArray[i].recursions;

					input.orig_num1 = nodeArray[i].orig_num1;
					input.orig_num2 = nodeArray[i].orig_num2;
					input.num1 = nodeArray[i].num1 * nodeArray[i].orig_num1;
					input.num2 = nodeArray[i].num2 * nodeArray[i].orig_num2;
					
					x += incr;
					input.x = x;
					input.y = nodeArray[i].y - branchHeight;

					input.numChildren = 0;
					input.expanded = false;

					input.boundary = nodeArray[i].boundary / nodeArray[i].recursions;

					
					nodeArray.push(input);
		
					nodeArray[i].numChildren++;

					// Push points to draw line
					P.push(GsVec(nodeArray[i].x, nodeArray[i].y - 0.065f, 0.0f));
					P.push(GsVec(input.x, input.y , 0.0f));

					if (depth == searchDepth)
					{
						C.push(GsColor::red);
						C.push(GsColor::red);
					}
					else
					{
						C.push(GsColor::black);
						C.push(GsColor::black);
					}

				}
				


			}


		}


		depth++;
	}

	std::cout << "NODE SIZE: " << nodeArray.size() << std::endl;





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

