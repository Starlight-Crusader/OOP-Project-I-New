#include <iostream>
#include <string>

// MAIN CLASS

class Object {
	private: int id; int x, y; int type;

	public: int getId() { return id; }
		void setId(int val) { id = val; }

		int getX() { return x; }
                int getY() { return y; }
		void setCoords(int newX, int newY) { x = newX; y = newY; }

		int getType() {return type; }
		void setType(int val) { type = val; }
};

// PROPERTIES CLASSES

class Damager {
	private: float dmg;

	public:	float getDmg() { return dmg; }
		void setDmg(float val) { dmg = val; }
};

class Creature: protected Object {
	private: int hp;

	public:	float getHp() { return hp; }
		void setHp(float val) { hp = val; }
};

class Walker: {
	private: int path[128][2];

	public: void getPath();
		void calculatePath();
};

class Shooter {
        private: float range;

        public:	float getRange() { return range; }
		void setRange(float val) { range = val; }
};

// ACTORS CLASSES

class Enemy: protected Entity, protected Creature, protected Walker, protected Damager {
	public: Enemy();
};

class Trap: protected Entity, protected Damager {
	public:	Trap();
		void trigger();
};

class Ranger: protected Damager, protected Shooter, protected Entity {
	public: Ranger();
		void shoot();
};
