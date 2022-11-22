#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"

using namespace std;


class Data: public Object {
    private:
		// STATS

		int round;
		float money;
		int phase;
		bool abort;
		int kills;

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

    public:
        char fieldStateSchema[dim*dim];

        Data(int idV) {
            id = idV; roundV = 0; phase = 0; abortV = False; kills = 0;
        }

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

        void constructSchema();
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
