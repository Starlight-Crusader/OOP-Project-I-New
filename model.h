// TILE 1:
/*
    ..1..1..
    ........
    1.1111.1
    ..1.....
    .....1..
    1.1111.1
    ........
    ..1..1..
*/

// TILE 2:
/*
    111..111
    1......1
    1.1..1.1
    ..1..1..
    ..1..1..
    1.1..1.1
    1......1
    111..111
*/

// TILE 3:
/*
	111..111
	1......1
	111..1.1
	..1.....
	.....1..
	1.1..111
	1......1
	111..111
*/

// TILE 4:
/*
    1....111
    ..1..111
    .11..111
    ........
    ........
    111..11.
    111..1..
    111....1
*/


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "entities.h"

using namespace std;


// MAP GENERATOR

class Generator: public Object  {
    private:
        int id = 0;

        int tile1[8][8], tile2[8][8], tile3[8][8], tile4[8][8];

    public:
        int field[16][16];

	    void setupTiles();
		void drawTiles();
		void generateField();
		void drawField();
};


void Generator::setupTiles() {
    // TILE 1

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            tile1[i][j] = 0;
        }
    }

    tile1[0][2] = -1; tile1[0][5] = -1;

    tile1[2][0] = -1;
    for(int i = 2; i < 6; i++) { tile1[2][i] = -1; }
    tile1[2][7] = -1;

    tile1[3][2] = -1; tile1[4][5] = -1;

    tile1[5][0] = -1;
                for(int i = 2; i < 6; i++) { tile1[5][i] = -1; }
                tile1[5][7] = -1;

    tile1[7][2] = -1; tile1[7][5] = -1;

    // TILE 2

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            tile2[i][j] = 0;
        }
    }

    for(int i = 0; i < 8; i++) {
        if(i != 3 && i != 4) {
            tile2[0][i] = -1;
            tile2[7][i] = -1;
        }
    }

    for(int i = 0; i < 8; i++) {
        if(i != 3 && i != 4) {
            tile2[i][0] = -1;
            tile2[i][7] = -1;
        }
    }

    for(int i = 2; i < 6; i++) {
        tile2[i][2] = -1;
        tile2[i][5] = -1;
    }

    // TILE 3

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            tile3[i][j] = 0;
        }
    }

    for(int i = 0; i < 8; i++) {
        if(i != 4 && i !=  3) {
            tile3[0][i] = -1;
            tile3[7][i] = -1;
            
            tile3[i][0] = -1;
            tile3[i][7] = -1;	
        }
    }

    tile3[2][1] = -1; tile3[2][2] = -1; tile3[2][5] = -1;
    tile3[3][2] = -1; tile3[4][5] = -1;
    tile3[5][2] = -1; tile3[5][5] = -1; tile3[5][6] = -1;

    // TILE 4

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            tile4[i][j] = 0;
        }
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 5; j < 8; j++) {
            tile4[i][j] = -1;
        }
    }

    for(int i = 5; i < 8; i++) {
        for(int j = 0; j < 3; j++) {
            tile4[i][j] = -1;
        }
    }

    tile4[0][0] = -1; tile4[7][7] = -1;

    tile4[1][2] = -1; tile4[2][1] = -1; tile4[2][2] = -1;
    tile4[5][5] = -1; tile4[5][6] = -1; tile4[6][5] = -1;
};

void Generator::drawTiles() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(tile1[i][j] == 0) {
                cout << '.';
            } else {
                cout << '1';
            }
        }

        cout << '\n';
    }

	cout << '\n';

	for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(tile2[i][j] == 0) {
                cout << '.';
            } else {
                cout << '1';
            }
        }

        cout << '\n';
    }

    cout << '\n';

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(tile3[i][j] == 0) {
                cout << '.';
            } else {
                cout << '1';
            }
        }

        cout << '\n';
    }
    
    cout << '\n';

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(tile4[i][j] == 0) {
                cout << '.';
            } else {
                cout << '1';
            }
        }

        cout << '\n';
    }

    cout << '\n';
};

