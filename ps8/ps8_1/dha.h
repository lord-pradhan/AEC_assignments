#ifndef DHA_IS_INCLUDED
#define DHA_IS_INCLUDED

#include <ysshellext.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Input parameters are a mesh and dihedral-angle threshold.
// The return value is a map from a polygon search key to a segment identifier.
std::unordered_map <YSHASHKEY,int> MakeDihedralAngleBasedSegmentation(const YsShellExt &mesh,
	const double dhaThr);

void segmentDFS(const YsShellExt &mesh, YsShellExt::PolygonHandle& startPoly, 
	std::unordered_map <YSHASHKEY,int>& faceGrpIn, int groupIDIn, const double dhaThr);

double angleVecs(YsVec3 in1, YsVec3 in2);

// Input parameters are a mesh and the segmentation (face grouping) obtained from MakeDihedralAngleBasedSegmentaion.
// Output is a vertex array that can be drawn as GL_LINES.
std::vector <float> MakeGroupBoundaryVertexArray(const YsShellExt &mesh,const std::unordered_map <YSHASHKEY,int> &faceGroupInfo);

// For bonus questions:
// Input parameters are a mesh and the segmentation (face grouping) obtained from MakeDihedralAngleBasedSegmentaion.
// Paint polygons so that no two neighboring face groups have a same color.
void MakeFaceGroupColorMap(YsShellExt &mesh,const std::unordered_map <YSHASHKEY,int> &faceGroupInfo);

#endif
