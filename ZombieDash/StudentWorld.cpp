#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), m_numCitizens(0),m_levelFinished(false)
{
	
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	Level lev(assetPath());
	ostringstream oss;
	oss << "level0" << getLevel() << ".txt";
	string levelFile = oss.str();
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found || getLevel()>99)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int i = 0;i < 16;i++)
			for (int j = 0;j < 16;j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(i, j);// level_x=i, level_y=j
				switch (ge)                                    // so x=16i and y=16j   
				{
					case Level::empty:
						//do nothing
						break;
					case Level::wall:
						addActor(new Wall(this, i * 16, j * 16));
						break;
					case Level::player:
						p = new Penelope(this, i * 16, j * 16);
						break;
					case Level::citizen:
						addActor(new Citizen(this, i * 16, j * 16));
						m_numCitizens++;
						m_levelFinished = false;
						break;
					case Level::pit:
						addActor(new Pit(this, i * 16, j * 16));
						break;
					case Level::vaccine_goodie:
						addActor(new VaccineGoodie(this, i * 16, j * 16));
						break;
					case Level::gas_can_goodie:
						addActor(new GasCanGoodie(this, i * 16, j * 16));
						break;
					case Level::landmine_goodie:
						addActor(new LandmineGoodie(this, i * 16, j * 16));
						break;
					case Level::exit:
						addActor(new Exit(this, i * 16, j * 16));
						break;
					case Level::dumb_zombie:
						addActor(new DumbZombie(this, i * 16, j * 16));
						break;
					case Level::smart_zombie:
						addActor(new SmartZombie(this, i * 16, j * 16));
						break;
				}
			}
		return GWSTATUS_CONTINUE_GAME;
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	p->doSomething();
	if (p->isDead())
		return GWSTATUS_PLAYER_DIED;
	for (int i = 0;i < m_actors.size();i++)
	{
		if (!m_actors[i]->isDead())
			m_actors[i]->doSomething();
		if (p->isDead())
			return GWSTATUS_PLAYER_DIED;
		if (m_levelFinished)
			return GWSTATUS_FINISHED_LEVEL;
	}
	vector<Actor*>::iterator it;
	for (it = m_actors.begin();it != m_actors.end();it++)
	{
		if ((*it)->isDead())
		{
			delete *it;
			if (it != m_actors.end())
				swap(*it, m_actors.back());
			m_actors.pop_back();
			it = m_actors.begin();
		}
	}
	ostringstream oss;
	oss << "Score: ";
	oss.fill('0');
	oss << setw(6) << getScore();
	oss << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Vaccines: " <<
		p->getNumVaccines() << "  Flames: " << p->getNumFlameCharges() << "  Mines: " << 
		p->getNumLandmines() <<"  Infected: " << p->getInfectionDuration();
	string status = oss.str();
	setGameStatText(status);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0;i < m_actors.size();++i)
	{
		
		delete m_actors[i];
	}
	m_actors.clear();
	delete p;
}

void StudentWorld::recordCitizenGone()
{
	 m_numCitizens--;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
	if (!m_numCitizens)
		m_levelFinished = true;
}

void StudentWorld::activateOnAppropriateActors(Actor * a)
{
	for (int i = 0;i < m_actors.size();i++)
		if (pow(a->getX() - m_actors[i]->getX(), 2) + pow(a->getY() - m_actors[i]->getY(), 2) <= 100&&a!=m_actors[i])
			a->activateIfAppropriate(m_actors[i]);
	if (pow(a->getX() - p->getX(), 2) + pow(a->getY() - p->getY(), 2) <= 100)
		a->activateIfAppropriate(p);
}

bool StudentWorld::isAgentMovementBlockedAt(double x, double y, Actor* me) const
{
		vector<Actor*>::const_iterator it;
		for (it = m_actors.begin();it != m_actors.end();it++)
			if ((*it) != me && (*it)->blocksMovement() &&
				y <= (((*it)->getY()) + SPRITE_HEIGHT - 1) && y >= ((*it)->getY() - SPRITE_HEIGHT + 1)
				&& (x <= (((*it)->getX()) + SPRITE_WIDTH - 1) && x >= (*it)->getX() - SPRITE_WIDTH + 1))
				return true;
		if (p != me &&
			y <= (p->getY() + SPRITE_HEIGHT - 1) && y >= (p->getY() - SPRITE_HEIGHT + 1)
			&& (x <= ((p->getX()) + SPRITE_WIDTH - 1) && x >= p->getX() - SPRITE_WIDTH + 1))
			return true;
		return false;
}

bool StudentWorld::isFlameBlockedAt(double x, double y) const
{
	vector<Actor*>::const_iterator it;
	for (it = m_actors.begin();it != m_actors.end();it++)
		if ((*it)->blocksFlame() &&
			y <= (((*it)->getY()) + SPRITE_HEIGHT - 1) && y >= ((*it)->getY() - SPRITE_HEIGHT + 1)
			&& (x <= (((*it)->getX()) + SPRITE_WIDTH - 1) && x >= (*it)->getX() - SPRITE_WIDTH + 1))
			return true;
	return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const
{
	for (int i = 0;i < m_actors.size();i++)
	{
		if (m_actors[i]->triggersZombieVomit())
			if (pow(m_actors[i]->getX() - x, 2) + pow(m_actors[i]->getY() - y, 2) <= 100)
				return true;
	}
	if (pow(p->getX() - x, 2) + pow(p->getY() - y, 2) <= 100)
		return true;
	return false;
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, double & otherX, double & otherY, double & distance)
{
	return false;
}

bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double & otherX, double & otherY, double & distance, bool & isThreat) const
{
	return false;
}

bool StudentWorld::locateNearestCitizenThreat(double x, double y, double & otherX, double & otherY, double & distance) const
{
	return false;
}

bool StudentWorld::wouldOverlap(double x, double y)
{
	for (int i = 0;i < m_actors.size();i++)
		if (pow(x - m_actors[i]->getX(), 2) + pow(y - m_actors[i]->getY(), 2) <= 100)
			return true;
	if (pow(x - p->getX(), 2) + pow(y - p->getY(), 2) <= 100)
		return true;
	return false;
}
