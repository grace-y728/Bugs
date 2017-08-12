#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

/////////////////// ACTOR ///////////////////

Actor::Actor(StudentWorld* world, int imgID, int startX, int startY, Direction startDir, int depth)
: GraphObject(imgID, startX, startY, startDir, depth)
{
    m_world = world;
    m_isDead = false;
}

Actor::~Actor(){
}

// Is this actor dead?
bool Actor::isDead() const {
    return m_isDead;
}

// Does this actor block movement?
bool Actor::blocksMovement() const {
    return false;
}

// Cause this actor to be be bitten, suffering an amount of damage.
void Actor::getBitten(int amt){
    //
}

// Cause this actor to be be poisoned.
void Actor::getPoisoned(int amt) {
    
}

// Cause this actor to be be stunned.
void Actor::getStunned() {
    //
}

// Can this actor be picked up to be eaten?
bool Actor::isEdible() const {
    return false;
}

// Is this actor detected by an ant as a pheromone?
bool Actor::isPheromone(int colony) const {
    return false;
}

// Is this actor an enemy of an ant of the indicated colony?
bool Actor::isEnemy(int colony) const {
    return false;
}

// Is this actor detected as dangerous by an ant of the indicated colony?
bool Actor::isDangerous(int colony) const {
    return false;
}

// Is this actor the anthill of the indicated colony?
bool Actor::isMyHill(int colony) const {
    return false;
}

// Get this actor's world.
StudentWorld* Actor::getWorld() const {
    return m_world;
}

// set random direction
void Actor::setRandomDirection(){
    int randDir = randInt(1, 4);
    if (randDir == 1){
        setDirection(up);
    }
    else if (randDir == 2){
        setDirection(right);
    }
    else if (randDir == 3){
        setDirection(down);
    }
    else if (randDir == 4){
        setDirection(left);
    }
}

// set dead
void Actor::setDead(){
    m_isDead = true;
}

//get pheromone id
int Actor::getPheromoneID(int colony){
    if (colony == 0){
        return IID_PHEROMONE_TYPE0;
    }
    if (colony == 1){
        return IID_PHEROMONE_TYPE1;
    }
    if (colony == 2){
        return IID_PHEROMONE_TYPE2;
    }
    if (colony == 3){
        return IID_PHEROMONE_TYPE3;
    }
    return IID_PHEROMONE_TYPE0;
}

///////////////// PEBBLE /////////////////////

Pebble::Pebble(StudentWorld* w, int x, int y)
: Actor(w, IID_ROCK, x, y, right, 2)
{
}

Pebble::~Pebble(){}

void Pebble::doSomething(){}

bool Pebble::blocksMovement() const { return true; }

//////////////// ENERGY HOLDER ///////////////////

EnergyHolder::EnergyHolder(StudentWorld* w, int imgID, int x, int y, Direction dir, int depth, int energy)
: Actor(w, imgID, x, y, dir, depth)
{
    m_energy = energy;
    m_foodHeld = 0;
}

EnergyHolder::~EnergyHolder(){}

// Get this actor's amount of energy (for a Pheromone, same as strength).
int EnergyHolder::getEnergy() const {
    return m_energy;
}

// Adjust this actor's amount of energy upward or downward.
void EnergyHolder::updateEnergy(int amt){
    m_energy += amt;
}

// Add an amount of food to this actor's location.
void EnergyHolder::addFood(int amt){
    bool neededNew = getWorld()->addFood(100, getX(), getY());
    if (neededNew == false){
        Food* food = dynamic_cast <Food*> (getWorld()->getEdibleAt(getX(), getY()));
        food->updateEnergy(100);
    }
}

// Have this actor pick up an amount of food.
int EnergyHolder::pickupFood(int amt){
    if ((m_foodHeld + amt) < 1800){
        m_foodHeld += amt;
        return amt;
    }
    int temp = 1800 - m_foodHeld;
    m_foodHeld += (1800 - m_foodHeld);
    return temp;
}

// Have this actor pick up an amount of food and eat it.
int EnergyHolder::pickupAndEatFood(int amt){
    Food* food = dynamic_cast<Food*> (getWorld()->getEdibleAt(getX(), getY()));
    if (amt < food->getEnergy()){
        food->updateEnergy(-amt);
        updateEnergy(amt);
        return amt;
    }
    else{
        int actual = food->getEnergy();
        food->updateEnergy(-actual);
        food->setDead();
        updateEnergy(actual);
        return actual;
    }

    return 0;
}

bool EnergyHolder::becomesFoodUponDeath() const {
    return true;
}

/////////////// FOOD //////////////////

