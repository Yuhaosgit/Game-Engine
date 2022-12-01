#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Vector4.h"
#include "Component.h"

class Material;
class Mesh;
class GameObject;

enum class LightType {
	Direct, Point
};

class Light:public Component {
public:
	void AddInstance(std::shared_ptr<Component> component);
	static std::vector<std::shared_ptr<Light>> lights;

	static const std::string Type;
	std::string GetType() override { return Type; }

	Light();
	Light(const Light& copy);

	void Render();

	void SetLightType(LightType type);
	LightType GetLightType() { return lightType; }

	Vector4 color;
	std::weak_ptr<Material> material;
private:
	std::weak_ptr<Mesh> mesh;
	LightType lightType;
};