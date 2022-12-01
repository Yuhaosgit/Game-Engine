#include <iostream>
#include <random>
#include <ctime>

#include "Renderer.h"
#include "PositionData.h"

Renderer::~Renderer() {
	Importer::ReleaseAllResources();
	GameObject::GameObjectSet.clear();
	delete camera;
}

void Renderer::Initialize() {
	auto ImportShader = []() {
		Importer::LoadShader("Shadow", "ShadowVertex.glsl", "ShadowFragment.glsl");
		Importer::LoadShader("Combine", "CombineVertex.glsl", "CombineFragment.glsl");
		Importer::LoadShader("OIT", "CombineVertex.glsl", "OITFragment.glsl");
		Importer::LoadShader("Present", "CombineVertex.glsl", "PresentFragment.glsl");
		Importer::LoadShader("Land", "LightTerrain_vertex.glsl", "LightTerrain_fragment.glsl");
		Importer::LoadShader("Water", "Water_vertex.glsl", "Water_fragment.glsl");
		Importer::LoadShader("Decal", "UnlitVertex.glsl", "UnlitFragment.glsl");
		Importer::LoadShader("DirectLight", "CombineVertex.glsl", "DirectLightFragment.glsl");
		Importer::LoadShader("PointLight", "UnlitVertex.glsl", "PointLightFragment.glsl");
		Importer::LoadShader("Gbuffer", "StaticObjectVertex.glsl", "DeferredObjectFragment.glsl");
		Importer::LoadShader("Debug", "DebugVertex.glsl", "DebugFragment.glsl");
	};
	auto CreateMaterial = []() {
		auto shadowMatertial = std::make_shared<Material>();
		shadowMatertial->shader = Importer::ShaderSet["Shadow"];
		Importer::SetMaterial("ShadowMaterial", shadowMatertial);

		auto decalMaterial = std::make_shared<Material>();
		decalMaterial->shader = Importer::ShaderSet["Decal"];
		decalMaterial->SetTexture2D("stencilDepthTex", FrameBuffer<GeometryFBO>::instance()->stencilDepthTarget);
		decalMaterial->SetTexture2D("normals", FrameBuffer<GeometryFBO>::instance()->normalTarget);
		decalMaterial->SetTexture2D("targetTex", Importer::TextureSet["doge.png"]);
		decalMaterial->SetTexture2D("targetNormalTex", Importer::TextureSet["dogeNormal.png"]);
		Importer::SetMaterial("DecalMaterial", decalMaterial);

		auto directLightMaterial = std::make_shared<Material>();
		decalMaterial->shader = Importer::ShaderSet["DirectLight"];
		decalMaterial->SetTexture2D("normTex", FrameBuffer<GeometryFBO>::instance()->normalTarget);
		decalMaterial->SetTexture2D("depthTex", FrameBuffer<GeometryFBO>::instance()->stencilDepthTarget);
		decalMaterial->SetTexture2D("shadowTex", FrameBuffer<ShadowFBO>::instance()->depthTarget);
		Importer::SetMaterial("DirectLightMaterial", directLightMaterial);

		auto pointLightMaterial = std::make_shared<Material>();
		pointLightMaterial->shader = Importer::ShaderSet["PointLight"];
		pointLightMaterial->SetTexture2D("normTex", FrameBuffer<GeometryFBO>::instance()->normalTarget);
		pointLightMaterial->SetTexture2D("depthTex", FrameBuffer<GeometryFBO>::instance()->stencilDepthTarget);
		Importer::SetMaterial("PointLightMaterial", pointLightMaterial);

		auto combineMaterial = std::make_shared<Material>();
		combineMaterial->shader = Importer::ShaderSet["Combine"];
		combineMaterial->SetTexture2D("diffuseTex", FrameBuffer<GeometryFBO>::instance()->colorTarget);
		combineMaterial->SetTexture2D("diffuseLight", FrameBuffer<LightFBO>::instance()->diffuseTarget);
		combineMaterial->SetTexture2D("specularLight", FrameBuffer<LightFBO>::instance()->specularTareget);
		Importer::SetMaterial("CombineMaterial", combineMaterial);
	};
	ImportShader();
	CreateMaterial();
}

