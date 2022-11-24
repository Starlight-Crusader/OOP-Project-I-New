#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "classes.h"
#include "simualtion.h"
#include "visualisation.h"
#include "data.h"

using namespace std;


int main() {
	int option1, option2, t; bool check;
	unsigned int second = 1000000;
	srand(time(0));

	Generator fieldGenerator;

	PriceList pl(rand()); OptionsLists ol(rand());
	Game session(rand(), 16, 1, 0.41f, 0, false, 0);
	session.setupField();

	session.setupSpawns();

	while(!session.getAbort()) {
		if(!session.getPhase()) {
			system("clear");

			session.addEnemies(); session.calculatePaths();
			session.printPhase(); session.printStats();
			session.drawState(); ol.printOptionsMain();

			scanf("%i", &option1);
			cout << '\n';

			switch(option1) {
				case 0: session.displayPaths(); break;
				case 1:
					if(!session.checkMoney(pl.getBPrice())) {
						cout << "ERROR: You have not enough money!\n";
						usleep(2*second);

						break;
					} else {
						ol.printOptionsBuy();

						scanf("%i %i", &option1, &option2);

						if(option1 == 0 && option2 == 0) { break; }

						if(!session.checkTrail(option1, option2)) {
							cout << "ERROR: You are not allowed to place traps in trees!\n";

							usleep(2*second);
							break;
						}

						if(session.checkOcc(option1, option2)) {
							cout << "ERROR: This tile is not empty!\n";

							usleep(2*second);
							break;
						}

						check = false;
						for(int i = 0; i < 4; i++) {
							if(session.spawns[i][1] == option1 && session.spawns[i][0] == option2) {
								cout << "ERROR: You are not allowed to place traps on the spawns!\n";
								check = true;

								usleep(2*second);
							}

							if(check) { break; }
						}

						if(check) {
							break;
						} else {
							session.addTrap(0, option1, option2);
	                        session.setM(session.getM()-pl.getBPrice());

							break;
						}
					}
				case 2:
					if(!session.checkMoney(pl.getNPrice())) {
                        cout << "ERROR: You have not enough money!\n";
                        usleep(2*second);

						break;
                    } else {
                        ol.printOptionsBuy();

                        scanf("%i %i", &option1, &option2);

                        if(option1 == 0 && option2 == 0) { break; }

                        if(!session.checkTrail(option1, option2)) {
                            cout << "ERROR: You are not allowed to place traps in trees!\n";

                            usleep(2*second);
                            break;
                        }

                        if(session.checkOcc(option1, option2)) {
                            cout << "ERROR: This tile is not empty!\n";

                            usleep(2*second);
                            break;
                        }

                        check = false;
                        for(int i = 0; i < 4; i++) {
                            if(session.spawns[i][1] == option1 && session.spawns[i][0] == option2) {
                                cout << "ERROR: You are not allowed to place traps on the spawns!\n";
                                check = true;

                                usleep(2*second);
                            }

                            if(check) { break; }
                        }

                        if(check) {
                            break;
                        } else {
                            session.addTrap(1, option1, option2);
                            session.setM(session.getM()-pl.getNPrice());

                            break;
                        }
                    }
				case 3:
					if(!session.checkMoney(pl.getRPrice())) {
                        cout << "ERROR: You have not enough money!\n";
                        
                        usleep(2*second);
						break;
                    } else {
                        ol.printOptionsBuy();

                        scanf("%i %i", &option1, &option2);

                        if(option1 == 0 && option2 == 0) { break; }

                        if(session.checkTrail(option1, option2)) {
                            cout << "ERROR: You have to place rangers in the trees!\n";

                            usleep(2*second);
                            break;
                        }

                        if(session.checkOcc(option1, option2)) {
                            cout << "ERROR: This tile is not empty!\n";

                            usleep(2*second);
                            break;
                        }

                        session.addRanger(option1, option2);
                        session.setM(session.getM()-pl.getRPrice());

                        break;
                    }
				case 4:
					ol.printOptionsBuy();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) {
                        break;
                    }

                    if(!session.checkTrail(option1, option2)) {
                        cout << "ERROR: You have to place baits on in the roads!\n";

                        usleep(2*second);
                        break;
                    }

                    if(session.checkOcc(option1, option2)) {
                        cout << "ERROR: This tile is not empty!\n";

                        usleep(2*second);
                        break;
                    }

                    session.addBait(option1, option2);
                    break;
                case 5:
					ol.printOptionsBuy();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) {
                        break;
                    }

                    if(!session.checkTrail(option1, option2)) {
                        cout << "ERROR: You have to place the fence on in the road!\n";

                        usleep(2*second);
                        break;
                    }

                    if(session.checkOcc(option1, option2)) {
                        cout << "ERROR: This tile is not empty!\n";

                        usleep(2*second);
                        break;
                    }

                    session.addFence(option1, option2);
                    session.calculatePaths();

					break;
				case 6:
					ol.printOptionsSell();

                    scanf("%i %i", &option1, &option2);

                    if(option1 == 0 && option2 == 0) { break; }

					t = session.sellDefender(option1, option2);

					switch(t) {
						case 0: session.setM(session.getM()+pl.getBPrice()/2); break;
						case 1: session.setM(session.getM()+pl.getNPrice()/2); break;
						case 2: session.setM(session.getM()+pl.getRPrice()/2); break;
						case 3: break;
						case 4: break;
						case -1: usleep(2*second); break;
					}

					break;
				case 7:
					session.setPhase(1); break;
				case 8:
					system("clear"); session.printFinalStats(); return 0;
			}
		} else {
			while(session.getE() > 0) {
				system("clear");
				session.printPhase();
				session.enemiesMove();
				cout << '\n';
				session.drawState();
				// usleep(1*second);

				system("clear");
				session.printPhase();
				session.defendersMove();
				cout << '\n';
				session.drawState();
				usleep(1*second);

				session.checkFinish();
			}

			session.setPhase(0);
			session.setRound(session.getRound()+1);

			if(session.checkGO()) {
				session.setAbort(true);
			} else {
				session.setupSpawns();
			}
		}
	}

	system("clear"); session.printFinalStats();

	return 0;
};