void Generator::generateField() {
	for(int i = 0; i < 16; i++) {
	    for(int j = 0; j < 16; j++) {
			field[i][j] = 0;
		}
	}

    for(int y = 0; y < 2; y++) {
        for(int x = 0; x < 2; x++) {
            switch(rand() % 4) {
		        case 0:
			        for(int i = y*8; i < 8+y*8; i++) {
				        for(int j = x*8; j < 8+x*8; j++) {
					        field[i][j] = tile1[i-y*8][j-x*8];
				        }
			        }

			        break;
		        case 1:
                    for(int i = y*8; i < 8+y*8; i++) {
                        for(int j = x*8; j < 8+x*8; j++) {
                            field[i][j] = tile2[i-y*8][j-x*8];
                        }
                    }

			        break;
		        case 2:
                    for(int i = y*8; i < 8+y*8; i++) {
                        for(int j = x*8; j < 8+x*8; j++) {
                            field[i][j] = tile3[i-y*8][j-x*8];
                        }
                    }

			        break;
		        case 3:
                    for(int i = y*8; i < 8+y*8; i++) {
                        for(int j = x*8; j < 8+x*8; j++) {
                            field[i][j] = tile4[i-y*8][j-x*8];
                        }
                    }

                    break;
            }
        }
    }
};

void Generator::drawField() {
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			if(field[i][j] == 0) {
                cout << '.';
            } else {
                cout << '1';
            }
		}

		cout << '\n';
	}
};


// DATA STORAGE AND BASIC MANIPULATION

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
            id = idV; round = 1; phase = 0; abort = 0; kills = 0; money = 0.4f;

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

    unsigned int second = 1000000;

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


// COMPLEX LOGIC

class Logic: public Object {
    public:
        Logic(int idV) {
            id = idV;
        }

        void addEnemies(Data*);

		void enemiesMove(Data*);
		void defendersMove(Data*);
		void checkFinish(Data*);

        void destroyEnemy(Data*, int);

        void setupSpawns(Data*);
        void constructSchema(Data*);
		void calculatePaths(Data*);

        bool checkGO(Data *data) {
			if(data->target.getHp() <= 0.0f) {
				return true;
			} else {
				return false;
			}
		}

        char *fieldStateSchema = (char*)malloc(256*sizeof(char));
};

// BASIC ACTIONS AND INTERACTIONS OF ACTORS

void Logic::enemiesMove(Data *data) {
	bool delayed, devoured;

    // MOVE ENEMIES

	for(int i = 0; i < data->nE; i++) {
		delayed = false;

		for(int j = 0; j < data->nB; j++) {
			if(data->baits[j].trigger(data->enemies[i].getX(), data->enemies[i].getY())) {
				delayed = true;

				data->baits[j].setHp(data->baits[j].getHp()-data->enemies[i].getDmg());
			} else {
				continue;
			}
		}

		if(!delayed) {
			data->enemies[i].makeStep();
        } else { 
            continue; 
        }
	}

    // DESTROY DEVOURED BAITS

	devoured = true;

	while(devoured) {
        devoured = false;

        for(int i = 0; i < data->nB; i++) {
            if(data->baits[i].getHp() <= 0.1f) {
                data->field[(data->baits[i].getY()-1)*data->dim+(data->baits[i].getX()-1)].setOcc(false);

                for(int k = i; k < data->nB-1; k++) {
                    data->baits[k].setup(data->baits[k+1].getId(),
                        data->baits[k+1].getX(), data->baits[k+1].getY(),
                        data->baits[k+1].getHp());
                }

                data->nB--; devoured = true; 
                break;
            } else { continue; }
        }
    }
};

