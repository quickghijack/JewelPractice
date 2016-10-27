#include "GameScene.h"
#include "JewelsGrid.h"
//#include "GameOverScene.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

LoadingBar* GameScene::m_bonusbar = nullptr;
int GameScene::m_score = 0;
Label* GameScene::m_scorelabel = nullptr;

bool GameScene::init()
{
	Layer::init();

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto texturecache = TextureCache::getInstance();

	m_bg = Sprite::createWithTexture(texturecache->getTextureForKey("bground1.png"));
	m_bg->setAnchorPoint(Vec2(0, 0));
	m_bg->setTag(100);
	addChild(m_bg);

	auto board = Sprite::createWithTexture(texturecache->getTextureForKey("board.png"));
	board->setAnchorPoint(Vec2(0, 1));
	board->setPosition((Vec2(0, visibleSize.height)));
	board->setOpacity(80);
	addChild(board);

	m_jewelsgrid = JewelsGrid::create(8, 8);
	addChild(m_jewelsgrid);
	m_jewelsgrid->setPosition(0, visibleSize.height - m_jewelsgrid->getRow()*GRID_WIDTH);

	auto bonusbar_frame = Sprite::createWithTexture(texturecache->getTextureForKey("bonusbar.png"));
	bonusbar_frame->setPosition(Vec2(visibleSize.width / 2, bonusbar_frame->getContentSize().height / 2 + 10));
	addChild(bonusbar_frame);
}
