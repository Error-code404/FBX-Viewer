#include "FBXModel.h"

FBXModel::FBXModel(const std::string & filePath, ShaderLoader& programName) : file(filePath.c_str())
{
	
	LoadScene();
	setProgram(programName);
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	LoadNode(scene->GetRootNode());
	//ProcessGeometry(scene->GetRootNode());
	for (auto c : meshData)
	{
		std::cout << "Number of mVertices: " << c.mVertices.size() << "\n";
	}
	//glBindVertexArray(0);
	/*for (auto c : renderingData)
	{
		std::cout << "Number of vertices in mesh: " << c.numVertices << "\n";
		std::cout << "Number of vertices indices in mesh: " << c.numIndices << "\n";
	}
*/
}

FBXModel::~FBXModel()
{
	scene->Destroy();
	ios->Destroy();
	manager->Destroy();
	for (auto c : renderingData)
	{ 
		glDeleteVertexArrays(1, &c.VAO);
		glDeleteBuffers(1, &c.VBO);
		glDeleteBuffers(1, &c.EBO);
	}
	for (auto c : meshData)
	{
		glDeleteVertexArrays(1, &c.VAO);
		glDeleteBuffers(1, &c.VBO);
		glDeleteBuffers(1, &c.EBO);
	}
	//	glDeleteVertexArrays(1, &c);
	////glDeleteBuffers(1, &VBO);
	//for (auto c : vbos)
	//	glDeleteBuffers(1, &c);
	//for (auto c : ebos)
	//	glDeleteBuffers(1, &c);
}

void FBXModel::setProgram(ShaderLoader& shaderName)
{
	program = &shaderName;
}

void FBXModel::LoadNode(FbxNode * fbxNode)
{
	int numAttributes = fbxNode->GetNodeAttributeCount();
	for (int i = 0; i < numAttributes; i++)
	{
		FbxNodeAttribute *nodeAttributeFbx = fbxNode->GetNodeAttributeByIndex(i);
		FbxNodeAttribute::EType attributeType = nodeAttributeFbx->GetAttributeType();

		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{
			// Load keyframe transformations
			//this->LoadNodeKeyframeAnimation(fbxNode);

			// Load mesh vertices, texcoords, normals, etc
			this->LoadMesh((FbxMesh*)nodeAttributeFbx);

			// Load mesh skeleton
			//this->LoadMesh_Skeleton((FbxMesh*)nodeAttributeFbx);
			break;
		}
		}
	}

	// Load the child nodes
	int numChildren = fbxNode->GetChildCount();
	for (int i = 0; i < numChildren; i++)
	{
		this->LoadNode(fbxNode->GetChild(i));
	}
}

void FBXModel::LoadMesh(FbxMesh * fbxMesh)
{
	// Load Vertices
	numVerts += fbxMesh->GetControlPointsCount();
	const int numVertices = fbxMesh->GetControlPointsCount();
	auto vertices =  new vertexData[numVertices];
	FbxVector4 *verticesFbx = fbxMesh->GetControlPoints();
	//fbxsdk::FbxArray<FbxVector2> UVs;
	//fbxMesh->GetPolygonVertexUVs("map1", UVs);
	int index = 0;
	for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		(float)vertices[index].x = (float)verticesFbx[vertexIndex][0];
		(float)vertices[index].y = (float)verticesFbx[vertexIndex][1];
		(float)vertices[index].z = (float)verticesFbx[vertexIndex][2];
		
		++index;
	}
	//Load indices
	RenderingData meshData;
	meshData.numVertices = std::move(numVertices);
	meshData.numIndices = fbxMesh->GetPolygonVertexCount();
	int *indices = fbxMesh->GetPolygonVertices();

	//////Load texture cords 


	//Prepare data for OpenGL
	GLuint VAO,VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * numVertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.numIndices, indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	meshData.VBO = std::move(VBO);
	meshData.EBO = std::move(EBO);
	meshData.VAO = std::move(VAO);
	//meshData.numIndices = numIndices;
	renderingData.push_back(std::move(meshData));
	//delete indices;
	delete vertices;
}

void FBXModel::ProcessGeometry(FbxNode * inNode)
{
	if (inNode->GetNodeAttribute())
	{
		switch (inNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			ProcessControlPoints(inNode);
			/*if (mHasAnimation)
			{
				ProcessJointsAndAnimations(inNode);
			}*/
			//ProcessMesh(inNode);
			//AssociateMaterialToMesh(inNode);
			//ProcessMaterials(inNode);
			break;
		}
	}

	for (int i = 0; i < inNode->GetChildCount(); ++i)
	{
		ProcessGeometry(inNode->GetChild(i));
	}
}

