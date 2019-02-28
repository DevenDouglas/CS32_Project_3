#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
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
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find"<<levelFile<< "data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
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
						addActor(new Penelope(this, i * 16, j * 16));
					case Level::citizen:
						break;
						addActor(new Citizen(this, i * 16, j * 16));
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
	return GWSTATUS_PLAYER_DIED;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	for (int i = 0;i < m_actors.size();i++)
		m_actors[i]->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//int numThings = m_actors.size();
	//cout <<"\t\t\tmy size is"<< m_actors.size() << endl;
	for (int i = 0;i < m_actors.size();++i)
	{
		//cout << i << endl;
		delete m_actors[i];
	}
	m_actors.clear();
	//cout << "squeaky clean" << endl;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
}

void StudentWorld::activateOnAppropriateActors(Actor * a)
{
}

bool StudentWorld::isAgentMovementBlockedAt(double x, double y, Actor* me) const
{
		vector<Actor*>::const_iterator it;
		for (it = m_actors.begin();it != m_actors.end();it++)
			if ((*it) != me && (*it)->blocksMovement() &&
				y <= (((*it)->getY()) + SPRITE_HEIGHT - 1) && y >= ((*it)->getY() - SPRITE_HEIGHT + 1)
				&& (x <= (((*it)->getX()) + SPRITE_WIDTH - 1) && x >= (*it)->getX() - SPRITE_WIDTH + 1))
				return true;
		return false;
}

bool StudentWorld::isFlameBlockedAt(double x, double y) const
{
	return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const
{
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
