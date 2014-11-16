#include <iostream>
#include <fstream>
#include <string>
//#include "gtclass.h"
//#include "schedule.h"
//#include "fileIO.h"

#include "algorithm.h"
//#define DEBUG 1

using namespace std;


#define DEBUG 1


int  helpMain(int time, int factor){
#ifdef DEBUG
	 //cout<<"debug mode"<<endl;
#endif 

#ifndef DEBUG
	 //cout<<"debug mode off"<<endl;
#endif 

	
	//gtclass a;
	//schedule q;
	gtclass oneB(0,1);
	gtclass oneC(1,1);
	gtclass oneD(2,1);
	//q.addClass(1,0,&b);
	//q.printInfo();
	//gtclass* c = q.getClass(1,0,2);
	//c->printInfo();


	fileIO io;
	//string appl = "apple";
	//string b = io.toUpper(appl);
	//cout<<"b is: "<<b<<endl;
	//void readCourseFile(string filename);
	//void readStudentFile(string filename);
	io.readStudentFile("summerStudentsEasy.csv");
	io.readCourseFile("summerCoursesEasy.csv");
	//cout<<io.numStudents<<endl;
	//cout<<io.numClasses<<endl;
	conflict con(&io);
	
	
	int arr[10];
	arr[0] = 0;
	arr[1] = 4;
	arr[2] = 5;
	arr[3] = 1;
	arr[4] = 2;
	arr[5] = 3;
	int length = 6;
	
	//int b = con.nextHighIndex(arr,length);
	//cout<<"net highet:  "<<b<<endl;
	//io.printInfo();
	 algorithm al(&con);

	 con.randomize(factor);

	 	 con.printInfo2();
	for (int i=0;i<10;i++){


		if (i==0){break;}
	}
	 //bool canAddClass(int day, int hour, gtclass* gtclass);
	//bool canAddClass(int day, int tier, int hour, gtclass* gtclass);
	al.maincontrol(time);
	/*int output = al.sched.addClass(0,0,&oneB);
	al.sched.addClass(0,0,&oneB);
	al.sched.addClass(0,0,&oneB);
	al.sched.addClass(0,0,&oneB);
	al.sched.addClass(0,0,&oneB);
	al.sched.addClass(0,0,&oneB);*/
	//output = al.sched.canAddClass(0,2,&oneC);
	//cout<<"can add class:  "<<output<<endl;
	
	//cout<<"conflict ?:  "<<al.hasConflict(2,aa,2)<<endl;
	//con.printInfo();
	//al.sched.printInfo();
//	cout<<con.nextHighIndex(aa,2);
	//con.printInfo2();
	//cout<<"is finished:  "<<al.isFinished()<<endl;
	//cout<<al.numPlaced<<endl;
	//con.printInfo();
	//con.printInfo2();
	//cin.get();
	if (al.isFinished()){
		al.sched.printInfo();
		al.sched.printCSV();
		return 0;
	}else{
		return 1;
	}

	

}

int main(){

	bool win = false;
	for (int i=0;i<400;i++){
		cout<<"loop:  "<<i<<endl;
		if(helpMain(21,7)==0){ // as the factor increases, more randomization = more possibilities = set more likely to have success in set, but method of placing takes longer
			cout<<"you win"<<endl; // time on left goes shorter, then harder to schedule
			win = true;
			break;
		}
	}
	if(win){
		cout<<"success"<<endl;
	}else{
		cout<<"failure"<<endl;
	}

	return 0;
}

// what youre doing:
// finish off add tiers- needs to check to make sure its not adding conflicting classes together
// then just call add tiers a bunch, increment time, yadada, fill up
// code for stuff like adding pairing classes (modify adding function to see 3 hour class--> go break pair, etc.), those awkward ones where you cut the class in half (modify conflict matrix for these)
// then its about optimization