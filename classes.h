#include <iostream>
#include <unistd.h>

using namespace std;

// SUPERCLASS (ABSTRACT)

class Object {
	protected:
		int id;

	public:
		int getId() { return id; }
            	void setId(int val) { id = val; }
};

// GENERALIZED CLASS (ABSTRACT)

class Entity: public Object {
	protected:
		int x, y;

	public:
		int getX() { return x; }
            	int getY() { return y; }
		void setCoords(int newX, int newY) { x = newX; y = newY; }
};

// PROPERTIES CLASSES (ABSTRACT)

class Damager {
	protected:
		float dmg;

	public:
		float getDmg() { return dmg; }
		void setDmg(float val) { dmg = val; }
};

class Mortal {
	protected:
		float hp;

	public:
		float getHp() { return hp; }
		void setHp(float val) { hp = val; }
};

class Walker {
	protected:
		int path[32][2]; int lenOfPath;

	public:
		void makeStep();
		void calculatePath();
		void displayPath();

		int getLen() { return lenOfPath; }
		void setLen(int lV) { lenOfPath = lV; }
};

class Shooter {
	protected:
		float range; int bullets;

    	public:
		float getRange() { return range; }
		void setRange(float val) { range = val; }
		int getBullets() { return bullets; }
		void setBullets(int bV) { bullets = bV; }

		bool checkBullets() {
			if(bullets > 0) {
				return true;
			} else {
				return false;
			}
		}
};

class Defender {
	public:
		int trigger();
};

// ACTORS CLASSES

class Enemy: public Entity, public Mortal, public Walker, public Damager {
	protected:
		float reward;

	public:
		float getReward() { return reward; }
		void setReward(float rV) { reward = rV; }

		void setup(int idV, int xV, int yV, int hpV, int dmgV, float rV) {
			id = idV; x = xV; y = yV; hp = hpV; dmg = dmgV; reward = rV;
		}

		void makeStep();
                void calculatePath(int, int, int);
		void displayPath(int, int, int);
};

class Trap: public Entity, public Defender, public Damager {
	protected:
		int type;
		bool armed;

	public:
		int getType() { return type; }
		void setType(int tV) { type = tV; }

		bool getArm() { return armed; }
		void setArm(bool aV) { armed = aV; }

		void setup(int idV, int xV, int yV, int tV, int dmgV, bool aV) {
			id = idV; x = xV; y = yV; type = tV; dmg = dmgV; armed = aV;
		}

		int trigger(int xV, int yV) {
			if(type == 0) {
				if(xV == x && yV == y) {
					return 1;
				} else { return 0; }
			} else if(type == 1) {
				if((x-xV)*(x-xV) + (y-yV)*(y-yV) <= 1.5*1.5) {
					return 1;
				} else { return 0; }
			}
		}
};

class Ranger: public Entity, public Defender, public Damager, public Shooter {
	public:
		void setup(int idV, int xV, int yV, int dmgV, int rV, int bV) {
			id = idV; x = xV; y = yV; dmg = dmgV; range = rV; bullets = bV;
		}

		int trigger(int xV, int yV) {
			if((x-xV)*(x-xV) + (y-yV)*(y-yV) <= range*range) {
				return 1;
			} else {
				return 0;
			}
		}
};

class Structure: public Entity, public Defender, public Mortal {
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
	private:
		bool occupied;
		bool trail;

	public:
		bool getOcc() { return occupied; }
		void setOcc(bool val) { occupied = val; }

		bool getT() { return trail; }
		void setT(bool tV) { trail = tV; }
};

// SOME DECLARATIONS

void Enemy::makeStep() {
	x = path[lenOfPath-1][1]; y = path[lenOfPath-1][0]; lenOfPath--;
};

void Enemy::calculatePath(int d, int tX, int tY) {
	unsigned int second = 1000000;
	int fieldCopy[9][9]; int cX; int cY;
	lenOfPath = 0;

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

	fieldCopy[y-1][x-1] = 1;

	while(1) {
		if(fieldCopy[tY][tX]) {
			break;
		}

		lenOfPath++;

		for(int i = 0; i < d; i++) {
			for(int j = 0; j < d; j++) {
				if(fieldCopy[i][j] == lenOfPath) {
					if(!fieldCopy[i-1][j] && i > 0) { fieldCopy[i-1][j] = lenOfPath + 1; }
					if(!fieldCopy[i+1][j] && i < 8) { fieldCopy[i+1][j] = lenOfPath + 1; }
					if(!fieldCopy[i][j-1] && j > 0) { fieldCopy[i][j-1] = lenOfPath + 1; }
					if(!fieldCopy[i][j+1] && j < 8) { fieldCopy[i][j+1] = lenOfPath + 1; }
				}
			}
		}
	}

	path[0][1] = tX-1; path[0][0] = tY-1;

	for(int i = 1; i < lenOfPath; i++) {
		cX = path[i-1][1]; cY = path[i-1][0];

		if(fieldCopy[cY-1][cX] == fieldCopy[cY][cX]-1) {
			path[i][0] = cY-1; path[i][1] = cX;

		} else if(fieldCopy[cY+1][cX] == fieldCopy[cY][cX]-1) {
			path[i][0] = cY+1; path[i][1] = cX;

		} else if(fieldCopy[cY][cX-1] == fieldCopy[cY][cX]-1) {
			path[i][0] = cY; path[i][1] = cX-1;

		} else if(fieldCopy[cY][cX+1] == fieldCopy[cY][cX]-1){
			path[i][0] = cY; path[i][1] = cX+1;
		}
	}

	for(int i = 0; i < lenOfPath; i++) {
		path[i][0]++; path[i][1]++;
	}
};

void Enemy::displayPath(int d, int xE, int yE) {
	bool part;

	for(int i = 0; i < d; i++) {
		for(int a = 0; a < 14; a++) {
                	cout << ' ';
                }

		for(int j = 0; j < d; j++) {
			part = false;

			for(int k = 0; k < lenOfPath; k++) {
				if(i == path[k][0]-1 && j == path[k][1]-1) {
					part = true;
					break;
				}
			}

			if(i == yE-1 && j == xE-1) {
				cout << "\u001b[31mw\u001b[0m";
			} else if(part) {
				cout << "\u001b[31m~\u001b[0m";
			} else {
				cout << "\u001b[33m~\u001b[0m";
			}
		}

		cout << '\n';
	}
};
