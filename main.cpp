#include <iostream>
#include <cstdlib>
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
		int kills;

		// OBJECTS

		Tile field[9*9]; int dim;
		Enemy enemies[64]; int nE;
		Trap traps[64]; int nT;
		Ranger rangers[64]; int nR;
		Structure target;

	public:
		Game(int idV, int dimV, int rV, int mV, int pV, int aV, int kV) {
			id = idV; dim = dimV; round = rV; money = mV; phase = pV; abort = aV; kills = kV;
		}

		void setupField();
		int drawState();

		void addEnemies();
		void addTrap(int, int, int);
		void addRanger(int, int);

		void enemiesMove();
		void defendersMove();
		void checkFinish();

		bool checkMoney(float);

		void printPhase();
		void printStats();
		void printFinalStats();

		void calculatePaths();
		void displayPaths();

		bool checkGO() {
			if(target.getHp() <= 0) {
				return true;
			} else {
				return false;
			}
		}

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

		bool getAbort() { return abort; }
		void detAbort(int aV) { abort = aV; }

		int getK() { return kills; }
		void setK(int kV) { kills = kV; }
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

// OPTIONS LISTS

class OptionsLists: public Object {
	public:
		OptionsLists(int idV) {
			id = idV;
		}

		void printOptionsMain() {
			cout << '\n';
                        cout << "-------------------------------------\n";
                        cout << "          Select an action:\n";
			cout << " 0. Highlight the paths of the en-s,\n";
                        cout << " 1. Place a new beartrap (0.1$),\n";
                        cout << " 2. Place a new nest (0.2$),\n";
			cout << " 3. Hire a ranger (0.4$)";
                        cout << " 4. Sell a defender (50% back),\n";
                        cout << " 5. Start the next round,\n";
                        cout << " 6. Finish the game\n";
                        cout << "-------------------------------------\n";
                        cout << '\n';
		}

		void printOptionsBuy() {
                        cout << '\n';
                        cout << "-------------------------------------\n";
                        cout << " Input coord-s of a new def. in the\n";
                        cout << " form: 'x y' OR enter '0 0' in order\n";
                        cout << " to abort this operation ...\n";
                        cout << "-------------------------------------\n";
                        cout << '\n';
                        cout << "INPUT: ";
		}

		void printOptionsSell() {
			cout << '\n';
                        cout << "-------------------------------------\n";
                        cout << " Input coord-s of a def. to sell in\n";
                        cout << " the form: 'x y' OR enter '0 0' in\n";
                        cout << " order to abort this operation ...\n";
                        cout << "-------------------------------------\n";
                        cout << '\n';
                        cout << "INPUT: ";
		}
};

int main() {
	srand(time(0)); system("clear");

	PriceList pl(rand(), 0.05f, 0.1f, 0.2f); OptionsLists ol(rand());
	Game session(rand(), 9, 0, 0.4f, 0, false, 0); session.setupField();

	while(!abort) {
		if(!session.getPhase()) {
			// PLANNING PHASE
		} else {
			// ACTION PHASE
		}

		if(session.checkGO()) { break; }
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
        target.setCoords(5, 5);
        target.setHp(10.0f);
};

int Game::drawState() {
	// RESET: \u001b[0m
	// RED: \u001b[31m
	// GREEN: \u001b[32m
	// YELLOW: \u001b[33m
	// BLUE: \u001b[34m

	bool drawed;

	for(int i = 0; i < dim; i++) {
		for(int k = 0; k < 14; k++) {
                	cout << ' ';
                }

		for(int j = 0; j < dim; j++) {
			drawed = false;

			for(int k = 0; k < nE; k++) {
				if(enemies[k].getY()-1 == i && enemies[k].getX()-1 == i) {
					cout << "\u001b[31mW\u001b[0m";

					drawed = true;
					break;
				} else { continue; }
			}

			if(drawed) { continue; }

			for(int k = 0; k < nT; k++) {
				if(traps[k].getY()-1 == i && traps[k].getX()-1) {
					if(!traps[k].getType()) {
						cout << "\u001b[34m+\u001b[0m";
					} else {
						cout << "\u001b[31m#\u001b[0m";
					}

					drawed = true;
					break;
				} else { continue; }
			}

			if(drawed) { continue; }

			for(int k = 0; k < nR; k++) {
				if(rangers[k].getY()-1 == i && rangers[k].getX()-1 == j) {
					cout << "\u001b[31mR\u001b[0m";

					drawed = true;
					break;
				} else { continue; }
			}

			if(drawed) { continue; }

			if(target.getX()-1 == j && target.getY()-1 == i) {
                                cout << "\u001b[34mH\u001b[0m";
                        } else if(field[i*dim+j].getT()) {
                                cout << "\u001b[33m~\u001b[0m";
                        } else {
                                cout << "\u001b[32m?\u001b[0m";
                        }
		}

		cout << '\n';
	}

	return 0;
};

// CONSTRUCTION OF ACTORS

void Game::addEnemies() {
	// AT THIS STAGE OF DEVELOPMENT, I WILL JUST SPAWN 4 WOLVES THAT WILL
	// RUN FROM DIFFERENT ENDS OF THE MAP TO THE CENTER

	int spawns[4][2] = { {1, 1}, {1, 9}, {9, 1}, {9, 9} };

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

void Game::defendersMove() {
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
					money += enemies[j].getReward();
					kills++;

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
                                	money += enemies[j].getReward();
					kills++;

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
};

void Game::checkFinish() {
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

	target.setHp(target.getHp()-damage);
};

// SOME OUTPUTS BY GAME

void Game::printPhase() {
	if(!phase) {
       		cout << "\n=========== PLANNIG PHASE ===========\n";
        } else {
                cout << "\n============ ACTION PHASE ===========\n";
               	cout << '\n';
        }
};

void Game::printStats() {
	cout << '\n';
        cout << "-------------------------------------\n";
        cout << "               Stats:\n";
	cout << " * Round: " << round + 1 << ",\n";
        cout << " * HP: " << target.getHp() << ",\n";
        cout << " * Money: " << money << '\n';
        cout << "-------------------------------------\n";
        cout << '\n';
};

void Game::printFinalStats() {
        cout << '\n';
      	cout << "-------------------------------------\n";
       	cout << "             Final stats:\n";
        cout << " * Rounds survived: " << round - 1 << ",\n";
        cout << " * HP left: " << target.getHp() << ",\n";
        cout << " * Enemies killed: " << kills << '\n';
        cout << "-------------------------------------\n";
        cout << '\n';
};

// PATHS HANDLING

void Game::calculatePaths() {
	for(int i = 0; i < nE; i++) {
		enemies[i].calculatePath(dim, target.getX(), target.getY());
	}
};

void Game::displayPaths() {
	unsigned int second = 1000000;

	for(int k = 0; k < nE; k++) {
		system("clear");

		cout << "          Enemy: " << enemies[k].getId() << '\n';
		cout << "-------------------------------------\n";

		enemies[k].displayPath(dim, enemies[k].getX(), enemies[k].getY());

		cout << "-------------------------------------\n";

		usleep(5*second);
	}
};