Food::Food(StudentWorld* w, int x, int y, int energy)
: EnergyHolder(w, IID_FOOD, x, y, right, 2, energy)
{}

void Food::doSomething(){}

bool Food::isEdible() const{ return true; }

////////////// ANTHILL /////////////////

AntHill::AntHill(StudentWorld* w, int x, int y, int colony, Compiler* program)
: EnergyHolder(w, IID_ANT_HILL, x, y, right, 2, 8999)
{
    m_colony = colony;
    m_compiler = program;
}

void AntHill::doSomething(){
    //decrease hitpoints
    updateEnergy(-1);
    
    //check if dead
    if (getEnergy() == 0){
        setDead();
        return;
    }
    
    //checks for food
    if (getWorld()->getEdibleAt(getX(), getY()) != nullptr){
        pickupAndEatFood(10000);
        return;
    }
    
    //check if it can produce new ant
    if (getEnergy() >= 2000){
        giveBirth();
        updateEnergy(-1500);
        getWorld()->increaseScore(m_colony);
    }
    
}

bool AntHill::isMyHill(int colony) const {
    return (colony == m_colony);
}

void AntHill::giveBirth() // used to give birth to a new ant
{
    // allocate a new ant, and pass in a pointer to the
    // Compiler object for this AntHill. The
    // m_pointerToMyCompilerObject below points to the Compiler
    // object that created in the StudentWorld::init() function
    // above. By passing this into each ant as it’s born, the ant
    // knows how to get hold of its instructions that govern it and
    //34
    // other members of its colony
    Ant* newAnt = nullptr;
    if (m_colony == 0)
        newAnt = new Ant(getWorld(), IID_ANT_TYPE0, getX(), getY(), m_colony, m_compiler);
    if (m_colony == 1)
        newAnt = new Ant(getWorld(), IID_ANT_TYPE1, getX(), getY(), m_colony, m_compiler);
    if (m_colony == 2)
        newAnt = new Ant(getWorld(), IID_ANT_TYPE2, getX(), getY(), m_colony, m_compiler);
    if (m_colony == 3)
        newAnt = new Ant(getWorld(), IID_ANT_TYPE3, getX(), getY(), m_colony, m_compiler);
    
    // now add our new ant to our simulation data structures
    // so it can be tracked and asked to do something during each
    // tick by our virtual world
    
    getWorld()->addActor(newAnt);
}

bool AntHill::isAntHill(int colony) const { return true; }

string AntHill::getName() const {
    return m_compiler->getColonyName();
}

/////////////// PHEROMONE ////////////////
Pheromone::Pheromone(StudentWorld* w, int x, int y, int colony)
: EnergyHolder(w, getPheromoneID(colony), x, y, right, 2, 256)
{
    m_colony = colony;
}

void Pheromone::doSomething(){
    updateEnergy(-1);
    if (getEnergy() <= 0){
        setDead();
    }
}

bool Pheromone::isPheromone(int colony) const {
    if (colony == m_colony)
        return true;
    return false;
}

// Increase the strength (i.e., energy) of this pheromone.
void Pheromone::increaseStrength() {
    if (getEnergy() + 256 > 768){
        updateEnergy(768 - getEnergy());
    }
    else{
        updateEnergy(256);
    }
}

/////////////// TRIGGERABLE ACTOR /////////////////
TriggerableActor::TriggerableActor(StudentWorld* w, int imgID, int x, int y, Direction dir, int depth)
: Actor(w, imgID, x, y, dir, depth)
{
    //
}
bool TriggerableActor::isDangerous(int colony) const {
    return true;
}

/////////////// POOL OF WATER /////////////////

WaterPool::WaterPool(StudentWorld* w, int x, int y)
: TriggerableActor(w, IID_WATER_POOL, x, y, right, 2)
{
}

void WaterPool::doSomething(){
    getWorld()->stunAllStunnableAt(getX(), getY());
}

/////// POISON

Poison::Poison(StudentWorld* sw, int x, int y)
: TriggerableActor(sw, IID_POISON, x, y, right, 2) {
}

void Poison::doSomething(){
    getWorld()->poisonAllPoisonableAt(getX(), getY());
}


/////////////// INSECT /////////////////

Insect::Insect(StudentWorld* w, int imgID, int x, int y, int energy)
: EnergyHolder(w, imgID, x, y, right, 1, energy)
{
    setRandomDirection();
    m_isStunned = false;
    m_stunnedByPool = false;
    sleepTicks = 0;
}

Insect::~Insect(){}

void Insect::doSomething(){}

