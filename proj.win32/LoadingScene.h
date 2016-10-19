#pragma once

#include "cocos2d.h"
USING_NS_CC;

class LoadingScene :
	public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(LoadingScene);
	bool init();
private:
	void onTextureLoading(float dt);//����Ƿ�������
	int m_texture_num;
};

