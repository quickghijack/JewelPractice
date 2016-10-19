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