void Insect::getBitten(int amt){
    if (amt < getEnergy()){
        updateEnergy(-amt);
    }
    else{
        setDead();
        addFood(100);
    }
}
void Insect::getPoisoned(int amt){
    if (amt < getEnergy()){
        updateEnergy(-amt);
    }
    else{
        setDead();
        addFood(100);
    }
}

void Insect::getStunned(){
    /*if (isStunned() == false){
        m_isStunned = true;
        increaseSleepTicks(2);
    }*/
}

bool Insect::isEnemy(int colony){
    return true;
}

bool Insect::becomesFoodUponDeath() const { return true; }

// get x,y coordinates of the spot one step in front of this insect.
void Insect::getXYInFrontOfMe(int x, int y, int &frontX, int &frontY) {
    if (getDirection() == right){
        frontX = x + 1;
        frontY = y;
    }
    else if (getDirection() == down){
        frontX = x;
        frontY = y - 1;
    }
    else if (getDirection() == left){
        frontX = x - 1;
        frontY = y;
    }
    else{
        frontX = x;
        frontY = y + 1;
    }
}

// Move this insect one step forward if possible, and return true;
// otherwise, return false without moving.
bool Insect::moveForwardIfPossible(){ return false; }

bool Insect::isStunned(){
    return m_isStunned;
}

void Insect::setStunned(){
    m_stunnedByPool = true;
    m_isStunned = true;
    setSleepTicks(2);
}

void Insect::setUnStunned(){
    m_isStunned = false;
}

void Insect::increaseSleepTicks(int amt){
    //sleepTicks = 0;
    sleepTicks += amt;
}

int Insect::getSleepTicks(){
    return sleepTicks;
}

void Insect::setSleepTicks(int amt){
    sleepTicks = amt;
}

bool Insect::getStunnedByPool(){
    return m_stunnedByPool;
}

void Insect::setStunnedByPool(bool value){
    m_stunnedByPool = value;
}

/////////////// ANT //////////////////
Ant::Ant(StudentWorld* w, int imgID, int x, int y, int colony, Compiler* program)
: Insect(w, imgID, x, y, 1500)
{
    m_colony = colony;
    m_compiler = program;
    m_counter = 0;
    m_wasBitten = false;
    
}

