#include <iostream>

// GENERALIZED CLASS (ABSTRACT)

class Object {
	private: int id; int type;

	public: int getId() { return id; }
                void setId(int val) { id = val; }

		int getType() {return type; }
                void setType(int val) { type = val; }
};


// MAIN CLASS (ABSTRACT)

class Entity: public Object {
	private: int x, y;

	public: int getX() { return x; }
                int getY() { return y; }
		void setCoords(int newX, int newY) { x = newX; y = newY; }
};

// PROPERTIES CLASSES (ABSTRACT)

class Damager {
	private: float dmg;

	public:	float getDmg() { return dmg; }
		void setDmg(float val) { dmg = val; }
};

class Mortal {
	private: float hp;

	public:	float getHp() { return hp; }
		void setHp(float val) { hp = val; }
};

class Walker {
	private: int path[128][2], int lenOfPath;

	public: void getPath();
		void calculatePath();
};

class Shooter {
        private: float range;

        public:	float getRange() { return range; }
		void setRange(float val) { range = val; }
};

class Defence {
	public: trigger();
}

// ACTORS CLASSES

class Enemy: public Entity, public Mortal, public Walker, public Damager {
	public:
		Enemy(int idV, int xV, int yV, int tV, float hpV, int lenV, float dmgV) {
			id = idV; x = xV; y = yV; type = tV; hp = hpV; lenOfPath = lenV; dmg = dmgV;
		}
};

class Trap: public Entity, public Defence, public Damager {
	public:
		Trap(int idV, int xV, int yV, int tV, float dmgV) {
			id = idV; x = xV; y = yV; type = tV; dmg = dmgV;
		}

		int trigger(int xV; int yV) {
			if(x == xV && y == yV) {
				return dmg;
			} else {
				return 0;
			}
		}
};

class Ranger: public Entity, public Defence, public Damager, public Shooter {
	public:
		Ranger(int idV, int xV, int yV, int tV, float dmgV, float rangeV) {
			id = idV; x = xV; y = yV; type = tV; dmg = dmgV; range = rangeV;
		}

		int trigger(int xV, int yY) {
			if(sqrt(sqr(x-xV) + sqr(y-yV)) < 1.5) {
				return dmg;
			} else {
				return 0;
			}
		}
};

class Structure: public Entity, public Defence, public Mortal {
	public:
		Structure(int idV, int xV, int yV, int tV, int hpV) {
			id = idV; x = xV; y = yV; type = tV; hp = hpV;
		}

		int trigger(int xV, int xY) {
			if(x == xV && y = yV) {
				return 1;
			} else {
				return 0;
			}
		}
};

class Tile: public Object {
	private: bool occupied;

	public:
		Tile(int idV, int tV, int oV) {
			id = idV; type = tV; occupied = oV;
		}

		bool getOcc() { return occupied; }
		void setOcc(bool val) { occupied = val; }
};
