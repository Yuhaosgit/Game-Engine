#include "MeshRender.h"
#include "Mesh.h"
#include "Material.h"
#include "Importer.h"
#include "GameObject.h"

const std::string MeshRender::Type = "MeshRender";
std::vector<std::shared_ptr<MeshRender>> MeshRender::meshRenders;

void MeshRender::AddInstance(std::shared_ptr<Component> component) {
	meshRenders.emplace_back(std::dynamic_pointer_cast<MeshRender>(component));
}

MeshRender::MeshRender(){
	material = std::make_unique <Material>();
	shadowMaterial = Importer::MaterialSet["ShadowMaterial"];
}

MeshRender::MeshRender(const MeshRender& copy):Component(copy) {
	mesh = copy.mesh.lock();
	Material* mat = new Material();
	mat = copy.material.get();
	material.reset(mat);
	shadowMaterial = Importer::MaterialSet["ShadowMaterial"];
}

void MeshRender::Render() {
	if (material != nullptr && mesh.lock() != nullptr) {
		material->SubmitData();
		mesh.lock()->Draw();
	}
}

void MeshRender::RenderShadow() {
	if (mesh.lock() != nullptr) {
		shadowMaterial.lock()->SubmitData();
		mesh.lock()->Draw();
	}
}