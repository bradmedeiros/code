#include <iostream>
#include "conflict.h"
#include "schedule.h"
//#define DEBUG 1
#define MININTERVAL 30
using namespace std;

class algorithm{

private:	
	
public:

	int numClasses;
	int time;
	//int day;

	int* placedClasses;
	int numPlaced;

	schedule sched;
	conflict* con;
	algorithm(conflict* con);


	bool hasConflict(int classOne, int classTwo);
	bool hasConflict(int classOne, int* classes, int length);
	
	bool isFinished();

	void changeConflict(conflict* con);

	void addTiers(int day, int time);
	void maincontrol(int time);
	~algorithm(){
		delete[] placedClasses;
	}
	//schedule
	//int addClass(int day, int hour, gtclass* gtclass);
	//printInfo--> exports the cool document

	//gt class
	//gtclass(int classID, int hours);

};