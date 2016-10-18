#pragma once

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 40
#define MOVE_SPEED 0.2
#define FIRST_JEWEL_ID 1
#define LAST_JEWEL_ID 7

class Jewel;

class JewelsGrid :
	public Node
{
public:
	JewelsGrid();
	~JewelsGrid();

	static JewelsGrid* create(int row, int col);

	bool init(int row, int col);

	void updateMap();//更新
	bool isDeadMap();

private:
	Jewel* createAJewel(int x, int y);
	bool isJewelLegal(Jewel* jewel, int x, int y);//判断是否会三消
	void setJewelPixPos(Jewel* jewel, float x, float y);  //设置游戏实际像素位置（为何要有jewel？）

	void swapJewels(Jewel *jewelA, Jewel* jewelB);
	void swapToNewPos(Jewel* jewel);//移动到更新后的位置，坐标在jewel内

	void refreshJewelsGrid();//刷新消除后的阵列
	void refreshJewelsToNewPos(int col);//刷新移动后的一列阵列

	//事件响应
	bool onTouchBegan(Touch* , Event*);
	void onTouchMove(Touch*, Event*);

	//逻辑部分
	bool canCrush();
	void goCrush();


	//捕捉函数,控制消除流程
	void onJewelsSwaping(float dt);
	void onJewelsSwapingBack(float dt);
	void onJewelsCrushing(float dt);
	void onJewelsRefreshing(float dt);

public:
	int getRow() { return m_row; }
	int getCol() { return m_col; }

private:
	int m_row;
	int m_col;

	Jewel* m_jewelSelected;
	Jewel* m_JewelSwaped;//欲交换的宝石

	vector<vector<Jewel*>> m_JewelsBox; //所有宝石
	Vector<Jewel*> m_crushJewelsBox; //欲消除宝石
	Vector<Jewel*> m_newJewelBox;


};
 