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

        Generator fieldGenerator;
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

        int *fieldSchema { new int[256] {} };

        Data(int idV) {
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

bool Data::checkMoney(float val) {
	if(money >= val) {
		return true;
	} else {
		return false;
	}
};
