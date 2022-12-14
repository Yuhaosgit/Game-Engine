#include "Material.h"

Matrix4 Material::viewMatrix;
Matrix4 Material::projMatrix;
Matrix4 Material::modelMatrix;
Matrix4 Material::shadowMatrix;

void Material::SubmitFloatDataSet() {
	for (auto& data : floatDataSet) {
		shader->SubmitFloat(data.second, data.first);
	}
}
void Material::SubmitIntDataSet() {
	for (auto& data : intDataSet) {
		shader->SubmitInt(data.second, data.first);
	}
}
void Material::SubmitBoolDataSet() {
	for (auto& data : boolDataSet) {
		shader->SubmitBool(data.second, data.first);
	}
}
void Material::SubmitVector2Set() {
	for (auto& data : Vector2Set) {
		shader->SubmitVector2(data.second, data.first);
	}
}
void Material::SubmitVector3Set() {
	for (auto& data : Vector3Set) {
		shader->SubmitVector3(data.second, data.first);
	}
}
void Material::SubmitVector4Set() {
	for (auto& data : Vector4Set) {
		shader->SubmitVector4(data.second, data.first);
	}
}
void Material::SubmitMatrixSet() {
	for (auto& data : MatrixSet) {
		shader->SubmitMatrix4(data.second, data.first);
	}
	shader->SubmitMatrix4(modelMatrix, "modelMatrix");
	shader->SubmitMatrix4(viewMatrix, "viewMatrix");
	shader->SubmitMatrix4(projMatrix, "projMatrix");
	shader->SubmitMatrix4(shadowMatrix, "shadowMatrix");
}
void Material::SubmitTextureSet() {
	int i = 0;
	for (auto& data : TextureSet) {
		data.second.Submit(shader, data.first, i);
		i++;
	}
}

void Material::SetFloat(std::string name, float value) {
	floatDataSet[name] = value;
}

void Material::SetInt(std::string name, int value) {
	intDataSet[name] = value;
}

void Material::SetBool(std::string name, bool value) {
	boolDataSet[name] = value;
}

void Material::SetVector2(std::string name, const Vector2& vec) {
	Vector2Set[name] = vec;
}

void Material::SetVector3(std::string name, const Vector3& vec) {
	Vector3Set[name] = vec;
}

void Material::SetVector4(std::string name, const Vector4& vec) {
	Vector4Set[name] = vec;
}

void Material::SetMatrix4(std::string name, const Matrix4& mat4) {
	MatrixSet[name] = mat4;
}

void Material::SetTexture2D(std::string name, Texture2D tex) {
	TextureSet[name] = tex;
}

void Material::BindShader() {
	glUseProgram(shader->GetProgram());
}

void Material::SubmitData() {
	BindShader();
	SubmitFloatDataSet();
	SubmitIntDataSet();
	SubmitBoolDataSet();
	SubmitVector2Set();
	SubmitVector3Set();
	SubmitVector4Set();
	SubmitMatrixSet();
	SubmitTextureSet();
}