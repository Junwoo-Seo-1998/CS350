/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "CameraLayer.h"

#include "imgui.h"
#include "Core/Application.h"
#include "Core/Time.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Input/Input.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/SceneManager.h"

void CameraLayer::OnAttach()
{

}

void CameraLayer::OnDetach()
{

}

void CameraLayer::OnUpdate()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();

	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::vec3 movement{ 0,0,0 };

	if (Input::IsPressed(KeyCode::W))
		movement += camTransform.GetForward();
	if (Input::IsPressed(KeyCode::S))
		movement -= camTransform.GetForward();

	if (Input::IsPressed(KeyCode::A))
		movement -= camTransform.GetRight();
	if (Input::IsPressed(KeyCode::D))
		movement += camTransform.GetRight();
	if (Input::IsPressed(KeyCode::Space))
	{
		movement += camTransform.GetUp();
	}
	if (Input::IsPressed(KeyCode::LeftControl))
	{
		movement -= camTransform.GetUp();
	}
	const float len = static_cast<float>(movement.length());
	if (len !=0.f)
	{
		movement /= len;
	}

	if(Input::IsPressed(KeyCode::LeftShift))
	{
		movement *= 5.f;
	}

	camTransform.Position += movement * speed * Time::GetDelta();

	if (Input::IsPressed(KeyCode::LeftAlt) && Input::IsPressed(MouseCode::Left))
	{
		auto [x_offset, y_offset] = Input::GetMouseOffset();

		x_offset *= MouseSensitivity * Time::GetDelta();
		y_offset *= MouseSensitivity * Time::GetDelta();

		float Pitch = camTransform.Rotation.x + y_offset;

		Pitch = glm::clamp(Pitch, glm::radians(-89.0f), glm::radians(89.0f));

		camTransform.Rotation.x = Pitch;
		camTransform.Rotation.y -= x_offset;
	}
	/*camTransform.LookAtDir(glm::vec3{ 0,0,0 } - camTransform.Position);
	if (glm::length(camTransform.Position) <= 2.f)
		camTransform.Position += camTransform.GetForward() * 8.f;*/
}

void CameraLayer::OnRender()
{

}

void CameraLayer::OnPostRender()
{

}

void CameraLayer::OnGuiRender()
{
	ImGui::Begin("Control");
	ImGui::TextColored({ 1,0,0,1 }, "WASD to move. Spacebar and Ctrl for Up and Down. Alt and Mouse left click for rotation");
	ImGui::DragFloat("Speed", &speed, 1000, 1.0f, 50000.f);
	ImGui::End();
}

void CameraLayer::OnEvent(Event& event)
{

}
