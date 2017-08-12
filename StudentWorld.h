#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Compiler.h"
#include "Field.h"
#include <list>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}
    
    virtual ~StudentWorld();

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    //return true if new food was added
    bool addFood(int amt, int x, int y);
    
    // add new pheromone
    bool addPheromone(int x, int y, int colony);
    
    //////////////////////
    
    // Can an insect move to x,y?
    bool canMoveTo(int x, int y) const;
    
    // Add an actor to the world
    void addActor(Actor* a);
    
    // If an item that can be picked up to be eaten is at x,y, return a
    // pointer to it; otherwise, return a null pointer.  (Edible items are
    // only ever going be food.)
    Actor* getEdibleAt(int x, int y) const;
    
    // If a pheromone of the indicated colony is at x,y, return a pointer
    // to it; otherwise, return a null pointer.
    Actor* getPheromoneAt(int x, int y, int colony) const;
    
    // Is an enemy of an ant of the indicated colony at x,y?
    bool isEnemyAt(int x, int y, int colony) const;
    
    // Is something dangerous to an ant of the indicated colony at x,y?
    bool isDangerAt(int x, int y, int colony) const;
    
    // Is the anthill of the indicated colony at x,y?
    bool isAntHillAt(int x, int y, int colony) const;
    
    // Bite an enemy of an ant of the indicated colony at me's location
    // (other than me; insects don't bite themselves).  Return true if an
    // enemy was bitten.
    bool biteEnemyAt(Actor* me, int colony, int biteDamage);
    
    // Poison all poisonable actors at x,y.
    bool poisonAllPoisonableAt(int x, int y);
    
    // Stun all stunnable actors at x,y.
    bool stunAllStunnableAt(int x, int y);
    
    // Record another ant birth for the indicated colony.
    void increaseScore(int colony);
    
    // get current ticks
    int getCurrentTicks();
    
    // get number of ants for colony
    int getNumberOfAntsForAnt(int colony);
    
    // get winning ant number
    int getWinningAntNumber();
    
    // format display text
    std::string formatDisplayText(int ticks, int antsAnts0, int antsAnts1, int antsAnts2, int antsAnts3);
    
    // set display text
    void setDisplayText();

private:
    std::list<Actor*> fieldArray [VIEW_HEIGHT][VIEW_WIDTH];
    int ticks;
    int numAnts0 = -1;
    int numAnts1 = -1;
    int numAnts2 = -1;
    int numAnts3 = -1;
    int currentWinningAnt = -1;
    
    std::string ant0Name;
    std::string ant1Name;
    std::string ant2Name;
    std::string ant3Name;
    
    Compiler* compilerForEntrant0;
    Compiler* compilerForEntrant1;
    Compiler* compilerForEntrant2;
    Compiler* compilerForEntrant3;
     

};

#endif // STUDENTWORLD_H_
