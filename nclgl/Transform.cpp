#include "Transform.h"
#include "GameObject.h"

const std::string Transform::Type = "Transform";
std::vector<std::shared_ptr<Transform>> Transform::transforms;

void Transform::AddInstance(std::shared_ptr<Component> component) {
	transforms.emplace_back(std::dynamic_pointer_cast<Transform>(component));
}

Transform::Transform() {
	scaleMatrix.ToIdentity();
	positionMatrix.ToIdentity();
	rotate.EulerAnglesToQuaternion(0, 0, 0);
	modelMatrix = rotate.RotationMatrix() * positionMatrix * scaleMatrix;
}

void Transform::SetScale(const Vector3& vec) {
	scaleMatrix = Matrix4::Scale(vec);
	Update();
}
const Vector3& Transform::GetScale() {
	return scaleMatrix.GetScalingVector();
}

void Transform::SetRotate(const Vector3& vec) {
	rotate = Quaternion::EulerAnglesToQuaternion(vec.x, vec.y, vec.z);
	Update();
}
const Vector3& Transform::GetRotate() {
	return rotate.ToEuler();
}

void Transform::SetPosition(const Vector3& vec) {
	positionMatrix.SetPositionVector(vec);
	Update();
}

void Transform::Translate(const Vector3& vec) {
	Vector3 position = positionMatrix.GetPositionVector();
	positionMatrix.SetPositionVector(position + vec);
	Update();
}

const Vector3& Transform::GetPosition() {
	return positionMatrix.GetPositionVector();
}

void Transform::Update() {
	if (parent.lock()) {
		Translate(parent.lock()->GetPosition());
	}

	for (int i = 0; i < children.size(); ++i)
		children[i].lock()->Update();

	modelMatrix = rotate.RotationMatrix() * positionMatrix * scaleMatrix;
}

void Transform::AddChild(std::weak_ptr<Transform> child){
	children.emplace_back(child);
	child.lock()->parent = shared_from_this();
}