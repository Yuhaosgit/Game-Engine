#include "RenderPass.h"
#include "OGLRenderer.h"
#include "Importer.h"
#include "GameObject.h"

#pragma region ShadowPass
void ShadowPass::RenderPreset() {
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, shadowTextureSize, shadowTextureSize);
	glCullFace(GL_FRONT);
}

void ShadowPass::RenderFunction(Camera* camera) {
	//for (int i = 0; i < camera->visibleObject.size(); ++i) {
	//	if (camera->visibleObject[i].lock()->meshRender->shadowCast) {
	//		camera->visibleObject[i].lock()->meshRender->RenderShadow();
	//	}
	//}
}

void ShadowPass::RenderAfterSet() {
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, OGLRenderer::GetWidth(), OGLRenderer::GetHeight());
	glCullFace(GL_BACK);
}
#pragma endregion

#pragma region Gbuffer
void GbufferPass::RenderPreset() {
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glDepthFunc(GL_LESS);

	glDisable(GL_BLEND);
}

void GbufferPass::RenderFunction(Camera* camera) {
	//for (int i = 0; i < camera->opaqueList.size(); ++i) {
	//	if (camera->visibleObject[i].lock()->meshRender != nullptr)
	//		camera->visibleObject[i].lock()->meshRender->Render();
	//}
}

void GbufferPass::RenderAfterSet() {

}
#pragma endregion

#pragma region Decal
void DecalPass::RenderPreset() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DecalPass::RenderFunction(Camera* camera) {
	//for (int i = 0; i < camera->opaqueList.size(); ++i) {
	//	if (camera->visibleObject[i].lock()->decal != nullptr)
	//		camera->visibleObject[i].lock()->decal->Render();
	//}
}

void DecalPass::RenderAfterSet() {
	glDisable(GL_BLEND);
}
#pragma endregion

#pragma region Light
void DirectLightPass::RenderPreset() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);
}

void DirectLightPass::RenderFunction(Camera* camera) {
	//for (int i = 0; i < camera->visibleObject.size(); ++i) {
	//	camera->visibleObject[i].lock()->light->Render();
	//}
}

void DirectLightPass::RenderAfterSet() {
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
#pragma endregion

#pragma region Combination
void CombinePass::RenderPreset() {

}

void CombinePass::RenderFunction(Camera* camera) {
	Importer::MaterialSet["CombineMaterial"]->SubmitData();
	Importer::MeshSet["Quad"]->Draw();
}

void CombinePass::RenderAfterSet() {

}
#pragma endregion
