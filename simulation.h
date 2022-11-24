#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"

using namespace std;


class Simulation: public Object {
    public:
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
};


void Simulation::enemiesMove(Data *model) {
	bool delayed, devoured;

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

	devoured = true;

	while(devoured) {
        devoured = false;

        for(int i = 0; i < model->nB; i++) {
            if(model->baits[i].getHp() <= 0.1f) {
                model->field[(model->baits[i].getY()-1)*dim+(model->baits[i].getX()-1)].setOcc(false);

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

void Simulation::defendersMove(Data *model) {
	bool killed, destroyed;
	unsigned int second = 1000000;
	int c = 0;

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
							model->money.setM(model->money.getM()+model->enemies[j].getReward());
                            model->money.setK(model->money.getK()+1);

                            for(int k = j; k < model->nE-1; k++) {
                                model->enemies[k].setup(model->enemies[k+1].getId(),
                                    model->enemies[k+1].getX(), model->enemies[k+1].getY(),
                                    model->enemies[k+1].getHp(), model->enemies[k+1].getDmg(),
                                    model->enemies[k+1].getReward());

                                model->enemies[k].calculatePath(dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
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
                                model->money.setM(model->money.getM()+model->enemies[j].getReward());
                                model->money.setK(model->money.getK()+1);

                                for(int k = j; k < model->nE-1; k++) {
                                    model->enemies[k].setup(model->enemies[k+1].getId(),
                                        model->enemies[k+1].getX(), model->enemies[k+1].getY(),
                                        model->enemies[k+1].getHp(), model->enemies[k+1].getDmg(),
                                        model->enemies[k+1].getReward());

                                    model->enemies[k].calculatePath(dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->fieldSchema);
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

	// CHECK RANGERS <-- [24/11] STOPED HERE

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