#include "GameObject.h"
#include "Importer.h"

vector<GameObject*> GameObject::GameObjectSet;

GameObject::GameObject() {
	this->AddComponent(std::make_shared<Transform>());
	GameObjectSet.emplace_back(this);
}

GameObject::GameObject(const GameObject& copy) {
	for (auto& component : copy.components) {
		this->AddComponent(component.second);
	}
	GameObjectSet.emplace_back(this);
}
GameObject::~GameObject() {
	components.clear();
}

void GameObject::AddComponent(std::shared_ptr<Component> component) {
	if (components[component->GetType()] != nullptr) {
		return;
	}
	component->gameObject = this;
	components[component->GetType()] = component;
	component->AddInstance(component);
}