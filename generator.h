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
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

using namespace std;


class Generator: public Object  {
    private:
        int id  = 0;

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
}

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
				        for(int j = y*8; j < 8+y*8; j++) {
					        field[i][j] = tile1[i][j];
				        }
			        }

			        break;
		        case 1:
                    for(int i = y*8; i < 8+y*8; i++) {
                        for(int j = y*8; j < 8+y*8; j++) {
                            field[i][j] = tile2[i][j];
                        }
                    }

			        break;
		        case 2:
                    for(int i = y*8; i < 8+y*8; i++) {
                        for(int j = y*8; j < 8+y*8; j++) {
                            field[i][j] = tile3[i][j];
                        }
                    }

			        break;
		        case 3:
                    for(int i = y*8; i < 8+y*8; i++) {
                        for(int j = y*8; j < 8+y*8; j++) {
                            field[i][j] = tile4[i][j];
                        }
                    }

                    break;
            }
        }
    }
}

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
}
