#include <iostream>
#include <math.h>

// SUPERCLASS (ABSTRACT)

class Object {
	protected: int id;

	public: int getId() { return id; }
                void setId(int val) { id = val; }
};


// GENERALIZED CLASS (ABSTRACT)

class Entity: public Object {
	protected: int x, y;

	public: int getX() { return x; }
                int getY() { return y; }
		void setCoords(int newX, int newY) { x = newX; y = newY; }
};

// PROPERTIES CLASSES (ABSTRACT)

class Damager {
	protected: float dmg;

	public:	float getDmg() { return dmg; }
		void setDmg(float val) { dmg = val; }
};

class Mortal {
	protected: float hp;

	public:	float getHp() { return hp; }
		void setHp(float val) { hp = val; }
};

class Walker {
	protected: int path[64][2]; int lenOfPath;

	public: void makeStep();
		void calculatePath();
};

class Shooter {
        protected: float range; int bullets;

        public:	float getRange() { return range; }
		void setRange(float val) { range = val; }
		int getBullets() { return bullets; }
		void setBullets(int bV) { bullets = bV; }

		bool checkBullets() {
			if(bullets) {
				return true;
			} else {
				return false;
			}
		}
};

class Defence {
	public: int trigger();
};

// ACTORS CLASSES

class Enemy: public Entity, public Mortal, public Walker, public Damager {
	protected: float reward;

	public: float getReward() { return reward; }
		void setReward(float rV) { reward = rV; }

		void setup(int idV, int xV, int yV, int hpV, int dmgV, int rV) {
			id = idV; x = xV; y = yV; hp = hpV; dmg = dmgV; reward = rV;
		}
};

class Trap: public Entity, public Defence, public Damager {
	protected: int type;

	public: int getType() { return type; }
		int setType(int tV) { type = tV; }

		void setup(int idV, int xV, int yV, int tV, int dmgV) {
			id = idV; x = xV; y = yV; type = tV; dmg = dmgV;
		}

		int trigger(int xV, int yV) {
			if(x == xV && y == yV) {
				return dmg;
			} else {
				return 0;
			}
		}
};

class Ranger: public Entity, public Defence, public Damager, public Shooter {
	public:
		void setup(int idV, int xV, int yV, int dmgV, int rV, int bV) {
			id = idV; x = xV; y = yV; dmg = dmgV; range = rV; bullets = bV;
		}

		int trigger(int xV, int yV) {
			if(sqrt(pow(x-xV, 2) + pow(y-yV, 2)) < range) {
				return dmg;
			} else {
				return 0;
			}
		}
};

class Structure: public Entity, public Defence, public Mortal {
	public:
		int trigger(int xV, int yV) {
			if(x == xV && y == yV) {
				return 1;
			} else {
				return 0;
			}
		}
};

class Tile: public Object {
	private: bool occupied;
		 bool trail;

	public: bool getOcc() { return occupied; }
		void setOcc(bool val) { occupied = val; }

		bool getT() { return trail; }
		void setT(bool tV) { trail = tV; }
};

// SOME DECLARATIONS

void Walker::makeStep() {
	x = path[lenOfPath-1][0]; y = path[lenOfPath-1][1]; lenOfPath--;
};

void Walker::calculatePath() {};
