#include "LoadingScene.h"
//#include "GameScene"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{
	Layer::init();
	auto winSize = Director::getInstance()->getWinSize();

	m_texture_num = 0; //当前已经加载的图片
	int *ptexture_num = &m_texture_num; //为了在回调函数中使用而用指针

	auto label = Label::createWithSystemFont("Loading...", "Arial", 36);
	label->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(label);

	auto addTextureCallback = [ptexture_num](Texture2D* texture) {
		(*ptexture_num)++;
		log("load a texture async");
	};

	for (int i = 1; i <= 7; i++) {
		char jewelName[] = { 0 };
		sprintf(jewelName, "jewel%d.png", i);
		TextureCache::getInstance()->addImageAsync(jewelName, addTextureCallback);
	}

	TextureCache::getInstance()->addImageAsync("bgroung1.png", addTextureCallback);

	TextureCache::getInstance()->addImageAsync("bgroung2.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bgroung3.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bgroung4.png", addTextureCallback);

	TextureCache::getInstance()->addImageAsync("gameover.png", addTextureCallback);

	TextureCache::getInstance()->addImageAsync("board.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bonusbar.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bonusbar_fill.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bonus.png", addTextureCallback);

	schedule(schedule_selector(LoadingScene::onTextureLoading));

	SimpleAudioEngine::getInstance()->preloadEffect("crush.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("swapback.ogg");

	return true;
}

void LoadingScene::onTextureLoading(float dt)
{
	if (m_texture_num == 16)
	{
		unschedule(schedule_selector(LoadingScene::onTextureLoading));
		log("loading up");

		auto call = CallFunc::create([]() {
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
		});

		this->runAction(Sequence::create(DelayTime::create(0.51), call, nullptr));
	}
}