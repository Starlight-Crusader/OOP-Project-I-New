#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"
#include "generator.h"

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

		Tile field[16*16]; int dim;
		Enemy enemies[16]; int nE = 0;
		Trap traps[16]; int nT = 0;
		Ranger rangers[16]; int nR = 0;
		Bait baits[16]; int nB = 0;
		Structure target;
		Entity fence;

		// FIELD GENERATION

		Generator fieldGenerator;
		int *schema { new int[256] {} };

	public:
		int spawns[4][2];

		Game(int idV, int dimV, int rV, float mV, int pV, int aV, int kV) {
			id = idV; dim = dimV; round = rV; money = mV; phase = pV; abort = aV; kills = kV;
			fence.setCoords(999, 999);
		}

		void setupField();
		int drawState();

		void addEnemies();
		void addTrap(int, int, int);
		void addRanger(int, int);
		void addBait(int, int);
		void addFence(int, int);

		int sellDefender(int, int);

		void enemiesMove();
		void defendersMove();
		void checkFinish();

		bool checkMoney(float);

		void printPhase();
		void printStats();
		void printFinalStats();

		void calculatePaths();
		void displayPaths();

		void setupSpawns();

		bool checkGO() {
			if(target.getHp() <= 0.0f) {
				return true;
			} else {
				return false;
			}
		}

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

		// GET- & SETTERS

		int getDim() { return dim; }

		int getRound() { return round; }
		void setRound(int rV) { round = rV; }

		float getM() { return money; }
		void setM(float mV) { money = mV; }

		int getE() { return nE; }
		void setE(int eV) { nE = eV; }

		int getT() { return nT; }
		void setT(int tV) { nT = tV; }

		int getR() { return nR; }
		void setR(int rV) { nR = rV; }

		int getPhase() { return phase; }
		void setPhase(int pV) { phase = pV; }

		bool getAbort() { return abort; }
		void setAbort(int aV) { abort = aV; }

		int getK() { return kills; }
		void setK(int kV) { kills = kV; }
};

// STORES THE INFO ABOUT THE COST OF THE DEFENSIVE ACTORS

class PriceList: public Object {
	private:
		float beartrap = 0.05f; float nest = 0.1f; float ranger = 0.2f;

