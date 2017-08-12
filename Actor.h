#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imgID, int startX, int startY, Direction startDir, int depth);
    
    virtual ~Actor();
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    virtual bool isDead() const;
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned(int amt);
    
    // Cause this actor to be be stunned.
    virtual void getStunned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone(int colony) const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony) const;
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous(int colony) const;
    
    // Is this actor the anthill of the indicated colony?
    virtual bool isMyHill(int colony) const;
    
    // Get this actor's world.
    StudentWorld* getWorld() const;
    
    //set random direction
    void setRandomDirection();
    
    //set as dead
    void setDead();
    
    //decipher pheromone id
    int getPheromoneID(int colony);
    
    virtual bool isStunned() { return false; }
    
private:
    StudentWorld* m_world;
    bool m_isDead;
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* w, int x, int y);
    ~Pebble();
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* w, int imgID, int x, int y, Direction dir, int depth, int energy);
    
    virtual ~EnergyHolder();
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    int getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
    // Have this actor pick up an amount of food.
    int pickupFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
    int pickupAndEatFood(int amt);
    
    // Does this actor become food when it dies?
    virtual bool becomesFoodUponDeath() const;
    
    //get food held
    int getFoodHeld(){
        return m_foodHeld;
    }
    
    //set food held
    void setFoodHeld(int amt){
        m_foodHeld = amt;
    }
    
private:
    int m_energy;
    int m_foodHeld;
    
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};


class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* w, int x, int y, int colony, Compiler* program);
    virtual void doSomething();
    virtual bool isMyHill(int colony) const;
    void giveBirth();
    
    virtual bool isAntHill(int colony) const;
    std::string getName() const;
private:
    int m_colony;
    Compiler* m_compiler;
};


class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* w, int x, int y, int colony);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    // Increase the strength (i.e., energy) of this pheromone.
    void increaseStrength();
private:
    int m_colony;
};


class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* w,  int imgID, int x, int y, Direction dir, int depth);
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* w, int x, int y);
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};
 

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world,  int imgID, int x, int y, int energy);
    virtual ~Insect();
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual void getPoisoned(int amt);
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    virtual bool becomesFoodUponDeath() const;
    
    // gets x and y coordinates of square in front
    void getXYInFrontOfMe(int x, int y, int &frontX, int &frontY);
    
    // Move this insect one step forward if possible, and return true;
    // otherwise, return false without moving.
    virtual bool moveForwardIfPossible();
    
    // Increase the number of ticks this insect will sleep by the indicated amount.
    void increaseSleepTicks(int amt);
    int getSleepTicks();
    void setSleepTicks(int amt);
    
    bool isStunned();
    void setStunned();
    void setUnStunned();
    
    bool getStunnedByPool();
    void setStunnedByPool(bool value);
    
private:
    bool m_isStunned;
    bool m_stunnedByPool;
    int sleepTicks;
};

class Ant : public Insect
{
public:
    Ant(StudentWorld* w, int imgID, int x, int y, int colony, Compiler* program);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
    virtual bool moveForwardIfPossible();
    
    
private:
    int m_colony;
    Compiler* m_compiler;
    int m_counter;
    bool m_isStunned;
    bool m_wasBitten;
};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* w, int imgID, int x, int y, int energy);
    virtual void doSomething();
    
    int getSleepState();
    void setSleepState(int state);
    
private:
    int m_sleepState;
    int m_distance;
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* w, int x, int y, int energy);
    virtual bool isEnemy(int colony) const;
    virtual void doSomething();
    virtual void getStunned();
private:
    int stunnedByX;
    int stunnedByY;
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* w, int x, int y);
    virtual void getBitten(int amt);
    virtual void getStunned();
    virtual void getPoisoned(int amt);
    virtual void doSomething();
};

#endif // ACTOR_H_
