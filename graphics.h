// RESET: \u001b[0m
// RED: \u001b[31m
// GREEN: \u001b[32m
// YELLOW: \u001b[33m
// BLUE: \u001b[34m
// GREY: \u001b[37m


#include <iostream>
#include <unistd.h>
#include <unistd.h>
#include <iomanip>

#include "entities.h"

using namespace std;


class Graphics: public Object {
    public:
        Visualisation(int idV) {
            id = idV;
        }

        void printPhase(int);
		void printStats(int, float, float);
		void printFinalStats(int, float, int);

        void drawState(char*, int);
        void displayPaths(Enemy*);

        void printOptionsMain();
		void printOptionsBuy();
		void printOptionsSell();
};


int Graphics::drawState(char *fieldStateSchema, int dim) {
	for(int k = 0; k < 11; k++) { cout << ' '; }
    for(int k = 1; k <= 16; k++) { cout << k % 10; }
    cout << '\n';

	for(int i = 0; i < dim; i++) {
		for(int k = 0; k < 9; k++) { cout << ' '; }
        cout << (i + 1) % 10 << ' ';

		for(int j = 0; j < dim; j++) {
            switch(*(fieldStateSchema+i*dim+j)) {
                case 't':
                    cout << "\u001b[32mt\u001b[0m"; break;
                case '~':
                    cout << "\u001b[33m~\u001b[0m"; break;
                case 'H':
                    cout << "\u001b[34mH\u001b[0m"; break;
                case '.':
                    cout << "\u001b[34m.\u001b[0m"; break;
                case 'R':
                    cout << "\u001b[34mR\u001b[0m"; break;
                case '#':
                    cout << "\u001b[34m#\u001b[0m"; break;
                case '@':
                    cout << "\u001b[34m@\u001b[0m"; break;
                case 'w':
                    cout << "\u001b[37mw\u001b[0m"; break;
                case ':':
                    cout << "\u001b[34m:\u001b[0m"; break;
            }
		}

		cout << '\n';
	}

	return 0;
};

void Graphics::printPhase(int phase) {
	if(!phase) {
       	cout << "\n=========== PLANNIG PHASE ===========\n";
    } else {
        cout << "\n============ ACTION PHASE ===========\n";
        cout << '\n';
    }
};

void Graphics::printStats(int round, float hp, float money) {
	cout << '\n';
    cout << "-------------------------------------\n";
    cout << "               Stats:\n";
	cout << " * Round: " << round << ",\n";
    cout << " * HP: " << hp << ",\n";
    cout << " * Money: " << money << "$\n";
    cout << "-------------------------------------\n";
    cout << '\n';
};

void Graphics::printFinalStats(int round, float hp, int kills) {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << "             Final stats:\n";
    cout << " * Rounds survived: " << round-1 << ",\n";
    cout << " * HP left: " << hp << ",\n";
    cout << " * Enemies killed: " << kills << '\n';
    cout << "-------------------------------------\n";
    cout << '\n';
};

void Graphics::displayPaths(Enemy *enemies) {
	unsigned int second = 1000000;

	for(int k = 0; k < nE; k++) {
		system("clear");
        cout << "\n            ENEMY: " << *(enemies+k).getId() << "\n\n";

		*(enemies+k).displayPath(dim, *(enemies+k).getX(), *(enemies+k).getY());

		usleep(4*second);
	}
};

void Graphics::printOptionsMain() {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << "          Select an action:\n";
    cout << " 0. Highlight the paths of the en-s,\n";
    cout << " 1. Place a new beartrap (0.05$),\n";
    cout << " 2. Place a new nest (0.1$),\n";
    cout << " 3. Hire a ranger (0.2$)\n";
    cout << " 4. Place a bait (free),\n";
    cout << " 5. Construct a fence (free),\n";
    cout << " 6. Sell a defender (50% back),\n";
    cout << " 7. Start the next round,\n";
    cout << " 8. Finish the game\n";
    cout << "-------------------------------------\n";
    cout << '\n';
    cout << "OPTION: ";
};

void Graphics::printOptionsBuy() {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << " Input coord-s of a new def. in the\n";
    cout << " form: 'x y' OR enter '0 0' in order\n";
    cout << " to abort this operation ...\n";
    cout << "-------------------------------------\n";
    cout << '\n';
    cout << "INPUT: ";
};

void Graphics::printOptionsSell() {
    cout << '\n';
    cout << "-------------------------------------\n";
    cout << " Input coord-s of a def. to sell in\n";
    cout << " the form: 'x y' OR enter '0 0' in\n";
    cout << " order to abort this operation ...\n";
    cout << "-------------------------------------\n";
    cout << '\n';
    cout << "INPUT: ";
}
