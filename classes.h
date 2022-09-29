#include <iostream>

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
	protected: int path[32][2]; int lenOfPath;

	public: void makeStep();
		void calculatePath();
		void showPath();
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

		void makeStep();
                void calculatePath(int, int, int);
		void showPath();
};

class Trap: public Entity, public Defence, public Damager {
	protected: int type; bool armed;

	public: int getType() { return type; }
		void setType(int tV) { type = tV; }

		bool getArm() { return armed; }
		void setArm(bool aV) { armed = aV; }

		void setup(int idV, int xV, int yV, int tV, int dmgV, bool aV) {
			id = idV; x = xV; y = yV; type = tV; dmg = dmgV; armed = aV;
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
			if((x-xV)*(x-xV) + (y-yV)*(y-yV) <= range*range) {
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

void Enemy::makeStep() {
	x = path[lenOfPath-1][0]; y = path[lenOfPath-1][1]; lenOfPath--;
};

void Enemy::calculatePath(int d, int tX, int tY) {
	int fieldCopy[9][9]; int len = 0; int temp = 0; bool step;

	for(int i = 0; i < d; i++) {
		for(int j = 0; j < d; j++) {
			fieldCopy[i][j] = -1;
		}
	}

	for(int i = 0; i < d; i++) {
		fieldCopy[1][i] = 0;
		fieldCopy[7][i] = 0;

		fieldCopy[i][1] = 0;
		fieldCopy[i][7] = 0;
	}

	for(int i = 3; i <= 5; i++) {
		for(int j = 3; j <= 5; j++) {
			fieldCopy[i][j] = 0;
		}
	}

	fieldCopy[5][2] = 0;
	fieldCopy[3][6] = 0;

	fieldCopy[0][0] = 0;
	fieldCopy[0][8] = 0;
	fieldCopy[8][0] = 0;
	fieldCopy[8][8] = 0;

	fieldCopy[y-1][x-1] = temp+1;

	while(1) {
		if(fieldCopy[tY][tX]) {
			break;
		}

		temp++;

		for(int i = 0; i < d; i++) {
			for(int j = 0; j < d; j++) {
				if(fieldCopy[i][j] == temp) {
					if(fieldCopy[i-1][j]) { fieldCopy[i-1][j] = temp + 1; }
					if(fieldCopy[i+1][j]) { fieldCopy[i+1][j] = temp + 1; }
					if(fieldCopy[i][j-1]) { fieldCopy[i][j-1] = temp + 1; }
					if(fieldCopy[i][j+1]) { fieldCopy[i][j+1] = temp + 1; }
				}
			}
		}
	}

	len = temp; temp++;

	for(int k = 0; k < len; k++) {
		step = false;

		for(int i = 0; i < d; i++) {
			for(int j = 0; j < d; j++) {
				if(fieldCopy[i][j] == temp) {
					path[k][0] = i + 1; path[k][1] = j + 1;
					step = true; temp--;
					break;
				}
			}

			if(step) { break; }
		}
	}
};
