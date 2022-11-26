#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "components.h"

using namespace std;


int main() {
	int option1, option2, t; bool check;
	unsigned int second = 1000000;
    int tType;

	srand(time(0));
    
    Logic logic(rand());
    Graphics graphics(rand());
    Data data(rand());

	data.setupField();
	logic.setupSpawns(&data);

	while(!data.getAbort()) {
		if(!data.getPhase()) {
			system("clear");

			data.addEnemies(); logic.calculatePaths(&data);
			graphics.printPhase(data.getPhase()); session.printStats(data.getRound(), data.target.getHp(), data.getM());
			logic.constructSchema(&model); graphics.drawState(&logic.fieldStateSchema, model.dim); graphics.printOptionsMain();

			scanf("%i", &option1);
			cout << '\n';

			switch(option1) {
				case 0: graphics.displayPaths(&data.enemies); break;
				case 1:
                case 2:
                    tType = option1;

                    if(tType == 0) {
                        if(!data.checkMoney(data.getBPrice())) {
						    cout << "ERROR: You have not enough money!\n";
						    usleep(2*second);

						    break;
					    }
                    } else {
                        if(!data.checkMoney(data.getNPrice())) {
						    cout << "ERROR: You have not enough money!\n";
						    usleep(2*second);

						    break;
					    }

                    }

                    graphics.printOptionsBuy();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) { break; }

                    if(!data.checkTrail(option1, option2)) {
                        cout << "ERROR: You are not allowed to place traps in trees!\n";

                        usleep(2*second);
                        break;
                    }

                    if(data.checkOcc(option1, option2)) {
                        cout << "ERROR: This tile is not empty!\n";

                        usleep(2*second);
                        break;
                    }

                    check = false;
                    for(int i = 0; i < 4; i++) {
                        if(data.spawns[i][1] == option1 && data.spawns[i][0] == option2) {
                            cout << "ERROR: You are not allowed to place traps on the spawns!\n";
                            check = true;

                            usleep(2*second);
                        }

                        if(check) { break; }
                    }

                    if(check) {
                        break;
                    } else {
                        data.addTrap(tType, option1, option2);

                        if(tType == 0) {
                            data.setM(data.getM()-data.getBPrice());
                        } else {
                            data.setM(data.getM()-data.getNPrice());
                        }

                        break;
                    }
				case 3:
					if(!data.checkMoney(data.getRPrice())) {
                        cout << "ERROR: You have not enough money!\n";
                        
                        usleep(2*second);
						break;
                    } else {
                        graphics.printOptionsBuy();

                        scanf("%i %i", &option1, &option2);

                        if(option1 == 0 && option2 == 0) { break; }

                        if(data.checkTrail(option1, option2)) {
                            cout << "ERROR: You have to place rangers in the trees!\n";

                            usleep(2*second);
                            break;
                        }

                        if(data.checkOcc(option1, option2)) {
                            cout << "ERROR: This tile is not empty!\n";

                            usleep(2*second);
                            break;
                        }

                        data.addRanger(option1, option2);
                        data.setM(data.getM()-data.getRPrice());

                        break;
                    }
				case 4:
					graphics.printOptionsBuy();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) {
                        break;
                    }

                    if(!data.checkTrail(option1, option2)) {
                        cout << "ERROR: You have to place baits on in the roads!\n";

                        usleep(2*second);
                        break;
                    }

                    if(data.checkOcc(option1, option2)) {
                        cout << "ERROR: This tile is not empty!\n";

                        usleep(2*second);
                        break;
                    }

                    data.addBait(option1, option2);
                    break;
                case 5:
					graphics.printOptionsBuy();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) {
                        break;
                    }

                    if(!data.checkTrail(option1, option2)) {
                        cout << "ERROR: You have to place the fence on in the road!\n";

                        usleep(2*second);
                        break;
                    }

                    if(data.checkOcc(option1, option2)) {
                        cout << "ERROR: This tile is not empty!\n";

                        usleep(2*second);
                        break;
                    }

                    data.addFence(option1, option2);
                    logic.calculatePaths(&data);

					break;
				case 6:
					graphics.printOptionsSell();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) { break; }

					t = data.sellDefender(option1, option2);

					switch(t) {
						case 0: data.setM(data.getM()+data.getBPrice()/2); break;
						case 1: data.setM(data.getM()+data.getNPrice()/2); break;
						case 2: data.setM(data.getM()+data.getRPrice()/2); break;
						case 3: break;
						case 4: break;
						case -1: usleep(2*second); break;
					}

					break;
				case 7:
					data.setPhase(1); break;
				case 8:
					system("clear"); graphics.printFinalStats(data.getR()-1, data.target.getHp(), data.getK()); return 0;
			}
		} else {
			while(data.nE > 0) {
				system("clear");

				graphics.printPhase();
				logic.enemiesMove(&data);

				cout << '\n';

                logic.constructSchema(&data);
				graphics.drawState(&logic.fieldStateSchema, model.dim);

				system("clear");

				graphics.printPhase();
				logic.defendersMove(&data);

				cout << '\n';

				logic.constructSchema(&data);
				graphics.drawState(&logic.fieldStateSchema, model.dim);

				usleep(1*second);
                logic.checkFinish(&data);
			}

			data.setPhase(0);
			data.setRound(data.getRound()+1);

			if(logic.checkGO(&data)) {
				data.setAbort(true);
			} else {
				logic.setupSpawns(&data);
			}
		}
	}

	system("clear"); 
    graphics.printFinalStats(data.getR()-1, data.target.getHp(), data.getK());

	return 0;
};
