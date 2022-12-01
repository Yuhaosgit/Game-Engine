#include "FrameBuffer.h"
#include "TextureGenerateFuncs.h"

#pragma region Prototype
FrameBufferPrototype::FrameBufferPrototype() {}

void FrameBufferPrototype::BindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void ConnectFrameBuffer(const FrameBufferPrototype& read, const FrameBufferPrototype& draw) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, read.frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw.frameBuffer);
}
#pragma endregion

#pragma region ScreenFBO
void ScreenFBO::GenerateFrameBuffer() {}

void ScreenFBO::ClearBuffer() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
#pragma endregion


#pragma region Gbuffer
void GeometryFBO::GenerateFrameBuffer() {
	int width = OGLRenderer::GetWidth();
	int height = OGLRenderer::GetHeight();

	bool initFlag = false;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	initFlag = stencilDepthTarget.Generate(GL_DEPTH24_STENCIL8, GenerateDepthTexture, width, height);
	initFlag = colorTarget.Generate(GL_COLOR_ATTACHMENT0, GenerateColorTexture, width, height);
	initFlag = normalTarget.Generate(GL_COLOR_ATTACHMENT1, GenerateColorTexture, width, height);

	initFlag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!initFlag) {
		throw "Gbuffer FBO initialization failed";
	}
}

void GeometryFBO::ClearBuffer() {
	glStencilMask(0xFF);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
#pragma endregion

#pragma region DecalFBO
void DecalFBO::GenerateFrameBuffer() {
	int width = OGLRenderer::GetWidth();
	int height = OGLRenderer::GetHeight();

	bool initFlag = false;
	glGenFramebuffers(1, &frameBuffer);
	//lighting buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	initFlag = colorTarget.Generate(GL_COLOR_ATTACHMENT0, GenerateColorTexture, width, height);
	initFlag = normalTarget.Generate(GL_COLOR_ATTACHMENT1, GenerateColorTexture, width, height);
	initFlag = stencilDepthTarget.Generate(GL_DEPTH24_STENCIL8, GenerateDepthTexture, width, height);

	initFlag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!initFlag) {
		throw "Outcome FBO initialization failed";
	}
}

void DecalFBO::ClearBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//ConnectFrameBuffer(*Renderer::instance()->gBuffer, *this);

	//glReadBuffer(GL_COLOR_ATTACHMENT0);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//glReadBuffer(GL_COLOR_ATTACHMENT1);
	//glDrawBuffer(GL_COLOR_ATTACHMENT1);
	//glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//glDrawBuffers(2, attachment);
}
#pragma endregion


#pragma region LightFBO
void LightFBO::GenerateFrameBuffer() {
	int width = OGLRenderer::GetWidth();
	int height = OGLRenderer::GetHeight();

	bool initFlag = false;
	glGenFramebuffers(1, &frameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	initFlag = diffuseTarget.Generate(GL_COLOR_ATTACHMENT0, GenerateColorTexture, width, height);
	initFlag = specularTareget.Generate(GL_COLOR_ATTACHMENT1, GenerateColorTexture, width, height);
	initFlag = stencilDepthTarget.Generate(GL_DEPTH24_STENCIL8, GenerateDepthTexture, width, height);

	initFlag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!initFlag) {
		throw "LightFBO FBO initialization failed";
	}
}

void LightFBO::ClearBuffer() {
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//void LightFBO::RenderPreset() {
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE, GL_ONE);
//
//	glStencilMask(0xFF);
//
//	glDepthMask(GL_FALSE);
//}
//
//void LightFBO::RenderAfterSet() {
//	glDepthFunc(GL_LESS);
//	glDepthMask(GL_TRUE);
//
//	glCullFace(GL_BACK);
//
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//	glDisable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
#pragma endregion

#pragma region OITFBO
void OITFBO::GenerateFrameBuffer() {
	int width = OGLRenderer::GetWidth();
	int height = OGLRenderer::GetHeight();

	bool initFlag = false;
	glGenFramebuffers(1, &frameBuffer);
	//lighting buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	initFlag = accumulationTarget.Generate(GL_COLOR_ATTACHMENT0, GenerateAccumulationTexture, width, height);
	initFlag = revealTarget.Generate(GL_COLOR_ATTACHMENT1, GenerateRevealTexture, width, height);
	initFlag = depthTarget.Generate(GL_DEPTH24_STENCIL8, GenerateDepthTexture, width, height);

	initFlag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!initFlag) {
		throw "OIT FBO initialization failed";
	}
}