	public:
		PriceList(int idV) {
			id = idV;
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
	int option1, option2, t; bool check;
	unsigned int second = 1000000;
	srand(time(0));

	Generator fieldGenerator;

	PriceList pl(rand()); OptionsLists ol(rand());
	Game session(rand(), 16, 1, 0.41f, 0, false, 0);
	session.setupField();

	session.setupSpawns();

	while(!session.getAbort()) {
		if(!session.getPhase()) {
			system("clear");

			session.addEnemies(); session.calculatePaths();
			session.printPhase(); session.printStats();
			session.drawState(); ol.printOptionsMain();

			scanf("%i", &option1);
			cout << '\n';

			switch(option1) {
				case 0: session.displayPaths(); break;
				case 1:
					if(!session.checkMoney(pl.getBPrice())) {
						cout << "ERROR: You have not enough money!\n";
						usleep(2*second);

						break;
					} else {
						ol.printOptionsBuy();

						scanf("%i %i", &option1, &option2);

						if(option1 == 0 && option2 == 0) {
                                                        break;
                                                }

						if(!session.checkTrail(option1, option2)) {
							cout << "ERROR: You are not allowed to place traps in trees!\n";

							usleep(2*second);
							break;
						}

						if(session.checkOcc(option1, option2)) {
							cout << "ERROR: This tile is not empty!\n";

							usleep(2*second);
							break;
						}

						check = false;
						for(int i = 0; i < 4; i++) {
							if(session.spawns[i][1] == option1 && session.spawns[i][0] == option2) {
								cout << "ERROR: You are not allowed to place traps on the spawns!\n";
								check = true;

								usleep(2*second);
							}

							if(check) { break; }
						}

						if(check) {
							break;
						} else {
							session.addTrap(0, option1, option2);
	                                                session.setM(session.getM()-pl.getBPrice());

							break;
						}
					}
				case 2:
					if(!session.checkMoney(pl.getNPrice())) {
                                                cout << "ERROR: You have not enough money!\n";
                                                usleep(2*second);

						break;
                                        } else {
                                                ol.printOptionsBuy();

                                                scanf("%i %i", &option1, &option2);

                                                if(option1 == 0 && option2 == 0) {
                                                        break;
                                                }

                                                if(!session.checkTrail(option1, option2)) {
                                                        cout << "ERROR: You are not allowed to place traps in trees!\n";

                                                        usleep(2*second);
                                                        break;
                                                }

                                                if(session.checkOcc(option1, option2)) {
                                                        cout << "ERROR: This tile is not empty!\n";

                                                        usleep(2*second);
                                                        break;
                                                }

                                                check = false;
                                                for(int i = 0; i < 4; i++) {
                                                        if(session.spawns[i][1] == option1 && session.spawns[i][0] == option2) {
                                                                cout << "ERROR: You are not allowed to place traps on the spawns!\n";
                                                                check = true;

                                                                usleep(2*second);
                                                        }

                                                        if(check) { break; }
                                                }

						if(check) {
                                                        break;
                                                } else {
                                                        session.addTrap(1, option1, option2);
                                                        session.setM(session.getM()-pl.getNPrice());

                                                        break;
                                                }
                                        }
				case 3:
					if(!session.checkMoney(pl.getRPrice())) {
                                                cout << "ERROR: You have not enough money!\n";
                                                usleep(2*second);

						break;
                                        } else {
                                                ol.printOptionsBuy();

                                                scanf("%i %i", &option1, &option2);

                                                if(option1 == 0 && option2 == 0) {
                                                        break;
                                                }

                                                if(session.checkTrail(option1, option2)) {
                                                        cout << "ERROR: You have to place rangers in the trees!\n";

                                                        usleep(2*second);
                                                        break;
                                                }

                                                if(session.checkOcc(option1, option2)) {
                                                        cout << "ERROR: This tile is not empty!\n";

                                                        usleep(2*second);
                                                        break;
                                                }

                                 		session.addRanger(option1, option2);
                                                session.setM(session.getM()-pl.getRPrice());

                                                break;
                                        }
				case 4:
					ol.printOptionsBuy();

                                        scanf("%i %i", &option1, &option2);

                                	if(option1 == 0 && option2 == 0) {
                                        	break;
                                        }

                                        if(!session.checkTrail(option1, option2)) {
                                        	cout << "ERROR: You have to place baits on in the roads!\n";

                                                usleep(2*second);
                                                break;
                                        }

                                        if(session.checkOcc(option1, option2)) {
                                        	cout << "ERROR: This tile is not empty!\n";

                                                usleep(2*second);
                                               	break;
                                        }

                                        session.addBait(option1, option2);
                                       	break;
				case 5:
					ol.printOptionsBuy();

                                        scanf("%i %i", &option1, &option2);

                                        if(option1 == 0 && option2 == 0) {
                                                break;
                                        }

                                        if(!session.checkTrail(option1, option2)) {
                                                cout << "ERROR: You have to place the fence on in the road!\n";

                                                usleep(2*second);
                                                break;
                                        }

                                        if(session.checkOcc(option1, option2)) {
                                                cout << "ERROR: This tile is not empty!\n";

                                                usleep(2*second);
                                                break;
                                        }

                                        session.addFence(option1, option2);
                                        session.calculatePaths();
					break;
				case 6:
					ol.printOptionsSell();

                                        scanf("%i %i", &option1, &option2);

                                        if(option1 == 0 && option2 == 0) {
                                        	break;
                                        }

					t = session.sellDefender(option1, option2);

					switch(t) {
						case 0: session.setM(session.getM()+pl.getBPrice()/2); break;
						case 1: session.setM(session.getM()+pl.getNPrice()/2); break;
						case 2: session.setM(session.getM()+pl.getRPrice()/2); break;
						case 3: break;
						case 4: break;
						case -1: usleep(2*second); break;
					}

					break;
				case 7:
					session.setPhase(1); break;
				case 8:
					system("clear"); session.printFinalStats(); return 0;
			}
		} else {
			while(session.getE() > 0) {
				system("clear");
				session.printPhase();
				session.enemiesMove();
				cout << '\n';
				session.drawState();
				// usleep(1*second);

				system("clear");
				session.printPhase();
				session.defendersMove();
				cout << '\n';
				session.drawState();
				usleep(1*second);

				session.checkFinish();
			}

			session.setPhase(0);
			session.setRound(session.getRound()+1);

			if(session.checkGO()) {
				session.setAbort(true);
			} else {
				session.setupSpawns();
			}
		}
	}

	system("clear"); session.printFinalStats();

	return 0;
};

// BASIC WORK WITH THE FIELD

void Game::setupField() {
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
			*(schema+i*dim+j) = fieldGenerator.field[i][j];
		}
	}
};

