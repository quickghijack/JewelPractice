#include "JewelsGrid.h"
#include "Jewel.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
;

JewelsGrid* JewelsGrid::create(int row, int col)
{
	auto jewelsgrid = new JewelsGrid();
	if (jewelsgrid&& jewelsgrid->init(row, col))
	{
		jewelsgrid->autorelease();
		return jewelsgrid;
	}
	else
	{
		CC_SAFE_DELETE(jewelsgrid);
		return nullptr;
	}
}

bool JewelsGrid::init(int row, int col)
{
	Node::init();

	m_row = row;
	m_col = col;

	m_jewelSelected = nullptr;
	m_JewelSwaped = nullptr;

	//初始化阵列容器
	m_JewelsBox.resize(m_row);
	for(auto &vec:m_JewelsBox) {
		vec.resize(m_col);
	}

	for (int x = 0; x < m_col; x++) {
		for (int y = 0; y < m_row; y++) {
			m_JewelsBox[x][y] = createAJewel(x, y);
		}
	}

	while (isDeadMap())
	{
		log("is dead Map");
		updateMap();
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(JewelsGrid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(JewelsGrid::onTouchMove, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	log("JewelsGrid init!");
	return true;
}

void JewelsGrid::updateMap()
{
	for (int x = 0; x < m_col;x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_JewelsBox[x][y]->removeFromParent();
			m_JewelsBox[x][y] = createAJewel(x, y);
		}
	}
	log("update a new map");
}

//模拟交换，判断交换后是否可以消除
bool JewelsGrid::isDeadMap()
{
	auto swap = [](Jewel** jewelA, Jewel** jewelB)
	{
		auto tmp = *jewelA;
		*jewelA = *jewelB;
		*jewelB = tmp;
	};

	bool isDeadMap = true;

	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++) 
		{
			//跟左边交换
			if (x > 0)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x - 1][y]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x - 1][y]);//还原

			}
			if (x < m_col - 1) 
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x + 1][y]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x + 1][y]);//还原	
			}
			if (y > 0)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y-1]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y-1]);//还原
			}
			if (y < m_row - 1)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y+1]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y+1]);//还原
			}

			m_crushJewelsBox.clear();

			return isDeadMap;
		}
	}
}

Jewel* JewelsGrid::createAJewel(int x, int y)
{
	Jewel* jewel = nullptr;
	while (1)
	{
		jewel = Jewel::createByType(random(FIRST_JEWEL_ID, LAST_JEWEL_ID), x, y);
		if (isJewelLegal(jewel, x, y))
		{
			break;
		}
	}
	setJewelPixPos(jewel, x, y);
	addChild(jewel);
}

bool JewelsGrid::isJewelLegal(Jewel* jewel, int x, int y)
{
	bool isXLegal = true;
	bool isYLegal = true;

	if (x > 1)
	{
		if (jewel->getType() == m_JewelsBox[x - 1][y]->getType() &&
			jewel->getType() == m_JewelsBox[x - 2][y]->getType()) {
			isXLegal = false;
		}
		else
			isXLegal = true;
		
	}
	if (y > 1) 
	{
		if (jewel->getType() == m_JewelsBox[x][y - 1]->getType() &&
			jewel->getType() == m_JewelsBox[x][y - 2]->getType())
		{
			isYLegal = false;
		}
		else
			isYLegal = true;
	}
	return isXLegal&&isYLegal;
}

void JewelsGrid::setJewelPixPos(Jewel* jewel, float x, float y)
{
	jewel->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
}

bool JewelsGrid::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	//触摸坐标转化
	auto pos = this->convertToNodeSpace(pTouch->getLocation());
	
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;
		//得到当前触摸的是阵列坐标

		m_jewelSelected = m_JewelsBox[x][y];
		log("touch the jewel on x=%d,y=%d", x, y);
		return true;
	}
	else return false;
}

void JewelsGrid::onTouchMove(Touch* pTouch, Event* pEvent)
{
	if (!m_jewelSelected)
		return;
	int startX = m_jewelSelected->getX();
	int startY = m_jewelSelected->getY();

	auto pos = this->convertToNodeSpace(pTouch->getLocation());

	int touchX = pos.x / GRID_WIDTH;
	int touchY = pos.y / GRID_WIDTH;

	//如果点按没有移动或者
	if (!Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) || Vec2(touchX, touchY) == Vec2(startX, startY))
	{
		return;
	}

	if (abs(startX - touchX) + abs(startY - touchY) != 1)
	{
		return;
	}

	m_JewelSwaped = m_JewelsBox[touchX][touchY];

	swapJewels(m_jewelSelected, m_JewelSwaped);
	schedule(schedule_selector(JewelsGrid::onJewelsSwaping));
}

void JewelsGrid::swapJewels(Jewel* jewelA, Jewel* jewelB)
{
	_eventDispatcher->pauseEventListenersForTarget(this);//暂时停止触摸监听

	auto tmp = m_JewelsBox[jewelA->getX()][jewelB->getY()];
	m_JewelsBox[jewelA->getX()][jewelA->getY()] = m_JewelsBox[jewelB->getX()][jewelB->getY()];
	m_JewelsBox[jewelB->getX()][jewelB->getY()] = tmp;

	auto tempX = jewelA->getX();
	jewelA->setX(jewelB->getX());
	jewelB->setX(tempX);

	auto tempY = jewelA->getY();
	jewelA->setY(jewelB->getY());
	jewelB->setY(tempY);

	swapToNewPos(jewelA);
	swapToNewPos(jewelB);
}

