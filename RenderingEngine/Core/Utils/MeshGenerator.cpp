/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MeshGenerator.cpp
Purpose: helper class for generating meshes.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "MeshGenerator.h"
#include <unordered_map>
#include <glm/ext/scalar_constants.hpp>
#include "Math.h"
#include <algorithm>

std::tuple<MeshGenerator::PointsPtr, MeshGenerator::IndicesPtr> MeshGenerator::
GeneratePlanePointsWithIndices(float size)
{
	PointsPtr points = std::make_shared<std::vector<glm::vec3>>();
	IndicesPtr indices = std::make_shared<std::vector<unsigned int>>();
	points->push_back({ -size, -size,0 });
	points->push_back({ size, -size,0 });
	points->push_back({ size, size,0 });
	points->push_back({ -size, size,0 });
	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(0);
	indices->push_back(2);
	indices->push_back(3);

	return { points,indices };
}

std::tuple<MeshGenerator::PointsPtr, MeshGenerator::IndicesPtr>
MeshGenerator::GenerateSpherePointsWithIndices(float radius, int segments, int rings)
{
	//clamp
	segments = std::max(segments, 3);
	rings = std::max(rings, 3);
	constexpr float pi = glm::pi<float>();
	PointsPtr points = std::make_shared<std::vector<glm::vec3>>();
	float theta_step = pi / static_cast<float>(rings);
	float p_step = 2.f * pi / static_cast<float>(segments);

	float theta = theta_step;
	for (int ring = 1; ring < rings; ++ring)
	{
		float ring_rad = radius * std::sin(theta);
		float p = 0.f;
		for (int segment = 0; segment < segments; ++segment)
		{
			points->push_back({ ring_rad * glm::sin(p),radius * std::cos(theta) , ring_rad * glm::cos(p) });
			p += p_step;
		}
		theta += theta_step;
	}
	IndicesPtr indices = std::make_shared<std::vector<unsigned int>>();
	int sides = rings - 2; //- top and bottom

	//top
	points->push_back({ 0,radius,0 });
	int	top_index = static_cast<int>(points->size() - 1);
	for (int segment = 0; segment < segments; ++segment)
	{
		//triangle 1
		//*  top
		//** first circle
		indices->push_back(top_index);
		indices->push_back(segment % segments);
		indices->push_back((segment + 1) % segments);
	}

	for (int side = 0; side < sides; ++side)
	{
		int current_circle = side * segments;
		int next_circle = (side + 1) * segments;
		for (int segment = 0; segment < segments; ++segment)
		{
			//triangle 1
			//*
			//**
			indices->push_back(current_circle + segment);
			indices->push_back(next_circle + segment);
			indices->push_back(next_circle + (segment + 1) % segments);

			//triangle 2
			//**
			// *
			indices->push_back(current_circle + segment);
			indices->push_back(next_circle + (segment + 1) % segments);
			indices->push_back(current_circle + (segment + 1) % segments);
		}	
	}
	
	//bottom
	int last_circle = sides * segments;
	points->push_back({ 0,-radius,0 });
	int	bottom_index = static_cast<int>(points->size() - 1);
	for (int segment = 0; segment < segments; ++segment)
	{
		//triangle
		//** last_circle
		// * bottom
		indices->push_back(last_circle + segment % segments);
		indices->push_back(bottom_index);
		indices->push_back(last_circle + (segment + 1) % segments);
	}
	return { points,indices };
}

