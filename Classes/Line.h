#ifndef LINE_H
#define LINE_H
#include <vector>
#include "cocos2d.h"
using cocos2d::Vec2;
using std::vector;

class myLine
{
private :
	vector<Vec2> line;
	int now_point = 0;
public:
	void addPoint(int x, int y)
	{
		line.push_back(Vec2(x, y));
	}
	Vec2 getPoint(void)
	{
		return line[now_point];
	}
	/*
	����Ƿ����ǰ����һ���㣬������ԣ�ǰ����������У�����
	*/
	bool checkNext(Vec2 current)
	{
		if (current.distance(line[now_point]) < 20.0f)
		{
			if (!endPoint())
			{
				nextPoint();
			}

			return true;
		}
		return false;
	}

	//����Ƿ��ߵ����յ�
	bool checkEnd(Vec2 current)
	{
		if (current.distance(line[now_point]) < 20.0f && endPoint())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void nextPoint(void)
	{
		now_point++;
	}

	bool endPoint(void)
	{
		if (now_point == line.size() - 1)
			return true;
		else
			return false;
	}

	void init()
	{
		now_point = 0;
	}

	myLine& operator = (const myLine& rhs)
	{
		if (this == &rhs) return *this;
		this->line = rhs.line;
		this->now_point = rhs.now_point;
		return *this;
	}

};

#endif