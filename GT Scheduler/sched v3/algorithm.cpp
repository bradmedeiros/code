#include "algorithm.h"

/*
int numClasses;
	int time;
	int day;
	int tier;

	int* placedClasses;
	int lenPC;

	schedule sched;
	conflict* con;
	algorithm(conflict* con);
	*/

//schedule sched;
//conflict* con;
//algorithm(conflict* con);
/*void printInfo();
	void evaluateConflicts();
	void addConflict(string first, string second); //increments conflict table by 1
	bool calculateTotalConflicts(); // returns false if there are no more conflicts
	
	int nextHighIndex(int* notTheseIndices,int length);
	void reevaluateConflicts(int indexClass); // revelate the conflcit matrix as if the class specified at index had just been added (so dont count these conflicts)
	*/
algorithm::algorithm(conflict* con){
	this->con = con;
	time = 0; numPlaced = 0;

	numClasses = con->numConflicts.size();
	placedClasses = new int[numClasses];
}

void algorithm::changeConflict(conflict* con){
	this->con = con;
	delete[] placedClasses;
	time = 0; numPlaced = 0;
		

	numClasses = con->numConflicts.size();
	placedClasses = new int[numClasses];
}

void algorithm::maincontrol(int time){

	//int time = 18; // duration--> note this is off the begginnign to time the last class can start
	//high score = 17 without = 21
	for (int i=0;i<time;i++){
		addTiers(0,i);
		
	}
	for (int i=0;i<time;i++){
		addTiers(1,i);
		
	}
	for (int i=0;i<time;i++){
		addTiers(2,i);
		
	}
	for (int i=0;i<time;i++){
		addTiers(3,i);
		
	}




	
	
	//cout<<"isFinished?:  "<<isFinished()<<endl;
	
	//con->printInfo();
	//con->printInfo2();
	//cout<<"Printing Schedule"<<endl;
	//sched.printInfo();
	//cin.get();
	
	//cout<<"num of classes is:  "<<numClasses<<endl;
	//cout<<"classesplaced  is: "<<lenPC<<endl;
	
	//int addClass(int day, int hour, gtclass* gtclass);
}

bool algorithm::hasConflict(int classOne, int classTwo){
	if ((con->conflicts2[classOne][classTwo])>0 || (con->io->classProfList[classOne]==con->io->classProfList[classTwo])){
		return true;
	}else{
		return false;
	}
}

bool algorithm::hasConflict(int classOne, int* classes, int length){
	for (int i=0;i<length;i++){
		if (hasConflict(classOne,classes[i])){
			return true;
		}
	}
	return false;
}




