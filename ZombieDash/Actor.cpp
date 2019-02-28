#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
using namespace std;


//============================================================ACTOR===========================================
Actor::Actor(StudentWorld * w, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID,x,y,dir,depth), m_dead(false),m_world(w)
{
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
	getWorld()->activateOnAppropriateActors(this);
}

void Exit::activateIfAppropriate(Actor * a)
{
		a->useExitIfAppropriate();
}

bool Exit::blocksFlame() const
{
	return true;
}

//============================================================PIT==========================================
Pit::Pit(StudentWorld * w, double x, double y)
	:ActivatingObject(w,IID_PIT,x,y,0,right)
{
}

void Pit::doSomething()
{
	getWorld()->activateOnAppropriateActors(this);
}

void Pit::activateIfAppropriate(Actor * a)
{
	a->dieByFallOrBurnIfAppropriate();
}

//============================================================FLAME===========================================
Flame::Flame(StudentWorld * w, double x, double y, int dir)
	:ActivatingObject(w, IID_FLAME, x, y, 0, dir), m_burnOut(2)
{
}

void Flame::doSomething()
{
	if (isDead())
		return;
	m_burnOut--;
	if (!m_burnOut)
	{
		setDead();
		return;
	}
	getWorld()->activateOnAppropriateActors(this);
}

void Flame::activateIfAppropriate(Actor * a)
{
	a->dieByFallOrBurnIfAppropriate();
}

//============================================================VOMIT===========================================
Vomit::Vomit(StudentWorld * w, double x, double y)
	:ActivatingObject(w,IID_VOMIT,x,y,0,right),m_bileCount(2)
{
}

void Vomit::doSomething()
{
	if (isDead())
		return;
	m_bileCount--;
	if (!m_bileCount)
	{
		setDead();
		return;
	}
	getWorld()->activateOnAppropriateActors(this);
}

void Vomit::activateIfAppropriate(Actor * a)
{
	a->beVomitedOnIfAppropriate();
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
	a->pickUpGoodieIfAppropriate(this);
	getWorld()->increaseScore(50);
	setDead();
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

void Goodie::dieByFallOrBurnIfAppropriate()
{
	setDead();
}

//============================================================VACCINE_GOODIE===========================================
VaccineGoodie::VaccineGoodie(StudentWorld * w, double x, double y)
	:Goodie(w,IID_VACCINE_GOODIE,x,y)
{
}

void VaccineGoodie::doSomething()
{
	if (isDead())
		return;
	getWorld()->activateOnAppropriateActors(this);
}

void VaccineGoodie::pickUp(Penelope * p)
{
	p->increaseVaccines();
}

//============================================================GAS_CAN_GOODIE===========================================
GasCanGoodie::GasCanGoodie(StudentWorld * w, double x, double y)
	:Goodie(w,IID_GAS_CAN_GOODIE,x,y)
{
}

void GasCanGoodie::doSomething()
{
	if (isDead())
		return;
	getWorld()->activateOnAppropriateActors(this);
}

void GasCanGoodie::pickUp(Penelope * p)
{
	p->increaseFlameCharges();
	
}

//============================================================LANDMINE_GOODIE===========================================
LandmineGoodie::LandmineGoodie(StudentWorld * w, double x, double y)
	:Goodie(w,IID_LANDMINE_GOODIE,x,y)
{
}

void LandmineGoodie::doSomething()
{
	if (isDead())
		return;
	getWorld()->activateOnAppropriateActors(this);
}

void LandmineGoodie::pickUp(Penelope * p)
{
	p->increaseLandmines();
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
	:Agent(w,imageID,x,y),m_isInfected(false),m_infection(0)
{
}

void Human::beVomitedOnIfAppropriate()
{
	m_isInfected = true;
	if (getInfectionDuration() == 0)
		m_infection = 1;
}

bool Human::triggersZombieVomit() const
{
	return false;
}

void Human::clearInfection()
{
}

//============================================================PENELOPE===========================================
Penelope::Penelope(StudentWorld * w, double x, double y)
	:Human(w,IID_PLAYER,x,y),m_numVaccines(0),m_numFlameCharges(0),m_numLandmines(0)
{
}

void Penelope::doSomething()
{
	if (isDead())
		return;
	if (getInfectionDuration()>0)
	{
		incrementInfect();
		if (getInfectionDuration() == 500)
		{
			setDead();
			getWorld()->playSound(SOUND_PLAYER_DIE);
			return;
		}
	}

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
		case KEY_PRESS_SPACE: //not done yet
			break;
		case KEY_PRESS_TAB: //not done yet
			break;
		case KEY_PRESS_ENTER: //not done yet
			break;
		}
	}
	

}

void Penelope::useExitIfAppropriate()
{
	getWorld()->recordLevelFinishedIfAllCitizensGone();
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::pickUpGoodieIfAppropriate(Goodie * g)
{
	g->pickUp(this);

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
	setDead();
	getWorld()->playSound(SOUND_CITIZEN_SAVED);
	getWorld()->increaseScore(500);
	getWorld()->recordCitizenGone();
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
