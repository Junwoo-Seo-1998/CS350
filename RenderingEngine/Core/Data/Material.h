#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Core/UUID.h"
#include "Core/Utils/DataType.h"
#include "Texture.h"
class Texture;
struct MaterialData
{
	MaterialData() = default;
	DataType type = DataType::None;

	union Data
	{
		bool Bool;
		int Int;
		float Float;
		glm::vec2 Float2;
		glm::vec3 Float3;
		glm::vec4 Float4;
		glm::mat3 Mat3;
		glm::mat4 Mat4;
	} data;

	MaterialData& operator=(const bool val);
	MaterialData& operator=(const int val);
	MaterialData& operator=(const float val);
	MaterialData& operator=(const glm::vec2& val);
	MaterialData& operator=(const glm::vec3& val);
	MaterialData& operator=(const glm::vec4& val);
	MaterialData& operator=(const glm::mat3& val);
	MaterialData& operator=(const glm::mat4& val);
};

struct Material
{
	std::shared_ptr<Texture> Diffuse = Texture::CreateTexture(glm::vec4{ 0.8f,0.8f,0.8f ,1.f });
	std::shared_ptr<Texture> Specular = Texture::CreateTexture(glm::vec4{ 0.5f,0.5f,0.5f,1.f });
	float Shininess = 32.f;
	std::shared_ptr<Texture> Emissive = Texture::CreateTexture(glm::vec4{ 0.f,0.f,0.f,1.f });
	std::unordered_map<std::string, MaterialData> Data;
	MaterialData& operator[](const std::string& key);
};