int Game::drawState() {
	// RESET: \u001b[0m
	// RED: \u001b[31m
	// GREEN: \u001b[32m
	// YELLOW: \u001b[33m
	// BLUE: \u001b[34m
	// GREY: \u001b[37m

	bool drawed;

	for(int k = 0; k < 11; k++) {
        	cout << ' ';
        }

	for(int k = 1; k <= 16; k++) {
		cout << k % 10;
	}

	cout << '\n';

	for(int i = 0; i < dim; i++) {
		for(int k = 0; k < 9; k++) {
                	cout << ' ';
                }

		cout << (i + 1) % 10 << ' ';

		for(int j = 0; j < dim; j++) {
			drawed = false;

			for(int k = 0; k < nE; k++) {
				if(enemies[k].getY()-1 == i && enemies[k].getX()-1 == j) {
					cout << "\u001b[37mw\u001b[0m";

					drawed = true;
					break;
				} else { continue; }
			}

			if(drawed) { continue; }

			for(int k = 0; k < nT; k++) {
				if(traps[k].getY()-1 == i && traps[k].getX()-1 == j) {
					if(!traps[k].getType()) {
						cout << "\u001b[34m@\u001b[0m";
					} else {
						cout << "\u001b[34m#\u001b[0m";
					}

					drawed = true;
					break;
				} else { continue; }
			}

			if(drawed) { continue; }

			for(int k = 0; k < nR; k++) {
				if(rangers[k].getY()-1 == i && rangers[k].getX()-1 == j) {
					cout << "\u001b[34mR\u001b[0m";

					drawed = true;
					break;
				} else { continue; }
			}

			if(drawed) { continue; }

			for(int k = 0; k < nB; k++) {
				if(baits[k].getY()-1 == i && baits[k].getX()-1 == j) {
					cout << "\u001b[34m.\u001b[0m";

					drawed = true;
                                        break;
				} else { continue; }
			}

			if(drawed) { continue; }

			if(fence.getY()-1 == i && fence.getX()-1 == j) {
				cout << "\u001b[34m:\u001b[0m";

                              	drawed = true;
			}

			if(drawed) { continue; }

			if(target.getY()-1 == i && target.getX()-1 == j) {
                                cout << "\u001b[34mH\u001b[0m";
                        } else if(field[i*dim+j].getT()) {
                                cout << "\u001b[33m~\u001b[0m";
                        } else {
                                cout << "\u001b[32mt\u001b[0m";
                        }
		}

		cout << '\n';
	}

	return 0;
};

// CONSTRUCTION OF ACTORS

