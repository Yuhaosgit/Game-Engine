#include "Renderer.h"

Renderer::Renderer(Window& parent) :OGLRenderer(parent) {
	Initialize();
	LoadCamera();
	LoadPrefabs();

	//auto test = new GameObject(Importer::PrefabSet["Pegasus statue"]);
	//auto land = std::make_shared<Terrain>(Importer::PrefabSet["Terrain"],"../Prefab/Terrain/Terrain.png", 1, 0.1, 4);
	//auto terrain = new GameObject();
	//terrain->AddComponent(land);
	//land->GetHeight(0, 0);
	//test->GetComponent<Transform>()->Translate(Vector3(0, 10, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glViewport(0, 0, OGLRenderer::GetWidth(), OGLRenderer::GetHeight());

	init = true;
}

void Renderer::UpdateScene(const float& dt) {
	camera->UpdateCamera(dt);
	camera->UpdateList();
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	camera->RenderOpaque();
}
