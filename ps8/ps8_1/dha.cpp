#include "dha.h"

#include <unordered_set>

// Input parameters are a mesh and dihedral-angle threshold.
// The return value is a map from a polygon search key to a segment identifier.
std::unordered_map <YSHASHKEY,int> MakeDihedralAngleBasedSegmentation(const YsShellExt &mesh,
	const double dhaThr){

	std::unordered_map <YSHASHKEY,int> faceGrp;
	int groupID = 0;
	for(auto plHd = mesh.NullPolygon(); true==mesh.MoveToNextPolygon(plHd); ){

		if(faceGrp.find(mesh.GetSearchKey(plHd)) == faceGrp.end() ){
			
			// faceGrp[mesh.GetSearchKey(plHd)] = groupID;
			faceGrp.insert( {mesh.GetSearchKey(plHd), groupID} );
			segmentDFS(mesh, plHd, faceGrp, groupID, dhaThr);
			groupID++;
		}
	}
	printf("num IDs is %d\n", groupID );
	return faceGrp;
}

void segmentDFS(const YsShellExt &mesh, YsShellExt::PolygonHandle& startPoly, 
	std::unordered_map <YSHASHKEY,int>& faceGrpIn, int groupIDIn, const double dhaThr){

	auto vtHdArray = mesh.GetPolygonVertex(startPoly);
	auto currNorm = mesh.GetNormal(startPoly);
	// printf("vtHdArray.size() is %d \n", vtHdArray.size());

	for(int i =0; i< vtHdArray.size(); i++)	
	{
		// printf("entered for loop\n");
		if( i < vtHdArray.size()-1 ){

			auto tempPoly = mesh.GetNeighborPolygon( startPoly, vtHdArray[i], vtHdArray[i+1] );

			if( fabs(angleVecs(currNorm, mesh.GetNormal(tempPoly)) ) < dhaThr && 
				(faceGrpIn.find(mesh.GetSearchKey(tempPoly))==faceGrpIn.end()) ){

				// faceGrpIn[mesh.GetSearchKey(tempPoly)]=groupIDIn;
				faceGrpIn.insert( {mesh.GetSearchKey(tempPoly), groupIDIn} );

				// pass into recursion
				// printf("passed into recursion\n");
				segmentDFS(mesh, tempPoly, faceGrpIn, groupIDIn, dhaThr);
			}
		}
		else{

			auto tempPoly = mesh.GetNeighborPolygon( startPoly, vtHdArray[i], vtHdArray[0] );

			if( fabs(angleVecs(currNorm, mesh.GetNormal(tempPoly)) ) < dhaThr && 
				(faceGrpIn.find(mesh.GetSearchKey(tempPoly))==faceGrpIn.end()) ){

				// faceGrpIn[mesh.GetSearchKey(tempPoly)]=groupIDIn;
				faceGrpIn.insert( {mesh.GetSearchKey(tempPoly), groupIDIn} );
				// pass into recursion
				// printf("passed into recursion\n");
				segmentDFS(mesh, tempPoly, faceGrpIn, groupIDIn, dhaThr);
			}
		}
	}
	return;
}

double angleVecs(YsVec3 in1, YsVec3 in2){

	in1.Normalize(); in2.Normalize();

	double dot = in1.x()*in2.x() + in1.y()*in2.y() + in1.z()*in2.z();

	if(dot>1)
		dot=1;
	else if(dot<-1)
		dot=-1;

	return acos(dot);
}