void Game::setupSpawns() {
	int valid;
	int x, y;

	valid = false;

	while(!valid) {
		x = 1;
		y = rand() % 16 + 1;

		if(field[(y-1)*dim+(x-1)].getT()) {
			spawns[0][1] = x;
			spawns[0][0] = y;

			valid = true;
		}
	}

	valid = false;

	while(!valid) {
                x = 16;
                y = rand() % 16 + 1;

                if(field[(y-1)*dim+(x-1)].getT()) {
                        spawns[1][1] = x;
                        spawns[1][0] = y;

                        valid = true;
                }
        }

        valid = false;

	while(!valid) {
                x = rand() % 16 + 1;
                y = 1;

                if(field[(y-1)*dim+(x-1)].getT()) {
                        spawns[2][1] = x;
                        spawns[2][0] = y;

                        valid = true;
                }
        }

        valid = false;

	while(!valid) {
                x = rand() % 16 + 1;
                y = 16;

                if(field[(y-1)*dim+(x-1)].getT()) {
                        spawns[3][1] = x;
                        spawns[3][0] = y;

                        valid = true;
                }
        }
};

void Game::addEnemies() {
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

			field[(yV-1)*dim+(xV-1)].setOcc(true);

			break;

		case 1:
			traps[nT].setup(rand(), xV, yV, 1, 4.0f, true);
			nT++;

			field[(yV-1)*dim+(xV-1)].setOcc(true);

			break;
	}
};

void Game::addRanger(int xV, int yV) {
	rangers[nR].setup(rand(), xV, yV, 4.0f, 1.5f, 2);
	nR++;

	field[(yV-1)*dim+(xV-1)].setOcc(true);
};

void Game::addBait(int xV, int yV) {
	baits[nB].setup(rand(), xV, yV, 1.0f);
	nB++;

	field[(yV-1)*dim+(xV-1)].setOcc(true);
};

void Game::addFence(int xV, int yV) {
	if(fence.getX() != 999 && fence.getY() != 999) {
		field[(fence.getY()-1)*dim+(fence.getX()-1)].setOcc(false);
        	field[(fence.getY()-1)*dim+(fence.getX()-1)].setT(true);
	}

	fence.setCoords(xV, yV);

	field[(yV-1)*dim+(xV-1)].setOcc(true);
	field[(yV-1)*dim+(xV-1)].setT(false);
};

// SELLING OF ACTORS

