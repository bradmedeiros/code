#include "MemoryManager.h"
#include <iostream>


namespace MemoryManager
{

  /*
	      The organization of the memory is as follows: 
	
	Byte: 0                             4                                             8 
		  [Size of data remaing(4 bytes), Pointer to next potential free space(4 bytes),....
		
	     	and for every data segment allocated:
		  
	Byte:    8                                      9                                 13            13+n
		  ... FLAG-X=in use, N=deallocated (1 byte), size of next data segment=n (4 bytes), data of size,...
		  .
    Byte:  13+n   14+n              18+n      18+n         MAX_POOL_SIZE
		     ...FLAG,size of next data = m, data.............]  
   */

  const int MM_POOL_SIZE = 65536;
  char MM_pool[MM_POOL_SIZE];

  void cleanMemory(); // refactors the memory so consecutive deallocated memories are joined

  inline void decrementSizeRemaining(int); // decreases memory space remaining
  inline int getIntValue(char* intPointer); //gets the int value at the 4 bytes the pointer points at
  inline void storeIntIntoChars(char*, int); //stores an int into the 4 char array elements

  // Initialize set up any data needed to manage the memory pool
  void initializeMemoryManager(void)
  {
     for (int i=0;i<MM_POOL_SIZE;i++){
		MM_pool[0] = 0;  //initilizes memory pool to 0s
	 }
   
	 storeIntIntoChars(MM_pool,MM_POOL_SIZE); // store size of free space at first 4 bytes of MM_pool array
	 storeIntIntoChars(&MM_pool[4],13); // pointer set to next free space (13=  4 size of mem pool, 4 size of next pointer, 1 size of flag, 4 size of data size) --> this is an offset value pointing to the next data
	  
	 MM_pool[8] = 'N'; // used in later logic 
	 storeIntIntoChars(&MM_pool[9],MM_POOL_SIZE-13);//set space of that unused pool space
	 decrementSizeRemaining(8); // decrement size remaining by 8 since we use 8 bytes above (the flags and size will acount for themselves in first allocation)
  
	///////////// testing code 
  char* p1 = (char*)allocate(20);
  char* p2 = (char*)allocate(30);

  char* p3 = (char*) allocate(40);

  deallocate(p1); deallocate(p2); cleanMemory();
  char* p4 = (char*)allocate(50);
  
  cleanMemory();
  std::cout<<"memory remaining is:  "<<freeRemaining()<<std::endl;
  std::cout<<"address of p1 is:  "<<(unsigned int)p1<<std::endl;
  std::cout<<"address of p2 is:  "<<(unsigned int)p2<<std::endl;
  std::cout<<"address of p3 is:  "<<(unsigned int)p3<<std::endl;
  std::cout<<"address of p4 is:  "<<(unsigned int)p4<<std::endl;

	  
	 std::cin.get();
  
  }

  // return a pointer inside the memory pool
  // If no chunk can accommodate aSize call onOutOfMemory()
  //every allocation has a 5 byte overhead
  void* allocate(int aSize)
  {
      int firstPointer = getIntValue(&MM_pool[4]);
	  
	  if (aSize > freeRemaining() || aSize > largestFree()){
	      onOutOfMemory();
	  }

	  if (freeRemaining()<10000 && ( largestFree()<1000)){ // I'm not really sure what to set these values at, an analysis  
		  // cleans up memory via merging                  // particular to how we're allocating memory would help to make this
		  cleanMemory();                                   // functionality more efficient-- eg. if we're allocating often
	  }                                                    // we might want to set these thresholds lower but if we're allocating 
														   // big chunks rarely we might set this higher
	 
	  if (firstPointer > MM_POOL_SIZE-10000){ // move pointer to beginning element if we're coming closer to the end of the pool
		  firstPointer = 9;
	  }

	  //this is the logic that will trace throught the array to find the next place we can allocate memory
	  while (MM_pool[firstPointer-5]=='X' || (MM_pool[firstPointer-5]=='N' && getIntValue(&MM_pool[firstPointer-4])<aSize)){ // this makes it so when threshold gets low i can move pointer back down
			int value = getIntValue(&(MM_pool[firstPointer-4]));
			firstPointer +=(value+5); // move pointer to element that is not in use

			if(firstPointer >=MM_POOL_SIZE-aSize-5){  // not enough memory
					//throw some sort of cant do stuff exception
				onOutOfMemory();
			}
	  }

	  
	  int dataFree = getIntValue(&MM_pool[firstPointer-4]);
	  // super duper code

	  //change location of MM_pool[4] to point to the next location
	  storeIntIntoChars(&(MM_pool[4]),firstPointer+aSize+5); // accounts for (size+flag) = 5, aSize = data, plus initial offset

	  //decrease remaining memory available
	  decrementSizeRemaining(aSize+5);

	//  std::cout<<"the value im checking here is:  "<<firstPointer+aSize<<"\n";
	  MM_pool[firstPointer-5] = 'X'; // this sets the flag to one to mark in use
	  
	 // MM_pool[firstPointer-4] = aSize; // this marks the size for later cleanup
	  storeIntIntoChars(&(MM_pool[firstPointer-4]), aSize);
	  
	  //char b = MM_pool[firstPointer-5];
	  //std::cout<<"what falue is this:  "<<b<<"\n\n";
     return ((void*) &MM_pool[firstPointer]);
  }

