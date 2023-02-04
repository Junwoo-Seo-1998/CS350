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
#include <glm/vec4.hpp>

#include "Layer.h"
#include "Scene/DemoEnums/DemoEnums.h"
class VertexArray;

class LineRenderLayer:public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
	void OnGuiRender() override;
	void OnEvent(Event& event) override;
private:
	std::shared_ptr<VertexArray> vao;
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);
	glm::vec4 lineColor = { 0.f,0.5f,2.f,1.f };
};