void Ant::doSomething(){
    
    //gets hungrier
    updateEnergy(-1);
    
    //check if dead
    if (getEnergy() <= 0){
        addFood(100);
        setDead();
        return;
    }
    
    //check if stunned
    
    //interpret commands
    Compiler::Command cmd;
    int ic = 0; // start at the beginning of the vector
    
    bool wasBlocked = false;
    

    for (m_counter = 0; m_counter < 10; m_counter++) // keep running forever for now
    {
        if (m_counter == 10) {
            return;
        }

        // get the command from element ic of the vector
        if ( ! (*m_compiler).getCommand(ic, cmd) ) {
            setDead();
            return; // error - no such instruction!
        }
        int randNum = -1;
        
        switch (cmd.opcode)
        {
    
            case (Compiler::moveForward):
            {
                // cause the ant to move forward by
                // updating its x,y coordinates
                bool isDone = moveForwardIfPossible();
                if (isDone == true){
                    wasBlocked = false;
                    m_wasBitten = false;

                }
                wasBlocked = true;
                ++ic; // advance to next instruction
                break;
            }
                
            case (Compiler::eatFood):
                if (getFoodHeld() > 0){
                    Food* food = dynamic_cast <Food*> (getWorld()->getEdibleAt(getX(), getY()));
                    if (getFoodHeld() > 100){
                        updateEnergy(100);
                    }
                    else{
                        updateEnergy(food->getEnergy());
                        food->updateEnergy(-(food->getEnergy()));
                    }
                }
                ++ic;
                break;
                
            case (Compiler::dropFood):
                if (getFoodHeld() > 0){
                    Food* food = dynamic_cast <Food*> (getWorld()->getEdibleAt(getX(), getY()));
                    if (food != nullptr){
                        food->updateEnergy(-(food->getEnergy()));
                    }
                    else{
                        addFood(getFoodHeld());
                    }
                    setFoodHeld(0);
                }
                ++ic;
                break;
            
            case (Compiler::bite):
                cout << "bite" << endl;
                if (getWorld()->isEnemyAt(getX(), getY(), m_colony) == true){
                    getWorld()->biteEnemyAt(this, m_colony, 15);
                }
                ++ic;
                break;
            
            case (Compiler::pickupFood):
                if (getWorld()->getEdibleAt(getX(), getY()) != nullptr){
                    pickupFood(400);
                }
                
                ++ic;
                break;
            
            case (Compiler::emitPheromone):
                if(getWorld()->getPheromoneAt(getX(), getY(), m_colony) != nullptr){
                    Pheromone* pher = dynamic_cast<Pheromone*> (getWorld()->getPheromoneAt(getX(), getY(), m_colony));
                    pher->increaseStrength();
                }
                else{
                    getWorld()->addPheromone(getX(), getY(), m_colony);
                }
                ++ic;
                break;
            
            case (Compiler::faceRandomDirection):
                setRandomDirection();
                ++ic;
                break;
                
            case (Compiler::rotateClockwise):
                if (getDirection() == up)
                    setDirection(right);
                else if (getDirection() == right)
                    setDirection(down);
                else if (getDirection() == down)
                    setDirection(left);
                else
                    setDirection(up);
                
                ++ic;
                break;
    
            case (Compiler::rotateCounterClockwise):
                if (getDirection() == up)
                    setDirection(right);
                else if (getDirection() == right)
                    setDirection(down);
                else if (getDirection() == down)
                    setDirection(left);
                else
                    setDirection(up);
                ++ic;
                break;
    
            case (Compiler::generateRandomNumber):
                if (stoi(cmd.operand1) == 0){
                    randNum = 0;
                }
                else{
                    randNum = randInt(0, stoi(cmd.operand1));
                }
                m_counter++;
                ++ic; // advance to next instruction
                break;
            
            case (Compiler::goto_command):
                // just set ic the specified position
                // in operand1
                ic = stoi(cmd.operand1);
                m_counter++;
                break;
            
            case (Compiler::if_command):
                // if the condition of the if command is
                // is true, then go to the target position
                // in the vector; otherwise fall through to
                // the next position
                //last number was zero
 
                m_counter++;
                
                if (stoi(cmd.operand1) == 9){
                    if (randNum == 0){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //is carrying food
                else if (stoi(cmd.operand1) == 3){
                    if (getFoodHeld() > 0){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //is hungry
                else if (stoi(cmd.operand1) == 4){
                    if (getEnergy() <= 25){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //is standing with enemy
                else if (stoi(cmd.operand1) == 7){
                    if (getWorld()->isEnemyAt(getX(), getY(), m_colony)){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //is standing on food
                else if (stoi(cmd.operand1) == 6){
                    if (getWorld()->getEdibleAt(getX(), getY()) != nullptr){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //is standing on anthill
                else if (stoi(cmd.operand1) == 5){
                    if (getWorld()->isAntHillAt(getX(), getY(), m_colony) == true){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //smells pheromone
                else if (stoi(cmd.operand1) == 1){
                    int frontX;
                    int frontY;
                    getXYInFrontOfMe(getX(), getY(), frontX, frontY);
                    if (getWorld()->getPheromoneAt(frontX, frontY, m_colony)){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //smells danger
                else if (stoi(cmd.operand1) == 0){
                    // FINISH
                    //ic = stoi(cmd.operand2);
                    break;
                }
                //i was bit
                else if (stoi(cmd.operand1) == 2){
                    if (m_wasBitten == true){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
                //i was blocked from moving
                else if (stoi(cmd.operand1) == 8){
                    if (wasBlocked == true){
                        ic = stoi(cmd.operand2);
                        break;
                    }
                }
            
                ++ic; // just advance to the next line
                break;
            
             
        }
    }

    
} //end of function

void Ant::getBitten(int amt){
    updateEnergy(-amt);
    m_wasBitten = true;
    if (getEnergy() <= 0){
        setDead();
        addFood(100);
    }
}

bool Ant::isEnemy(int colony) const {
    if (colony == m_colony)
        return false;
    return true;
}

bool Ant::moveForwardIfPossible(){
    if (getDirection() == up){
        if (getWorld()->canMoveTo(getX(), getY() + 1)){
            moveTo(getX(), getY() + 1);
            return true;
        }
    }
    else if (getDirection() == right){
        if (getWorld()->canMoveTo(getX() + 1, getY())){
            moveTo(getX() + 1, getY());
            return true;
        }
    }
    else if (getDirection() == down){
        if (getWorld()->canMoveTo(getX(), getY() - 1)){
            moveTo(getX(), getY() - 1);
            return true;
        }
    }
    else if (getDirection() == left){
        if (getWorld()->canMoveTo(getX() - 1, getY()) == true){
            moveTo(getX() - 1, getY());
            return true;
        }
    }
    return false;
}

/////////////// GRASSHOPPER ////////////////

Grasshopper::Grasshopper(StudentWorld* w, int imgID, int x, int y, int energy)
: Insect(w, imgID, x, y, energy)
{
    m_distance = randInt(2, 10);
    m_sleepState = 2;
}

int Grasshopper::getSleepState(){ return m_sleepState; }

void Grasshopper::setSleepState(int state){
    m_sleepState = state;
}

void Grasshopper::doSomething() {
    //if there is food on this square
    int eaten = 0;
    if (getWorld()->getEdibleAt(getX(), getY()) != nullptr){
        eaten = pickupAndEatFood(200);
    }
    //if grasshopper ate food
    if (eaten != 0){
        int shouldRest = randInt(1, 2);
        if (shouldRest == 1){
            return;
        }
    }
    //if distance is 0
    if (m_distance == 0){
        setRandomDirection();
        m_distance = randInt(2, 10);
    }
    //if not
    else{
        if (getDirection() == up){
            if (getWorld()->canMoveTo(getX(), getY() + 1) == false){
                m_distance = 0;
            }
            else{
                moveTo(getX(), getY() + 1);
                setSleepTicks(0);
                setStunnedByPool(false);
                
            }
        }
        else if (getDirection() == right){
            if (getWorld()->canMoveTo(getX() + 1, getY()) == false){
                m_distance = 0;
            }
            else{
                moveTo(getX() + 1, getY());
                setSleepTicks(0);
                setStunnedByPool(false);
            }
        }
        else if (getDirection() == down){
            if (getWorld()->canMoveTo(getX(), getY() - 1) == false){
                m_distance = 0;
            }
            else{
                moveTo(getX(), getY() - 1);
                setSleepTicks(0);
                setStunnedByPool(false);
            }
        }
        else if (getDirection() == left){
            if (getWorld()->canMoveTo(getX() - 1, getY()) == false){
                m_distance = 0;
            }
            else{
                moveTo(getX() - 1, getY());
                setSleepTicks(0);
                setStunnedByPool(false);
            }
        }
        if (m_distance != 0){
            m_distance--;
        }
    }

} //end of doSomething

////////////// BABY GRASSHOPPER ///////////////

BabyGrasshopper::BabyGrasshopper(StudentWorld* w, int x, int y, int energy)
: Grasshopper(w, IID_BABY_GRASSHOPPER, x, y, 500){
    setRandomDirection();
}

bool BabyGrasshopper::isEnemy(int colony) const {
    return true;
}

void BabyGrasshopper::doSomething() {
    //gets hungrier
    updateEnergy(-1);
    
    //check if dead
    if (getEnergy() <= 0){
        addFood(100);
        setDead();
        return;
    }
    
    //check if stunned
    if (getSleepTicks() > 0){
        increaseSleepTicks(-1);
        return;
    }
    
    // check sleep state
    if (getSleepState() == 2){
        
        //check if ready to moult
        if (getEnergy() >= 1600){
            Actor* adult = new AdultGrasshopper(getWorld(), getX(), getY());
            getWorld()->addActor(adult);
            setDead();
            addFood(100);
            return;
        }
        
        Grasshopper::doSomething();
        
        setSleepState(0);
    }
    else{
        setSleepState(getSleepState() + 1);
        return;
    }
    
}

void BabyGrasshopper::getStunned(){

    if (getStunnedByPool() == true){
        return;
    }
    setStunned();
}

////////////// ADULT GRASSHOPPER ///////////////
AdultGrasshopper::AdultGrasshopper(StudentWorld* w, int x, int y)
: Grasshopper(w, IID_ADULT_GRASSHOPPER, x, y, 1600)
{}

void AdultGrasshopper::getBitten(int amt){
    Insect::getBitten(amt);
    int willBite = randInt(1, 2);
    if (willBite == 1){
        getWorld()->biteEnemyAt(this, 10, 50);
    }
}

void AdultGrasshopper::getStunned(){
    return;
}

void AdultGrasshopper::getPoisoned(int amt){
    return;
}

void AdultGrasshopper::doSomething(){
    //gets hungrier
    updateEnergy(-1);
    
    //check if dead
    if (getEnergy() <= 0){
        addFood(100);
        setDead();
        return;
    }

    //check if grasshopper will bite
    int willBite = randInt(1, 3);
    if (willBite == 1){
        //bite
        getWorld()->biteEnemyAt(this, 10, 50);
        return;
    }
    //check if it wants to jump
    int willJump = randInt(1, 10);
    if (willJump == 1){
        // will jump
        return;
    }
    //do something else
    if (getSleepState() == 2){
        Grasshopper::doSomething();
        setSleepState(0);
    }
    else{
        setSleepState(getSleepState() + 1);
    }
    

    
    
    }

