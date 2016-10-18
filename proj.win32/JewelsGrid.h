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

	void updateMap();//����
	bool isDeadMap();

private:
	Jewel* createAJewel(int x, int y);
	bool isJewelLegal(Jewel* jewel, int x, int y);//�ж��Ƿ������
	void setJewelPixPos(Jewel* jewel, float x, float y);  //������Ϸʵ������λ�ã�Ϊ��Ҫ��jewel����

	void swapJewels(Jewel *jewelA, Jewel* jewelB);
	void swapToNewPos(Jewel* jewel);//�ƶ������º��λ�ã�������jewel��

	void refreshJewelsGrid();//ˢ�������������
	void refreshJewelsToNewPos(int col);//ˢ���ƶ����һ������

	//�¼���Ӧ
	bool onTouchBegan(Touch* , Event*);
	void onTouchMove(Touch*, Event*);

	//�߼�����
	bool canCrush();
	void goCrush();


	//��׽����,������������
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
	Jewel* m_JewelSwaped;//�������ı�ʯ

	vector<vector<Jewel*>> m_JewelsBox; //���б�ʯ
	Vector<Jewel*> m_crushJewelsBox; //��������ʯ
	Vector<Jewel*> m_newJewelBox;


};
 