#include "StudentWorld.h"
#include <string>
#include "Compiler.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init(){
    //Compiler *compilerForEntrant0, *compilerForEntrant1, *compilerForEntrant2, *compilerForEntrant3;
    
    AntHill *ah0, *ah1, *ah2, *ah3;
    ah0 = nullptr; ah1 = nullptr; ah2 = nullptr; ah3 = nullptr;
    
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    
    compilerForEntrant0 = new Compiler;
    compilerForEntrant1 = new Compiler;
    compilerForEntrant2 = new Compiler;
    compilerForEntrant3 = new Compiler;
    std::string error;
    
    // compile the source file… If the compile function returns
    // false, there was a syntax error during compilation!
    if (fileNames.size() > 0 && ! compilerForEntrant0->compile(fileNames[0], error) )
    {
        // entrant 0’s source code had a syntax error!
        // send this error to our framework to warn the user.
        setError(fileNames[0] + " " + error);
        // return an error to tell our simulation framework
        // that something went wrong, and it’ll inform the user
        return GWSTATUS_LEVEL_ERROR;
    }
    
    if (fileNames.size() > 1 && !compilerForEntrant1->compile(fileNames[1], error)){
        setError(fileNames[1] + " " + error);
        return GWSTATUS_LEVEL_ERROR;
    }
    if (fileNames.size() > 2 && !compilerForEntrant2->compile(fileNames[2], error)){
        setError(fileNames[2] + " " + error);
        return GWSTATUS_LEVEL_ERROR;
    }
    if (fileNames.size() > 3 && !compilerForEntrant3->compile(fileNames[3], error)){
        setError(fileNames[3] + " " + error);
        return GWSTATUS_LEVEL_ERROR;
    }
    
    // otherwise, the entrant’s file compiled correctly!
    ticks = 0;
    
    std::string fieldName = getFieldFilename();
    Field f;
    
    std::string fieldError;
    if (f.loadField(fieldName, fieldError) != Field::LoadResult::load_success)
    {
        setError(fieldName + " " + fieldError);
        return GWSTATUS_LEVEL_ERROR; // something bad happened!
    }
    
    
    for (int i = 0; i < VIEW_HEIGHT; i++){
        for (int j = 0; j < VIEW_WIDTH; j++){
            Field::FieldItem current = f.getContentsOf(i, j);
            
            if (current == Field::FieldItem::anthill0){
                ah0 = new AntHill(this, i, j, 0, compilerForEntrant0);
                ant0Name = ah0->getName();
                fieldArray[i][j].push_back(ah0);
                numAnts0 = 0;
            }
            if (current == Field::FieldItem::anthill1){
                ah1 = new AntHill(this, i, j, 1, compilerForEntrant1);
                ant1Name = ah1->getName();
                fieldArray[i][j].push_back(ah1);
                numAnts1 = 0;
            }
            if (current == Field::FieldItem::anthill2){
                ah2 = new AntHill(this, i, j, 2, compilerForEntrant2);
                ant2Name = ah2->getName();
                fieldArray[i][j].push_back(ah2);
                numAnts2 = 0;
            }
            if (current == Field::FieldItem::anthill3){
                ah3 = new AntHill(this, i, j, 3, compilerForEntrant3);
                ant3Name = ah3->getName();
                fieldArray[i][j].push_back(ah3);
                numAnts3 = 0;
            }
            
            if (current == Field::FieldItem::rock){
                Pebble* pebble = new Pebble(this, i, j);
                (fieldArray[i][j]).push_back(pebble);
                
            }
            if (current == Field::FieldItem::water){
                WaterPool* water = new WaterPool(this, i, j);
                fieldArray[i][j].push_back(water);
            }
            if (current == Field::FieldItem::poison){
                Poison* poison = new Poison(this, i, j);
                fieldArray[i][j].push_back(poison);
            }
            if (current == Field::FieldItem::grasshopper){
                BabyGrasshopper* bg = new BabyGrasshopper(this, i, j, 500);
                (fieldArray[i][j]).push_back(bg);
            }
            if (current == Field::FieldItem::food){
                Food* food = new Food(this, i, j, 6000);
                fieldArray[i][j].push_back(food);
            
            }
            
        }
    }
    /*
    addActor(ah0);
    addActor(ah1);
    addActor(ah2);
    addActor(ah3);
     */
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
		  // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
    
    //loop through data structure, make objects do something
    for (int i = 0; i < VIEW_HEIGHT; i++){
        for (int j = 0; j < VIEW_WIDTH; j++){
            if (!fieldArray[i][j].empty()){
                //go through all objects at this location
                std::list<Actor*>::iterator it = fieldArray[i][j].begin();
                while (it != fieldArray[i][j].end()){
                    (*it)->doSomething();
                    //check if object has moved
                    if (i != (*it)->getY() || j != (*it)->getX()){
                        int a = (*it)->getX();
                        int b = (*it)->getY();
                        Actor* temp = *it;
                        it = fieldArray[i][j].erase(it);
                        fieldArray[b][a].push_back(temp);
                    }
                    else{
                        it++;
                    }
                }
            }
        }
    }
    
    //loop through data structor, remove dead actors
    for (int i = 0; i < VIEW_HEIGHT; i++){
        for (int j = 0; j < VIEW_WIDTH; j++){
            if (!fieldArray[i][j].empty()){
                std::list<Actor*>::iterator it = fieldArray[i][j].begin();
                while (it != fieldArray[i][j].end()){
                    if ((*it)->isDead()){
                        delete (*it);
                        it = fieldArray[i][j].erase(it);
                    }
                    else
                        it++;
                }
            }
        }
    }
    
    //increment ticks
    ticks++;
    
    //update status text
    setDisplayText();

    
    //check endgame situation
    if (ticks == 2000){
        if (getWinningAntNumber() == 0){
            if (numAnts0 > 5)
                setWinner(ant0Name);
                return GWSTATUS_PLAYER_WON;
        }
        if (getWinningAntNumber() == 1){
            if (numAnts1 > 5)
                setWinner(ant1Name);
            return GWSTATUS_PLAYER_WON;
        }
        if (getWinningAntNumber() == 2){
            if (numAnts2 > 5)
                setWinner(ant2Name);
            return GWSTATUS_PLAYER_WON;
        }
        if (getWinningAntNumber() == 3){
            if (numAnts3 > 5)
                setWinner(ant3Name);
            return GWSTATUS_PLAYER_WON;
        }
        return GWSTATUS_NO_WINNER;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    for (int i = 0; i < VIEW_HEIGHT; i++){
        for (int j = 0; j < VIEW_WIDTH; j++){
            if (!fieldArray[i][j].empty()){
                std::list<Actor*>::iterator it = fieldArray[i][j].begin();
                while (it != fieldArray[i][j].end()){
                    delete (*it);
                    it = fieldArray[i][j].erase(it);
                }
            }
        }
    }
    ticks = 0;
}

