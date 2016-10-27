#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class JewelsGrid;

class GameScene :
	public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	bool init();

	static void addBonus(int bonus);
private:
	static LoadingBar* m_bonusbar;
	static int m_score;
	static Label* m_scorelabel;

	void onUpdateMenuCallback(Ref*);
	void onReducingBonus(float dt);
	void publishScore();
	JewelsGrid* m_jewelsgrid;
	Sprite* m_bg;
};

