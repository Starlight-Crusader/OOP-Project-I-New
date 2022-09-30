#include <cstdlib>
#include <list>
#include <unistd.h>

#include "classes.h"

using namespace std;

class Game: public Object {
	private:
		// STATS

		int round;
		float money;
		int phase;
		bool abort;

		// OBJECTS

		Tile field[9*9]; int dim;
		Enemy enemies[64]; int nE;
		Trap traps[64]; int nT;
		Ranger rangers[64]; int nR;
		Structure target;

	public:
		Game(int idV, int dimV, int rV, int mV, int pV, int aV) {
			id = idV; dim = dimV; round = rV; money = mV; phase = pV; abort = aV;
		}

		void setupField();
		void drawState();

		void addEnemies();
		void addTrap(int, int, int);
		void addRanger(int, int);

		void enemiesMove();
		float defendersMove();
		float checkFinish();

		bool checkMoney(float);

		// GET- & SETTERS

		int getDim() { return dim; }

		int getRound() { return round; }
		void setRound(int rV) { round = rV; }

		float getM() { return money; }
		void setM(int mV) { money = mV; }

		int getE() { return nE; }
		void setE(int eV) { nE = eV; }

		int getT() { return nT; }
		void setT(int tV) { nT = tV; }

		int getR() { return nR; }
		void setR(int rV) { nR = rV; }

		int getPhase() { return phase; }
		void setPhase(int pV) { phase = pV; }
};

// STORES THE INFO ABOUT THE COST OF THE DEFENSIVE ACTORS

class PriceList: public Object {
	private:
		float beartrap; float nest; float ranger;

	public:
		PriceList(int idV, int pB, int pN, int pR) {
			id = idV; beartrap = pB; nest = pN; ranger = pR;
		}

		float getBPrice() { return beartrap; }
		float getNPrice() { return nest; }
		float getRPrice() { return ranger; }
};

int main() {
	srand(time(0)); system("clear");

	PriceList pl(rand(), 0.05f, 0.1f, 0.2f);
	Game session(rand(), 9, 0, 0.4f, 0, false); session.setupField(); session.drawState();

	while(!abort) {
		if(!session.getPhase()) {
			// PLANNING PHASE
		} else {
			// ACTION PHASE
		}

		if(session.target.getHp() <= 0) { break; }
	}

	return 0;
};

// BASIC WORK WITH THE FIELD

void Game::setupField() {
	for(int i = 0; i < dim; i++) {
        	for(int j = 0; j < dim; j++) {
                	field[i*dim+j].setOcc(false);
                       	field[i*dim+j].setT(false);
                        field[i*dim+j].setId(rand());
                }
        }

        for(int i = 0; i < dim; i++) {
        	field[1*dim+i].setT(true);
                field[7*dim+i].setT(true);

                field[i*dim+1].setT(true);
                field[i*dim+7].setT(true);
        }

        for(int i = 3; i <= 5 ; i++) {
       		for(int j = 3; j <= 5; j++) {
                	field[i*dim+j].setT(true);
                }
        }

        field[5*dim+2].setT(true);
        field[3*dim+6].setT(true);

	field[0*dim+0].setT(true);
	field[0*dim+8].setT(true);
	field[8*dim+0].setT(true);
	field[8*dim+8].setT(true);

        target.setId(rand());
        target.setCoords(dim/2+1, dim/2+1);
        target.setHp(10.0f);
};

void Game::drawState() {
	// RESET: \u001b[0m
	// RED: \u001b[31m
	// GREEN: \u001b[32m
	// YELLOW: \u001b[33m
	// BLUE: \u001b[34m

	for(int i = 0; i < dim; i++) {
		for(int k = 0; k < 14; k++) {
                	cout << ' ';
                }

		for(int j = 0; j < dim; j++) {
			if(j == target.getX()-1 && i == target.getY()-1) {
				cout << "\u001b[34mH\u001b[0m";
			} else if(field[i*dim+j].getT()) {
				cout << "\u001b[33m~\u001b[0m";
			} else {
				cout << "\u001b[32m?\u001b[0m";
			}
		}

		cout << '\n';
	}
};

// CONSTRUCTION OF ACTORS

void Game::addEnemies() {
	// AT THIS STAGE OF DEVELOPMENT, I WILL JUST SPAWN 4 WOLVES THAT WILL
	// RUN FROM DIFFERENT ENDS OF THE MAP TO THE CENTER

	int spawns[4][2] = { {0, 0}, {0, 8}, {8, 0}, {8, 8} };

	nE = 4;

	for(int i = 0; i < nE; i++) {
		enemies[i].setup(rand(), spawns[i][1], spawns[i][0], 4.0f, 1.0f, 0.1f);
	}
};

void Game::addTrap(int tV, int xV, int yV) {
	switch(tV) {
		case 0:
			traps[nT].setup(rand(), xV, yV, 0, 2.0f, true);
			nT++;

			break;
		case 1:
			traps[nT].setup(rand(), xV, yV, 1, 4.0f, true);
			nT++;

			break;
	}
};

void Game::addRanger(int xV, int yV) {
	rangers[nR].setup(rand(), xV, yV, 4.0f, 1.5f, 2);
	nR++;
};

// BALANCE CHECK UPON PURCHASE

bool Game::checkMoney(float val) {
	if(money >= val) {
		return true;
	} else {
		return false;
	}
};

