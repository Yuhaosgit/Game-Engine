#include "Camera.h"
#include "MeshRender.h"
#include "Material.h"
#include "OGLRenderer.h"
#include "GameObject.h"

void Camera::UpdateShadowData() {
	for (int i = 0; i < opaqueList.size(); ++i) {
		Matrix4 lightProjectjMatrix = Matrix4::Orthographic(0, 4000, 4000, -4000, 4000, -4000);
		Matrix4 lightViewMatrix = Matrix4::BuildViewMatrix(Vector3(0, 0, 0),
			(Matrix4::Rotation(45, Vector3(1, 0, 0)) * Vector3(0, 0, 1)));
		Matrix4 shadowMatrix = lightProjectjMatrix * lightViewMatrix;
		const Matrix4& modelMatrix = opaqueList[i].lock()->gameObject->GetComponent<Transform>()->GetModelMatrix();
		shadowMatrix = shadowMatrix * modelMatrix;

		if (opaqueList[i].lock()->shadowCast) {
			auto currentMat = opaqueList[i].lock()->shadowMaterial.lock();
			currentMat->SetMatrix4("shadowMatrix", shadowMatrix);
		}
	}
}

void Camera::UpdateLightData() {
	//Vector2 pixelSize = Vector2(1 / OGLRenderer::GetWidth(), 1 / OGLRenderer::GetHeight());
	//Matrix4 inverseProjView = (projMatrix * viewMatrix).Inverse();

	//Matrix4 lightProjectjMatrix = Matrix4::Orthographic(0, 4000, 4000, -4000, 4000, -4000);
	//Matrix4 lightViewMatrix = Matrix4::BuildViewMatrix(Vector3(0, 0, 0),
	//	(Matrix4::Rotation(45, Vector3(1, 0, 0)) * Vector3(0, 0, 1)));
	//Matrix4 shadowMatrix = lightProjectjMatrix * lightViewMatrix;

	//for (int i = 0; i < visibleObject.size(); ++i) {
	//	if (visibleObject[i].lock()->light != nullptr) {
	//		auto currentMat = visibleObject[i].lock()->light->material.lock();
	//		if (visibleObject[i].lock()->light->GetLightType() == LightType::Direct) {
	//			currentMat->SetVector2("pixelSize", pixelSize);
	//			currentMat->SetVector3("cameraPos", position);

	//			currentMat->SetVector3("lightOrientation", visibleObject[i].lock()->transform->GetRotate());
	//			currentMat->SetVector4("lightColour", visibleObject[i].lock()->light->color);

	//			currentMat->SetMatrix4("inverseProjView", inverseProjView);
	//			currentMat->SetMatrix4("shadowMatrix", shadowMatrix);
	//		}
	//	}
	//}
}