//still need to check for internal conflicts
void algorithm::addTiers(int day, int time){

	int placedGood; 
	gtclass* nextClass;

	int dontPlaceThese[500]; // a big buffer cause im lazy and had problems doing dynamic memory allocation oh well  it's ok
	int lenDPT = numPlaced;

	int consecutiveClasses[6];// 5 possible other classes to check against--need only these cause dont wanna check for conflicts against every placed class
	int lenCC = 0;

	int placedNow[6];
	int lenPN = 0;
	
	for (int z=0;z<numPlaced;z++){
		dontPlaceThese[z] = placedClasses[z]; // copying classes you shouldnt place
		//cout<<dontPlaceThese[z]<<endl;
	}

	for (int r=0;r<6;r++){ // put the consecutive classes already in sched into consecutiveclass array
		nextClass = sched.getClass(day,r,time);
		if (nextClass!=NULL){
			consecutiveClasses[lenCC] = nextClass->getclassID();
			lenCC++;
			//cout<<"added : "<<nextClass->getclassID()<<endl;
		}
	}
	///////////
	for (int n =0;n<numClasses;n++){ // this and the above chunk makes it so it will add all the guys above and below into the do not add list--> i think there might be redudancy i am a lazy coder
		for (int m=0;m<lenCC;m++){
			if (hasConflict(n,consecutiveClasses[m])){
				dontPlaceThese[lenDPT] = n;
				lenDPT++;
			}
		}
	}
	///////

	bool conflict = false;
	//int placeNext= con->nextHighIndex(placedClasses,numPlaced);
	int placeNext = con->nextHighIndex(dontPlaceThese,lenDPT); // index of the next potential class to place
	
	if(placeNext == -1){ // if no more classes to place exit function
		return;
	}
	
	nextClass = new gtclass(placeNext,con->io->classHoursList[placeNext]); // creates the class
	
	/*if (sched.canAddClass(day,time,nextClass){
		placedGood=  1;
		sched.addClass(day,time,nextClass);
	}else{
		placedGood = 0;
	}*/

	/*int getSelectorOneSch(int hour, gtclass* gtclass,bool anyDay); // coded
	int getSelectorTwoSch(int hour, gtclass* gtclass,bool anyDay); //codes
	int getSelectorThreeSch(int hour, gtclass* gtclass, bool anyDay); // coded
	int getSelectorSixSch(int hour, gtclass* gtclass, bool anyDay);  // codes*/

	int numSch = nextClass->getHours();
	//cout<<"rrr is :  "<<numSch<<endl;
	
	placedGood = sched.callSchSelector(time, nextClass);
	//placedGood = sched.addClass(day,time,nextClass); //adds the class (if it can't 0 is returned, 1 if success)
//	cin.get();
	while(placedGood!=0){ // add while you can

		if (!conflict){
			placedClasses[numPlaced] = placeNext; // add the class you just placed to the list of placed classes
			numPlaced++;

			placedNow[lenPN] = placeNext;  
			lenPN++;

			dontPlaceThese[lenDPT] = placeNext;  // dont place these classes
			lenDPT++;

			consecutiveClasses[lenCC] = placeNext; // gets the classes that are in the section--> there can be prexisting classes do to placing them in big chunks
			lenCC++;
		}
		

		
		/*if(numPlaced == numClasses){
			cout<<"all classes placed"<<endl;
		}else{
			cout<<numPlaced<<" out of "<<numClasses<<" classes placed"<<endl;
		}*/
		/*for (int l = 0;l<numPlaced;l++){
			cout<<placedClasses[l]<<"  ";
		}cout<<endl<<endl;*/

//		placeNext = con->nextHighIndex(placedClasses,numPlaced);
	/*	for (int deb= 0;deb<lenDPT;deb++){
			cout<<dontPlaceThese[deb]<<" ";
		}cout<<endl;*/

		placeNext = con->nextHighIndex(dontPlaceThese,lenDPT); // gets the next one to place = highest conflicts that arent in donot place
		
		if(placeNext == -1){
			//cout<<"end now"<<endl;
			break;
		}
		if(hasConflict(placeNext,consecutiveClasses,lenCC)){ //hasConflict(int classOne, int* classes, int length)
			dontPlaceThese[lenDPT] = placeNext;
			lenDPT++;
			conflict = true;
			continue;
		}else{
			conflict = false;
		}
		//cout<<"place Next is:  "<<placeNext<<endl;
		nextClass = new gtclass(placeNext,con->io->classHoursList[placeNext]); // makes the class
		//placedGood = sched.addClass(day,time,nextClass); // add the schedule to the class
		placedGood = sched.callSchSelector(time, nextClass);
		// need something that checks if this is a paired class or not
	}	
	//delete[] dontPlaceThese;
	for (int w =0;w<lenPN;w++){
		con->reevaluateConflicts(placedNow[w]); //r
		//cin.get();
		//con->reevaluateConflicts(w); 
	}
	con->calculateTotalConflicts();  //calculate sums of remainign conflicts

}


bool algorithm::isFinished(){
	if (numClasses == numPlaced){
		return true;
	}else{
		return false;
	}
}