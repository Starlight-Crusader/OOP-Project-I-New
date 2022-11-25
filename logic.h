#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"
#include "data.h"

using namespace std;


class Logic: public Object {
    public:
        Simualtion(int idV) {
            id = idV;
        }

        void addEnemies(Data*);

		void enemiesMove(Data*);
		void defendersMove(Data*);
		void checkFinish(Data*);

        void setupSpawns(Data*);
        void constructSchema(Data*);
		void calculatePaths(Data*);

        bool checkGO() {
			if(target.getHp() <= 0.0f) {
				return true;
			} else {
				return false;
			}
		}

        char fieldStateSchema[dim*dim];
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
							model->money.setM(model->money.getM()+model->enemies[j].getReward());
                            model->money.setK(model->money.getK()+1);

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
                                model->money.setM(model->money.getM()+model->enemies[j].getReward());
                                model->money.setK(model->money.getK()+1);

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
                    model->money += enemies[j].getReward();
					model->kills++;

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
		model->enemies[i].calculatePath(model->dim, model->target.getX(), model->target.getY(), model->fence.getX(), model->fence.getY(), model->schema);
	}
};

// BALANCE CHECK UPON PURCHASE

bool Logic::checkMoney(float val) {
	if(money >= val) {
		return true;
	} else {
		return false;
	}
};

// SOME ADDITIONAL FUNCTIONALITY

int Logic::constructSchema(Data *model) {
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
            } else if(field[i*dim+j].getT()) {
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

		    if(field[(y-1)*model->dim+(x-1)].getT()) {
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
