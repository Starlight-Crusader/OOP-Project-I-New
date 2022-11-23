#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"
#include "simualtion.h"
#include "visualisation.h"
#include "data.h"

using namespace std;


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
