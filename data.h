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
        int fieldStateSchema[dim*dim];

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


void Game::constructSchema() {
    for(int i = 0; i < dim; i++) {
        for(int i = 0; i < )
    }
};
