#pragma once
#include <vector>
#include "../nclgl/GameObject.h"
#include "../nclgl/FrameBuffer.h"
#include "../nclgl/Importer.h"

class Renderer : public OGLRenderer {
public:
	static Renderer* instance(Window* window = nullptr) {
		static Renderer single(*window);
		return &single;
	}
	~Renderer();
	void RenderScene() override;
	void UpdateScene(const float& dt) override;

private:
	Renderer(Window& parent);

	void Initialize();
	void LoadCamera();
	void LoadPrefabs();

	Camera* camera;
};