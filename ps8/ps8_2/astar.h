#ifndef ASTAR_IS_INCLUDED
#define ASTAR_IS_INCLUDED

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ysshellext.h>
#include <cmath>
#include <queue>
#include <limits>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>	

std::vector <YsShellExt::VertexHandle> A_Star(const YsShellExt &shl,YsShellExt::VertexHandle startVtHd,YsShellExt::VertexHandle goalVtHd);
std::vector <YsShellExt::VertexHandle> A_Star_ReconstructPath(
    const YsShellExt &shl,const std::unordered_map <YSHASHKEY,YsShellExt::VertexHandle> &cameFrom,YsShellExt::VertexHandle current);

double distanceFn(YsVec3 currPos, YsVec3 goalPos);

// class definitions for A* //

class TreeNode{
private:

	YsShellExt::VertexHandle vtHd;
	double g_val, h_val;
	std::vector<TreeNode*> succesors;
	TreeNode* parent;	

public:

	YsShellExt::VertexHandle getHandle() const{ return vtHd; }
	TreeNode* getParent() const { return parent; }
	double getG() const{return g_val;}
	double getH() const{return h_val;}
	std::vector<TreeNode*> getSuccesors() const { return succesors; }

	void setHandle(YsShellExt::VertexHandle vtHdIn){ vtHd=vtHdIn; }
	void setG(double g_valIn){ g_val=g_valIn; }
	void setH(double h_valIn){ h_val=h_valIn; }
	void setParent(TreeNode* parentIn){parent = parentIn;}
	void addSuccesor(TreeNode* succesorIn){ succesors.push_back(succesorIn); }

	void calcH(const YsShellExt::VertexHandle& goalHd, const YsShellExt &shl){

		YsVec3 currPos, goalPos;
		shl.GetVertexPosition(currPos, vtHd);
		shl.GetVertexPosition(goalPos, goalHd);
		h_val = distanceFn(currPos, goalPos);
	}

	TreeNode() {}

	TreeNode( YsShellExt::VertexHandle vtHdIn, double g_valIn ): vtHd(vtHdIn), g_val(g_valIn){}
};

struct CompareF{
    bool operator()(TreeNode* const & s1, TreeNode* const & s2) {
        // return "true" if "p1" is ordered before "p2", for example:
        long eps = 1;
        return eps*s1->getH() + s1->getG() > eps*s2->getH() + s2->getG();
    }
};

#endif
