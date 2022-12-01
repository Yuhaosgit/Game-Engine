#include "Window.h"
#include "Camera.h"
#include "OGLRenderer.h"
#include "MeshRender.h"
#include "Material.h"
#include "GameObject.h"

#include <algorithm>
#include <iostream>

void Camera::UpdateCamera(float dt) {
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	pitch = std::max(pitch, -90.0f);
	pitch = std::min(pitch, 90.0f);

	if (yaw > 360)
		yaw -= 360.0f;
	if (yaw < 0)
		yaw += 360.0f;

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f));
	Vector3 forward = rotation * Vector3(0.0f, 0.0f, -1.0f);
	Vector3 right = rotation * Vector3(1.0f, 0.0f, 0.0f);
	
	float speed = speed_coe * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
		position += (forward * speed);
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
		position -= (forward * speed);
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
		position -= (right * speed);
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))
		position += (right * speed);
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))
		position.y -= speed;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))
		position.y += speed;

	UpdateViewMatrix();
	FixPoint();
}

void Camera::UpdateList() {
	for (int i = 0; i < MeshRender::meshRenders.size(); ++i) {
		opaqueList.emplace_back(MeshRender::meshRenders[i]);
	}
}

void Camera::RenderOpaque() {
	Material::projMatrix = projMatrix;
	Material::viewMatrix = viewMatrix;

	for (int i = 0; i < opaqueList.size(); ++i) {
		Material::modelMatrix = opaqueList[i].lock()->gameObject->GetComponent<Transform>()->GetModelMatrix();
		opaqueList[i].lock()->Render();
	}
	opaqueList.clear();
}

void Camera::UpdateViewMatrix() {
	viewMatrix =  Matrix4::Rotation(-pitch, Vector3(1.0f, 0.0f, 0.0f))
		* Matrix4::Rotation(-yaw, Vector3(0.0f, 1.0f, 0.0f))
		* Matrix4::Translation(-position);
}

void Camera::SetProjMatrix() {
	float width = static_cast<float>(OGLRenderer::GetWidth());
	float height = static_cast<float>(OGLRenderer::GetHeight());

	projMatrix = Matrix4::Perspective(0.01f, 1000.0f, width / height, 45.0f);
}

void Camera::FixPoint(){
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_N)) {
		if (point_data.empty())
			point_data.emplace_back(position);
		
		else if (position != point_data.back())
			point_data.emplace_back(position); 
	}
}

void Camera::PrintPosition(const std::vector<Vector3>& positions) {
	for (int i = 0; i < positions.size(); ++i) {
		writenFile << "Vector3(" << positions[i].x << "," << positions[i].y << "," << positions[i].z << ")," <<std::endl;
	}
}