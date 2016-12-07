#include "terrain_mesh.h"
#include <random>
#include <time.h>

#include <system/debug_log.h>
#include <PerlinNoise.h>

TerrainMesh::TerrainMesh()
{
	terrain_verticies.clear();
	terrain_verticies.clear();

	std::srand((int)time);
float x = std::rand();

	perlin_noise_ = new PerlinNoise(2, 0.25, 4, 1, x);

	x_length = 100;
	z_length = 100;
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
	CalculateNormals();
}

void TerrainMesh::GenerateRandomHeights()
{
	srand(time(NULL));
	for (int vertexCount = 0; vertexCount < terrain_verticies.size(); vertexCount++)
	{
		
		terrain_verticies[vertexCount].py = perlin_noise_->GetHeight(terrain_verticies[vertexCount].px, terrain_verticies[vertexCount].pz);

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



void TerrainMesh::CalculateNormals()
{


	/*

					u, (v-1)
				1-------2-------3
				|     / | A   / |
				|   /   |   /   |
				| /   F | /   B |
	v, (u-1)	4-------5-------6	v, (u+1)
				| E   / | C   / |
				|   /   |   /   |
				| /   D | /     |
				7-------8-------9
					u, (v+1)

	*/

	for (int z = 0; z < z_length-1; z++)
	{
		for (int x = 0; x < x_length-1; x++)
		{

			
			// The vertices used for all the surrounding surfaces, vertices 2-8 and surfaces A-F
			// Vertices initialised to vertex 5 since there are cases where some vertices don't exist,
			// i.e. the edges of the terrain
			// Vertex 5 is always required since it is a common vertex of all the surfaces
			gef::Vector4 position_vertice_5((float)x, terrain_verticies[(z*x_length + z) + 1].py, (float)z);
			gef::Vector4 position_vertice_2 = position_vertice_5;
			gef::Vector4 position_vertice_3 = position_vertice_5;
			gef::Vector4 position_vertice_4 = position_vertice_5;
			gef::Vector4 position_vertice_6 = position_vertice_5;
			gef::Vector4 position_vertice_7 = position_vertice_5;
			gef::Vector4 position_vertice_8 = position_vertice_5;

			// Determine which vertices are required
			if (x > 0)
			{
				// Vertex 4 is definitely required
				position_vertice_4 = gef::Vector4(x - 1, terrain_verticies[(z*x_length + x - 1) + 1].py, (float)z);
				if (z < z_length - 1)
				{
					// Vertex 7 is definitely required
					position_vertice_7 = gef::Vector4((float)x - 1, terrain_verticies[((z + 1)*x_length + x - 1) + 1].py, (float)z + 1);
				}
			}
			if (x < x_length - 1) {
				// Vertex 6 is definitely required
				position_vertice_6 = gef::Vector4((float)x + 1, terrain_verticies[(z*x_length + x + 1) + 1].py, (float)z);
				if (z > 0)
				{
					// Vertex 3 is definitely required
					position_vertice_3 = gef::Vector4((float)x + 1, terrain_verticies[((z - 1)*x_length + x + 1) + 1].py, (float)z - 1);
				}
			}
			if (z > 0) {
				// Vertex 2 is definitely required
				position_vertice_2 = gef::Vector4((float)x, terrain_verticies[ ((z - 1)*x_length + x) + 1].py, (float)z - 1);
			}
			if (z < z_length - 1) {
				// Vertex 8 is definitely required
				position_vertice_8 = gef::Vector4((float)x, terrain_verticies[((z + 1)*x_length + x) + 1].py, (float)z + 1);
			}

			// The direction vectors needed for calculating the normals of all six surfaces, A-F
			gef::Vector4 direction_vector_5_2 = position_vertice_2 - position_vertice_5;
			gef::Vector4 direction_vector_5_3 = position_vertice_3 - position_vertice_5;
			gef::Vector4 direction_vector_5_4 = position_vertice_4 - position_vertice_5;
			gef::Vector4 direction_vector_5_6 = position_vertice_6 - position_vertice_5;
			gef::Vector4 direction_vector_5_7 = position_vertice_7 - position_vertice_5;
			gef::Vector4 direction_vector_5_8 = position_vertice_8 - position_vertice_5;

			// The nomals of all six surfaces, A-F, calculated by the cross product of corresponding direction vectors (anti-clockwise)
			gef::Vector4 surface_normal_a = direction_vector_5_3.CrossProduct(direction_vector_5_2);
			gef::Vector4 surface_normal_b = direction_vector_5_6.CrossProduct(direction_vector_5_3);
			gef::Vector4 surface_normal_c = direction_vector_5_8.CrossProduct(direction_vector_5_6);
			gef::Vector4 surface_normal_d = direction_vector_5_7.CrossProduct(direction_vector_5_8);
			gef::Vector4 surface_normal_e = direction_vector_5_4.CrossProduct(direction_vector_5_7);
			gef::Vector4 surface_normal_f = direction_vector_5_2.CrossProduct(direction_vector_5_4);

			// Normal of vertex is the average of all surrounding surface normals, A-F
			gef::Vector4 normal(surface_normal_a + surface_normal_b + surface_normal_c + surface_normal_d + surface_normal_e + surface_normal_f);

			normal.Normalise();

			terrain_verticies[(z*x_length + z)].nx = normal.x();
			terrain_verticies[(z*x_length + z)].ny = normal.y();
			terrain_verticies[(z*x_length + z)].nz = normal.z();

			if ( (normal.y() < 0) )
			{
				gef::DebugOut("Normal Less then 0");
				gef::DebugOut("Normal Less then 0", normal);
			}

			if (x == z)
			{
				gef::DebugOut("Normal ", normal);
			}

		}
	}

	for (int z = 0; z < z_length - 1; z++)
	{
		for (int x = 0; x < x_length - 1; x++)
		{

			//if(terrain_verticies)

		}
	}


}

