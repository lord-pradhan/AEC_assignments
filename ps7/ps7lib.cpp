#include <fssimplewindow.h> // For FsSubSecondTimer
#include "meshlattice.h"
#include <stdio.h>

#include "ps7lib.h"

std::vector <float> MakeSliceVertexArray(const YsShellExt &mesh)
{
	auto t0=FsSubSecondTimer();
	std::vector <float> sliceVtx;

	// You fill this part.
	int Nx_S =  1, Ny_S = 100, Nz_S = 1;
	MeshLattice meshlatS;
	meshlatS.SetDomain(mesh, Nx_S, Ny_S, Nz_S);

	YsVec3 norm(0,1,0);

	for(int j=0; j<=Ny_S; j++){

		YsVec3i tempIdx(0, j, 0);
		YsVec3 point = meshlatS.GetBlockPosition(0, j, 0);

		auto plHdArrayTemp = meshlatS[tempIdx].plHd;

		for(auto pl : plHdArrayTemp){

			std::vector<YsVec3> pts;
			auto vtTemp = mesh.GetPolygonVertex(pl);

			std::vector <YsVec3> plVtPos;
			for(auto vtHd : vtTemp)
			{
				plVtPos.push_back(mesh.GetVertexPosition(vtHd));
			}
			
			YsPlane pln(point, norm);

			YsVec3 crs;
			auto penetrated = pln.GetPenetration(crs,plVtPos[0],plVtPos[1]);

			if(YSOK== penetrated){

				// printf("added a vertex\n");
				sliceVtx.push_back(crs.x()); sliceVtx.push_back(crs.y()); sliceVtx.push_back(crs.z());
			}

			penetrated = pln.GetPenetration(crs,plVtPos[1],plVtPos[2]);

			if(YSOK==penetrated ){

				// printf("added a vertex\n");
				sliceVtx.push_back(crs.x()); sliceVtx.push_back(crs.y()); sliceVtx.push_back(crs.z());				
			}

			penetrated = pln.GetPenetration(crs,plVtPos[2],plVtPos[0]);

			if(YSOK==penetrated ){

				// printf("added a vertex\n");
				sliceVtx.push_back(crs.x()); sliceVtx.push_back(crs.y()); sliceVtx.push_back(crs.z());				
			}
		}
	}

	printf("%d milli seconds\n",(int)FsSubSecondTimer()-t0);
	return sliceVtx;
}


#define USE_LATTICE


std::vector <float> MakeRayIntersectionVertexArray(const YsShellExt &mesh)
{
	auto t0=FsSubSecondTimer();
	std::vector <float> rayItscVtx;
	
	// You fill this part.
	MeshLattice meshlatL;
	int Nx_L = 100, Ny_L = 100, Nz_L = 1;
	meshlatL.SetDomain(mesh, Nx_L, Ny_L, Nz_L);

	YsVec3 norm(0, 0, 1);

	for(int i = 0; i<=Nx_L ; i++){

		for(int j = 0; j<=Ny_L; j++){

			YsVec3i tempIdx(i, j, 0);
			YsVec3 point = meshlatL.GetBlockPosition(i, j, 0);

			auto plHdArrayTemp = meshlatL[tempIdx].plHd;

			for(auto pl : plHdArrayTemp){

				std::vector <YsVec3> plVtPos;
				for(auto vt : mesh.GetPolygonVertex(pl))
				{
					plVtPos.push_back(mesh.GetVertexPosition(vt));
				}

				YsPlane pln;
				YsVec3 crs;
				if(YSOK==pln.MakeBestFitPlane(plVtPos) &&
				   YSOK==pln.GetIntersection(crs,point, norm) &&
				   YSINSIDE==YsCheckInsidePolygon3(crs,plVtPos)){

					// AddVertex( vtx, crs.x(), crs.y(), crs.z() );
					// AddColor(col, 0,0,0,0);
					rayItscVtx.push_back(crs.x()); rayItscVtx.push_back(crs.y()); 
					rayItscVtx.push_back(crs.z());
				}
			}
		}
	}


	printf("%d milli seconds\n",(int)FsSubSecondTimer()-t0);
	return rayItscVtx;
}

void Paint(YsShellExt &mesh,YsShell::PolygonHandle from,const double angleTolerance)
{
	auto startNom=mesh.GetNormal(from);
	YsColor red(1,0,0,1);
	mesh.SetPolygonColor(from, red);
	paintDFS(mesh, from, red, angleTolerance, startNom);
}


void paintDFS( YsShellExt& mesh, YsShellExt::PolygonHandle& startPoly , 
	YsColor col, double degTol, YsVec3 startNom){

	auto vtHdArray = mesh.GetPolygonVertex(startPoly);
	// printf("vtHdArray.size() is %d \n", vtHdArray.size());

	for(int i =0; i< vtHdArray.size(); i++)	
	{
		// printf("entered for loop\n");
		if( i < vtHdArray.size()-1 ){

			auto tempPoly = mesh.GetNeighborPolygon( startPoly, vtHdArray[i], vtHdArray[i+1] );
			// int currID = mesh.GetPolygonIdFromHandle(tempPoly);
			// printf("currId is %d\n", currID);
			// printf("angle is %lf compared to %lf\n", fabs(angleVecs(startNom, mesh.GetNormal(tempPoly))), 
				// degTol );

			if( fabs(angleVecs(startNom, mesh.GetNormal(tempPoly)) ) < degTol && 
				mesh.GetColor(tempPoly)!= col )
			{
				mesh.SetPolygonColor(tempPoly,col);	

				// pass into recursion
				// printf("passed into recursion\n");
				paintDFS(mesh, tempPoly, col, degTol, startNom);
			}
		}
		else{

			auto tempPoly = mesh.GetNeighborPolygon( startPoly, vtHdArray[i], vtHdArray[0] );
			// int currID = mesh.GetPolygonIdFromHandle(tempPoly);
			// printf("currId is %d\n", currID);
			// printf("angle is %lf compared to %lf\n", fabs(angleVecs(startNom, mesh.GetNormal(tempPoly))), 
			// 	degTol );

			if( fabs(angleVecs(startNom, mesh.GetNormal(tempPoly)) ) < degTol &&
			 mesh.GetColor(tempPoly)!= col )
			{
				mesh.SetPolygonColor(tempPoly,col);

				// printf("passed into recursion\n");
				// pass into recursion
				paintDFS(mesh, tempPoly, col, degTol, startNom);
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