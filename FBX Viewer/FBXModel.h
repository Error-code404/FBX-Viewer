#pragma once
#include "fbxsdk.h"
#include <string>
#include <vector>
#include "vertexData.h"
#include "GL/glew.h"
#include "ShaderLoader.h"
#include <unordered_map>
#include <algorithm>

struct RenderingData
{
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
	unsigned int numVertices;
};

struct MeshData
{
	std::unordered_map<unsigned int, floatx3*> mControlPoints; // dynamic memory ,must be freed
	std::vector<Triangle> mTriangles;
	std::vector<PNTIWVertex> mVertices;
	unsigned int mTriangleCount;
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
};
class FBXModel
{
public:
	FBXModel(const std::string &filePath, ShaderLoader& programName);
	FBXModel(const FBXModel&) = delete;
	FBXModel& operator= (const FBXModel&) = delete;
	~FBXModel();
	void setProgram(ShaderLoader& shaderName);
//private:
	void LoadNode(FbxNode * fbxNode);
	void LoadMesh(FbxMesh * fbxMesh);
	//void LoadUVs(FbxMesh * fbxMesh);
	//void ConvertMapping(FbxLayerElementUV* pUVs, FbxMesh* pMesh);

	//nowe metody
	void ProcessGeometry(FbxNode * inNode);	//odpowiednik load node
	void ProcessControlPoints(FbxNode* inNode);
	void ProcessMesh(FbxNode* inNode);
	void AssociateMaterialToMesh(FbxNode* inNode);
	void ProcessMaterials(FbxNode* inNode);
	//void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, floatx3& outNormal);
	void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, floatx2& outUV);
	void Optimize();
	int  FindVertex(const PNTIWVertex& inTargetVertex, const std::vector<PNTIWVertex>& uniqueVertices);
	void LoadScene();
	void Draw();
	const char* file;
	FbxManager * manager;
	FbxGeometryConverter * converter;
	FbxScene* scene;
	FbxIOSettings* ios;
	int numVerts = 0;		//number of vertices
	//vertexData vertices[30000];		//array of vertices
	//int index = 0; //indexing for loading vertex data
	//unsigned int numIndices = 0; //store number of polygon vertices
	//int * indices;
	std::vector<RenderingData> renderingData;
	std::vector<MeshData> meshData;
	

	ShaderLoader * program;
	//GLuint VBO;
	//GLuint EBO;
	//GLuint VAO;
	//std::vector<GLuint> vbos;
	//std::vector<GLuint> vaos;
	//std::vector<GLuint> ebos;
};