  // Free up a chunk previously allocated
  void deallocate(void* aPointer)
  {
	  char* pointer = (char*)aPointer;
	  pointer[-5] = 'N';  // set flag to N to say NOT IN USE
	  int size = getIntValue((char*)aPointer-4);
	 
	  //increase the size by size + 5 overhead elimnated
	  decrementSizeRemaining(-(size+5));
	
  }

  //---
  //--- support routines
  //--- 

  // Will scan the memory pool and return the total free space remaining
  int freeRemaining(void)
  {
    return getIntValue(MM_pool);
  }

  // Will scan the memory pool and return the largest free space remaining
  int largestFree(void)
  {
	  //what's happening:
      //go to first element
	  // - check flag 
	  //	- if being used-X, check size then nagivate to next element
	  //	- if not used - N, mark size, then go to next element
	  // go until the offset is going to the element with biggest free space

	 int offset = 8;
	 int datasize = 0;
	 int maxSize = 0;
	 char test =  MM_pool[8];
	 while(test=='X' || test == 'N'){
		 datasize = getIntValue(&MM_pool[offset+1]);
		 if (datasize >maxSize && test=='N'){
			 maxSize = datasize;
		 }
		offset = offset+datasize+5;
		test = MM_pool[offset];
		//std::cout<<"offset is:  "<<offset<<"\n\n";
		//std::cout<<"datasize is:  "<<datasize<<"\n\n";
	 }
	int dataRemaining=  MM_POOL_SIZE-offset;
	if (dataRemaining >maxSize){
		maxSize = dataRemaining;
	}
	maxSize-=5; //to account for overhead 
    return maxSize;
  }

  // will scan the memory pool and return the smallest free space remaining
  int smallestFree(void)
  {
    // this is the same algorithm as largestFree, just checks minimum element instead
	 int offset = 8;
	 int datasize = 0;
	 int minSize = 0x2147483647; // largest signed int value
	 char test =  MM_pool[8];
	 while(test=='X' || test == 'N'){
		 datasize = getIntValue(&MM_pool[offset+1]);
		 if (datasize <minSize && test == 'N'){
			 minSize = datasize;
		 }
		offset = offset+datasize+5;
		test = MM_pool[offset];
	 }
	int dataRemaining=  MM_POOL_SIZE-offset;
	if (dataRemaining <minSize){
		minSize = dataRemaining;
	}
	minSize-=5; //to account for overhead 
    return minSize;
  }

  // refactors the memory by combining consecutive deallocated memory segement
  // if there are two consecutive segments
   void cleanMemory(){
	 int offset = 8;
	 int datasize = 0;
	 int lastDataPointer = 9;
	 int lastDatasize = 0;
	 char lastChar = 'p';

	 int maxSize = 0;
	 int tempData;
	 char test =  MM_pool[8];
	 while(test=='X' || test == 'N'){  // if it is in use or not used 
	    tempData = offset+1; // last is the last data pointer
		datasize = getIntValue(&MM_pool[offset+1]); // gets the data size (flag + 1 byte)
		offset = offset+datasize+5; // fixes offset to point to the next flag
		test = MM_pool[offset]; // the value of the flag

		// if the element is not in use
		if (lastChar == test == 'N'){
			lastDataPointer = tempData;
			storeIntIntoChars(&(MM_pool[lastDataPointer]), MM_pool[lastDataPointer]+datasize); //+5 because overhead
		}
		lastChar = test; // the value of the last character
		lastDatasize = datasize;
	 }
	    storeIntIntoChars(&MM_pool[4],9); // resets pointer position
  }


   //packs the integer value into 4 1-byte (char) values
   inline void storeIntIntoChars(char* storage, int value){
	  storage[0] = (storage[0]&0x00000000)|(value&0xff); //  Least Significant Bit
	  storage[1] = (storage[1]&0x00000000)|((value&0xff00)>>8);
	  storage[2] = (storage[2]&0x00000000)|((value&0xff0000)>>16);
	  storage[3] = (storage[3]&0x00000000)|((value&0xff000000)>>24); // Most Significant Bit
  }


  // get the int value packs at next 4 bytes from intPointer
  inline int getIntValue(char* intPointer){
	   unsigned int a = (unsigned int)(unsigned char)intPointer[3]; //MSB
	   unsigned int b = (unsigned int)(unsigned char)intPointer[2];
	   unsigned int c = (unsigned int)(unsigned char)intPointer[1];
	   unsigned int d = (unsigned int)(unsigned char)intPointer[0]; //LSB
	   return ((a<<24)|(b<<16)|(c<<8)|d); // recombined integer from individual 1-byte chars
  }


  // decrements the size of the memors remaining by number specified
  inline void decrementSizeRemaining(int number){
		int oldMemoryRemaining = freeRemaining();
		storeIntIntoChars(&MM_pool[0],oldMemoryRemaining-number);
  }  

 }