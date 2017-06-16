
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
class RecursiveTree : public GlObjects
{
private:
	GlShader _vsh, _fsh;
	GlProgram _prog;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsVec>   orig; // saves unaltered coordinates
	GsArray<GsColor> C; // colors
	int _numpoints;     // saves the number of points

	int num_recursions;
	int depth;
	float rootXPos;
	float rootYPos;
	float branchWidth;

	struct recursiveNode
	{
		// holds the number of recursions 4T(n)
		int recursions;

		//num1 and num2 are for division purposes, 4T(3/2 n) num1 = 3, num2 = 2
		int num1;
		int num2;
	};

	GsArray<recursiveNode> equationArr;

public:
	RecursiveTree();
	void init();
	float getRootXPos() { return rootXPos; }
	float getRootYPos() { return rootYPos; }
	void setRootXPos(float x) { rootXPos = x; }
	void setRootYPos(float y) { rootYPos = y; }
	float getBranchWidth() { return branchWidth; }
	void setBranchWidth(float w) {  branchWidth = w; }
	GsArray<GsVec> getPoints() { return orig; }

	void addNode(int r, int n1, int n2);
	void build(int num_recursions, int depth);
	void draw(GsMat& tr, GsMat& pr);
};

#endif // RECURSIVE_TREE_H