// Input parameters are a mesh and the segmentation (face grouping) obtained from MakeDihedralAngleBasedSegmentaion.
// Output is a vertex array that can be drawn as GL_LINES.
std::vector <float> MakeGroupBoundaryVertexArray(const YsShellExt &mesh,
	const std::unordered_map <YSHASHKEY,int> &faceGroupInfo)
{
	std::vector <float> vtxArray;

	for(auto plHd = mesh.NullPolygon(); true==mesh.MoveToNextPolygon(plHd); ){

		auto vtHdArray = mesh.GetPolygonVertex(plHd);

		for(int i =0; i< vtHdArray.size(); i++)	
		{
			if( i < vtHdArray.size()-1 ){

				auto tempPoly = mesh.GetNeighborPolygon( plHd, vtHdArray[i], vtHdArray[i+1] );

				if(tempPoly != nullptr &&
					faceGroupInfo.find(mesh.GetSearchKey(plHd))->second != 
					faceGroupInfo.find(mesh.GetSearchKey(tempPoly))->second ){

					YsShellExt::VertexHandle edgePiece[2] = {vtHdArray[i], vtHdArray[i+1]};
					YsVec3 tempPos1; mesh.GetVertexPosition(tempPos1, edgePiece[0]);
					YsVec3 tempPos2; mesh.GetVertexPosition(tempPos2, edgePiece[1]);

					vtxArray.push_back(tempPos1.x());
					vtxArray.push_back(tempPos1.y());
					vtxArray.push_back(tempPos1.z());
					vtxArray.push_back(tempPos2.x());
					vtxArray.push_back(tempPos2.y());
					vtxArray.push_back(tempPos2.z());					
				}
			}
			else{

				auto tempPoly = mesh.GetNeighborPolygon( plHd, vtHdArray[i], vtHdArray[0] );

				if(tempPoly != nullptr &&
					faceGroupInfo.find(mesh.GetSearchKey(plHd))->second != 
					faceGroupInfo.find(mesh.GetSearchKey(tempPoly))->second ){

					YsShellExt::VertexHandle edgePiece[2] = {vtHdArray[i], vtHdArray[0]};
					YsVec3 tempPos1; mesh.GetVertexPosition(tempPos1, edgePiece[0]);
					YsVec3 tempPos2; mesh.GetVertexPosition(tempPos2, edgePiece[1]);

					vtxArray.push_back(tempPos1.x());
					vtxArray.push_back(tempPos1.y());
					vtxArray.push_back(tempPos1.z());
					vtxArray.push_back(tempPos2.x());
					vtxArray.push_back(tempPos2.y());
					vtxArray.push_back(tempPos2.z());					
				}
			}
		}
	}

	return vtxArray;
}

// For bonus questions:
// Input parameters are a mesh and the segmentation (face grouping) obtained from MakeDihedralAngleBasedSegmentaion.
// Paint polygons so that no two neighboring face groups have a same color.
void MakeFaceGroupColorMap(YsShellExt &mesh,const std::unordered_map <YSHASHKEY,int> &faceGroupInfo)
{
	// find max hash value
	double hash_max = 0;
	for (auto i : faceGroupInfo){

		if(hash_max< i.second)
			hash_max=i.second;
	}	

	for(auto plHd = mesh.NullPolygon(); true==mesh.MoveToNextPolygon(plHd); ){

		double hashVal = faceGroupInfo.find(mesh.GetSearchKey(plHd))->second;
		// std::cout<<"hashVal is "<<hashVal<<"\n size is "<<numGroups<<std::endl;
		if(hash_max>0){

			if(fmod(hashVal,7)==0){

				YsColor colorFromHash(1, 1, 0, 0.5);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}
			else if(fmod(hashVal,7)==1){

				YsColor colorFromHash(0, 1, 0, 1);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}
			else if(fmod(hashVal,7)==2){

				YsColor colorFromHash(0, 0.5, 1, 1);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}
			else if(fmod(hashVal,7)==3){

				YsColor colorFromHash(1, 0, 0, 1);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}
			else if(fmod(hashVal,7)==4){

				YsColor colorFromHash(0, 1, 0.5, 0.5);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}
			else if(fmod(hashVal,7)==5){

				YsColor colorFromHash(0.5, 0, 0.5, 1);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}
			else if(fmod(hashVal,7)==6){

				YsColor colorFromHash(0.25, 0.25, 1, 0);	
				mesh.SetPolygonColor(plHd, colorFromHash);	
			}

			// double r = 1.0 - (double) hashVal / hash_max;
			// double g = (double) hashVal / hash_max;
			// double b = 1.0 - (double) hashVal / hash_max;
			// double alpha = (double) hashVal / hash_max;
			// std::cout<<"red is "<<r<<"\n green is "<<g<<"\n blue is "<<b<<"\n alpha is "<<alpha<<std::endl;
			
		}
	}
}