void Logic::defendersMove(Data *data) {
	bool killed, destroyed;
	unsigned int second = 1000000;
	int c = 0;

    // CHECK TRAPS

	for(int i = 0; i < data->nT; i++) {
		if(!data->traps[i].getArm()) { continue; }

		switch(data->traps[i].getType()) {
			case 0:
				for(int j = 0; j < data->nE; j++) {
					if(data->traps[i].trigger(data->enemies[j].getX(), data->enemies[j].getY())) {
						data->enemies[j].setHp(data->enemies[j].getHp()-data->traps[i].getDmg());
						data->traps[i].setArm(false);
                    }
				}

				killed = true;

				while(killed) {
					killed = false;

					for(int j = 0; j < data->nE; j++) {
						if(data->enemies[j].getHp() <= 0) {
							data->setM(data->getM()+data->enemies[j].getReward());
                            data->setK(data->getK()+1);

                            this->destroyEnemy(data, j);

                            data->nE--;
							killed = true;

							break;
						} else { continue; }
					}
				}

				break;
			case 1:
				for(int j = 0; j < data->nE; j++) {
					c += data->traps[i].trigger(data->enemies[j].getX(), data->enemies[j].getY());
				}

				if(c >= 2) {
					for(int j = 0; j < data->nE; j++) {
						if(data->traps[i].trigger(data->enemies[j].getX(), data->enemies[j].getY())) {
                            data->enemies[j].setHp(data->enemies[j].getHp()-data->traps[i].getDmg());
                            data->traps[i].setArm(false);
                        }
					}

					killed = true;

                    while(killed) {
                        killed = false;

                        for(int j = 0; j < data->nE; j++) {
                            if(data->enemies[j].getHp() <= 0) {
                                data->setM(data->getM()+data->enemies[j].getReward());
                                data->setK(data->getK()+1);

                                this->destroyEnemy(data, j);

                                data->nE--;
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

	for(int i = 0; i < data->nR; i++) {
		for(int j = 0; j < data->nE; j++) {
			if(!data->rangers[i].checkBullets()) {
				break;
			} else if(data->rangers[i].trigger(data->enemies[j].getX(), data->enemies[j].getY())) {
				data->enemies[j].setHp(data->enemies[j].getHp()-data->rangers[i].getDmg());
				data->rangers[i].setBullets(data->rangers[i].getBullets()-1);

				if(data->enemies[j].getHp() <= 0) {
                    data->setM(data->getM()+data->enemies[j].getReward());
                    data->setK(data->getK()+1);

                    this->destroyEnemy(data, j);

                    data->nE--;
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

		for(int i = 0; i < data->nT; i++) {
			if(!data->traps[i].getArm()) {
				data->field[(data->traps[i].getY()-1)*data->dim+(data->traps[i].getX()-1)].setOcc(false);

				for(int k = i; k < data->nT-1; k++) {
                    data->traps[k].setup(data->traps[k+1].getId(),
                        data->traps[k+1].getX(), data->traps[k+1].getY(),
                        data->traps[k+1].getType(), data->traps[k+1].getDmg(),
						data->traps[k+1].getArm());
                }

				data->nT--;
				destroyed = true; 
                
                break;
			} else { continue; }
		}
	}

	destroyed = true;

	while(destroyed) {
        destroyed = false;

        for(int i = 0; i < data->nR; i++) {
            if(!data->rangers[i].checkBullets()) {
				data->field[(data->rangers[i].getY()-1)*data->dim+(data->rangers[i].getX()-1)].setOcc(false);

                for(int k = i; k < data->nR; k++) {
                    data->rangers[k].setup(data->rangers[k+1].getId(),
                        data->rangers[k+1].getX(), data->rangers[k+1].getY(),
                        data->rangers[k+1].getDmg(), data->rangers[k+1].getRange(),
                        data->rangers[k+1].getBullets());
                }

				data->nR--;
                destroyed = true; 
                
                break;
            } else { continue; }
        }
    }
};

void Logic::checkFinish(Data *data) {
	float damage = 0.0f;
	bool finished = true;

	while(finished) {
		finished = false;

		for(int i = 0; i < data->nE; i++) {
            if(data->enemies[i].getX() == data->target.getX() && data->enemies[i].getY() == data->target.getY()) {
				damage += data->enemies[i].getDmg();

                for(int k = i; k < data->nE-1; k++) {
                    this->destroyEnemy(data, k);
                }

				data->nE--;
                finished = true; 
                
                break;
            } else { continue; }
        }
	}

	data->target.setHp(data->target.getHp()-damage);
};

void Logic::destroyEnemy(Data *data, int j) {
    for(int k = j; k < data->nE-1; k++) {
        data->enemies[k].setup(data->enemies[k+1].getId(),
            data->enemies[k+1].getX(), data->enemies[k+1].getY(),
            data->enemies[k+1].getHp(), data->enemies[k+1].getDmg(),
            data->enemies[k+1].getReward());

        data->enemies[k].calculatePath(data->dim, data->target.getX(), data->target.getY(), data->fence.getX(), data->fence.getY(), data->fieldSchema);
    }
};

// PATHS HANDLING

void Logic::calculatePaths(Data *data) {
	for(int i = 0; i < data->nE; i++) {
		data->enemies[i].calculatePath(data->dim, data->target.getX(), data->target.getY(), data->fence.getX(), data->fence.getY(), data->fieldSchema);
	}
};

// SOME ADDITIONAL FUNCTIONALITY

void Logic::constructSchema(Data *data) {
    bool conv;

	for(int i = 0; i < data->dim; i++) {
		for(int j = 0; j < data->dim; j++) {
            conv = false;
		
            for(int k = 0; k < data->nE; k++) {
				if(data->enemies[k].getY()-1 == i && data->enemies[k].getX()-1 == j) {
					fieldStateSchema[i*data->dim+j] = 'w';

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < data->nT; k++) {
				if(data->traps[k].getY()-1 == i && data->traps[k].getX()-1 == j) {
					if(!data->traps[k].getType()) {
						fieldStateSchema[i*data->dim+j] = '@';
					} else {
						fieldStateSchema[i*data->dim+j] = '#';
					}

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < data->nR; k++) {
				if(data->rangers[k].getY()-1 == i && data->rangers[k].getX()-1 == j) {
					fieldStateSchema[i*data->dim+j] = 'R';

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < data->nB; k++) {
				if(data->baits[k].getY()-1 == i && data->baits[k].getX()-1 == j) {
					fieldStateSchema[i*data->dim+j] = '.';

					conv = true;
                    break;
				} else { continue; }
			}

			if(conv) { continue; }

			if(data->fence.getY()-1 == i && data->fence.getX()-1 == j) {
				fieldStateSchema[i*data->dim+j] = ':';

                conv = true;
			}

			if(conv) { continue; }

			if(data->target.getY()-1 == i && data->target.getX()-1 == j) {
                fieldStateSchema[i*data->dim+j] = 'H';
            } else if(data->field[i*data->dim+j].getT()) {
                fieldStateSchema[i*data->dim+j] = '~';
            } else {
                fieldStateSchema[i*data->dim+j] = 't';
            }
		}

		cout << '\n';
	}
};

void Logic::setupSpawns(Data *data) {
	int valid;
	int x, y;

    for(int i = 0; i < 4; i++) {
        valid = false;

        while(!valid) {
            switch(i) {
                case 0:
                    x = 1; y = rand() % data->dim + 1;
                    break;
                case 1:
                    x = data->dim; y = rand() % data->dim + 1;
                    break;
                case 2:
                    x = rand() % data->dim + 1; y = 1;
                    break;
                case 3:
                    x = rand() % data->dim + 1; y = data->dim;
                    break;
            }

		    if(data->field[(y-1)*data->dim+(x-1)].getT()) {
			    data->spawns[i][1] = x;
			    data->spawns[i][0] = y;

			    valid = true;
		    }
	    }
    }
};

void Logic::addEnemies(Data *data) {
	data->nE = 4;

	for(int i = 0; i < data->nE; i++) {
		data->enemies[i].setup(rand(), data->spawns[i][1], data->spawns[i][0], 4.0f, 1.0f, 0.1f);
	}
};
