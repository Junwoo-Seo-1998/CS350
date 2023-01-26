#include "CameraLayer.h"

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
	constexpr float speed = 10.f;
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
	const float len = static_cast<float>(movement.length());
	if (len !=0.f)
	{
		movement /= len;
	}
	camTransform.Position += movement * speed * Time::GetDelta();
}

void CameraLayer::OnRender()
{

}

void CameraLayer::OnPostRender()
{

}

void CameraLayer::OnGuiRender()
{

}

void CameraLayer::OnEvent(Event& event)
{

}
