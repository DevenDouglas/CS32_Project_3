#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
using namespace std;


//============================================================ACTOR===========================================
Actor::Actor(StudentWorld * w, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID,x,y,dir,depth), m_dead(false),m_world(w)
{
}

bool Actor::isDead() const
{
	return m_dead;
}

void Actor::setDead()
{
	m_dead = true;
}

StudentWorld * Actor::getWorld() const
{
	return m_world;
}

void Actor::activateIfAppropriate(Actor * a)
{
}

void Actor::useExitIfAppropriate()
{
}

void Actor::dieByFallOrBurnIfAppropriate()
{
}

void Actor::beVomitedOnIfAppropriate()
{
}

void Actor::pickUpGoodieIfAppropriate(Goodie * g)
{
}

bool Actor::blocksMovement() const
{
	return false;
}

bool Actor::blocksFlame() const
{
	return false;
}

bool Actor::triggersOnlyActiveLandmines() const
{
	return false;
}

bool Actor::triggersZombieVomit() const
{
	return false;
}

bool Actor::threatensCitizens() const
{
	return false;
}

bool Actor::triggersCitizens() const
{
	return false;
}

//============================================================WALL==========================================
Wall::Wall(StudentWorld * w, double x, double y)
	:Actor(w,IID_WALL,x,y,right,0)
{
}

void Wall::doSomething()
{
	//does nothing
}

bool Wall::blocksMovement() const
{
	return true;
}

bool Wall::blocksFlame() const
{
	return true;
}

//=======================================================ACTIVATING_OBJECT==================================
ActivatingObject::ActivatingObject(StudentWorld * w, int imageID, double x, double y, int depth, int dir)
	:Actor(w,imageID,x,y,dir,depth)
{
}

//============================================================EXIT===========================================
Exit::Exit(StudentWorld * w, double x, double y)
	:ActivatingObject(w,IID_EXIT,x,y,1,right)
{
}

void Exit::doSomething()
{
}

void Exit::activateIfAppropriate(Actor * a)
{
}

bool Exit::blocksFlame() const
{
	return false;
}

//============================================================PIT==========================================
Pit::Pit(StudentWorld * w, double x, double y)
	:ActivatingObject(w,IID_PIT,x,y,0,right)
{
}

void Pit::doSomething()
{
}

void Pit::activateIfAppropriate(Actor * a)
{
}

//============================================================FLAME===========================================
Flame::Flame(StudentWorld * w, double x, double y, int dir)
	:ActivatingObject(w,IID_FLAME,x,y,0,dir)
{
}

void Flame::doSomething()
{
}

void Flame::activateIfAppropriate(Actor * a)
{
}

//============================================================VOMIT===========================================
Vomit::Vomit(StudentWorld * w, double x, double y)
	:ActivatingObject(w,IID_VOMIT,x,y,0,right)
{
}

void Vomit::doSomething()
{
}

void Vomit::activateIfAppropriate(Actor * a)
{
}

//============================================================LANDMINE===========================================
Landmine::Landmine(StudentWorld * w, double x, double y)
	:ActivatingObject(w,IID_LANDMINE,x,y,1,right)
{
}

void Landmine::doSomething()
{
}

void Landmine::activateIfAppropriate(Actor * a)
{
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
}

//============================================================GOODIE===========================================
Goodie::Goodie(StudentWorld * w, int imageID, double x, double y)
	:ActivatingObject(w,imageID,x,y,1,right)
{
}

void Goodie::activateIfAppropriate(Actor * a)
{
}

void Goodie::dieByFallOrBurnIfAppropriate()
{
}

//============================================================VACCINE_GOODIE===========================================
VaccineGoodie::VaccineGoodie(StudentWorld * w, double x, double y)
	:Goodie(w,IID_VACCINE_GOODIE,x,y)
{
}

void VaccineGoodie::doSomething()
{
}

void VaccineGoodie::pickUp(Penelope * p)
{
}

//============================================================GAS_CAN_GOODIE===========================================
GasCanGoodie::GasCanGoodie(StudentWorld * w, double x, double y)
	:Goodie(w,IID_GAS_CAN_GOODIE,x,y)
{
}

void GasCanGoodie::doSomething()
{
}

void GasCanGoodie::pickUp(Penelope * p)
{
}

//============================================================LANDMINE_GOODIE===========================================
LandmineGoodie::LandmineGoodie(StudentWorld * w, double x, double y)
	:Goodie(w,IID_LANDMINE_GOODIE,x,y)
{
}

void LandmineGoodie::doSomething()
{
}

void LandmineGoodie::pickUp(Penelope * p)
{
}

//============================================================AGENT===========================================
Agent::Agent(StudentWorld * w, int imageID, double x, double y)
	:Actor(w,imageID,x,y,right,0)
{
}

bool Agent::blocksMovement() const
{
	return true;
}

bool Agent::triggersOnlyActiveLandmines() const
{
	return false;
}

//============================================================HUMAN===========================================
Human::Human(StudentWorld * w, int imageID, double x, double y)
	:Agent(w,imageID,x,y)
{
}

void Human::beVomitedOnIfAppropriate()
{
}

bool Human::triggersZombieVomit() const
{
	return false;
}

void Human::clearInfection()
{
}

int Human::getInfectionDuration() const
{
	return 0;
}

//============================================================PENELOPE===========================================
Penelope::Penelope(StudentWorld * w, double x, double y)
	:Human(w,IID_PLAYER,x,y)
{
}

void Penelope::doSomething()
{
	int button;
	if (getWorld()->getKey(button))
	{
		switch (button)
		{
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (!getWorld()->isAgentMovementBlockedAt(getX(), getY() - 4, this))
				moveTo(getX(), getY() - 4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (!getWorld()->isAgentMovementBlockedAt(getX(), getY() + 4, this))
				moveTo(getX(), getY() + 4);
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (!getWorld()->isAgentMovementBlockedAt(getX() - 4, getY(), this))
				moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!getWorld()->isAgentMovementBlockedAt(getX() + 4, getY(), this))
				moveTo(getX() + 4, getY());
			break;
		}
	}

}

void Penelope::useExitIfAppropriate()
{
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
}

void Penelope::pickUpGoodieIfAppropriate(Goodie * g)
{
}

void Penelope::increaseVaccines()
{
}

void Penelope::increaseFlameCharges()
{
}

void Penelope::increaseLandmines()
{
}

int Penelope::getNumVaccines() const
{
	return 0;
}

int Penelope::getNumFlameCharges() const
{
	return 0;
}

int Penelope::getNumLandmines() const
{
	return 0;
}

//============================================================CITIZEN===========================================
Citizen::Citizen(StudentWorld * w, double x, double y)
	:Human(w,IID_CITIZEN,x,y)
{
}

void Citizen::doSomething()
{
}

void Citizen::useExitIfAppropriate()
{
}

void Citizen::dieByFallOrBurnIfAppropriate()
{
}

//============================================================ZOMBIE===========================================
Zombie::Zombie(StudentWorld * w, double x, double y)
	:Agent(w,IID_ZOMBIE,x,y)
{
}

//============================================================DUMB_ZOMBIE===========================================
DumbZombie::DumbZombie(StudentWorld * w, double x, double y)
	: Zombie(w, x, y)
{
}

void DumbZombie::doSomething()
{
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
}

//============================================================SMART_ZOMBIE===========================================
SmartZombie::SmartZombie(StudentWorld * w, double x, double y)
	:Zombie(w,x,y)
{
}

void SmartZombie::doSomething()
{
}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
}
