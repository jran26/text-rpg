#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>

enum Stat{MAX_HEALTH, PHYS_ATK, PHYS_DEF, MAG_ATK, MAG_DEF, SPEED};

class Entity{
protected:
    std::string name;
    std::string description;
    std::string deathMessage = "It dies.";
    // basic stat fields below
    int health = 0, maxHealth = 0, physAtk = 0, physDef = 0, magAtk = 0, magDef = 0, speed = 0, turnBar;
    // fields for buffs and debuffs below
    // a positive value means the unit is buffed for (value) turns. a negative value means the opposite. 
    int pAtkBuff = 0, pDefBuff = 0, mAtkBuff = 0, mDefBuff = 0, spdBuff = 0;
    // pre-buff/debuff values. 
    int pAtkOrig = 0, pDefOrig = 0, mAtkOrig = 0, mDefOrig = 0, spdOrig = 0;

public:
    Entity(){
        name = "";
        description = "";
        turnBar = 0;
    }

    Entity(std::string name, std::string description, int maxHealth, int physAtk, int physDef, int magAtk, int magDef, int speed) {
        this->name = name;
        this->description = description;
        this->maxHealth = maxHealth;
        health = maxHealth;
        this->physAtk = physAtk;
        this->physDef = physDef;
        this->magAtk = magAtk;
        this->magDef = magDef;
        this->speed = speed;
        turnBar = 0;
    }

    void inspect(){
        std::cout << name << ": " << description << "\n";
        std::cout << "Health: \t\t" << health << "/" << maxHealth << "\n"
        "Physical ATK/DEF: \t" << physAtk << "/" << physDef << "\n"
        "Magical ATK/DEF: \t" << magAtk << "/" << magDef << "\n"
        "Speed: \t\t\t" << speed << "\n";
    }

    bool isAlive() const{
	    return health > 0; 
    }	

    /**
     * Getters for all attributes below.
     * args: none
     * outputs: the value of the desired stat
     * */

    int getCurrentHealth(){
        return health;
    }

    int getMaxHealth(){
        return maxHealth;
    }

    int getPAtk(){
        return physAtk;
    }

    int getPDef(){
        return physDef;
    }

    int getMAtk(){
        return magAtk;
    }

    int getMDef(){
        return magDef;
    }
    
    int getSpeed(){
        return speed;
    }

    int getTurnBar(){
        return turnBar;
    }

    std::string getName(){
        return name;
    }

    std::string getDeathMessage(){
        return deathMessage;
    }

    /**Setters for some attributes below.*/
    void addTurnBar(int turn){
        turnBar += turn;
    }

    void setTurnBar(int turn){
        turnBar = turn;
    }

    void setDeathMessage(std::string message){
        deathMessage = message;
    }

    /**Methods for healing and dealing damage to this target below.
     * Damage formula is equivalent to: Damage * (1 - Defense / (Defense + 100)).
     * Int return value is for displaying feedback e.g. "Enemy deals " << dealPDamage(5) << " damage." 
     * */
    virtual int dealPDamage(int damage){
        double reduction = 1 - (double) physDef / (physDef + 100);
        health -= (int)((double)damage * reduction + 0.5);
        return (int)((double)damage * reduction + 0.5);
    }

    virtual int dealMDamage(int damage){
        double reduction = 1 - (double) magDef / (magDef + 100);
        health -= (int)((double)damage * reduction + 0.5);
        return (int)((double)damage * reduction + 0.5);
    }

    void heal(int value){
        health += value; 
        if (health > maxHealth) health = maxHealth;
    }

