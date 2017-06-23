
// Ensure the header file is included only once in multi-file projects
#ifndef RECURSIVE_TREE_H
#define RECURSIVE_TREE_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:

// This structure holds the format for each part of the equation. ex: 4T(3/2n) is a recursive node
struct recursiveNode
{
	// holds the number of recursions 4T(n)
	int recursions;

	//num1 and num2 are for division purposes, 4T(3/2 n) num1 = 3, num2 = 2
	int num1;
	int num2;
};


struct node
{
	// holds the number of recursions 
	int recursions;

	// Holds original num1 and num2 are for division purposes, 4T(3/2 n) num1 = 3, num2 = 2
	int orig_num1;
	int orig_num2;

	// num1 and num2 are for division purposes, used to calculate new value of node
	int num1;
	int num2;

	// Holds x,y position for the node
	float x;
	float y;

	// Number of children this node has, if numChildren = recursions, then node has been fully expanded
	int numChildren = 0;
	bool expanded = false;

	float boundary;
};


class RecursiveTree : public GlObjects
{
private:
	GlShader _vsh, _fsh;
	GlProgram _prog;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // colors
	GsArray<node> nodeArray;

	int _numpoints;     // saves the number of points

	int num_recursions;
	int depth;
	float rootXPos;
	float rootYPos;
	float branchWidth;
	float branchHeight;

public:
	RecursiveTree();


	void init();
	void build(int recursions, int searchDepth, GsArray<recursiveNode> equation);
	void draw(GsMat& tr, GsMat& pr);
	float getRootXPos() { return rootXPos; }
	float getRootYPos() { return rootYPos; }
	void setRootXPos(float x) { rootXPos = x; }
	void setRootYPos(float y) { rootYPos = y; }
	float getBranchWidth() { return branchWidth; }
	void setBranchWidth(float w) {  branchWidth = w; }
	float getBranchHeight() { return branchHeight; }
	void setBranchHeight(float h) { branchHeight = h; }
	GsArray<node> getNodes() { return nodeArray; }
	
};

#endif // RECURSIVE_TREE_H
