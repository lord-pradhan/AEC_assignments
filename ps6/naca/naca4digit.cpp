#include "polygonalmesh.h"
#include "naca4digit.h"
#include <math.h> 
// #include <iostream>
#include <stdio.h>
#include <iostream>   // std::cout
#include <string> 

std::vector <YsVec3> MakeNACA4DigitAirfoil(int naca,const double dz)
{
	// std::cout<<"Function called"<<"\n";
	std::vector <YsVec3> vtx;
	int c_naca = 1;

	int t_temp = naca%100;
	double t_naca = (double) t_temp / 100.0;
	// std::cout<<"t_naca is "<<t_naca<<"\n";
	naca /= 100;

	int p_temp =  (naca%10);
	double p_naca = (double) p_temp / 10.0;
	// std::cout<<"p_naca is "<<p_naca<<"\n";
	naca /= 10;

	int m_temp = (naca%10);
	double m_naca = (double) m_temp / 100.0;
	// std::cout<<"m_naca is "<<m_naca<<"\n";

	// std::cout<<"Rounded value is "<< std::round(1.0/dz)<< "\n";

	for (int i = 0; i< std::round(1.0/dz) ; i++){

		double z = (double) i * dz;

		double y_t = 5*t_naca*( 0.2969*sqrt(z) - 0.1260*z - 0.3516*z*z + 0.2843*z*z*z - 0.1015*z*z*z*z );

		double y_c = calcYc( z, p_naca, c_naca, m_naca );

		YsVec3 pushElem;
		pushElem.Set( 0.0, y_t + y_c , z );
		vtx.push_back(pushElem);
		// std::cout<<"First for-loop"<<"\n";
	}

	for (int i1= std::round(1.0/dz); i1 >= 0; i1--){

		double z = (double) i1 * dz;

		double y_t = 5*t_naca*( 0.2969*sqrt(z) - 0.1260*z - 0.3516*z*z + 0.2843*z*z*z - 0.1015*z*z*z*z );

		double y_c = calcYc( z, p_naca, c_naca, m_naca );

		YsVec3 pushElem;
		pushElem.Set( 0.0, - y_t + y_c , z );
		vtx.push_back(pushElem);
		// std::cout<<"Second for-loop"<<i1<<"\n";
	}

	return vtx;
}

double calcYc( double z_, double p_naca_, double c_naca_, double m_naca_){

	if(z_>=0 && z_< p_naca_*c_naca_){

		return m_naca_ * ( 2.0*p_naca_*(z_/c_naca_) - (z_/c_naca_)*(z_/c_naca_) ) / (p_naca_*p_naca_);
	}
	else if( z_ >= p_naca_*c_naca_ && z_ <= c_naca_ ){

		return m_naca_ * ( (1.0 - 2.0*p_naca_) + 2.0*p_naca_*(z_/c_naca_) - (z_/c_naca_)*(z_/c_naca_)  ) 
					/ ( (1.0-p_naca_)*(1.0-p_naca_) );
	}
	else
		return -1;
}

void MakeNACA4DigitAirfoilWing(class PolygonalMesh &mesh,int naca,float span,float sweepBack,float dihedral,float taper,float dz)
{
	// std::cout<<"entered airfoil wing \n";
	std::vector<YsVec3> centreAirfoil = MakeNACA4DigitAirfoil( naca, dz ), rightAirfoil, leftAirfoil;
	for(auto i : centreAirfoil){

		// i.AddZ(0.5);
		rightAirfoil.push_back( YsVec3( i.x() + span, i.y()*taper+dihedral, i.z()*taper+sweepBack ) );
		leftAirfoil.push_back( YsVec3( i.x() - span, i.y()*taper+dihedral, i.z()*taper+sweepBack ) );
	}

	for ( int i=0; i< centreAirfoil.size(); i++ ){

		std::vector<PolygonalMesh::VertexHandle> tempVtx1;
		tempVtx1.clear();
		tempVtx1.push_back( mesh.AddVertex( centreAirfoil[i] ) );
		tempVtx1.push_back( mesh.AddVertex( centreAirfoil[i+1] ) );
		tempVtx1.push_back( mesh.AddVertex( rightAirfoil[i] ) );
		auto meshHd1 = mesh.AddPolygon( tempVtx1 );
		auto normal1 = YsUnitVector( ( centreAirfoil[i+1] - centreAirfoil[i] )^( rightAirfoil[i] - centreAirfoil[i+1] ) );
		auto tempBool = mesh.SetPolygonNormal( meshHd1, normal1 );


		std::vector<PolygonalMesh::VertexHandle> tempVtx2;
		tempVtx2.clear();
		tempVtx2.push_back( mesh.AddVertex( rightAirfoil[i] ) );
		tempVtx2.push_back( mesh.AddVertex( centreAirfoil[i+1] ) );		
		tempVtx2.push_back( mesh.AddVertex( rightAirfoil[i+1] ) );
		auto meshHd2 = mesh.AddPolygon( tempVtx2 );
		auto normal2 = YsUnitVector( ( rightAirfoil[i+1] - centreAirfoil[i] )^( centreAirfoil[i] - centreAirfoil[i+1] ) );
		mesh.SetPolygonNormal( meshHd2, normal2 );

		std::vector<PolygonalMesh::VertexHandle> tempVtx3;
		tempVtx3.clear();
		tempVtx3.push_back( mesh.AddVertex( leftAirfoil[i]));
		tempVtx3.push_back( mesh.AddVertex( centreAirfoil[i+1]));
		tempVtx3.push_back( mesh.AddVertex( centreAirfoil[i]));
		auto meshHd3 = mesh.AddPolygon( tempVtx3 );
		auto normal3 = YsUnitVector( (centreAirfoil[i+1] - leftAirfoil[i])^(centreAirfoil[i] - centreAirfoil[i+1]) );
		mesh.SetPolygonNormal(meshHd3, normal3);


		std::vector<PolygonalMesh::VertexHandle> tempVtx4;
		tempVtx4.clear();
		tempVtx4.push_back( mesh.AddVertex( centreAirfoil[i+1]));
		tempVtx4.push_back( mesh.AddVertex( leftAirfoil[i]));
		tempVtx4.push_back( mesh.AddVertex( leftAirfoil[i+1]));
		auto meshHd4 = mesh.AddPolygon( tempVtx4 );
		auto normal4 = YsUnitVector( (leftAirfoil[i] - leftAirfoil[i+1])^(leftAirfoil[i] - centreAirfoil[i+1]) );
		mesh.SetPolygonNormal(meshHd4, normal4);


		YsColor up(1,0,0,1);
		YsColor down(0,0,1,1);

		if(i<centreAirfoil.size()/2){

			mesh.SetPolygonColor(meshHd1,up);
			mesh.SetPolygonColor(meshHd2,up);
			mesh.SetPolygonColor(meshHd3,up);
			mesh.SetPolygonColor(meshHd4,up);
		} else{

			mesh.SetPolygonColor(meshHd1,down);
			mesh.SetPolygonColor(meshHd2,down);
			mesh.SetPolygonColor(meshHd3,down);
			mesh.SetPolygonColor(meshHd4,down);
		}
	}

	// std::cout<<"exited airfoil wing \n";
	return;
}