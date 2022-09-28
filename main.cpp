#include <iostream>
#include <cstdlib>
#include <list>
#include <unistd.h>

#include "classes.h"

using namespace std;
srand(time(0));

class Game {
	private:
		// STATS
		int round;
		float money;

		// OBJECTS
		Tile field[9*9]; int dim;
		Enemy enemies[64]; int nE;
		Trap traps[64]; int nT;
		Ranger rangers[64]; int nR;
		Structure target;

	public:
		Game(int dimV, int rV, int mV) {
			dim = dimV; round = rV; money = mV;
		}

		void setupField() {
			for(int i = 0; i < dim; i++) {
				for(int j = 0; j < dim; j++) {
					field[i*dim+j].setOcc(false);
					field[i*dim+j].setT(false);
					field[i*dim+j].setId(rand());
				}
			}

			for(int i = 0; i < dim; i++) {
				field[1][i].setT(true);
				field[7][i].setT(true);

				field[i][1].setT(true);
				field[i][7].setT(true);
			}

			for(int i = 3; i <= 5 ; i++) {
				for(int j = 3; j <= 5; j++) {
					field[i][j].setT(true);
				}
			}

			field[5][2].setT(true);
			field[3][6].setT(true);

			target.setId(rand());
			target.setX(dim/2); target.setY(dim/2);
			target.setHp(10.0f);
		}

		void drawState();

		int getDim() { return dim; }

		int getR() { return rounds; }
		void setR(int rV) { round = rV; }

		float getM() { return money; }
		void setM(int mV) { money = mV; }

		int getE() { return nE; }
		void setE(int eV) { nE = eV; }

		int getT() { return nT; }
		void setE(int tV) { nT = tV; }

		int getR() { return nR; }
		void setR(int rV) { nR = rV; }
};

int main() {
	Game session(9, 0, 0.5f); session.setupField();

	return 0;
};

