#pragma once

#include <game_object.h>
#include <graphics\mesh.h>
#include <vector>
#include <random>

class TerrainMesh 
{
public:
	TerrainMesh();
	void GenerateVertices();
	void GenerateRandomHeights();
	void GenerateIndex();



	inline std::vector<gef::Mesh::Vertex> GetTerrainVerticies() { return terrain_verticies; }
	inline std::vector<int> GetTerrainIndices() { return terrain_index; }


private:
	std::vector<gef::Mesh::Vertex> terrain_verticies;
	std::vector<int> terrain_index;

	char* filename;



	float x_length;
	float z_length;




};