#ifndef ITEM_H
#define ITEM_H

#include "InputReader.cpp"

#include <iostream>
#include <string>
#include <math.h>

class Item{
protected:
    std::string name;
    std::string description;
    std::string abilityName;
    std::string abilityDescription;
	int maxHealth = 0, physAtk = 0, physDef = 0, magAtk = 0, magDef = 0, speed = 0, cooldown = 0, maxCooldown = 0, value = 0;
    bool consumable = false, selfUse = false; 
    // items with the self-use flag set to true should not make use of the second target field in their ability().
    // items with the consumable flag set to true should never have any stat bonuses. 

public:
    Item(){
        this->name = "an item";
        this->description = "a description";
    }

    void inspect(){
        std::cout << name << ": " << description << "\n" << abilityName << ": " << abilityDescription << "\n";
        if (maxHealth > 0) std::cout << "Max Health: +" << maxHealth << "\n";
        if (physAtk > 0) std::cout << "Physical Attack: +" << physAtk << "\n";
        if (physDef > 0) std::cout << "Physical Defense: +" << physDef << "\n";
        if (magAtk > 0) std::cout << "Magical Attack: +" << magAtk << "\n";
        if (magDef > 0) std::cout << "Magical Defense: +" << magDef << "\n";
    }

	virtual void ability(Entity* user, Entity* target) = 0;

    /** Various getters below. */
	int getMaxHealth(){
        return maxHealth;
    }

	int getPAtk() {
		return physAtk;
	}

	int getPDef() {
		return physDef;
	}

	int getMAtk() {
		return magAtk;
	}

	int getMDef() {
		return magDef;
	}

	int getSpeed() {
		return speed;
	}

    bool isSelfUse(){
        return selfUse;
    }

    bool isConsumable(){
        return consumable;
    }

	std::string getName() {
		return name;
	}

	std::string getDescription() {
		return description;
	}

    std::string getAbilityName(){
        return abilityName;
    }

    std::string getAbilityDescription(){
        return abilityDescription;
    }

    /** These methods handle the ability's cooldown, if it has one. */
    bool abilityAvailable(){
        return cooldown <= 0;
    }

    int getCooldown(){
        return cooldown;
    }

    int getMaxCooldown(){
        return maxCooldown;
    }

    void updateCooldown(){
        if (cooldown > 0) --cooldown;
    }
};


class DullBlade : public Item {
public:
	DullBlade() {
		name = "Dull Blade";
		description = "An ordinary-looking longsword. Scratches along its surface indicate it's passed through the hands of many a traveler.";
        abilityName = "Swift Cut";
        abilityDescription = "A powerful slash using the blade of the sword. Hits more through blunt force than anything. Deals 120% PAtk physical damage.";
		physAtk = 10;
        value = 100;
	}

	void ability(Entity* user, Entity* target) {
		std::cout << "You slash at " << target->getName() << " with the blade, dealing " 
        << target->dealPDamage(user->getPAtk() * 1.2) << " physical damage.\n";
	}
};

class WindRazor : public Item {
public:
    WindRazor() {
        name = "Windrazor";
        description = "This long, curved katana feels incredibly light in your hands. It hums, giving off an aura of magical power. "
        "Gripping it, you can feel the winds pick up at your feet, your steps becoming lighter.";
        abilityName = "Air Rend";
        abilityDescription = "This magical blade commands the power of air. Swinging it produces blades of wind, delivering devastating "
        "slashing attacks from range. Deals 120% PAtk + 20% speed physical damage.";
        physAtk = 10;
        speed = 10;
        value = 300;
    }

    void ability(Entity* user, Entity* target) {
		std::cout << "Channeling its power, you slash at " << target->getName() << " with the " << name << ". "
        << "The very air splits where you cut it, sending several sharp blades of air towards your target. They deal " 
        << target->dealPDamage(user->getPAtk() * 1.2 + user->getSpeed() * 0.2) << " physical damage.\n";
	}
};

class StickWand : public Item {
public:
	StickWand() {
		name = "Ordinary Wand";
		description = "Some say this wand was once touched by a grand sorcerer, who blessed it with magical affinity...or maybe it's just a stick from the pile of kindling he used...";
		abilityName = "Magic Missile";
        abilityDescription = "A bolt of concentrated magic fired from the tip of the wand. Deals 120% MATK magic damage.";
		magAtk = 10;
        consumable = false;
        value = 100;
	}

	void ability(Entity* user, Entity* target) {
		std::cout << "You wave your " << name << " while summoning the traces of magical energy within, dealing " 
        << target->dealMDamage(user->getMAtk() * 1.2) << " damage.\n";
	}
};

class BasicPotion : public Item {
protected: 
	int healstrength;

public:
	BasicPotion() {
		name = "Healing Potion";
		description = "A concoction of herbs and magical essence. It carries the soothing scent of medicinal plants. Surely, this would heal a couple of scratches.";
		abilityName = "Drink";
        abilityDescription = "Drink the potion to heal yourself a little.";
        healstrength = 50;
        consumable = true;
        selfUse = true;
        value = 30;
	}

	void ability(Entity* user, Entity* target) {
		std::cout << "You pop off the cap and down the potion, restoring " << healstrength << " health. It tastes faintly of cherries.\n";
        user->heal(healstrength);
	}
};

class Swifties : public Item {
public:
	Swifties() {
		name = "Boots of Swiftness";
		description = "A pair of lightweight boots fashioned from fine leather. Putting these on, you feel as nimble as lightning.\n";
        abilityName = "Slap";
        abilityDescription = "These shoes don't have a special ability, but you can certainly hit things with them.";
		speed = 20;
        consumable = false;
        value = 300;
	}
	
