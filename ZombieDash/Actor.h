#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
/*
GraphObject(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
	: m_imageID(imageID), m_x(startX), m_y(startY), m_destX(startX), m_destY(startY),
	m_animationNumber(0), m_direction(dir), m_depth(depth), m_size(size)
{
	if (m_size <= 0)
		m_size = 1;

	getGraphObjects(m_depth).insert(this);
	int     m_imageID;
	double  m_x;
	double  m_y;
	double  m_destX;
	double  m_destY;
	int     m_animationNumber;
	Direction   m_direction;
	int     m_depth;
	double  m_size;
}*/

class Actor: public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, StudentWorld* myWorld);
	virtual void doSomething()=0;
	StudentWorld * getWorld() { return m_world; };
	bool isAlive() { return m_alive; };
	void generateBoundLine(double startPix, double endPix, std::vector<double>& line);
private:
	StudentWorld* m_world;
	bool m_alive;
};

class Biological : public Actor
{
public:
	Biological(int imageID, double startX, double startY, StudentWorld* myWorld);
	virtual void doSomething() = 0;
private:
};
//class Zombie: public Biological
class Penelope : public Biological
{
public:
	Penelope(double startX, double startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
//class SmartZombie:public Zombie
//class DumbZombie:public Zombie
//class Citizen: public Biological
class Item : public Actor
{
public:
	Item(int imageID, double startX, double startY, StudentWorld* myWorld);
	virtual void doSomething() = 0;
};
//class Goodie: public Item
//class GasCan: public Goodie
//class Vaccine: public Goodie
//class LandmineGoodie: public Goodie
class Hazard : public Item
{
public:
	Hazard(int imageID, double startX, double startY, StudentWorld* myWorld);
	virtual void doSomething() = 0;
private:
};
//class Pit: public Hazard
//class Flame: public Hazard
//class Vomit: public Hazard
//class Landmine: public Hazard
class Wall : public Hazard
{
public:
	Wall(double startX, double startY, StudentWorld* myWorld);
	virtual void doSomething();
private:
};
//class Exit: public Hazard
#endif // ACTOR_H_
