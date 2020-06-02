#include <vector>
#include "polygonalmesh.h"
#include <stdio.h>
#include <iostream>   // std::cout
#include <string> 

bool PolygonalMesh::LoadBinStl(const char fn[])
{
    FILE *fp=fopen(fn,"rb");
    if(nullptr!=fp)
    {
        unsigned char title[80];
        fread(title,1,80,fp); // Skip title

        unsigned char dw[4];
        fread(dw,4,1,fp);  // Read 4 bytes
        auto nTri=BinaryToInt(dw);
        printf("%d triangles\n",nTri);

        int nTriActual=0;
        for(int i=0; i<nTri; ++i)
        {
            unsigned char buf[50];  // 50 bytes per triangle
            if(50==fread(buf,1,50,fp))
            {
                AddBinaryStlTriangle(buf);
                ++nTriActual;
            }
            else
            {
                break;
            }
        }
        printf("Actually read %d\n",nTriActual);

        fclose(fp);
		return true;
    }
	return false;
}

bool PolygonalMesh::CPUisLittleEndian(void)
{
    unsigned int one=1;
    auto *dat=(const unsigned char *)&one;
    if(1==dat[0])
    {
        return true;
    }
    return false;
}
int PolygonalMesh::BinaryToInt(const unsigned char dw[4])
{
    int b0=(int)dw[0];
    int b1=(int)dw[1];
    int b2=(int)dw[2];
    int b3=(int)dw[3];
    return b0+b1*0x100+b2*0x10000+b3*0x1000000;
}
float PolygonalMesh::BinaryToFloat(const unsigned char dw[4])
{
    if(true==CPUisLittleEndian())
    {
        const float *fPtr=(const float *)dw;
        return *fPtr;
    }
    else
    {
        float value;
        auto *valuePtr=(unsigned char *)&value;
        valuePtr[0]=dw[3];
        valuePtr[1]=dw[2];
        valuePtr[2]=dw[1];
        valuePtr[3]=dw[0];
        return value;
    }
}

void PolygonalMesh::AddBinaryStlTriangle(const unsigned char buf[50])
{
    float nx=BinaryToFloat(buf),ny=BinaryToFloat(buf+4),nz=BinaryToFloat(buf+8);
    const YsVec3 nom(nx,ny,nz);

	const YsVec3 tri[3]=
	{
	    YsVec3(BinaryToFloat(buf+12),BinaryToFloat(buf+16),BinaryToFloat(buf+20)),
	    YsVec3(BinaryToFloat(buf+24),BinaryToFloat(buf+28),BinaryToFloat(buf+32)),
	    YsVec3(BinaryToFloat(buf+36),BinaryToFloat(buf+40),BinaryToFloat(buf+44)),
	};

	const VertexHandle triVtHd[3]=
	{
		AddVertex(tri[0]),
		AddVertex(tri[1]),
		AddVertex(tri[2]),
	};

	auto plHd=AddPolygon(3,triVtHd);
	SetPolygonColor(plHd,YsBlue());
	SetPolygonNormal(plHd,nom);
}

bool PolygonalMesh::SaveBinStl(const char fn[]) const
{
	FILE* fp = fopen(fn, "wb");
    char head[80];

    for(int i=0; i<sizeof(head); i++){

        head[i] = 'x';
    }

    fwrite(head, 1, 80, fp);

    auto polyNum = GetNumPolygon();
    fwrite((unsigned char*)& polyNum, 1, 4, fp);

    auto polyCurr = NullPolygon();

    bool polyNext = true;

    while(true){

        polyNext = MoveToNextPolygon(polyCurr);

        if(!polyNext)
            break;

        auto norm = GetNormal(polyCurr);
        auto xNorm = norm.xf(), yNorm = norm.yf(), zNorm = norm.zf();

        fwrite((unsigned char*)&xNorm , 1, 4, fp);
        fwrite((unsigned char*)&yNorm, 1, 4, fp);
        fwrite((unsigned char*)&zNorm , 1, 4, fp);

        auto vert = GetPolygonVertex(polyCurr);

        for(int i=0 ; i< vert.size() ; i++){

            auto vtxHd = vert[i];
            auto x = GetVertexPosition(vtxHd).xf();
            auto y = GetVertexPosition(vtxHd).yf();
            auto z = GetVertexPosition(vtxHd).zf();

            fwrite((unsigned char*)& x, 1, 4, fp);
            fwrite((unsigned char*)& y, 1, 4, fp);
            fwrite((unsigned char*)& z, 1, 4, fp);
        }

        char volID[2];
        volID[0] = 'x';
        volID[1] = 'x';
        fwrite(volID, 1, 2, fp);
    }
    fclose(fp);

    return true;
}