	void ability(Entity* user, Entity* target) {
        std::cout << "You pull the shoes off your feet and slap " << target->getName() << ". "
                  << "It does " << target->dealPDamage(user->getPAtk() * 0.5) << " damage.\n";
	}
};

class FlareOrb : public Item {
public:
    FlareOrb() {
        name = "Flare Orb";
		description = "This smooth red orb is warm to the touch and fits in your palm. You can feel a deep magical energy swirling underneath its surface.";
        abilityName = "Blast";
        abilityDescription = "Channel the magical energy in the orb to blast the enemy with magical fire. "
        "Randomly deals magical damage equal to somewhere between 200% and 300% of your MAtk. 3 turn cooldown.";
		maxHealth = 30;
        magAtk = 10;
        magDef = 5;
        maxCooldown = 3;
        value = 500;
    }

    void ability(Entity* user, Entity* target) {
        int modifier = rand() % 100 + 1;
        int damage = user->getMAtk() * (200 + modifier) / 100;
        std::cout << "You hold the orb out towards " << target->getName() << " and focus your mind. A magical fire engulfs your arm "
                  << "briefly before billowing outwards towards your target. It deals " << target->dealMDamage(damage) << " magical damage.\n";
        cooldown = maxCooldown;
    }
};

class RedPaw : public Item {
public:
    RedPaw() {
        name = "Mort's Red Paw";
        description = "It's a... severed cat paw with red fur? Didn't Mort have black fur? What the hell is going on?";
        abilityName = "Squeak";
        abilityDescription = "It makes a squeaking noise when you squeeze it.";
        physAtk = 20;
        physDef = 10;
        value = 400;
    }

    void ability(Entity* user, Entity* target) {
        std::cout << "You press on the red cat's paw. It squeaks.\n";
    }
};

class BluePaw : public Item {
public:
    BluePaw() {
        name = "Mort's Blue Paw";
        description = "It's a... severed cat paw with blue fur? Didn't Mort have black fur? What the hell is going on?";
        abilityName = "Squeak";
        abilityDescription = "It makes a squeaking noise when you squeeze it.";
        magAtk = 20;
        magDef = 10;
        value = 400;
    }

    void ability(Entity* user, Entity* target) {
        std::cout << "You press on the blue cat's paw. It squeaks.\n";
    }
};

class GoldPaw : public Item {
public:
    GoldPaw() {
        name = "Mort's Gold Paw";
        description = "It's a severed cat paw with golden fur. Upon closer inspection, it appears to change color when you hold it up to the light.";
        abilityName = "Smite";
        abilityDescription = "You can channel energy into the paw to smite a target, damaging it for 80% MAtk and healing yourself for 6% max health.";
        speed = 10;
        magAtk = 5;
        value = 1000;
    }

    void ability(Entity* user, Entity* target) {
        std::cout << "You press on the gold cat's paw. It squeaks. All of a sudden, a golden ray of light blasts out towards "
                  << target->getName() << ", dealing " << target->dealMDamage(user->getMAtk() * 0.8) << " magic damage. Additionally, "
                  << "the paw bathes you in warm golden sparkles, curing some of your wounds and restoring " << (int)round(user->getMaxHealth() * 0.06) << " health.\n";
        user->heal((int)round(user->getMaxHealth() * 0.06));
    }
};

class MirrorKnife : public Item {
private:
    bool sheathed;
    unsigned damage;
public:
    MirrorKnife() {
        name = "Mirror's Edge";
        description = "It's a gorgeous dagger with an elegant sheath. You can see your face in its reflection.\nYou'd... rather not explain to anyone how you got this.";
        abilityName = "Unsheathe";
        abilityDescription = "Draw the blade from its scabbard.";
        magAtk = 22;
        physAtk = 0;
        speed = 11;
        value = 1331;
        selfUse = false;
        consumable = false;
        sheathed = true;
        damage = 0;
    }

    void ability(Entity* user, Entity* target) {
        if (sheathed) {
            std::cout << "You unsheathe the knife. It makes a sound like resonating crystal.\n";
            sheathed = false;
            abilityName = "Wield";
            abilityDescription = "Throw the blade, or return it to its home.";
            physAtk = 22;
            magAtk = 0;
        }
        else {
            InputReader read;
            int choices[2] = {1, 2};
            std::cout << "You grip the Mirror's Edge in your hand. It feels ";
            if (damage == 0) { std::cout << "cold.\n"; }
            else if (damage <= 100) { std::cout << "warm.\n"; }
            else { std::cout << "hot.\n"; }
            std::cout << "1.\tThrow the blade\n"
                      << "2.\tSheathe the blade\n";
            int select = read.readInput(choices,2);
            if (select == 1) {
                int dmg = target->dealPDamage(user->getPAtk());
                std::cout << "You twirl the knife in your hand and hurl it at " << target->getName()
                          << ", dealing " << dmg << " physical damage.\n"
                          << "You see it hit, and yet the knife stays in your hand.\n";
                damage += dmg;
            }
            else {
                std::cout << ".latsyrc gnitanoser ekil dnuos a sekam tI .efink eht ehtaehs uoY\n"
                          << ".dloc sleef niaga edalb ehT\n";
                user->heal(damage);
                damage = 0;
                sheathed = true;
                magAtk = 22;
                physAtk = 0;
                abilityName = "Unsheathe";
                abilityDescription = "Draw the blade from its scabbard.";
            }
        }
    }
};
#endif