// BASIC ACTIONS AND INTERATIONS OF ACTORS

void Game::enemiesMove() {
	for(int i = 0; i < nE; i++) {
		enemies[i].makeStep();

		cout << ">>> EVENT: Enemy {id: " << enemies[i].getId() << "} made a step towards the target <<<\n";
	}
};

float Game::defendersMove() {
	float rewards;
	bool destroyed;

	// CHECK TRAPS

	for(int i = 0; i < nT; i++) {
		for(int j = 0; j < nE; j++) {
			if(!traps[i].getArm()) {
				break;
			} else if(traps[i].trigger(enemies[j].getX(), enemies[j].getY())) {
				enemies[j].setHp(enemies[j].getHp()-traps[i].getDmg());
				traps[i].setArm(false);

				cout << ">>> EVENT: TRAP {id: " << traps[i].getId() << "} was triggered by ENEMY {id: " << enemies[j].getId() << "} <<<\n";
				cout << ">>> EVENT: TRAP {id: " << traps[i].getId() << "} was disarmed <<<\n";

				if(enemies[j].getHp() <= 0) {
					rewards += enemies[j].getReward();

					cout << ">>> EVENT: ENEMY {id: " << enemies[j].getId() << "} was killed <<<\n";

					for(int k = j; k < nE-1; k++) {
						enemies[k].setup(enemies[k+1].getId(),
							enemies[k+1].getX(), enemies[k+1].getY(),
							enemies[k+1].getHp(), enemies[k+1].getDmg(),
							enemies[k+1].getReward());
					}

					nE--;
				}

				break;
			} else {
				continue;
			}
		}
	}

	// CHECK RANGERS

	for(int i = 0; i < nR; i++) {
		for(int j = 0; j < nE; j++) {
			if(!rangers[i].checkBullets()) {
				break;
			} else if(rangers[i].trigger(enemies[j].getX(), enemies[j].getY())) {
				enemies[j].setHp(enemies[j].getHp()-rangers[i].getDmg());
				rangers[i].setBullets(rangers[i].getBullets()-1);

				cout << ">>> EVENT: RANGER {id: " << rangers[i].getId() << "shot at ENEMY {id: " << enemies[j].getId() << "} <<<\n";
				cout << ">>> EVENT: RANGER {id: " << rangers[i].getId() << "spend 1 bullet; bullets left: " << rangers[i].getBullets() << "<<<\n";

				if(enemies[j].getHp() <= 0) {
                                	rewards += enemies[j].getReward();

                                	cout << ">>> EVENT: ENEMY {id: " << enemies[j].getId() << "} was killed <<<\n";

                                	for(int k = j; k < nE-1; k++) {
                                        	enemies[k].setup(enemies[k+1].getId(),
                                                	enemies[k+1].getX(), enemies[k+1].getY(),
                                                	enemies[k+1].getHp(), enemies[k+1].getDmg(),
                                                	enemies[k+1].getReward());
					}

                                	nE--;
                        	}

				break;
			} else {
				continue;
			}
		}
	}

	// DESTROY DISARMED TRAPS & EMPTY RANGERS

	while(1) {
		destroyed = false;

		for(int i = 0; i < nT; i++) {
			if(!traps[i].getArm()) {
				cout << ">>> LOG: TRAP {id: " << traps[i].getId() << "} was destroyed <<<\n";

				for(int k = i; k < nT-1; k++) {
                                	traps[k].setup(traps[k+1].getId(),
                                        	traps[k+1].getX(), traps[k+1].getY(),
                                                traps[k+1].getType(), traps[k+1].getDmg(),
						traps[k+1].getArm());
                                }

				destroyed = true;
			} else { continue; }

			if(destroyed) {	break; }
		}

		if(!destroyed) { break; }
	}

	while(1) {
                destroyed = false;

                for(int i = 0; i < nR; i++) {
                        if(!rangers[i].checkBullets()) {
                                cout << ">>> LOG: RANGER {id: " << rangers[i].getId() << "} was destroyed <<<\n";

                                for(int k = i; k < nR-1; k++) {
                                        rangers[k].setup(rangers[k+1].getId(),
                                                rangers[k+1].getX(), rangers[k+1].getY(),
                                                rangers[k+1].getDmg(), rangers[k+1].getRange(),
						rangers[k+1].getBullets());
                                }

                                destroyed = true;
                        } else { continue; }

                        if(destroyed) { break; }
                }

                if(!destroyed) { break; }
        }

	return rewards;
};

float Game::checkFinish() {
	float damage = 0.0f;
	bool finished;

	while(1) {
		finished = false;

		for(int i = 0; i < nE; i++) {
                        if(enemies[i].getX() == target.getX() && enemies[i].getY() == target.getY()) {
				damage += enemies[i].getDmg();

                                cout << ">>> EVENT: ENEMY {id: " << enemies[i].getId() << "} has reached the TARGET {id: " << target.getId() << " <<<\n";

                                for(int k = i; k < nR-1; k++) {
                                        enemies[k].setup(enemies[k+1].getId(),
                                                enemies[k+1].getX(), enemies[k+1].getY(),
                                                enemies[k+1].getHp(), enemies[k+1].getDmg(),
                                                enemies[k+1].getReward());
                                }

                                finished = true;
                        } else { continue; }

                        if(finished) { break; }
                }

                if(!finished) { break; }
	}

	return damage;
};
