#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <algorithm>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//=========================ACTOR========================================================
Actor::Actor(int imageID, double startX, double startY, StudentWorld* myWorld)
	:GraphObject(imageID,startX,startY, right, 0 , 1.0)
{
	m_world = myWorld;
	m_alive = true;
}
/*
void Actor::generateBoundLine(double startPix, double endPix, vector<double>& line)
{
	line.clear();
	for (double i = startPix;i < endPix;i++)
		line.push_back(i);
}*/

//===========================BIOLOGICAL=================================================
Biological::Biological(int imageID, double startX, double startY, StudentWorld* myWorld)
	:Actor(imageID,startX, startY, myWorld)
{
}
//=========================PENELOPE=====================================================
Penelope::Penelope(double startX, double startY, StudentWorld* myWorld)
	:Biological(IID_PLAYER,startX,startY, myWorld)
{
}

void Penelope::doSomething()
{
	int button;
	if (getWorld()->getKey(button))
	{
		vector<Actor*> myVec(getWorld()->getActors());
		vector<double> myLine;
		cout << myVec.size();
		bool safeToMove = true;
		switch (button)
		{
		case KEY_PRESS_DOWN:
		{
			setDirection(down);
			int destX = getX();
			int destY = getY() - 4;
			vector<Actor*>::iterator it;
			for (it = myVec.begin();it != myVec.end();it++)
				if (destY <= ((*it)->getY() + SPRITE_HEIGHT - 1) && destY >= (*it)->getY()
					&& (destX <= (*it)->getX() + SPRITE_WIDTH - 1) && destX >= (*it)->getX())
				{
					safeToMove = false;
					break;
				}
			if (safeToMove)
				moveTo(destX, destY);
			/*


			generateBoundLine(getX(), getX() + SPRITE_WIDTH - 1, myLine);
			for (int i = 1;i < myVec.size();i++)
			{
				cout << "a lot of fucking tests" << endl;
				if (getY() - 4 != myVec[i]->getY() + SPRITE_HEIGHT - 1)
					if (find(myLine.begin(), myLine.end(), myVec[i]->getX()) == myLine.end() && //along myLine, we don't find the left corner
						find(myLine.begin(), myLine.end(), myVec[i]->getX() + SPRITE_WIDTH - 1) == myLine.end()) //'''' right corner
					{
						cout << "movin'" << endl;
						moveTo(getX(), getY() - 4);
						break;
					}
				/*if (getY() - 4 == myVec[i]->getY())
				{
					cout << "guaco!" << endl;
					flag = true;
					break;
				}
			}
			if (!flag)
				moveTo(getX(), getY() - 4);
			}*/
			break;
		}
		case KEY_PRESS_UP:
			setDirection(up);
			for (int i = 0;i < myVec.size();i++)
				if (getY() + 4 == myVec[i]->getY())
					break;
			moveTo(getX(), getY() + 4);
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			for (int i = 0;i < myVec.size();i++)
				if (getX() - 4 == myVec[i]->getX())
					break;
			moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			for (int i = 0;i < myVec.size();i++)
				if (getX() + 4 == myVec[i]->getX())
					break;
			moveTo(getX() + 4, getY());
			break;
		}
	}

}
//===============================ITEM===================================================
Item::Item(int imageID, double startX, double startY, StudentWorld * myWorld)
	:Actor(imageID,startX,startY,myWorld)
{
}
//=================================HAZARD=================================================
Hazard::Hazard(int imageID, double startX, double startY, StudentWorld* myWorld)
	:Item(imageID, startX, startY, myWorld)
{
}
//================================WALL======================================================
Wall::Wall(double startX, double startY, StudentWorld* myWorld)
	:Hazard(IID_WALL, startX, startY, myWorld)
{
}

void Wall::doSomething()
{
}