void Renderer::LoadCamera() {
	camera = new Camera(0, 0.0f, Vector3(0, 10, 0));
	camera->SetSpeed(3);
}

void Renderer::LoadPrefabs() {
	Importer::LoadPrefab("Pegasus statue");
	Importer::LoadPrefab("Terrain");
}


//void Renderer::LoadLights() {
//	mainLightSystem = new LightSystem(std::make_unique<Shader>("CombineVertex.glsl", "DirectLightFragment.glsl"), 
//		std::make_unique<Shader>("PointLightVertex.glsl", "PointLightFragment.glsl"));
//
//	std::minstd_rand gen(std::random_device{}());
//	std::uniform_real_distribution<float> randomPosition(2000, 4000);
//	std::uniform_real_distribution<float> randomHeight(50, 300);
//	std::uniform_real_distribution<float> randomColor(0, 1.0);
//
//	for (int i = 1; i < 10; ++i) {
//		for (int j = 1; j < 10; ++j) {
//			float x = randomPosition(gen);
//			float y = randomPosition(gen);
//			//float x = i * 1000;
//			//float y = j * 1000;
//
//			Vector4 color = Vector4(randomColor(gen), randomColor(gen) * 0.5, randomColor(gen), 1);
//
//			mainLightSystem->AddPointLight(Vector3(x, land->CoordHeight(x, y) + randomHeight(gen), y), color, 400);
//		}
//	}
//
//	Vector3 rotation = Matrix4::Rotation(45, Vector3(1, 0, 0)) * Vector3(0, 0, 1);
//	mainLightSystem->AddDirectLight(rotation, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
//}


//void Renderer::LoadObjects() {
	//mainScene = new SceneSystem();

	//quad = Mesh::GenerateQuad();
	//sphere = Mesh::LoadFromMeshFile("Sphere.msh");
	//cube = Mesh::LoadFromMeshFile("Cube.msh");

	//loadedObjects.emplace_back(std::make_unique<ObjectSample>("DogKnight", "DogKnight", true));
	//loadedObjects.emplace_back(std::make_unique<ObjectSample>("Tree", "Tree", false));
	////load root
	//std::shared_ptr<Shader> RolesShader = std::make_shared<Shader>
	//("DynamicObjectVertex.glsl", "DeferredObjectFragment.glsl");
	//std::shared_ptr<Shader> PlantShader = std::make_shared<Shader>
	//("StaticObjectVertex.glsl", "DeferredObjectFragment.glsl");

	////load dogknight
	//objectPtr dogKnight = std::make_shared<Object>(*loadedObjects[0], RolesShader);
	//mainScene->AddOpaqueObject(dogKnight);

	//Vector2 pos(land->MeshSize().x, land->MeshSize().z);
	//SceneNode::SettleDownModel(dogKnight.get(), Vector3(50.0f, 50.0f, 50.0f), Vector3(pos.x / 2, land->CoordHeight(pos.x / 2, pos.y / 2), pos.y / 2), 0);

	////load tree
	//for (int i = 0; i < 36; ++i) {
	//	objectPtr treeCopy = std::make_shared<Object>(*loadedObjects[1], PlantShader);
	//	mainScene->AddOpaqueObject(treeCopy);

	//	Vector3 position = Vector3(TREE_POSITION[i].x, land->CoordHeight(TREE_POSITION[i].x, TREE_POSITION[i].z), TREE_POSITION[i].z);
	//	SceneNode::SettleDownModel(treeCopy.get(), Vector3(1.0f, 1.0f, 1.0f), position, i * 30);
	//}
//}