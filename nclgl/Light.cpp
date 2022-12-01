#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Importer.h"
#include "GameObject.h"

const std::string Light::Type = "Light";
std::vector<std::shared_ptr<Light>> Light::lights;

void Light::AddInstance(std::shared_ptr<Component> component) {
	lights.emplace_back(std::dynamic_pointer_cast<Light>(component));
}

Light::Light() :lightType(LightType::Direct){
	mesh = Importer::MeshSet["Quad"];
	material = Importer::MaterialSet["DirectLightMaterial"];
}

Light::Light(const Light& copy) : color(copy.color) ,Component(copy){
	lightType = copy.lightType;
	mesh = copy.mesh.lock();
	material = copy.material.lock();
}

void Light::SetLightType(LightType type) {
	lightType = type;
	if (type == LightType::Direct) {
		mesh = Importer::MeshSet["Quad"];
		material = Importer::MaterialSet["DirectLightMaterial"];
	}
	else if (type == LightType::Point) {
		mesh = Importer::MeshSet["Sphere"];
		material = Importer::MaterialSet["PointLightMaterial"];
	}
}

void Light::Render() {
	material.lock()->SubmitData();
	mesh.lock()->Draw();
}