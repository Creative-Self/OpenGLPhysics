#include "Model.h"



Model::Model()
{
	vertArr = 0; 
	vertBuf = 0; 
}

Model::Model(int cType)
{
	vertArr = 0;
	vertBuf = 0;
	colliderType = cType; 
}

bool Model::buffer(string objFile)
{   
	vector<glm::vec3> locations; 
	vector<glm::vec2> uvs; 
	vector<glm::vec3> normals; 
	vector<VertInd> vertInds; 

	std::ifstream ifs;
	ifs.open(objFile);

	string line; 
	while (std::getline(ifs, line)) {

		std::istringstream stream(line);
		string label;
		stream >> label; 
		if (label == "v") { // locations 
			float one, two, three; 
			stream >> one; 
			stream >> two; 
			stream >> three; 
			glm::vec3 instance(one, two, three);
			locations.push_back(instance); 
		}
		if (label == "vt") { //uv
			float one, two;
			stream >> one;
			stream >> two;
			glm::vec2 instance(one, two);
			uvs.push_back(instance);
		}
		if (label == "vn") { //normals
			float one, two, three;
			stream >> one;
			stream >> two;
			stream >> three;
			glm::vec3 instance(one, two, three);
			normals.push_back(instance);
		}

		if (label == "f") {
			for (int i = 0; i < 3; i++)
			{
				unsigned int one, two, three;
				char s;

				stream >> one;
				stream >> s;
				stream >> two;
				stream >> s;
				stream >> three;

				one--;
				two--;
				three--;

				VertInd instance;
				instance.locInd = one;
				instance.uvInd = two;
				instance.normInd = three;
				vertInds.push_back(instance);
			}
		}
	}

	ifs.close();

	vertCount = vertInds.size(); 
	vector<Vertex> vertBufData;
	vertBufData.reserve(vertCount);

	for (unsigned int i = 0; i < vertCount; i++) {
		vertBufData.push_back({ 
			locations[vertInds[i].locInd],
			uvs[vertInds[i].uvInd],
			normals[vertInds[i].normInd]
		});
	}

	// get half XYZ for collisions 
	modelHalfsize.x = locations[0].x;
	modelHalfsize.y = locations[0].y;
	modelHalfsize.z = locations[0].z;

	for (int i = 0; i < locations.size(); i++)
	{
		if (modelHalfsize.x < locations[i].x) { modelHalfsize.x = locations[i].x; }
		if (modelHalfsize.y < locations[i].y) { modelHalfsize.y = locations[i].y; }
		if (modelHalfsize.z < locations[i].z) { modelHalfsize.z = locations[i].z; }

	}


	// upload model info to shaders and such 
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER, // where to copy to
		sizeof(Vertex)*vertCount, // # bytes to copy
		&vertBufData[0], // where to copy from 
		GL_STATIC_DRAW);  // "hint" to open gl

	glEnableVertexAttribArray(0); // attribute index 
	glVertexAttribPointer(
		0,					//attribute index
		3,					//number of components (x, y, z)
		GL_FLOAT,			//data type
		GL_FALSE,			//normalize?
		sizeof(Vertex),		//stride (bytes per vertex)
		0					//offset to this attr
	);

	glEnableVertexAttribArray(3); // UV
	glVertexAttribPointer(
		3,					//attribute index
		2,					//number of components (x, y, z)
		GL_FLOAT,			//data type
		GL_FALSE,			//normalize?
		sizeof(Vertex),		//stride (bytes per vertex)
		(void*)(sizeof(vec3))				//offset to this attr
	);

	
	glEnableVertexAttribArray(2); // NORMAL
	glVertexAttribPointer(
		2,					//attribute index
		3,					//number of components (x, y, z)
		GL_FLOAT,			//data type
		GL_FALSE,			//normalize?
		sizeof(Vertex),		//stride (bytes per vertex)
		(void*)(sizeof(vec3) + sizeof(vec2))				//offset to this attr
	);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}


Model::~Model()
{
}
