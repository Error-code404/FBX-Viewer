#pragma once
#include <vector>

struct vertexData
{
	//vertex position
	float x;
	float y;
	float z;

	//texture mapping
	float tx;
	float ty;
};

struct floatx3
{
	float x;
	float y;
	float z;
};

struct floatx2
{
	float x;
	float y;
};

struct Triangle
{
	std::vector<unsigned int> mIndices;
	std::string mMaterialName;
	unsigned int mMaterialIndex;

	bool operator<(const Triangle& rhs)
	{
		return mMaterialIndex < rhs.mMaterialIndex;
	}
};

struct PNTIWVertex
{
	floatx3 mPosition;
	floatx3 mNormal;
	floatx2 mUV;

	bool operator==(const PNTIWVertex& rhs) const
	{

		bool result1 = mPosition.x == rhs.mPosition.x && mPosition.y == rhs.mPosition.y && mPosition.z == rhs.mPosition.z;
		bool result2 = mNormal.x == rhs.mNormal.x && mNormal.y == rhs.mNormal.y && mNormal.z == rhs.mNormal.z;
		bool result3 = mUV.x == rhs.mUV.x && mUV.y == rhs.mUV.y;

		return result1 && result2 && result3;
	}
};

//// Loop for each polygon
//for (int Poly(0); Poly < fbxMesh->GetPolygonCount(); Poly++)
//{
//	// Get number of vertices in this polygon
//	const int NumVertices = fbxMesh->GetPolygonSize(Poly);
//
//	// Loop for each vertex in polygon
//	for (int Vertex(0); Vertex < NumVertices; Vertex++)
//	{
//		FbxVector2 fbxTexCoord;
//		FbxStringList UVSetNameList;
//
//		// Get the name of each set of UV coords
//		fbxMesh->GetUVSetNames(UVSetNameList);
//
//		// Get the UV coords for this vertex in this poly which belong to the first UV set
//		// Note: Using 0 as index into UV set list as this example supports only one UV set
//		//fbxMesh->GetPolygonVertexUV()
//		fbxMesh->GetPolygonVertexUV(Poly, Vertex, UVSetNameList.GetStringAt(0), fbxTexCoord, );
//
//		// Convert to floats
//		vertices[Vertex].tx = static_cast<float>(fbxTexCoord[0]);
//		vertices[Vertex].ty = static_cast<float>(fbxTexCoord[1]);
//		//float UVCoord[2];
//		//UVCoord[0] = static_cast<float>(fbxTexCoord[0]);
//		//UVCoord[1] = static_cast<float>(fbxTexCoord[1]);
//
//		//// Store UV
//		//UVList.push_back(UVCoord);
//	}
//}