    /**
     * Methods for buffing and debuffing the target below.
     * buff(): Pass in a negative value to debuff, positive value to buff. 
     * Buffs and debuffs are fixed values. 
     *      pAtk/mAtk buffs/debuffs: +50%/-50%
     *      pDef/mDef buffs/debuffs: +50%/-50%
     *      speed buffs/debuffs: +30%/-30%
     * If a target already has an existing buff and the same buff is applied, the duration is added on.
     * If a target already has an existing buff and a debuff is applied, the duration is subtracted. 
     *      In this case, if there is underflow (e.g. 2 turn buff + 3 turn debuff) then the target will have 1 turn of the reverse. 
     * args: stat (the stat to buff/debuff), duration (the duration of the buff)
     * outputs: none
     * */
    void buff(Stat stat, int duration){
        /**steps:
         * 1. go to the block of code for the right stat
         * 2. if the current duration is at 0 (we are at the original value), store the current stat value. 
         * 3. increment the buff duration. negatives cancel out positives and vice versa. 
         * */
        switch(stat){
            case PHYS_ATK:{
                if (pAtkBuff == 0) pAtkOrig = physAtk;
                pAtkBuff += duration; 
            } break;
            case PHYS_DEF:{
                if (pDefBuff == 0) pDefOrig = physDef;
                pDefBuff += duration; 
            } break;
            case MAG_ATK:{
                if (mAtkBuff == 0) mAtkOrig = magAtk;
                mAtkBuff += duration; 
            } break;
            case MAG_DEF:{
                if (mDefBuff == 0) mDefOrig = magDef;
                mDefBuff += duration; 
            } break;
            case SPEED:{
                if (spdBuff == 0) spdOrig = speed;
                spdBuff += duration; 
            } break;
        }
    }

    /**
     * initializeOrigStats(): Saves all original stat values.
     * Call this upon starting every combat
     * args: none
     * outputs: none
     * */
    void initializeOrigStats(){
        pAtkOrig = physAtk;
        pDefOrig = physDef;
        mAtkOrig = magAtk;
        mDefOrig = magDef;
        spdOrig = speed;
    }

    /**
     * updateBuffs: Updates all existing buffs. Reduces their duration by 1 and if the duration hits 0, resets the character's stats. 
     * Call this method AFTER an entity's turn.
     * args: none
     * outputs: none
     * */
    void updateBuffs(){
        // first update buff durations
        if (pAtkBuff > 0) pAtkBuff--;
        else if (pAtkBuff < 0) pAtkBuff++;

        // then check if the buff duration is positive, negative, or zero.
        // buff accordingly. set to original value if the duration reaches zero
        if (pAtkBuff > 0) physAtk = 1.5 * pAtkOrig;
        else if (pAtkBuff == 0) physAtk = pAtkOrig;
        else physAtk = 0.5 * pAtkOrig;

        // magic attack
        if (mAtkBuff > 0) mAtkBuff--;
        else if (mAtkBuff < 0) mAtkBuff++;

        if (mAtkBuff > 0) magAtk = 1.5 * mAtkOrig;
        else if (mAtkBuff == 0) magAtk = mAtkOrig;
        else magAtk = 0.5 * mAtkOrig;

        // physical defense
        if (pDefBuff > 0) pDefBuff--;
        else if (pDefBuff < 0) pDefBuff++;

        if (pDefBuff > 0) physDef = 1.5 * pDefOrig;
        else if (pDefBuff == 0) physDef = pDefOrig;
        else physDef = 0.5 * pDefOrig;

        // magical defense
        if (mDefBuff > 0) mDefBuff--;
        else if (mDefBuff < 0) mDefBuff++;

        if (mDefBuff > 0) magDef = 1.5 * mDefOrig;
        else if (mDefBuff == 0) magDef = mDefOrig;
        else magDef = 0.5 * mDefOrig;

        // speed
        if (spdBuff > 0) spdBuff--;
        else if (spdBuff < 0) spdBuff++;

        if (spdBuff > 0) speed = 1.3 * spdOrig;
        else if (spdBuff == 0) speed = spdOrig;
        else speed = 0.7 * spdOrig;
    }

    /**
     * clearBuffs: Clears all buffs (resets them to 0).
     * Call this method after combat.
     * args: none
     * outputs: none 
     * */
    void clearBuffs(){
        pAtkBuff = 0;
        pDefBuff = 0;
        mAtkBuff = 0;
        mDefBuff = 0;
        spdBuff = 0;
    }

    /**
     * cleanse(): Cleanses all debuffs.
     * args: none
     * outputs: none
     * */
    void cleanse(){
        if (pAtkBuff < 0){
            pAtkBuff = 0;
            physAtk = pAtkOrig;
        }
        if (pDefBuff < 0){
            pDefBuff = 0;
            physDef = pDefOrig;
        }
        if (mAtkBuff < 0){
            mAtkBuff = 0;
            magAtk = mAtkOrig;
        }
        if (mDefBuff < 0){
            mDefBuff = 0;
            magDef = mDefOrig;
        }
        if (spdBuff < 0){
            spdBuff = 0;
            speed = spdOrig;
        }
    }

    virtual ~Entity() = default;
};

#endif
