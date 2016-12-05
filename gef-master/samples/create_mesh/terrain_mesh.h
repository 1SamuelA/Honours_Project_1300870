#pragma once

#include <game_object.h>
#include <graphics\mesh.h>
#include <vector>
#include <random>


class PerlinNoise;

class TerrainMesh 
{
public:
	TerrainMesh();
	void GenerateVertices();
	void GenerateRandomHeights();
	void GenerateIndex();
	void CalculateNormals();


	inline std::vector<gef::Mesh::Vertex> GetTerrainVerticies() { return terrain_verticies; }
	inline std::vector<int> GetTerrainIndices() { return terrain_index; }

PerlinNoise* perlin_noise_;
private:
	std::vector<gef::Mesh::Vertex> terrain_verticies;
	std::vector<int> terrain_index;

	char* filename;

	


	float x_length;
	float z_length;




};