#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"
#include "generator.h"

using namespace std;


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

        Generator fieldGenerator();
        int spawns[4][2];

    public:
        // ACTORS

        int dim = 16;
		Tile field[dim*dim]; 

		Enemy enemies[4]; 
        int nE = 0;

		Trap traps[dim]; 
        int nT = 0;

		Ranger rangers[dim]; 
        int nR = 0;

		Bait baits[dim]; 
        int nB = 0;

		Structure target;
		Entity fence;

        char fieldStateSchema[dim*dim];
        int *fieldSchema { new int[256] {} };

        Data(int idV) {
            srand(time(0));
            
            id = idV; roundV = 0; phase = 0; abortV = False; kills = 0;
            fieldGenerator = new Generator(rand());

            fence.setCoords(999, 999);
        }

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

        void constructSchema();
        void setupSpawns();

        float getBPrice() { return beartrap; }
		float getNPrice() { return nest; }
		float getRPrice() { return ranger; }

        void addEnemies();
};


int Data::constructSchema() {
    bool conv;

	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
            conv = false;
		
            for(int k = 0; k < nE; k++) {
				if(enemies[k].getY()-1 == i && enemies[k].getX()-1 == j) {
					fieldStateSchema[i*dim+j] = 'w';

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < nT; k++) {
				if(traps[k].getY()-1 == i && traps[k].getX()-1 == j) {
					if(!traps[k].getType()) {
						fieldStateSchema[i*dim+j] = '@';
					} else {
						fieldStateSchema[i*dim+j] = '#';
					}

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < nR; k++) {
				if(rangers[k].getY()-1 == i && rangers[k].getX()-1 == j) {
					fieldStateSchema[i*dim+j] = 'R';

					conv = true;
					break;
				} else { continue; }
			}

			if(conv) { continue; }

			for(int k = 0; k < nB; k++) {
				if(baits[k].getY()-1 == i && baits[k].getX()-1 == j) {
					fieldStateSchema[i*dim+j] = '.';

					conv = true;
                    break;
				} else { continue; }
			}

			if(conv) { continue; }

			if(fence.getY()-1 == i && fence.getX()-1 == j) {
				fieldStateSchema[i*dim+j] = ':';

                conv = true;
			}

			if(conv) { continue; }

			if(target.getY()-1 == i && target.getX()-1 == j) {
                fieldStateSchema[i*dim+j] = 'H';
            } else if(field[i*dim+j].getT()) {
                fieldStateSchema[i*dim+j] = '~';
            } else {
                fieldStateSchema[i*dim+j] = 't';
            }
		}

		cout << '\n';
	}
};

void Data::setupSpawns() {
	int valid;
	int x, y;

    srand(time(0));

    for(int i = 0; i < 4; i++) {
        valid = false;

        while(!valid) {
            switch(i) {
                case 0:
                    x = 1; y = rand() % 16 + 1;
                    break;
                case 1:
                    x = 16; y = rand() % 16 + 1;
                    break;
                case 2:
                    x = rand() % 16 + 1; y = 1;
                    break;
                case 3:
                    x = rand() % 16 + 1; y = 16;
                    break;
            }

		    if(field[(y-1)*dim+(x-1)].getT()) {
			    spawns[i][1] = x;
			    spawns[i][0] = y;

			    valid = true;
		    }
	    }

    }
};

void Data::setupField() {
	bool placed = false;
	int x, y;

    srand(time(0));

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

void Data::addEnemies() {
	nE = 4;

	for(int i = 0; i < nE; i++) {
		enemies[i].setup(rand(), spawns[i][1], spawns[i][0], 4.0f, 1.0f, 0.1f);
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