void JewelsGrid::swapToNewPos(Jewel* jewel)
{
	jewel->setSwapState(true);

	auto move = MoveTo::create(MOVE_SPEED, Vec2(jewel->getX()*GRID_WIDTH, jewel->getY()*GRID_WIDTH));
	auto call = CallFunc::create([jewel]() {
		jewel->setSwapState(false);
	});
	jewel->runAction(Sequence::create(move, call, nullptr));
}

bool JewelsGrid::canCrush()
{
	int count = 0; //本次判断能消除的数目
	Jewel* JewelBegin = nullptr;
	Jewel* JewelNext = nullptr;

	for (int x = 0; x < m_col; x++) {
		for (int y = 0; y < m_row;) {
			count = 1;
			JewelBegin = m_JewelsBox[x][y];
			JewelNext = m_JewelsBox[x][y + 1];

			while (JewelBegin->getType() == JewelNext->getType())
			{
				count++;
				int nextIndex = y + count;
				if (nextIndex > m_row - 1)
					break;
				JewelNext = m_JewelsBox[x][nextIndex];
			}

			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto jewel = m_JewelsBox[x][y + n];
					m_crushJewelsBox.pushBack(jewel);
				}
			}
			y += count;

		}
	}
	for (int y = 0; y < m_row; y++)
	{
		for (int x = 0; x < m_col;)
		{
			count = 1;
			JewelBegin = m_JewelsBox[x][y];
			JewelNext = m_JewelsBox[x + 1][y];

			while (JewelBegin->getType() == JewelNext->getType())
			{
				count++;
				int nextIndex = x + count;
				if (nextIndex > m_col - 1)
				{
					break;
				}
				JewelNext = m_JewelsBox[nextIndex][y];
			}
			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto jewel = m_JewelsBox[x + n][y];
					if (m_crushJewelsBox.find(jewel) != m_crushJewelsBox.end())
					{
						continue;
					}
					m_crushJewelsBox.pushBack(jewel);
				}
			}
			x = +count;
		}
		

	}
	if (!m_crushJewelsBox.empty()) {
		return true;
	}
	else
		return false;
}

void JewelsGrid::goCrush()
{
	for (auto jewel : m_crushJewelsBox)
	{
		auto newJewel = Jewel::createByType(random(FIRST_JEWEL_ID, LAST_JEWEL_ID), jewel->getX(), m_row);
		setJewelPixPos(newJewel, newJewel->getX(), m_row);
		addChild(newJewel);

		m_newJewelBox.pushBack(newJewel);

		m_JewelsBox[jewel->getX()][jewel->getY()] = nullptr;
		//消除对象
		jewel->crush();

	}
}

void JewelsGrid::refreshJewelsGrid()
{
	for (int x = 0; x < m_col; x++)
	{
		int empty_count = 0;

		for (int y = 0; y < m_row; y++)
		{
			auto jewel = m_JewelsBox[x][y];
			if (!jewel)
				empty_count++;//计算每一列需要消除多少宝石

		}
		if (empty_count)
		{
			refreshJewelsToNewPos(x);
		}
	}
}


void JewelsGrid::refreshJewelsToNewPos(int col)
{
	int n = 0;
	auto pJewelsBox = &m_JewelsBox;

	for (int y = 0; y < m_row; y++)
	{
		auto jewel = m_JewelsBox[col][y];
		if (!jewel)
		{
			n++;
			continue;
		}
		else
			if (n != 0)
			{
				jewel->setY(jewel -> getY() - n);
				auto move = MoveBy::create(0.2, Vec2(0, -n*GRID_WIDTH));
				auto call = CallFunc::create([pJewelsBox, jewel]() {
					(*pJewelsBox)[jewel->getX()][jewel->getY()] = jewel;
				});
				jewel->runAction(Sequence::create(move, call, nullptr));
			}
	}

	int i = n;
	int delta = 1;

	for (auto jewel : m_newJewelBox)
	{
		if (jewel->getX() == col)
		{
			jewel->setY(m_row - i);

			auto delay = DelayTime::create(0.2);

			auto move = MoveBy::create(0.2*delta++, Vec2(0, -i--*GRID_WIDTH));
			auto call = CallFunc::create([jewel, pJewelsBox, this]() {
				(*pJewelsBox)[jewel->getX()][jewel->getY()] = jewel;
				m_newJewelBox.eraseObject(jewel);
			});

			jewel->runAction(Sequence::create(delay, move, call, nullptr));
		}
	}
}

void JewelsGrid::onJewelsSwaping(float dt)
{
	if (m_jewelSelected->isSwaping() || m_JewelSwaped->isSwaping())
	{
		return;
	}
	else
	{
		unschedule(schedule_selector(JewelsGrid::onJewelsSwaping));
	}

}

JewelsGrid::JewelsGrid()
{
}


JewelsGrid::~JewelsGrid()
{
}


