#include "terrain_mesh.h"

TerrainMesh::TerrainMesh()
{
	terrain_verticies.clear();
	terrain_verticies.clear();

	x_length = 3;
	z_length = 3;
}

void TerrainMesh::GenerateVertices()
{
	for (int y_axis = 0; y_axis < z_length; y_axis++)
	{
		for (int x_axis = 0; x_axis < x_length; x_axis++)
		{
			int pos = (y_axis * x_length) + x_axis;

			gef::Mesh::Vertex vertex;

			vertex.px = x_axis;
			vertex.py = 0.5f;// y_axis;
			vertex.pz = y_axis;

			vertex.nx = x_axis;
			vertex.ny = y_axis;
			vertex.nz = x_axis;

			vertex.u = 0;
			vertex.v = 0;

			terrain_verticies.push_back(vertex);

		}
	}
	GenerateRandomHeights();
}

void TerrainMesh::GenerateRandomHeights()
{

	for (int vertexCount = 0; vertexCount < terrain_verticies.size(); vertexCount++)
	{

		terrain_verticies[vertexCount].py = std::rand();

	}

}

void TerrainMesh::GenerateIndex()
{

	for (int y_axis = 0; y_axis < z_length - 1; y_axis++)
	{
		for (int x_axis = 0; x_axis < x_length - 1; x_axis++)
		{

			/*

			y+1	A-----B
			|    /|
			| 1 / |
			|  /  |
			| /	  |
			|/	2 |
			y	C-----D

			x    x+1
			*/

			float a = (x_length * (y_axis + 1)) + x_axis;
			float b = (x_length * (y_axis + 1)) + x_axis + 1;
			float c = (x_length * (y_axis)) + x_axis;
			float d = (x_length * (y_axis)) + x_axis + 1;


			//// Face 1

			// Upper left.
			terrain_index.push_back(c);

			// Upper right.
			terrain_index.push_back(b);

			// lower left
			terrain_index.push_back(a);


			//// Face 2

			// Bottom right
			terrain_index.push_back(c);

			// Upper right
			terrain_index.push_back(d);

			// Lower left.
			terrain_index.push_back(b);

		}

	}
}
