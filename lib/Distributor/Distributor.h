#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

void distributorPill1();
void distributorPill2();
void distributorCenter();
void servoInit();
void servoOff();
void distribute(int cylinder = 0);
bool endDistributor();

#endif