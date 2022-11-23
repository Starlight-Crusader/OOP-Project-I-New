#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"

using namespace std;

class Simulation: public Object {
    public:
        void setupField();

		void addEnemies();
		void addTrap(int, int, int);
		void addRanger(int, int);
		void addBait(int, int);
		void addFence(int, int);

		int sellDefender(int, int);

		void enemiesMove();
		void defendersMove();
		void checkFinish();

        void setupSpawns();
		void calculatePaths();

        bool checkMoney(float);
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
}

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

// PATHS HANDLING

void Game::calculatePaths() {
	for(int i = 0; i < nE; i++) {
		enemies[i].calculatePath(dim, target.getX(), target.getY(), fence.getX(), fence.getY(), schema);
	}
};