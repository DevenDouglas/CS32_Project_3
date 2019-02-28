#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
using namespace std;


//============================================================ACTOR===========================================
Actor::Actor(StudentWorld * w, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID,x,y,dir,depth), m_dead(false),m_world(w)
{
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
	:ActivatingObject(w,IID_LANDMINE,x,y,1,right),safetyTicks(30),isActive(false)
{
}

void Landmine::doSomething()
{
	if (isDead())
		return;
	if (safetyTicks)
	{
		safetyTicks--;
		if (!safetyTicks)
			isActive = true;
		return;
	}
	getWorld()->activateOnAppropriateActors(this);

}

void Landmine::activateIfAppropriate(Actor * a)
{
	if (a->triggersOnlyActiveLandmines())
		dieByFallOrBurnIfAppropriate();
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
	for (int x = getX() - SPRITE_WIDTH;x <= getX() + SPRITE_WIDTH;x += SPRITE_WIDTH)
		for (int y = getY() - SPRITE_HEIGHT;y <= getY() + SPRITE_HEIGHT;y += SPRITE_HEIGHT)
			if (!getWorld()->isFlameBlockedAt(x, y))
				getWorld()->addActor(new Flame(getWorld(), x, y, up));
	getWorld()->addActor(new Pit(getWorld(), getX(), getY()));
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

//============================================================HUMAN===========================================
Human::Human(StudentWorld * w, int imageID, double x, double y)
	:Agent(w,imageID,x,y),m_isInfected(false),m_infection(0)
{
}

void Human::beVomitedOnIfAppropriate()
{
	if (!m_isInfected && !isPlayer())
		getWorld()->playSound(SOUND_CITIZEN_INFECTED);
	m_isInfected = true;
	if (getInfectionDuration() == 0)
		m_infection = 1;
}

void Human::clearInfection()
{
	m_isInfected = false;
	m_infection = 0;
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
		case KEY_PRESS_SPACE:
			if (m_numFlameCharges)
			{
				m_numFlameCharges--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				switch (getDirection())
				{
				case up:
					for (int i = 1;i < 4;i++)
						if (getWorld()->isFlameBlockedAt(getX(), getY() + i * SPRITE_HEIGHT))
							break;
						else
							getWorld()->addActor(new Flame(getWorld(), getX(), getY() + i * SPRITE_HEIGHT, up));
					break;
				case down:
					for (int i = 1;i < 4;i++)
						if (getWorld()->isFlameBlockedAt(getX(), getY() - i * SPRITE_HEIGHT))
							break;
						else
							getWorld()->addActor(new Flame(getWorld(), getX(), getY() - i * SPRITE_HEIGHT, down));
					break;
				case left:
					for (int i = 1;i < 4;i++)
						if (getWorld()->isFlameBlockedAt(getX() - i * SPRITE_HEIGHT, getY()))
							break;
						else
							getWorld()->addActor(new Flame(getWorld(), getX() - i * SPRITE_HEIGHT, getY(), left));
					break;
				case right:
					for (int i = 1;i < 4;i++)
						if (getWorld()->isFlameBlockedAt(getX() + i * SPRITE_HEIGHT, getY()))
							break;
						else
							getWorld()->addActor(new Flame(getWorld(), getX() + i * SPRITE_HEIGHT, getY(), right));
					break;
				}
			}
			break;
		case KEY_PRESS_TAB:
			if (m_numLandmines)
			{
				m_numLandmines--;
				getWorld()->addActor(new Landmine(getWorld(), getX(), getY()));
			}
			break;
		case KEY_PRESS_ENTER:
			if (m_numVaccines)
			{
				m_numVaccines--;
				clearInfection();
			}
			break;
		}
	}
	

}

void Penelope::useExitIfAppropriate()
{
	getWorld()->recordLevelFinishedIfAllCitizensGone();
	getWorld()->playSound(SOUND_LEVEL_FINISHED);
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_PLAYER_DIE);
	getWorld()->decLives();
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
	setDead();
}

//============================================================ZOMBIE===========================================
Zombie::Zombie(StudentWorld * w, double x, double y)
	:Agent(w,IID_ZOMBIE,x,y), m_stuck(false),m_movementPlan(0)
{
}
void Zombie::flipStuck()
{
	if (m_stuck == false)
		m_stuck = true;
	else
		m_stuck = false;
}

