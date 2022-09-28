#include <cstdlib>
#include <list>
#include <unistd.h>

#include "classes.h"

using namespace std;

class Game: public Object {
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
		Game(int idV, int dimV, int rV, int mV) {
			id = idV; dim = dimV; round = rV; money = mV;
		}

		void setupField();
		void drawState();

		void addEnemies();
		void addTrap(int, int, int);
		void addRanger(int, int);

		void enemiesMove();
		void defendersMove();

		bool checkMoney(float);

		int getDim() { return dim; }

		int getRound() { return round; }
		void setRound(int rV) { round = rV; }

		float getM() { return money; }
		void setM(int mV) { money = mV; }

		int getE() { return nE; }
		void setE(int eV) { nE = eV; }

		int getT() { return nT; }
		void setT(int tV) { nT = tV; }

		int getR() { return nR; }
		void setR(int rV) { nR = rV; }
};

class PriceList: public Object {
	private:
		float beartrap; float nest; float ranger;

	public:
		PriceList(int idV, int pB, int pN, int pR) {
			id = idV; beartrap = pB; nest = pN; ranger = pR;
		}

		float getBPrice() { return beartrap; }
		float getNPrice() { return nest; }
		float getRPrice() { return ranger; }
};

int main() {
	srand(time(0)); system("clear");

	PriceList pl(rand(), 0.05f, 0.1f, 0.2f);
	Game session(rand(), 9, 0, 0.4f); session.setupField(); session.drawState();

	return 0;
};

void Game::setupField() {
	for(int i = 0; i < dim; i++) {
        	for(int j = 0; j < dim; j++) {
                	field[i*dim+j].setOcc(false);
                       	field[i*dim+j].setT(false);
                        field[i*dim+j].setId(rand());
                }
        }

        for(int i = 0; i < dim; i++) {
        	field[1*dim+i].setT(true);
                field[7*dim+i].setT(true);

                field[i*dim+1].setT(true);
                field[i*dim+7].setT(true);
        }

        for(int i = 3; i <= 5 ; i++) {
       		for(int j = 3; j <= 5; j++) {
                	field[i*dim+j].setT(true);
                }
        }

        field[5*dim+2].setT(true);
        field[3*dim+6].setT(true);

	field[0*dim+0].setT(true);
	field[0*dim+8].setT(true);
	field[8*dim+0].setT(true);
	field[8*dim+8].setT(true);

        target.setId(rand());
        target.setCoords(dim/2+1, dim/2+1);
        target.setHp(10.0f);
};

// RESET: \u001b[0m
// RED: \u001b[31m
// GREEN: \u001b[32m
// YELLOW: \u001b[33m
// BLUE: \u001b[34m


void Game::drawState() {
	for(int i = 0; i < dim; i++) {
		for(int k = 0; k < 14; k++) {
                	cout << ' ';
                }

		for(int j = 0; j < dim; j++) {
			if(j == target.getX()-1 && i == target.getY()-1) {
				cout << "\u001b[34mH\u001b[0m";
			} else if(field[i*dim+j].getT()) {
				cout << "\u001b[33m~\u001b[0m";
			} else {
				cout << "\u001b[32m?\u001b[0m";
			}
		}

		cout << '\n';
	}
};

// AT THIS STAGE OF DEVELOPMENT, I WILL JUST SPAWN 4 WOLVES THAT WILL
// RUN FROM DIFFERENT ENDS OF THE MAP TO THE CENTER

void Game::addEnemies() {
	int spawns[4][2] = { {0, 0}, {0, 8}, {8, 0}, {8, 8} };

	nE = 4;

	for(int i = 0; i < nE; i++) {
		enemies[i].setup(rand(), spawns[i][1], spawns[i][0], 4.0f, 1.0f, 0.1f);
	}
};

void Game::addTrap(int tV, int xV, int yV) {
	switch(tV) {
		case 0:
			traps[nT].setup(rand(), xV, yV, 0, 2.0f);
			nT++;

			break;
		case 1:
			traps[nT].setup(rand(), xV, yV, 1, 4.0f);
			nT++;

			break;
	}
};

void Game::addRanger(int xV, int yV) {
	rangers[nR].setup(rand(), xV, yV, 4.0f, 1.5f, 2);
	nR++;
};

bool Game::checkMoney(float val) {
	if(money >= val) {
		return true;
	} else {
		return false;
	}
};
