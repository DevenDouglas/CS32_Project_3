#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
	Actor();
	doSomething();
private:
}

class Biological : public Actor
{
}
//class Zombie: public Biological
class Penelope: public Biological
//class SmartZombie:public Zombie
//class DumbZombie:public Zombie
//class Citizen: public Biological
class Item: public Actor
//class Goodie: public Item
//class GasCan: public Goodie
//class Vaccine: public Goodie
//class LandmineGoodie: public Goodie
class Hazard: public Item
//class Pit: public Hazard
//class Flame: public Hazard
//class Vomit: public Hazard
//class Landmine: public Hazard
class Wall: public Hazard
//class Exit: public Hazard
#endif // ACTOR_H_