bool Zombie::vomitIfAppropriate()
{
	switch (getDirection())
	{
	case right:
		if (getWorld()->isZombieVomitTriggerAt(getX() + SPRITE_WIDTH, getY()))
			if (!randInt(0, 2))
			{
				getWorld()->addActor(new Vomit(getWorld(), getX() + SPRITE_WIDTH, getY()));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		break;
	case left:
		if (getWorld()->isZombieVomitTriggerAt(getX() - SPRITE_WIDTH, getY()))
			if (!randInt(0, 2))
			{
				getWorld()->addActor(new Vomit(getWorld(), getX() - SPRITE_WIDTH, getY()));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		break;
	case up:
		if (getWorld()->isZombieVomitTriggerAt(getX(), getY() + SPRITE_HEIGHT))
			if (!randInt(0, 2))
			{
				getWorld()->addActor(new Vomit(getWorld(), getX(), getY() + SPRITE_HEIGHT));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		break;
	case down:
		if (getWorld()->isZombieVomitTriggerAt(getX(), getY() - SPRITE_HEIGHT))
			if (!randInt(0, 2))
			{
				getWorld()->addActor(new Vomit(getWorld(), getX(), getY() - SPRITE_HEIGHT));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}
		break;
	}
	return false;

}

void Zombie::zombieShuffle()
{
	switch (getDirection())
	{
	case right:
		if (!getWorld()->isAgentMovementBlockedAt(getX() + 1, getY(), this))
		{
			moveTo(getX() + 1, getY());
			decPlan();
		}
		else
			setPlan(0);
		break;
	case left:
		if (!getWorld()->isAgentMovementBlockedAt(getX() - 1, getY(), this))
		{
			moveTo(getX() - 1, getY());
			decPlan();
		}
		else
			setPlan(0);
		break;
	case up:
		if (!getWorld()->isAgentMovementBlockedAt(getX(), getY() + 1, this))
		{
			moveTo(getX(), getY() + 1);
			decPlan();
		}
		else
			setPlan(0);
		break;
	case down:
		if (!getWorld()->isAgentMovementBlockedAt(getX(), getY() - 1, this))
		{
			moveTo(getX(), getY() - 1);
			decPlan();
		}
		else
			setPlan(0);
		break;
	}
}
//============================================================DUMB_ZOMBIE===========================================
DumbZombie::DumbZombie(StudentWorld * w, double x, double y)
	: Zombie(w, x, y)
{
}

void DumbZombie::doSomething()
{
	if (isDead())
		return;
	flipStuck();
	if (!isStuck())
		return;
	if (vomitIfAppropriate())
		return;
	if (!getPlan())
	{
		setPlan(randInt(3,10));
		setDirection(90 *randInt(0, 3));
	}
	zombieShuffle();
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(1000);
	if (!randInt(0, 9))
		switch (getDirection())
		{
		case up:
			if (!getWorld()->wouldOverlap(getX(), getY() + SPRITE_HEIGHT))
				getWorld()->addActor(new VaccineGoodie(getWorld(), getX(), getY() + SPRITE_HEIGHT));
			break;
		case down:
			if (!getWorld()->wouldOverlap(getX(), getY() - SPRITE_HEIGHT))
				getWorld()->addActor(new VaccineGoodie(getWorld(), getX(), getY() - SPRITE_HEIGHT));
			break;
		case right:
			if (!getWorld()->wouldOverlap(getX() + SPRITE_WIDTH, getY()))
				getWorld()->addActor(new VaccineGoodie(getWorld(), getX() + SPRITE_WIDTH, getY()));
			break;
		case left:
			if (!getWorld()->wouldOverlap(getX()-SPRITE_WIDTH, getY() ))
				getWorld()->addActor(new VaccineGoodie(getWorld(), getX()-SPRITE_WIDTH, getY()));
			break;
		}
}

//============================================================SMART_ZOMBIE===========================================
SmartZombie::SmartZombie(StudentWorld * w, double x, double y)
	:Zombie(w,x,y)
{
}

void SmartZombie::doSomething()
{
	if (isDead())
		return;
	flipStuck();
	if (!isStuck())
		return;
	if (vomitIfAppropriate())
		return;
	if (!getPlan())
	{
		setPlan(randInt(3, 10));
		double x=0;
		double y=0;
		double distance=6401;
		if (getWorld()->locateNearestVomitTrigger(getX(), getY(), x, y, distance))
		{
			x -= getX();
			y -= getY();
			if (x > 0 && y > 0)
				setDirection(90 * randInt(0, 1));
			else if (x < 0 && y > 0)
				setDirection(90 * randInt(1, 2));
			else if (x < 0 && y < 0)
				setDirection(90 * randInt(2, 3));
			else if (x > 0 && y < 0)
				setDirection(270 * randInt(0, 1));
			else if (x > 0 && y == 0)
				setDirection(right);
			else if (x < 0 && y == 0)
				setDirection(left);
			else if (x == 0 && y > 0)
				setDirection(up);
			else if (x == 0 && y < 0)
				setDirection(down);
		}
		else
			setDirection(90 * randInt(0, 3));
	}
	zombieShuffle();
}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	getWorld()->increaseScore(2000);
}
