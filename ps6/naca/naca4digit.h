#ifndef NACA4DIGIT_IS_INCLUDED
#define NACA4DIGIT_IS_INCLUDED

#include <vector>
#include <cstddef>
#include <ysclass.h>
#include <stdio.h>
#include <iostream>   // std::cout
#include <string> 

double calcYc( double z_, double p_naca_, double c_naca_, double m_naca_);
std::vector <YsVec3> MakeNACA4DigitAirfoil(int naca,const double dz);
void MakeNACA4DigitAirfoilWing(class PolygonalMesh &mesh,int naca,float span,float sweepBack,float dihedral,float taper,float dz);

#endif