int Game::sellDefender(int xV, int yV) {
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

                        nT--; return type;
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

                        nR--; return type;
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

bool Game::checkMoney(float val) {
	if(money >= val) {
		return true;
	} else {
		return false;
	}
};

// BASIC ACTIONS AND INTERATIONS OF ACTORS

void Game::enemiesMove() {
	bool delayed, devoured;

	for(int i = 0; i < nE; i++) {
		delayed = false;

		for(int j = 0; j < nB; j++) {
			if(baits[j].trigger(enemies[i].getX(), enemies[i].getY())) {
				delayed = true;

				baits[j].setHp(baits[j].getHp()-enemies[i].getDmg());
			} else {
				continue;
			}
		}

		if(!delayed) {
			enemies[i].makeStep();

			// cout << ">>> EVENT: Enemy {id: " << enemies[i].getId() << "} made a step towards the target <<<\n";
		} else { continue; }
	}

	devoured = true;

	while(devoured) {
                devoured = false;

                for(int i = 0; i < nB; i++) {
                        if(baits[i].getHp() <= 0.1f) {
                                field[(baits[i].getY()-1)*dim+(baits[i].getX()-1)].setOcc(false);

                                // cout << ">>> LOG: BAIT {id: " << baits[i].getId() << "} was devoured <<<\n";

                                for(int k = i; k < nB-1; k++) {
                                	baits[k].setup(baits[k+1].getId(),
                                        	baits[k+1].getX(), baits[k+1].getY(),
                                        	baits[k+1].getHp());
                        	}

                        	nB--;

                                devoured = true; break;
                        } else { continue; }
                }
        }
};

void Game::defendersMove() {
	bool killed, destroyed;
	unsigned int second = 1000000;
	int c = 0;

	// CHECK TRAPS

	for(int i = 0; i < nT; i++) {
		if(!traps[i].getArm()) {
			continue;
		}

		switch(traps[i].getType()) {
			case 0:
				for(int j = 0; j < nE; j++) {
					if(traps[i].trigger(enemies[j].getX(), enemies[j].getY())) {
						enemies[j].setHp(enemies[j].getHp()-traps[i].getDmg());
						traps[i].setArm(false);

						// cout << ">>> EVENT: TRAP {id: " << traps[i].getId() << "} was triggered by ENEMY {id: " << enemies[j].getId() << "} <<<\n";
		                                // cout << ">>> EVENT: TRAP {id: " << traps[i].getId() << "} was disarmed <<<\n";
					}
				}

				killed = true;

				while(killed) {
					killed = false;

					for(int j = 0; j < nE; j++) {
						if(enemies[j].getHp() <= 0) {
							money += enemies[j].getReward();
							kills++;

							// cout << ">>> EVENT: ENEMY {id: " << enemies[j].getId() << "} was killed <<<\n";

                                        		for(int k = j; k < nE-1; k++) {
                                                		enemies[k].setup(enemies[k+1].getId(),
                                                        		enemies[k+1].getX(), enemies[k+1].getY(),
                                                        		enemies[k+1].getHp(), enemies[k+1].getDmg(),
                                                        		enemies[k+1].getReward());

                                                		enemies[k].calculatePath(dim, target.getX(), target.getY(), fence.getX(), fence.getY(), schema);
		                                        }

                		                        nE--;
							killed = true;

							break;
						} else { continue; }
					}
				}

				break;
			case 1:
				for(int j = 0; j < nE; j++) {
					c += traps[i].trigger(enemies[j].getX(), enemies[j].getY());
				}

				if(c >= 2) {
					for(int j = 0; j < nE; j++) {
						if(traps[i].trigger(enemies[j].getX(), enemies[j].getY())) {
                                                	enemies[j].setHp(enemies[j].getHp()-traps[i].getDmg());
                                                	traps[i].setArm(false);

	                                                // cout << ">>> EVENT: TRAP {id: " << traps[i].getId() << "} was triggered by ENEMY {id: " << enemies[j].getId() << "} <<<\n";
        	                                        // cout << ">>> EVENT: TRAP {id: " << traps[i].getId() << "} was disarmed <<<\n";
                	                        }
					}

					killed = true;

                                	while(killed) {
                                        	killed = false;

                                        	for(int j = 0; j < nE; j++) {
                                                	if(enemies[j].getHp() <= 0) {
                                                        	money += enemies[j].getReward();
                                                        	kills++;

                                                        	// cout << ">>> EVENT: ENEMY {id: " << enemies[j].getId() << "} was killed <<<\n";

                                                        	for(int k = j; k < nE-1; k++) {
                                                                	enemies[k].setup(enemies[k+1].getId(),
                                                                        	enemies[k+1].getX(), enemies[k+1].getY(),
                                                                        	enemies[k+1].getHp(), enemies[k+1].getDmg(),
                                                                        	enemies[k+1].getReward());

	                                                                enemies[k].calculatePath(dim, target.getX(), target.getY(), fence.getX(), fence.getY(), schema);
        	                                                }

	                                                        nE--;
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

	for(int i = 0; i < nR; i++) {
		for(int j = 0; j < nE; j++) {
			if(!rangers[i].checkBullets()) {
				break;
			} else if(rangers[i].trigger(enemies[j].getX(), enemies[j].getY())) {
				enemies[j].setHp(enemies[j].getHp()-rangers[i].getDmg());
				rangers[i].setBullets(rangers[i].getBullets()-1);

				// cout << ">>> EVENT: RANGER {id: " << rangers[i].getId() << "} shot at ENEMY {id: " << enemies[j].getId() << "} <<<\n";
				// cout << ">>> EVENT: RANGER {id: " << rangers[i].getId() << "} spend 1 bullet; bullets left: " << rangers[i].getBullets() << "<<<\n";
				// usleep(3*second);

				if(enemies[j].getHp() <= 0) {
                                	money += enemies[j].getReward();
					kills++;

                                	// cout << ">>> EVENT: ENEMY {id: " << enemies[j].getId() << "} was killed <<<\n";

                                	for(int k = j; k < nE-1; k++) {
                                        	enemies[k].setup(enemies[k+1].getId(),
                                                	enemies[k+1].getX(), enemies[k+1].getY(),
                                                	enemies[k+1].getHp(), enemies[k+1].getDmg(),
                                                	enemies[k+1].getReward());

						enemies[k].calculatePath(dim, target.getX(), target.getY(), fence.getX(), fence.getY(), schema);
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

	destroyed = true;

	while(destroyed) {
		destroyed = false;

		for(int i = 0; i < nT; i++) {
			if(!traps[i].getArm()) {
				field[(traps[i].getY()-1)*dim+(traps[i].getX()-1)].setOcc(false);

				// cout << ">>> LOG: TRAP {id: " << traps[i].getId() << "} was destroyed <<<\n";

				for(int k = i; k < nT-1; k++) {
                                	traps[k].setup(traps[k+1].getId(),
                                        	traps[k+1].getX(), traps[k+1].getY(),
                                                traps[k+1].getType(), traps[k+1].getDmg(),
						traps[k+1].getArm());
                                }

				nT--;

				destroyed = true; break;
			} else { continue; }
		}
	}

	destroyed = true;

	while(destroyed) {
                destroyed = false;

                for(int i = 0; i < nR; i++) {
                        if(!rangers[i].checkBullets()) {
				field[(rangers[i].getY()-1)*dim+(rangers[i].getX()-1)].setOcc(false);

                                // cout << ">>> LOG: RANGER {id: " << rangers[i].getId() << "} was destroyed <<<\n";

                                for(int k = i; k < nR; k++) {
                                        rangers[k].setup(rangers[k+1].getId(),
                                                rangers[k+1].getX(), rangers[k+1].getY(),
                                                rangers[k+1].getDmg(), rangers[k+1].getRange(),
						rangers[k+1].getBullets());
                                }

				nR--;

                                destroyed = true; break;
                        } else { continue; }
                }
        }
};

void Game::checkFinish() {
	float damage = 0.0f;
	bool finished = true;

	while(finished) {
		finished = false;

		for(int i = 0; i < nE; i++) {
                        if(enemies[i].getX() == target.getX() && enemies[i].getY() == target.getY()) {
				damage += enemies[i].getDmg();

                                cout << ">>> EVENT: ENEMY {id: " << enemies[i].getId() << "} has reached the TARGET {id: " << target.getId() << " <<<\n";

                                for(int k = i; k < nE-1; k++) {
                                        enemies[k].setup(enemies[k+1].getId(),
                                                enemies[k+1].getX(), enemies[k+1].getY(),
                                                enemies[k+1].getHp(), enemies[k+1].getDmg(),
                                                enemies[k+1].getReward());

					enemies[k].calculatePath(dim, target.getX(), target.getY(), fence.getX(), fence.getY(), schema);
                                }

				nE--;

                                finished = true; break;
                        } else { continue; }
                }
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
	cout << " * Round: " << round << ",\n";
        cout << " * HP: " << target.getHp() << ",\n";
        cout << " * Money: " << money << "$\n";
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
		enemies[i].calculatePath(dim, target.getX(), target.getY(), fence.getX(), fence.getY(), schema);
	}
};

void Game::displayPaths() {
	unsigned int second = 1000000;

	for(int k = 0; k < nE; k++) {
		system("clear");

		cout << "\n            ENEMY: " << enemies[k].getId() << "\n\n";

		enemies[k].displayPath(dim, enemies[k].getX(), enemies[k].getY());

		usleep(4*second);
	}
};