void FBXModel::ProcessControlPoints(FbxNode * inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	MeshData ourMesh;
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		floatx3* currCtrlPoint = new floatx3;
		floatx3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
		*currCtrlPoint = currPosition;
		ourMesh.mControlPoints[i] = currCtrlPoint;
	}

	//process Mesh function

	ourMesh.mTriangleCount = currMesh->GetPolygonCount();
	int vertexCounter = 0;
	ourMesh.mTriangles.reserve(ourMesh.mTriangleCount);

	for (unsigned int i = 0; i < ourMesh.mTriangleCount; ++i)
	{
		floatx3 normal[3];
		floatx3 tangent[3];
		floatx3 binormal[3];
		floatx2 UV[3][2];
		Triangle currTriangle;
		ourMesh.mTriangles.push_back(currTriangle);

		for (unsigned int j = 0; j < 3; ++j)
		{
			int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
			floatx3* currCtrlPoint = ourMesh.mControlPoints[ctrlPointIndex];

			//ReadNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);

			for (int k = 0; k < 1; ++k)
			{
				ReadUV(currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), k, UV[j][k]);
			}

			PNTIWVertex temp;
			temp.mPosition = *currCtrlPoint;
			temp.mNormal = normal[j];
			temp.mUV = UV[j][0];

			ourMesh.mVertices.push_back(temp);
			ourMesh.mTriangles.back().mIndices.push_back(vertexCounter);
			++vertexCounter;
		}
	}
	for (auto itr = ourMesh.mControlPoints.begin(); itr != ourMesh.mControlPoints.end(); ++itr)
	{
		delete itr->second;
	}
	ourMesh.mControlPoints.clear();
	//load vertex Indices
	ourMesh.numIndices = currMesh->GetPolygonVertexCount();
	int *indices = currMesh->GetPolygonVertices();


	//optimize, delete duplicated vertices
	std::vector<PNTIWVertex> uniqueVertices;
	for (unsigned int i = 0; i < ourMesh.mTriangles.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			// If current vertex has not been added to
			// our unique vertex list, then we add it
			if (FindVertex(ourMesh.mVertices[i * 3 + j], uniqueVertices) == -1)
			{
				uniqueVertices.push_back(ourMesh.mVertices[i * 3 + j]);
			}
		}
	}
		// Now we update the index buffer
		for (unsigned int i = 0; i < ourMesh.mTriangles.size(); ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				ourMesh.mTriangles[i].mIndices[j] = FindVertex(ourMesh.mVertices[i * 3 + j], uniqueVertices);
			}
		}

		ourMesh.mVertices.clear();
		ourMesh.mVertices = uniqueVertices;
		uniqueVertices.clear();

		// Now we sort the triangles by materials to reduce 
		// shader's workload
		std::sort(ourMesh.mTriangles.begin(), ourMesh.mTriangles.end());
	
	
	////////////////////////////////////////
		//opengl data
		//Prepare data for OpenGL
		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 3 *sizeof(float) * ourMesh.mVertices.size(), &ourMesh.mVertices[0],GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ourMesh.numIndices, indices, GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		ourMesh.VBO = std::move(VBO);
		ourMesh.EBO = std::move(EBO);
		ourMesh.VAO = std::move(VAO);
	meshData.push_back(ourMesh);
}

void FBXModel::ProcessMesh(FbxNode * inNode)
{
}

void FBXModel::AssociateMaterialToMesh(FbxNode * inNode)
{
}

void FBXModel::ProcessMaterials(FbxNode * inNode)
{
}

void FBXModel::ReadUV(FbxMesh * inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, floatx2 & outUV)
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
	{
		throw std::exception("Invalid UV Layer Number");
	}
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

int FBXModel::FindVertex(const PNTIWVertex & inTargetVertex, const std::vector<PNTIWVertex>& uniqueVertices)
{
	for (unsigned int i = 0; i < uniqueVertices.size(); ++i)
	{
		if (inTargetVertex == uniqueVertices[i])
		{
			return i;
		}
	}

	return -1;
}

void FBXModel::LoadScene()
{
	manager = FbxManager::Create();
	converter = &FbxGeometryConverter(manager);
	ios = FbxIOSettings::Create(manager, IOSROOT);
	FbxImporter* lImporter = FbxImporter::Create(manager, "");
	if (!lImporter->Initialize(file, -1, manager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	scene = FbxScene::Create(manager, "myScene");

	converter->SplitMeshesPerMaterial(scene, true);
	if (converter->SplitMeshesPerMaterial(scene, true))
		std::cout << "Mesh conversion complete ! \n";
	// Import the contents of the file into the scene.
	lImporter->Import(scene);


	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	//cene->FillTextureArray()
	fbxsdk::FbxArray<FbxTexture*> textures;
	scene->FillTextureArray(textures);
	
}

void FBXModel::Draw()
{
	program->Use();
	for (auto c : renderingData)
	{
		glBindVertexArray(c.VAO);

		glDrawElements(GL_LINES, c.numIndices,GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	//glBindVertexArray(vaos[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	//glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINES,0, numVerts);
	//glBindVertexArray(0);
}
