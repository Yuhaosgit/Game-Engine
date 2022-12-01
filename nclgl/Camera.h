#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include <vector>
#include <fstream>
#include <memory>

class MeshRender;
class Material;

class Camera {
public:
	Camera(const float& yaw_a = 0.0f, const float& pitch_a = 0.0f, const Vector3& pos_a = Vector3(2000,200,2000)) {
		yaw = yaw_a;
		pitch = pitch_a;
		position = pos_a;
		speed_coe = 50.0f;
		SetProjMatrix();

		writenFile.open("position.txt");
	}
	~Camera() {
		if (!point_data.empty())
			PrintPosition(point_data);
		writenFile.close();
		point_data.clear();
	};

	void UpdateCamera(float dt);
	void UpdateList();

	void UpdateViewMatrix();
	const Matrix4& GetViewMatrix() { return viewMatrix; }

	void SetProjMatrix();
	const Matrix4& GetProjMatrix() { return projMatrix; }

	Vector3 GetPosition() { return position; }
	void SetSpeed(const float& speed) { speed_coe = speed; }

	std::vector<std::weak_ptr<MeshRender>> opaqueList;
	std::vector<std::weak_ptr<MeshRender>> transparentList;

	void UpdateShadowData();
	void UpdateLightData();

	void RenderOpaque();
private:
	float yaw;
	float pitch;
	float speed_coe;
	Vector3 position;

	Matrix4 viewMatrix;
	Matrix4 projMatrix;

	//debug
	void FixPoint();
	void PrintPosition(const std::vector<Vector3>& positions);
	std::ofstream writenFile;
	std::vector<Vector3> point_data;
};