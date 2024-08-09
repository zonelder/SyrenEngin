#pragma once
#include "../common/Scene/BaseSystem.h"
#include "../component/Render.h"
#include "../component/Transform.h"
#include "../graphics/Graphics.h"
#include "../component/Parent.h"

class RenderSystem:public BaseSystem
{
public:
	using WithComponents = With<Parent, Render, Transform>;
	using RenderView = ComponentView <WithComponents,Without<>>;
	RenderSystem(SceneManager& scene);
	void onFrame(SceneManager&) override;
	void onUpdate(SceneManager&, float dt) override;

private:
	void DeepRender(RenderView& view, Transform& cam, EntityID id);
	void renderOne(Render& render, Transform& transform, const Transform& camTr);
	//void renderOne(Render& render, Graphics& gfx, Transform& transform,const Transform& camTr);
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_colorConstantBuffer;
	SceneManager& _scene;
};