std::shared_ptr<LineMesh> MeshGenerator::GenerateOrbit(float radius, int numDivisions)
{
	//clamp
	radius = std::max(radius, 0.0001f);
	numDivisions = std::max(numDivisions, 10);

	std::shared_ptr<LineMesh> mesh = std::make_shared<LineMesh>();
	mesh->SetUseIndex(false);

	constexpr float pi = glm::pi<float>();
	float d_theta = 2.f * pi / static_cast<float>(numDivisions);
	float theta = 0.f;
	std::shared_ptr<std::vector<glm::vec3>> new_vertex = std::make_shared<std::vector<glm::vec3>>();
	glm::vec3 prev{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
	theta += d_theta;
	for (int step=1; step<numDivisions; ++step)
	{
		new_vertex->push_back(prev);
		prev = glm::vec3{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
		new_vertex->push_back(prev);
		theta += d_theta;
	}
	new_vertex->push_back(prev);
	new_vertex->push_back((*new_vertex)[0]);

	std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::CreateVertexBuffer(new_vertex->size() * sizeof(glm::vec3));

	vertex_buffer->SetData(static_cast<unsigned int>(new_vertex->size() * sizeof(glm::vec3)), new_vertex->data());

	vertex_buffer->SetDataTypes({ {0, DataType::Float3} });
	mesh->AttachBuffer(vertex_buffer);

	mesh->SetVertices(new_vertex);

	return mesh;
}

std::shared_ptr<Mesh> MeshGenerator::GenerateFaceNormalMesh(const std::vector<glm::vec3>& loaded_points,
                                                            const std::vector<unsigned>& loaded_index, const std::vector<glm::vec3>& face_normals)
{
	std::shared_ptr<Mesh> mesh=std::make_shared<Mesh>();
	mesh->SetUseIndex(false);
	
 	const unsigned int faces = static_cast<unsigned int>(loaded_index.size() / 3);
	std::shared_ptr<std::vector<Vertex>> new_vertex = std::make_shared<std::vector<Vertex>>();
	for (unsigned int i=0; i<faces; ++i)
	{
		const unsigned int offset = i * 3;
		new_vertex->push_back({ loaded_points[loaded_index[offset]],face_normals[i] });
		new_vertex->push_back({ loaded_points[loaded_index[offset + 1]],face_normals[i] });
		new_vertex->push_back({ loaded_points[loaded_index[offset + 2]],face_normals[i] });
	}
	std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::CreateVertexBuffer(new_vertex->size() * sizeof(Vertex));
	vertex_buffer->SetData(static_cast<unsigned int>(new_vertex->size() * sizeof(Vertex)), new_vertex->data());
	vertex_buffer->SetDataTypes({ {0, DataType::Float3},{1, DataType::Float3} });

	mesh->AttachBuffer(vertex_buffer);

	mesh->SetVertices(new_vertex);

	return mesh;
}

std::shared_ptr<LineMesh> MeshGenerator::GenerateFaceNormalLineMesh(const std::vector<glm::vec3>& loaded_points,
	const std::vector<unsigned>& loaded_index, const std::vector<glm::vec3>& face_normals, float normal_len)
{
	std::shared_ptr<LineMesh> mesh = std::make_shared<LineMesh>();
	std::shared_ptr<std::vector<glm::vec3>> new_vertex = std::make_shared<std::vector<glm::vec3>>();
	const unsigned int faces = static_cast<unsigned int>(loaded_index.size() / 3);
	for (unsigned int i = 0; i < faces; ++i)
	{
		const unsigned int offset = i * 3;
		const glm::vec3& v0 = loaded_points[loaded_index[offset]];
		const glm::vec3& v1 = loaded_points[loaded_index[offset + 1]];
		const glm::vec3& v2 = loaded_points[loaded_index[offset + 2]];
		const glm::vec3 face_center = (v0 + v1 + v2) / 3.f;
		new_vertex->push_back(face_center);
		new_vertex->push_back(face_center + normal_len * face_normals[i]);
	}
	std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::CreateVertexBuffer(new_vertex->size() * sizeof(glm::vec3));

	vertex_buffer->SetData(static_cast<unsigned int>(new_vertex->size() * sizeof(glm::vec3)), new_vertex->data());

	vertex_buffer->SetDataTypes({ {0, DataType::Float3} });
	mesh->AttachBuffer(vertex_buffer);

	mesh->SetVertices(new_vertex);

	return mesh;
}

std::shared_ptr<Mesh> MeshGenerator::GenerateVertexNormalMesh(const std::vector<glm::vec3>& loaded_points,
	const std::vector<unsigned>& loaded_index, const std::vector<glm::vec3>& vertex_normals)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->SetUseIndex(true);
	const unsigned int vertices = static_cast<unsigned int>(loaded_points.size());
	std::shared_ptr<std::vector<Vertex>> new_vertex = std::make_shared<std::vector<Vertex>>();
	new_vertex->reserve(vertices);
	for (unsigned int i = 0; i < vertices; ++i)
	{
		new_vertex->push_back({ loaded_points[i], vertex_normals[i] });
	}


	std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::CreateVertexBuffer(new_vertex->size() * sizeof(Vertex));
	vertex_buffer->SetData(static_cast<unsigned int>(new_vertex->size() * sizeof(Vertex)), new_vertex->data());
	vertex_buffer->SetDataTypes({ {0, DataType::Float3},{1, DataType::Float3} });

	std::shared_ptr<IndexBuffer> index_buffer = IndexBuffer::CreateIndexBuffer(sizeof(unsigned int) * loaded_index.size());
	index_buffer->SetData(sizeof(unsigned int) * loaded_index.size(), loaded_index.data());

	mesh->AttachBuffer(vertex_buffer);
	mesh->AttachBuffer(index_buffer);

	mesh->SetVertices(new_vertex);
	std::shared_ptr<std::vector<unsigned>> new_index = std::make_shared<std::vector<unsigned>>(loaded_index);
	mesh->SetIndices(new_index);
	return mesh;
}

std::shared_ptr<LineMesh> MeshGenerator::GenerateVertexNormalLineMesh(const std::vector<glm::vec3>& loaded_points,
	const std::vector<unsigned>& loaded_index, const std::vector<glm::vec3>& vertex_normals, float normal_len)
{
	std::shared_ptr<LineMesh> mesh = std::make_shared<LineMesh>();
	std::shared_ptr<std::vector<glm::vec3>> new_vertex = std::make_shared<std::vector<glm::vec3>>();
	new_vertex->reserve(vertex_normals.size() + loaded_index.size());
	const unsigned int total_vertex= static_cast<unsigned int>(loaded_points.size());
	for (unsigned int i = 0; i < total_vertex; ++i)
	{
		new_vertex->push_back(loaded_points[i]);
		new_vertex->push_back(loaded_points[i] + normal_len * vertex_normals[i]);
	}
	std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::CreateVertexBuffer(new_vertex->size() * sizeof(glm::vec3));

	vertex_buffer->SetData(static_cast<unsigned int>(new_vertex->size() * sizeof(glm::vec3)), new_vertex->data());

	vertex_buffer->SetDataTypes({ {0, DataType::Float3} });
	mesh->AttachBuffer(vertex_buffer);

	mesh->SetVertices(new_vertex);
	return mesh;
}

std::vector<glm::vec3> MeshGenerator::GenerateFaceNormals(std::vector<glm::vec3>& loaded_points,
                                                          std::vector<unsigned>& loaded_index)
{
	std::vector<glm::vec3> face_normals;
	const unsigned int total_faces = static_cast<unsigned int>(loaded_index.size()) / 3;
	face_normals.reserve(total_faces);
	for (unsigned int i = 0; i < total_faces; ++i)
	{
		const unsigned int offset = i * 3;
		const glm::vec3& v0 = loaded_points[loaded_index[offset]];
		const glm::vec3& v1 = loaded_points[loaded_index[offset + 1]];
		const glm::vec3& v2 = loaded_points[loaded_index[offset + 2]];
		face_normals.emplace_back(Math::ComputeFaceNormal(v0, v1, v2));
	}
	return face_normals;
}

std::vector<glm::vec3> MeshGenerator::GenerateVertexNormals(std::vector<glm::vec3>& loaded_points,
	std::vector<glm::vec3>& face_normals, std::vector<unsigned>& loaded_index)
{
	std::vector<glm::vec3> generated_normals{ loaded_points.size() };

	std::unordered_map<unsigned int, std::vector<unsigned int>> vertex_faces_map;
	vertex_faces_map.reserve(loaded_points.size());
	const unsigned int total_faces = static_cast<unsigned int>(loaded_index.size()) / 3;
	for (unsigned int i = 0; i < total_faces; ++i)
	{
		const unsigned int offset = i * 3;
		vertex_faces_map[loaded_index[offset]].push_back(i);
		vertex_faces_map[loaded_index[offset + 1]].push_back(i);
		vertex_faces_map[loaded_index[offset + 2]].push_back(i);
	}

	std::vector<glm::vec3> prev_normals;
	int vertex_index;
	for (auto& pair : vertex_faces_map)
	{
		vertex_index = pair.first;
		prev_normals.clear();
		glm::vec3 acc_normal(0.f);
		std::vector<unsigned int>& face_indices = pair.second;
		for (unsigned int i : face_indices)
		{
			if (std::find(prev_normals.begin(), prev_normals.end(), face_normals[i]) == prev_normals.end()) //check the normal is unique.
			{
				acc_normal += face_normals[i];
				prev_normals.push_back(face_normals[i]);
			}
		}
		generated_normals[vertex_index] = glm::normalize(acc_normal);
	}
	return generated_normals;
}

std::shared_ptr<BoundingBox> MeshGenerator::GenerateBoundingBox(std::vector<glm::vec3>& loaded_points)
{
	std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
	box->min = glm::vec3{ std::numeric_limits<float>().max() };
	box->max = glm::vec3{ std::numeric_limits<float>().min() };
	for (const glm::vec3& point : loaded_points)
	{
		box->min.x = std::min(box->min.x, point.x);
		box->min.y = std::min(box->min.y, point.y);
		box->min.z = std::min(box->min.z, point.z);

		box->max.x = std::max(box->max.x, point.x);
		box->max.y = std::max(box->max.y, point.y);
		box->max.z = std::max(box->max.z, point.z);
	}
	box->center = (box->max + box->min) * 0.5f;
	return box;
}