void OITFBO::ClearBuffer() {
	GLfloat zeroFill[4] = { 0.0f,0.0f,0.0f,0.0f };
	GLfloat oneFill[4] = { 1.0f,1.0f,1.0f,1.0f };

	glClearBufferfv(GL_COLOR, 0, zeroFill);
	glClearBufferfv(GL_COLOR, 1, oneFill);
	glClear(GL_DEPTH_BUFFER_BIT);
}

//void OITFBO::RenderPreset() {
//	glDepthMask(GL_TRUE);
//	glEnable(GL_BLEND);
//	glBlendFunci(0, GL_ONE, GL_ONE); // accumulation blend target
//	glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR); // revealge blend target
//}
//
//void OITFBO::RenderAfterSet() {
//
//}
//
//void OITFBO::RenderFunction() {
//	Renderer::instance()->waterSurface->Render();
//}
#pragma endregion

#pragma region shadowFBO
void ShadowFBO::GenerateFrameBuffer() {
	int width = OGLRenderer::GetWidth();
	int height = OGLRenderer::GetHeight();

	bool initFlag = false;
	glGenFramebuffers(1, &frameBuffer);
	//lighting buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	initFlag = depthTarget.Generate(0, GenerateShadowTexture, width, height);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	initFlag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!initFlag) {
		throw "Shadow FBO initialization failed";
	}
}

void ShadowFBO::ClearBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

//void ShadowFBO::RenderPreset() {
//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//	glViewport(0, 0, ShadowTextureSize, ShadowTextureSize);
//	glCullFace(GL_FRONT);
//}
//
//void ShadowFBO::RenderAfterSet() {
//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//	glViewport(0, 0, Renderer::instance()->GetWidth(), Renderer::instance()->GetHeight());
//	glCullFace(GL_BACK);
//}

//void ShadowFBO::RenderFunction() {
//	Renderer* renderer = Renderer::instance();
//
//	renderer->BindShader(shadowShader.get());
//
//	Matrix4 lightProjectjMatrix = Matrix4::Orthographic(0, 4000, 4000, -4000, 4000, -4000);
//	Matrix4 lightViewMatrix = Matrix4::BuildViewMatrix(Vector3(0, 0, 0),
//		(Matrix4::Rotation(45, Vector3(1, 0, 0)) * Vector3(0, 0, 1)));
//	renderer->shadowMatrix = lightProjectjMatrix * lightViewMatrix;
//
//	renderer->mainScene->RenderShadow(shadowShader.get(), renderer->shadowMatrix);
//}
#pragma endregion

#pragma region OutcomeFBO
void OutcomeFBO::GenerateFrameBuffer() {
	int width = OGLRenderer::GetWidth();
	int height = OGLRenderer::GetHeight();

	bool initFlag = false;
	glGenFramebuffers(1, &frameBuffer);
	//lighting buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	initFlag = colorTarget.Generate(GL_COLOR_ATTACHMENT0, GenerateColorTexture, width, height);

	initFlag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!initFlag) {
		throw "Outcome FBO initialization failed";
	}
}

void OutcomeFBO::ClearBuffer() {
	glClear(GL_COLOR_BUFFER_BIT);
}

//void OutcomeFBO::RenderPreset() {
//	glDepthFunc(GL_ALWAYS);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}

//void OutcomeFBO::RenderAfterSet() {
//
//}
//
//void OutcomeFBO::RenderFunction() {
//	CombineDeferredLighting();
//	CombineTransparent();
//}

//void OutcomeFBO::CombineDeferredLighting() {
//	Renderer* renderer = Renderer::instance();
//	renderer->BindShader(combineOpaqueShader.get());
//	renderer->UpdateShaderMatrices(true);
//
//	renderer->decalBuffer->colorTarget.Submit(combineOpaqueShader.get(), "diffuseTex", 0);
//	renderer->lightBuffer->diffuseTarget.Submit(combineOpaqueShader.get(), "diffuseLight", 1);
//	renderer->lightBuffer->specularTareget.Submit(combineOpaqueShader.get(), "specularLight", 2);
//
//	renderer->quad->Draw();
//}

//void OutcomeFBO::CombineTransparent() {
//	Renderer* renderer = Renderer::instance();
//
//	renderer->BindShader(combineTransparentShader.get());
//	renderer->UpdateShaderMatrices(true);
//
//	renderer->transparentBuffer->accumulationTarget.Submit(combineTransparentShader.get(), "accumulationTex", 0);
//	renderer->transparentBuffer->revealTarget.Submit(combineTransparentShader.get(), "revealTex", 1);
//	renderer->gBuffer->stencilDepthTarget.Submit(combineTransparentShader.get(), "opaqueDepthTex", 2);
//	renderer->transparentBuffer->depthTarget.Submit(combineTransparentShader.get(), "transparentDepthTex", 3);
//
//	renderer->quad->Draw();
//}
#pragma endregion