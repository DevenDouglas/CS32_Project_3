#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
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
	string levelFile = "level01.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int i = 0;i < 16;i++)
		{
			for (int j = 0;j < 16;j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(i, j); // level_x=i, level_y=j
				if (ge == Level::player)
				{
					m_actors.push_back(new Penelope(i * 16, j * 16, this));
					cout << "flag found!" << endl;
				}
			}
		}
		for (int i = 0;i < 16;i++)
			for (int j = 0;j < 16;j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(i, j);// level_x=i, level_y=j
				switch (ge)                                    // so x=16i and y=16j   
				{
					/*case Level::empty:
						//do nothing
						break;
					case Level::smart_zombie:
						cout << "Location 80,160 starts with a smart zombie" << endl;
						break;
					case Level::dumb_zombie:
						cout << "Location 80,160 starts with a dumb zombie" << endl;
						break;*/
						//case Level::player:
							//cout << "wooh!" << endl;
							//m_actors.push_back(new Penelope(i * 16, j * 16, this));
							//break;
						/*case Level::exit:
							cout << "Location 80,160 is where an exit is" << endl;
							break;*/
				case Level::wall:
					//cout << "wall!" << endl;
					m_actors.push_back(new Wall(i * 16, j * 16, this));
					break;
					/*case Level::pit:
						cout << "Location 80,160 has a pit in the ground" << endl;
						break;       // etc…   }  */
				default:
					//do nothing
					break;
				}
				cout << m_actors.size() << endl;
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
