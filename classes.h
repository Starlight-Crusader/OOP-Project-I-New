// RESET: \u001b[0m
// RED: \u001b[31m
// GREEN: \u001b[32m
// YELLOW: \u001b[33m
// BLUE: \u001b[34m
// GREY: \u001b[37m


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "generator.h"

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
		int path[64][2]; int lenOfPath;

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
        void calculatePath(int, int, int, int, int, int*);
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

class Bait: public Entity, public Defender, public Mortal {
	public:
		void setup(int idV, int xV, int yV, int hpV) {
            id = idV; x = xV; y = yV; hp = hpV;
        }

		int trigger(int xV, int yV) {
			if(xV == x && yV == y) {
				return 1;
			} else {
				return 0;
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

void Enemy::calculatePath(int d, int tX, int tY, int fX, int fY, int *fieldSchema) {
	unsigned int second = 1000000;
	int fieldCopy[16][16]; int cX; int cY;
	lenOfPath = 0;
	
	for(int i = 0; i < d; i++) {
		for(int j = 0; j < d; j++) {
			fieldCopy[i][j] = *(fieldSchema+i*d+j);
		}
	}
	
	fieldCopy[y-1][x-1] = 1;

	while(1) {
		if(fieldCopy[tY-1][tX-1] > 0) {
			break;
		}

		lenOfPath++;

		for(int i = 0; i < d; i++) {
			for(int j = 0; j < d; j++) {
				if(fieldCopy[i][j] == lenOfPath) {
					if(i > 0) { if(fieldCopy[i-1][j] == 0) { fieldCopy[i-1][j] = lenOfPath + 1; } }	
					if(i < d-1) { if(fieldCopy[i+1][j] == 0) { fieldCopy[i+1][j] = lenOfPath + 1; } }
					if(j > 0) { if(fieldCopy[i][j-1] == 0) { fieldCopy[i][j-1] = lenOfPath + 1; } }
					if(j < d-1) { if(fieldCopy[i][j+1] == 0) { fieldCopy[i][j+1] = lenOfPath + 1; } } 			
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

// ARCHITECTURE COMPONENTS

// CONTROLLER

class Logic: public Object {
    public:
        Logic(int idV) {
            id = idV;
        }

        void addEnemies(Data*);

		void enemiesMove(Data*);
		void defendersMove(Data*);
		void checkFinish(Data*);

        void setupSpawns(Data*);
        void constructSchema(Data*);
		void calculatePaths(Data*);

        bool checkGO(Data *model) {
			if(model->target.getHp() <= 0.0f) {
				return true;
			} else {
				return false;
			}
		}

        char fieldStateSchema[16*16];
};

// BASIC ACTIONS AND INTERACTIONS OF ACTORS

void Logic::enemiesMove(Data *model) {
	bool delayed, devoured;

    // MOVE ENEMIES

	for(int i = 0; i < model->nE; i++) {
		delayed = false;

		for(int j = 0; j < model->nB; j++) {
			if(model->baits[j].trigger(model->enemies[i].getX(), model->enemies[i].getY())) {
				delayed = true;

				model->baits[j].setHp(model->baits[j].getHp()-model->enemies[i].getDmg());
			} else {
				continue;
			}
		}

		if(!delayed) {
			model->enemies[i].makeStep();
        } else { 
            continue; 
        }
	}

    // DESTROY DEVOURED BAITS

	devoured = true;

	while(devoured) {
        devoured = false;

        for(int i = 0; i < model->nB; i++) {
            if(model->baits[i].getHp() <= 0.1f) {
                model->field[(model->baits[i].getY()-1)*model->dim+(model->baits[i].getX()-1)].setOcc(false);

                for(int k = i; k < model->nB-1; k++) {
                    model->baits[k].setup(model->baits[k+1].getId(),
                        model->baits[k+1].getX(), model->baits[k+1].getY(),
                        model->baits[k+1].getHp());
                }

                model->nB--; devoured = true; 
                break;
            } else { continue; }
        }
    }
};

void Logic::defendersMove(Data *model) {
	bool killed, destroyed;
	unsigned int second = 1000000;
	int c = 0;

    // CHECK TRAPS

	for(int i = 0; i < model->nT; i++) {
		if(!model->traps[i].getArm()) { continue; }

		switch(model->traps[i].getType()) {
			case 0:
				for(int j = 0; j < model->nE; j++) {
					if(model->traps[i].trigger(model->enemies[j].getX(), model->enemies[j].getY())) {
						model->enemies[j].setHp(model->enemies[j].getHp()-model->traps[i].getDmg());
						model->traps[i].setArm(false);
                    }
				}

				killed = true;

				while(killed) {
					killed = false;

					for(int j = 0; j < model->nE; j++) {
						if(model->enemies[j].getHp() <= 0) {
							model->setM(model->getM()+model->enemies[j].getReward());
                            model->setK(model->getK()+1);

                            for(int k = j; k < model->nE-1; k++) {
                                model->enemies[k].setup(model->enemies[k+1].getId(),
                                    model->enemies[k+1].getX(), model->enemies[k+1].getY(),
                                    model->enemies[k+1].getHp(), model->enemies[k+1].getDmg(),
                                    model->enemies[k+1].getReward());

                                model->enemies[k].calculatePath(model->dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
                            }

                            model->nE--;
							killed = true;

							break;
						} else { continue; }
					}
				}

				break;
			case 1:
				for(int j = 0; j < model->nE; j++) {
					c += model->traps[i].trigger(model->enemies[j].getX(), model->enemies[j].getY());
				}

				if(c >= 2) {
					for(int j = 0; j < model->nE; j++) {
						if(model->traps[i].trigger(model->enemies[j].getX(), model->enemies[j].getY())) {
                            model->enemies[j].setHp(model->enemies[j].getHp()-model->traps[i].getDmg());
                            model->traps[i].setArm(false);
                        }
					}

					killed = true;

                    while(killed) {
                        killed = false;

                        for(int j = 0; j < model->nE; j++) {
                            if(model->enemies[j].getHp() <= 0) {
                                model->setM(model->getM()+model->enemies[j].getReward());
                                model->setK(model->getK()+1);

                                for(int k = j; k < model->nE-1; k++) {
                                    model->enemies[k].setup(model->enemies[k+1].getId(),
                                        model->enemies[k+1].getX(), model->enemies[k+1].getY(),
                                        model->enemies[k+1].getHp(), model->enemies[k+1].getDmg(),
                                        model->enemies[k+1].getReward());

                                    model->enemies[k].calculatePath(model->dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
                                }

                                model->nE--;
							    killed = true;

							    break;
                            } else { continue; }
                        }
                    }
				}

			    break;
		}
	}

	// CHECK RANGERS

	for(int i = 0; i < model->nR; i++) {
		for(int j = 0; j < model->nE; j++) {
			if(!model->rangers[i].checkBullets()) {
				break;
			} else if(model->rangers[i].trigger(model->enemies[j].getX(), model->enemies[j].getY())) {
				model->enemies[j].setHp(model->enemies[j].getHp()-model->rangers[i].getDmg());
				model->rangers[i].setBullets(model->rangers[i].getBullets()-1);

				if(model->enemies[j].getHp() <= 0) {
                     model->setM(model->getM()+model->enemies[j].getReward());
                    model->setK(model->getK()+1);

                    for(int k = j; k < model->nE-1; k++) {
                        model->enemies[k].setup(model->enemies[k+1].getId(),
                            model->enemies[k+1].getX(), model->enemies[k+1].getY(),
                        	model->enemies[k+1].getHp(), model->enemies[k+1].getDmg(),
                        	model->enemies[k+1].getReward());

						model->enemies[k].calculatePath(model->dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
					}

                    model->nE--;
                }

				break;
			} else {
				continue;
			}
		}
	}

	// DESTROY DISARMED TRAPS & EMPTY RANGERS

	destroyed = true;

	while(destroyed) {
		destroyed = false;

		for(int i = 0; i < model->nT; i++) {
			if(!model->traps[i].getArm()) {
				model->field[(model->traps[i].getY()-1)*model->dim+(model->traps[i].getX()-1)].setOcc(false);

				for(int k = i; k < model->nT-1; k++) {
                    model->traps[k].setup(model->traps[k+1].getId(),
                        model->traps[k+1].getX(), model->traps[k+1].getY(),
                        model->traps[k+1].getType(), model->traps[k+1].getDmg(),
						model->traps[k+1].getArm());
                }

				model->nT--;
				destroyed = true; 
                
                break;
			} else { continue; }
		}
	}

	destroyed = true;

	while(destroyed) {
        destroyed = false;

        for(int i = 0; i < model->nR; i++) {
            if(!model->rangers[i].checkBullets()) {
				model->field[(model->rangers[i].getY()-1)*model->dim+(model->rangers[i].getX()-1)].setOcc(false);

                for(int k = i; k < model->nR; k++) {
                    model->rangers[k].setup(model->rangers[k+1].getId(),
                        model->rangers[k+1].getX(), model->rangers[k+1].getY(),
                        model->rangers[k+1].getDmg(), model->rangers[k+1].getRange(),
                        model->rangers[k+1].getBullets());
                }

				model->nR--;
                destroyed = true; 
                
                break;
            } else { continue; }
        }
    }
};

void Logic::checkFinish(Data *model) {
	float damage = 0.0f;
	bool finished = true;

	while(finished) {
		finished = false;

		for(int i = 0; i < model->nE; i++) {
            if(model->enemies[i].getX() == model->target.getX() && model->enemies[i].getY() == model->target.getY()) {
				damage += model->enemies[i].getDmg();

                for(int k = i; k < model->nE-1; k++) {
                    model->enemies[k].setup(model->enemies[k+1].getId(),
                        model->enemies[k+1].getX(), model->enemies[k+1].getY(),
                        model->enemies[k+1].getHp(), model->enemies[k+1].getDmg(),
                        model->enemies[k+1].getReward());

					model->enemies[k].calculatePath(model->dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
                }

				model->nE--;
                finished = true; 
                
                break;
            } else { continue; }
        }
	}

	model->target.setHp(model->target.getHp()-damage);
};

// PATHS HANDLING

void Logic::calculatePaths(Data *model) {
	for(int i = 0; i < model->nE; i++) {
		model->enemies[i].calculatePath(model->dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
	}
};

// SOME ADDITIONAL FUNCTIONALITY

void Logic::constructSchema(Data *model) {
    bool conv;

	for(int i = 0; i < model->dim; i++) {
		for(int j = 0; j < model->dim; j++) {
            conv = false;
		
            for(int k = 0; k < model->nE; k++) {
				if(model->enemies[k].getY()-1 == i && model->enemies[k].getX()-1 == j) {
					fieldStateSchema[i*model->dim+j] = 'w';

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < model->nT; k++) {
				if(model->traps[k].getY()-1 == i && model->traps[k].getX()-1 == j) {
					if(!model->traps[k].getType()) {
						fieldStateSchema[i*model->dim+j] = '@';
					} else {
						fieldStateSchema[i*model->dim+j] = '#';
					}

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < model->nR; k++) {
				if(model->rangers[k].getY()-1 == i && model->rangers[k].getX()-1 == j) {
					fieldStateSchema[i*model->dim+j] = 'R';

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < model->nB; k++) {
				if(model->baits[k].getY()-1 == i && model->baits[k].getX()-1 == j) {
					fieldStateSchema[i*model->dim+j] = '.';

					conv = true;
                    break;
				} else { continue; }
			}

			if(conv) { continue; }

			if(model->fence.getY()-1 == i && model->fence.getX()-1 == j) {
				fieldStateSchema[i*model->dim+j] = ':';

                conv = true;
			}

			if(conv) { continue; }

			if(model->target.getY()-1 == i && model->target.getX()-1 == j) {
                fieldStateSchema[i*model->dim+j] = 'H';
            } else if(model->field[i*model->dim+j].getT()) {
                fieldStateSchema[i*model->dim+j] = '~';
            } else {
                fieldStateSchema[i*model->dim+j] = 't';
            }
		}

		cout << '\n';
	}
};

void Logic::setupSpawns(Data *model) {
	int valid;
	int x, y;

    for(int i = 0; i < 4; i++) {
        valid = false;

        while(!valid) {
            switch(i) {
                case 0:
                    x = 1; y = rand() % model->dim + 1;
                    break;
                case 1:
                    x = model->dim; y = rand() % model->dim + 1;
                    break;
                case 2:
                    x = rand() % model->dim + 1; y = 1;
                    break;
                case 3:
                    x = rand() % model->dim + 1; y = model->dim;
                    break;
            }

		    if(model->field[(y-1)*model->dim+(x-1)].getT()) {
			    model->spawns[i][1] = x;
			    model->spawns[i][0] = y;

			    valid = true;
		    }
	    }
    }
};

void Logic::addEnemies(Data *model) {
	model->nE = 4;

	for(int i = 0; i < model->nE; i++) {
		model->enemies[i].setup(rand(), model->spawns[i][1], model->spawns[i][0], 4.0f, 1.0f, 0.1f);
	}
};

// VIEW

class Graphics: public Object {
    public:
        Visualisation(int idV) {
            id = idV;
        }

        void printPhase(int);
		void printStats(int, float, float);
		void printFinalStats(int, float, int);

        void drawState(char*, int);
        void displayPaths(Enemy*);

        void printOptionsMain();
		void printOptionsBuy();
		void printOptionsSell();
};


int Graphics::drawState(char *fieldStateSchema, int dim) {
	for(int k = 0; k < 11; k++) { cout << ' '; }
    for(int k = 1; k <= 16; k++) { cout << k % 10; }
    cout << '\n';

	for(int i = 0; i < dim; i++) {
		for(int k = 0; k < 9; k++) { cout << ' '; }
        cout << (i + 1) % 10 << ' ';

		for(int j = 0; j < dim; j++) {
            switch(*(fieldStateSchema+i*dim+j)) {
                case 't':
                    cout << "\u001b[32mt\u001b[0m"; break;
                case '~':
                    cout << "\u001b[33m~\u001b[0m"; break;
                case 'H':
                    cout << "\u001b[34mH\u001b[0m"; break;
                case '.':
                    cout << "\u001b[34m.\u001b[0m"; break;
                case 'R':
                    cout << "\u001b[34mR\u001b[0m"; break;
                case '#':
                    cout << "\u001b[34m#\u001b[0m"; break;
                case '@':
                    cout << "\u001b[34m@\u001b[0m"; break;
                case 'w':
                    cout << "\u001b[37mw\u001b[0m"; break;
                case ':':
                    cout << "\u001b[34m:\u001b[0m"; break;
            }
		}

		cout << '\n';
	}

	return 0;
};

void Graphics::printPhase(int phase) {
	if(!phase) {
       	cout << "\n=========== PLANNIG PHASE ===========\n";
    } else {
        cout << "\n============ ACTION PHASE ===========\n";
        cout << '\n';
    }
};

void Graphics::printStats(int round, float hp, float money) {
	cout << '\n';
    cout << "-------------------------------------\n";
    cout << "               Stats:\n";
	cout << " * Round: " << round << ",\n";
    cout << " * HP: " << hp << ",\n";
    cout << " * Money: " << money << "$\n";
    cout << "-------------------------------------\n";
    cout << '\n';
};

void Graphics::printFinalStats(int round, float hp, int kills) {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << "             Final stats:\n";
    cout << " * Rounds survived: " << round-1 << ",\n";
    cout << " * HP left: " << hp << ",\n";
    cout << " * Enemies killed: " << kills << '\n';
    cout << "-------------------------------------\n";
    cout << '\n';
};

void Graphics::displayPaths(Enemy *enemies) {
	unsigned int second = 1000000;

	for(int k = 0; k < nE; k++) {
		system("clear");
        cout << "\n            ENEMY: " << *(enemies+k).getId() << "\n\n";

		*(enemies+k).displayPath(dim, *(enemies+k).getX(), *(enemies+k).getY());

		usleep(4*second);
	}
};

void Graphics::printOptionsMain() {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << "          Select an action:\n";
    cout << " 0. Highlight the paths of the en-s,\n";
    cout << " 1. Place a new beartrap (0.05$),\n";
    cout << " 2. Place a new nest (0.1$),\n";
    cout << " 3. Hire a ranger (0.2$)\n";
    cout << " 4. Place a bait (free),\n";
    cout << " 5. Construct a fence (free),\n";
    cout << " 6. Sell a defender (50% back),\n";
    cout << " 7. Start the next round,\n";
    cout << " 8. Finish the game\n";
    cout << "-------------------------------------\n";
    cout << '\n';
    cout << "OPTION: ";
};

void Graphics::printOptionsBuy() {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << " Input coord-s of a new def. in the\n";
    cout << " form: 'x y' OR enter '0 0' in order\n";
    cout << " to abort this operation ...\n";
    cout << "-------------------------------------\n";
    cout << '\n';
    cout << "INPUT: ";
};

void Graphics::printOptionsSell() {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << " Input coord-s of a def. to sell in\n";
    cout << " the form: 'x y' OR enter '0 0' in\n";
    cout << " order to abort this operation ...\n";
    cout << "-------------------------------------\n";
    cout << '\n';
    cout << "INPUT: ";
};

// MODEL

class Data: public Object {
    private:
		// STATS

		int round;
		float money;
		int phase;
		bool abort;
		int kills;

        // PRICES

        float beartrap = 0.05f; 
        float nest = 0.1f; 
        float ranger = 0.2f;

        // OTHERS

        Generator fieldGenerator;

    public:
        int spawns[4][2];

        // ACTORS

        int dim = 16;
		Tile field[16*16]; 

		Enemy enemies[4]; 
        int nE = 0;

		Trap traps[16]; 
        int nT = 0;

		Ranger rangers[16]; 
        int nR = 0;

		Bait baits[16]; 
        int nB = 0;

		Structure target;
		Entity fence;

        int *fieldSchema = (int*)malloc(256*sizeof(int));

        Data(int idV) {
            id = idV; round = 0; phase = 0; abort = 0; kills = 0;

            fence.setCoords(999, 999);
        }

        void addTrap(int, int, int);
        void addRanger(int, int);
        void addBait(int, int);
        void addFence(int, int);

        int sellDefender(int, int);

		int getRound() { return round; }
		void setRound(int rV) { round = rV; }

		float getM() { return money; }
		void setM(float mV) { money = mV; }

		int getPhase() { return phase; }
		void setPhase(int pV) { phase = pV; }

		bool getAbort() { return abort; }
		void setAbort(int aV) { abort = aV; }

		int getK() { return kills; }
		void setK(int kV) { kills = kV; }

        float getBPrice() { return beartrap; }
		float getNPrice() { return nest; }
		float getRPrice() { return ranger; }

        void setupField();

        bool checkTrail(int xV, int yV) {
			if(field[(yV-1)*dim+(xV-1)].getT()) {
				return true;
			} else {
				return false;
			}
		}

		bool checkOcc(int xV, int yV) {
			if(field[(yV-1)*dim+(xV-1)].getOcc()) {
                return true;
            } else {
                return false;
            }
		}

        bool checkMoney(float);
};

void Data::setupField() {
	bool placed = false;
	int x, y;
    
	fieldGenerator.setupTiles();
	fieldGenerator.generateField();

	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			if(fieldGenerator.field[i][j] == -1) {
				field[i*dim+j].setOcc(false);
                field[i*dim+j].setT(false);
                field[i*dim+j].setId(rand());
			} else {
				field[i*dim+j].setOcc(false);
                field[i*dim+j].setT(true);
                field[i*dim+j].setId(rand());
			}
		}
	}

	while(!placed) {
		x = rand() % 8 + 5;
		y = rand() % 8 + 5;

		if(field[(y-1)*dim+(x-1)].getT()) {
			field[(y-1)*dim+(x-1)].setOcc(true);

			target.setId(rand());
        	target.setCoords(x, y);
        	target.setHp(10.0f);

			placed = true;
		} else { continue; }
	}

	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			*(fieldSchema+i*dim+j) = fieldGenerator.field[i][j];
		}
	}
};

void Data::addTrap(int tV, int xV, int yV) {
	switch(tV) {
		case 0:
			traps[nT].setup(rand(), xV, yV, 0, 2.0f, true); nT++;
            field[(yV-1)*dim+(xV-1)].setOcc(true);

			break;
        case 1:
			traps[nT].setup(rand(), xV, yV, 1, 4.0f, true); nT++;
            field[(yV-1)*dim+(xV-1)].setOcc(true);

			break;
	}
};

void Data::addRanger(int xV, int yV) {
	rangers[nR].setup(rand(), xV, yV, 4.0f, 1.5f, 2); nR++;
    field[(yV-1)*dim+(xV-1)].setOcc(true);
};

void Data::addBait(int xV, int yV) {
	baits[nB].setup(rand(), xV, yV, 1.0f); nB++;
    field[(yV-1)*dim+(xV-1)].setOcc(true);
};

void Data::addFence(int xV, int yV) {
	if(fence.getX() != 999 && fence.getY() != 999) {
		field[(fence.getY()-1)*dim+(fence.getX()-1)].setOcc(false);
        field[(fence.getY()-1)*dim+(fence.getX()-1)].setT(true);
	}

	fence.setCoords(xV, yV);

	field[(yV-1)*dim+(xV-1)].setOcc(true);
	field[(yV-1)*dim+(xV-1)].setT(false);
};

int Data::sellDefender(int xV, int yV) {
	int type;

	for(int i = 0; i < nT; i++) {
		if(traps[i].getX() == xV && traps[i].getY() == yV) {
			field[(yV-1)*dim+(xV-1)].setOcc(false);
			type = traps[i].getType();

			for(int k = i; k < nT-1; k++) {
                traps[k].setup(traps[k+1].getId(),
                    traps[k+1].getX(), traps[k+1].getY(),
                    traps[k+1].getType(), traps[k+1].getDmg(),
                    traps[k+1].getArm());
            }

            nT--; 
                
            return type;
		}
	}

	for(int i = 0; i < nR; i++) {
		if(rangers[i].getX() == xV && rangers[i].getY() == yV) {
			field[(yV-1)*dim+(xV-1)].setOcc(false);
            type = 2;

            for(int k = i; k < nR-1; k++) {
                rangers[k].setup(rangers[k+1].getId(),
                    rangers[k+1].getX(), rangers[k+1].getY(),
                    rangers[k+1].getDmg(), rangers[k+1].getRange(),
                    rangers[k+1].getBullets());
            }

            nR--;
            
            return type;
        }
	}

	for(int i = 0; i < nB; i++) {
		if(baits[i].getX() == xV && baits[i].getY() == yV) {
            field[(yV-1)*dim+(xV-1)].setOcc(false);
            type = 3;

            for(int k = i; k < nB-1; k++) {
                baits[k].setup(baits[k+1].getId(),
                    baits[k+1].getX(), baits[k+1].getY(),
                    baits[k+1].getHp());
            }

            nB--; return type;
        }
	}

	if(fence.getX() == xV && fence.getY() == yV) {
		field[(yV-1)*dim+(xV-1)].setOcc(false);
		field[(yV-1)*dim+(xV-1)].setT(true);

		fence.setCoords(999, 999);
		type = 4;

		return type;
	}

	cout << "ERROR: Unable to find a defender with this coord-s!\n";
	return -1;
};

// BALANCE CHECK UPON PURCHASE

bool Data::checkMoney(float val) {
	if(money >= val) {
		return true;
	} else {
		return false;
	}
};
