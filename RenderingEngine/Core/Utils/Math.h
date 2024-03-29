#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Math.h
Purpose: for maths.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <memory>
#include <vector>
#include<glm/glm.hpp>

#include "Core/BoundingVolume/AABB.h"
#include "Core/BoundingVolume/Sphere.h"
#include "Core/Data/BoundingBox.h"
//bit compute
#define BIT(x) (1)<<(x)
enum class EPOS
{
	EPOS6,
	EPOS14,
	EPOS26,
	EPOS98
};

class Math
{
public:
	//Matrix
	static glm::mat4 BuildCameraMatrix(glm::vec3 cam_position, glm::vec3 target, glm::vec3 world_up);
	static glm::mat4 BuildCameraMatrixWithDirection(const glm::vec3& cam_position, const glm::vec3& lookAt, const glm::vec3& world_up = { 0,1.f,0.f });
	static glm::mat4 BuildPerspectiveProjectionMatrix(float width, float height, float near, float far);
	static glm::mat4 BuildPerspectiveProjectionMatrixFovx(float fov_x, float aspect_ratio, float near, float far);
	static glm::mat4 BuildPerspectiveProjectionMatrixFovy(float fov_y, float aspect_ratio, float near, float far);
public:
	//Mesh
	static glm::vec3 ComputeFaceNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
public:
	static glm::vec2 ComputePlanarUV(const glm::vec3 TextureEntity, float min_x, float max_x, float min_y, float max_y);
	static glm::vec2 ComputeCylindricalUV(const glm::vec3& TextureEntity, float min_y, float max_y);
	static glm::vec2 ComputeSphericalUV(const glm::vec3& TextureEntity);
	static glm::vec2 ComputeCubeMapUV(const glm::vec3& TextureEntity);

	static std::shared_ptr<std::vector<glm::vec2>> ComputePlanarUVs(const std::vector<glm::vec3> TextureEntities, const BoundingBox& box);
	static std::shared_ptr<std::vector<glm::vec2>> ComputeCylindricalUVs(const std::vector<glm::vec3> TextureEntities, const BoundingBox& box);
	static std::shared_ptr<std::vector<glm::vec2>> ComputeSphericalUVs(const std::vector<glm::vec3> TextureEntities);
	static std::shared_ptr<std::vector<glm::vec2>> ComputeCubeMapUVs(const std::vector<glm::vec3> TextureEntities);

public:
	static std::vector<glm::vec3> RandomVec3(int how_many, const glm::vec3& min, const glm::vec3& max);

	static std::tuple<glm::vec3, glm::vec3> FindMinAndMax(const std::vector<glm::vec3>& vertices);
	static std::tuple<int, int> ExtremePairAlongDirection(glm::vec3 dir,
		const std::vector<glm::vec3>& vertices);
	
	static void ExtendSphere(Sphere& sphere, const std::vector<glm::vec3>& vertices);

	static Sphere MergeTwoSphere(const Sphere& left, const Sphere& right);
	static Sphere CreateEnclosingSphere(const std::vector<Sphere>& spheres);
	static AABB CreateEnclosingAABB(const std::vector<AABB>& aabbs);
	static float ComputeAABBVolume(const AABB& aabb);
	static glm::vec3 ComputePCAVector(const std::vector<glm::vec3>& vertices);
	static AABB CreateAABB(const std::vector<glm::vec3>& vertices);
	static Sphere CreateRitterSphere(const std::vector<glm::vec3>& vertices);
	static Sphere CreateLarssonSphere(const std::vector<glm::vec3>& vertices, EPOS epos = EPOS::EPOS98);
	static Sphere CreatePCASphere(const std::vector<glm::vec3>& vertices);
};