StudentWorld::~StudentWorld(){
    cleanUp();
    delete compilerForEntrant0;
    delete compilerForEntrant1;
    delete compilerForEntrant2;
    delete compilerForEntrant3;
}

////////////////////////////////////////////////

bool StudentWorld::addFood(int amt, int x, int y){
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            if ((*it)->isEdible()){
                return false;
            }
            it++;
        }
    }
    Food* food = new Food(this, x, y, amt);
    fieldArray[y][x].push_back(food);
    return true;
}

bool StudentWorld::addPheromone(int x, int y, int colony){
    if (getPheromoneAt(x, y, colony) != nullptr){
        return false;
    }
    Pheromone* pher = new Pheromone (this, x, y, colony);
    fieldArray[y][x].push_back(pher);
    return true;

}

///////////////////////////////////////////////

bool StudentWorld::canMoveTo(int x, int y) const {
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::const_iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            if ((*it)->blocksMovement()){
                return false;
            }
            it++;
        }
    }
    return true;
}

void StudentWorld::addActor(Actor* a){
    int x = a->getX();
    int y = a->getY();
    fieldArray[y][x].push_back(a);
}

Actor* StudentWorld::getEdibleAt(int x, int y) const {
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::const_iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            if ((*it)->isEdible()){
                return *it;
            }
            it++;
        }
    }
    return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const {
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::const_iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            if ((*it)->isPheromone(colony)){
                return *it;
            }
            it++;
        }
    }
    return nullptr;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const {
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::const_iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            if ((*it)->isEnemy(colony)){
                return true;
            }
            it++;
        }
    }
    return false;
}

bool StudentWorld::isAntHillAt(int x, int y, int colony) const {
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::const_iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            if ((*it)->isMyHill(colony) == true){
                return true;
            }
            it++;
        }
    }
    return false;
}

bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage){
    if (!fieldArray[me->getY()][me->getX()].empty()){
        std::list<Actor*>::iterator it = fieldArray[me->getY()][me->getX()].begin();
        while (it != fieldArray[me->getY()][me->getX()].end()){
            if ((*it)->isEnemy(colony) && (*it) != me){
                (*it)->getBitten(biteDamage);
                return true;
            }
            it++;
        }
    }
    return false;
}

bool StudentWorld::stunAllStunnableAt(int x, int y){
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            (*it)->getStunned();
            it++;
        }
        return true;
    }
    return false;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y){
    if (!fieldArray[y][x].empty()){
        std::list<Actor*>::iterator it = fieldArray[y][x].begin();
        while (it != fieldArray[y][x].end()){
            (*it)->getPoisoned(150);
            it++;
        }
        return true;
    }
    return false;
}

void StudentWorld::increaseScore(int colony){
    if (colony == 0)
        numAnts0++;
    if (colony == 1)
        numAnts1++;
    if (colony == 2)
        numAnts2++;
    if (colony == 3)
        numAnts3++;
}

int StudentWorld::getCurrentTicks(){
    return ticks;
}

int StudentWorld::getNumberOfAntsForAnt(int colony){
    if (colony == 0)
        return numAnts0;
    if (colony == 1)
        return numAnts1;
    if (colony == 2)
        return numAnts2;
    if (colony == 3)
        return numAnts3;
    
    return -1;
}

int StudentWorld::getWinningAntNumber(){
    //if there is a clear winner
    if (numAnts0 > numAnts1 &&
        numAnts0 > numAnts2 &&
        numAnts0 > numAnts3){
        currentWinningAnt = 0;
        return 0;
    }
    if (numAnts1 > numAnts0 &&
        numAnts1 > numAnts2 &&
        numAnts1 > numAnts3){
        currentWinningAnt = 1;
        return 1;
    }
    if (numAnts2 > numAnts0 &&
        numAnts2 > numAnts1 &&
        numAnts2 > numAnts3){
        currentWinningAnt = 2;
        return 2;
    }
    if (numAnts3 > numAnts1 &&
        numAnts3 > numAnts2 &&
        numAnts3 > numAnts0){
        currentWinningAnt = 3;
        return 3;
    }
    // if there is a tie
    return currentWinningAnt;
    
}

std::string StudentWorld::formatDisplayText(int ticks, int antsAnt0, int antsAnt1, int antsAnt2, int antsAnt3){
    string text = "";
    string ant0 = ""; string ant1 = ""; string ant2 = ""; string ant3 = "";
    
    if (antsAnt0 != -1){
        if (antsAnt0 < 10){
            ant0 += '0';
            char ch = '0' + antsAnt0;
            ant0 += ch;
        }
        else{
            ant0 = to_string(antsAnt0);
        }
    }
    if (antsAnt1 != -1){
        if (antsAnt1 < 10){
            ant1 += '0';
            char ch = '0' + antsAnt1;
            ant1 += ch;
        }
        else{
            ant1 = to_string(antsAnt1);
        }
    }
    if (antsAnt2 != -1){
        if (antsAnt2 < 10){
            ant2 += '0';
            char ch = '0' + antsAnt2;
            ant2 += ch;
        }
        else{
            ant2 = to_string(antsAnt2);
        }
    }
    if (antsAnt3 != -1){
        if (antsAnt3 < 10){
            ant3 += '0';
            char ch = '0' + antsAnt3;
            ant3 += ch;
        }
        else{
            ant3 = to_string(antsAnt3);
        }
    }
    
    string numTicks = "" + to_string(2000 - ticks);
    text = "Ticks: " + numTicks + " - " + ant0Name + " " + ant0 + " " + ant1Name + " " + ant1 + " " + ant2Name + " " + ant2 + " " + ant3Name + " " + ant3;
    return text;
}


void StudentWorld::setDisplayText(){
    int ticks = getCurrentTicks();
    
    int antsAnt0, antsAnt1, antsAnt2, antsAnt3;
    int winningAntNumber;
    
    antsAnt0 = getNumberOfAntsForAnt(0);
    antsAnt1 = getNumberOfAntsForAnt(1);
    antsAnt2 = getNumberOfAntsForAnt(2);
    antsAnt3 = getNumberOfAntsForAnt(3);
    winningAntNumber = getWinningAntNumber();
    
    string text = formatDisplayText(ticks, antsAnt0, antsAnt1, antsAnt2, antsAnt3);
    setGameStatText(text);
    
}
