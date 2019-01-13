// //LIBRARIES// //

#include <stdio.h>  
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

// //SUPPORTIVE PRECALLS// //

//predeclare useful methods defined below
int*scanFile(const char*filename);
int countLines(const char*filename);
void seekerMessage(int*sum,float*avg,int*q_size);
int*orderData(int*array);

int FCFS_init(int*headptr,int*ptr);
int SSTF_init(int*headptr,int*ptr);
int Scan_init(int*headptr,int*ptr);
int C_Scan_init(int*headptr,int*ptr);
int Look_init(int*headptr,int*ptr);
int C_Look_init(int*headptr,int*ptr);

//general boundary constant variables
const int COUNT_CYL = 5000, MAX_REQ = 1000, MIN_REQ = 0;

// //SCHEDULES// //

//FCFS//
int FCFS_init(int*headptr,int*ptr){
	
	//declare variables for headmovement seeking
	int tempHead = ptr[*headptr];
	int totalmov=0;
	
	//headmovement seeking operation from head to maximum position
	for(int c=*headptr+1; c<MAX_REQ; c++){
		totalmov = totalmov + abs(ptr[c]-tempHead);
	}

	//headmovement seeking operation from minimum position to head
	for(int c=MIN_REQ; c<*headptr-1; c++){
		totalmov = totalmov + abs(ptr[c]-tempHead);
	}
  return totalmov;

}

//SSTF//

int SSTF_init(int*headptr,int*ptr){

	//declare variables for headmovement seeking
	ptr = orderData(ptr);

	int headMinPos=*headptr, lambdaLowerBoundary=0;
	int headMaxPos=headMinPos+1, lambdaUpperBoundary=0;

	int totalmov=0;
	int tempHeadPos=*headptr;

	//get headmovement seeking amount in a boundaries and head positions
	for(int totalReq=MAX_REQ-2; totalReq>=0; totalReq--){
		//set boundaries
		lambdaLowerBoundary = abs(ptr[tempHeadPos]-ptr[headMinPos]);
		lambdaUpperBoundary = abs(ptr[tempHeadPos]-ptr[headMaxPos]);
		
		//check movement from head to upper boundary
		if(lambdaLowerBoundary>=lambdaUpperBoundary){
			totalmov = totalmov + lambdaUpperBoundary;
			tempHeadPos = headMaxPos++;
		}
		//check movement from lower boundary to head boundary
		else if(lambdaLowerBoundary<lambdaUpperBoundary){
			totalmov = totalmov + lambdaLowerBoundary;
			tempHeadPos = headMinPos--;
		}
	}
	
	return totalmov;
}

//Scan//

int Scan_init(int*headptr,int*ptr){
	//variables
  int lambda=0, totalmov=0;
	int tempData = ptr[*headptr];
	
	//find movement from minimum position to head
	for(int c=(*headptr)-1; c>=0; --c){
		int dump = ptr[c];
		lambda = abs(tempData-dump);
		totalmov = totalmov + lambda;
		tempData = dump;
	}
	//head movement so far
	totalmov = totalmov + tempData;  
	
	//find movement from head position to maximum position
	for(int c=(*headptr)+1; c<MAX_REQ; c++){
		int dump = ptr[c];
		lambda = abs(dump-0);
		totalmov = totalmov + lambda;
		tempData = dump;
	}
	return totalmov;
}

//C-Scan//

int C_Scan_init(int*headptr,int*ptr){

	//variables
	int tempData = ptr[*headptr];
	int totalmov=0;

  	//find movement from head to maximum position
	for(int c=(*headptr)+1; c<MAX_REQ; c++){
		int dump = ptr[c];
		totalmov = totalmov + abs(tempData-dump);
		tempData = dump;
	}

	//head movement so far
	totalmov = totalmov + 2*(COUNT_CYL-1) - tempData;
	
	//find movement from minimum position to head
	for(int c=MIN_REQ; c<*headptr; c++){
		int dump = ptr[c];
		totalmov = totalmov + abs(dump-MIN_REQ);
		tempData = dump;
	}
	return totalmov;
}

//LOOK//

int Look_init(int*headptr,int*ptr){
 
	//variables
	int tempData = ptr[*headptr];
	int totalmov=0;

  	//find movement from head to maximum position
	for(int c=(*headptr)+1; c<MAX_REQ; c++){
		int dump = ptr[c];
		totalmov = totalmov + abs(tempData-dump);
		tempData = dump;
	}
	
	//find movement from minimum position to head
	for(int c=*headptr-1; c>=0; --c){
		int dump = ptr[c];
		totalmov = totalmov + abs(tempData-dump);
		tempData = dump;
	}
	return totalmov; 
}

//C-Look//

int C_Look_init(int*headptr,int*ptr){
  
	//variables
	int tempData = ptr[*headptr];
	int totalmov=0;

  	//find movement from head to maximum position
	for(int c=(*headptr)+1; c<MAX_REQ; c++){
		int dump = ptr[c];
		totalmov = totalmov + abs(tempData-dump);
		tempData = dump;
	}

	//find movement from minimum position to head
	for(int c=0; c<*headptr; c++){
		int dump = ptr[c];
		totalmov = totalmov + abs(tempData-dump);
		tempData = dump;
	}
	return totalmov;
}

// //SUPPORTIVE FUNCTIONS// //
int*init_schedule(int*headptr, int*ptr, int opmod){
	static int schedule_total[6];
	
	//statistics mode
	if(!opmod){
		printf("Maximum request boundary: %d\n", MAX_REQ);
		printf("Minimum request boundary: %d\n", MIN_REQ);
		printf("Cylinder: %d\n\n", COUNT_CYL);

		printf("FCFS: %d head movements\n", FCFS_init(headptr,ptr));
		printf("SSTF: %d head movements\n", SSTF_init(headptr,ptr));
		printf("SCAN: %d head movements\n", Scan_init(headptr,ptr));
		printf("C-SCAN: %d head movements\n", C_Scan_init(headptr,ptr));
		printf("LOOK: %d head movements\n", Look_init(headptr,ptr));
		printf("C-LOOK: %d head movements", C_Look_init(headptr,ptr));
		
	}	
	
	//summation mode
	else{
		schedule_total[0] = FCFS_init(headptr,ptr);
		schedule_total[1] = SSTF_init(headptr,ptr);
		schedule_total[2] = Scan_init(headptr,ptr);
		schedule_total[3] = C_Scan_init(headptr,ptr);
		schedule_total[4] = Look_init(headptr,ptr);
		schedule_total[5] = C_Look_init(headptr,ptr);
	}
	return schedule_total;
}

//check file existance
int fileCheck(const char * filename){

 	//test read file and return whether operation is successful
	FILE *fileFlag = NULL;
	
	if (fileFlag != (fopen(filename, "r")))	return 0;
	
	return 1;
}

//check head validity
int isheadValid(char headptr[]){
    
    //checking for non-positive numbers
    if (headptr[0] == '-' || headptr[0]=='0') return 0;
    
    //checking whether non-digit character exists
    for (int c=0; headptr[c]!= 0; c++)
        if (!isdigit(headptr[c]))	return 0;
    
    return 1;
}

/*count lines in the file to set boundary
 *this function is used to check whether the input file has 1000 requests or lines as requested
 */
int countLines(const char*filename) {
	//amount of numbers in a file                                
	FILE *dataFile = fopen(filename,"r");
	int lines=0;
	
	//line counter
	while(!feof(dataFile)){
	  if(fgetc(dataFile) == '\n')	lines++;	 
	}
	
	fclose(dataFile);
	return lines;
}

//keep the array or list in an ordered form
int*orderData(int*newArr){

	//from minimum position to maximum, order them all!
	for (int c = MIN_REQ; c < MAX_REQ; ++c) {
		//while position is minimum request to max request
		for (int k = c + 1; k < MAX_REQ; ++k) {
			//check whether first data is greater than secon
			if (newArr[c] > newArr[k]) {
				//swap operation
				int dump =  newArr[c];
				newArr[c] = newArr[c];
				newArr[k] = dump;
			}
		}
	}

	return newArr;
}

//read file
int*scanFile(const char*filename){
	
	//open file and set a size for the array of items
	FILE *myFile = fopen(filename, "r");
	const int arraySize = countLines(filename);
	
	//give warning if file has no accurate integer amount and force exit
	if(arraySize!=1000){ printf("%s","the program requires 1000 integer requests, try again!\n"); exit(EXIT_FAILURE);}
 
	//read file into array
	static int myArray[1000];
		
	//scan numbers into an array	
	for (int c = 0; c < arraySize; c++)	fscanf(myFile, "%d", &myArray[c]);
	
	return myArray;
	
}

// main function 
int main(int argc, char *argv[])  {  
	char*lineSeparator = "\n--------------------------------------\n";
	
	//check headposition validity and input absentism
	if(argc>3 || argc<2 || !isheadValid(argv[1])) { 
		printf("invalid input\n\n");
		printf("%s",lineSeparator);
		return 1;
	}	
	
	//check no file condition -> generate random requests in this section
	else if (argc == 2) {
		printf("<generating random requests>");
		printf("%s",lineSeparator);		
		
			//random generate numbers and transfer them to schedule operations
			int headpos = atoi(argv[1]);
			int bagostuff[1000];
			int*totalAnalyzer=NULL;
			int FCFS_total=0,	SSTF_total=0, Scan_total=0,	C_Scan_total=0,	Look_total=0,	C_Look_total=0;	
			int FCFS_avg=0,	SSTF_avg=0, Scan_avg=0,	C_Scan_avg=0,	Look_avg=0,	C_Look_avg=0;			
			int FCFS_std=0,	SSTF_std=0, Scan_std=0,	C_Scan_std=0,	Look_std=0,	C_Look_std=0;			
			
			//set time null to prevent same randomized sequence everytime when program restarts
			
			srand(time(0));
					
			//create array of random numbers
				for(int c = MIN_REQ; c<MAX_REQ; c++)	{
					bagostuff[c] = rand() % COUNT_CYL;
					printf("\nbagostuff[%i] = %i",c ,bagostuff[c]);	
				}		
			
			//process head input and array of random variables to scheduling
			init_schedule(&headpos,bagostuff,0);
			
			//calculate total head movement
			for(int counter=0;counter<2;counter++){
				//create array of random numbers

				for(int c = MIN_REQ; c<MAX_REQ; c++)	{
					
					while(rand()%COUNT_CYL == bagostuff[c]);
					bagostuff[c] = rand() % COUNT_CYL;
					printf("[%i] %i\n",c,bagostuff[c]);
				}
				totalAnalyzer = init_schedule(&headpos,bagostuff,1);
				
				//update total for each schedule
				FCFS_total=FCFS_total + totalAnalyzer[0];
				SSTF_total=SSTF_total + totalAnalyzer[1];
				Scan_total=Scan_total + totalAnalyzer[2];
				C_Scan_total=C_Scan_total + totalAnalyzer[3];
				Look_total=Look_total + totalAnalyzer[4];
				C_Look_total=C_Look_total + totalAnalyzer[5];		
															
			}
			
			//mean calculation, mean = total movement / 100 trials
			FCFS_avg=FCFS_total/100;	SSTF_avg=SSTF_total/100; 
			Scan_avg=Scan_total/100;	C_Scan_avg=C_Scan_total/100;
			Look_avg=Look_total/100;	C_Look_avg=C_Look_total/100;			
									
			//print average of schedules
			printf("\n\naverage FCFS: %i",FCFS_avg);
			printf("\naverage SSTF: %i",SSTF_avg);
			printf("\naverage SCAN: %i",Scan_avg);
			printf("\naverage C-SCAN: %i",C_Scan_avg);
			printf("\naverage LOOK: %i",Look_avg);
			printf("\naverage C-LOOK: %i",C_Look_avg);
			
			//std calculation
			for(int counter=0;counter<100;counter++){
				totalAnalyzer = init_schedule(&headpos,bagostuff,1);
				
				//std = sqrt( Σ |each reported total head movement - mean| / 100 trials)
				
				FCFS_std=FCFS_std + abs(totalAnalyzer[0] - FCFS_avg);
				SSTF_std=SSTF_std + abs(totalAnalyzer[1] - SSTF_avg);
				Scan_std=Scan_std + abs(totalAnalyzer[2] - Scan_avg);
				C_Scan_std=C_Scan_std + abs(totalAnalyzer[3] - C_Scan_avg);
				Look_std=Look_std + abs(totalAnalyzer[4] - Look_avg);
				C_Look_std=C_Look_std + abs(totalAnalyzer[5] - C_Look_avg);		
															
			}
			
			//print std of schedules
			printf("\n\nstd FCFS: %lf",sqrt(FCFS_std/100));
			printf("\nstd SSTF: %lf",sqrt(SSTF_std/100));
			printf("\nstd SCAN: %lf",sqrt(Scan_std/100));
			printf("\nstd C-SCAN: %lf",sqrt(C_Scan_std/100));
			printf("\nstd LOOK: %lf",sqrt(Look_std/100));
			printf("\nstd C-LOOK: %lf",sqrt(C_Look_std/100));
			
	}
	
	//check filename correctness 
	else if(fileCheck(argv[2])){
		printf("file not found");
		printf("%s",lineSeparator);
	}

	//check file exists condition without any issue -> read files in this section
	else {
		//transfer read numbers to schedule operations
		printf("using requests from the file");
		printf("%s",lineSeparator);
		
		//random generate numbers and transfer them to schedule operations
		int headpos = atoi(argv[1]);
		char*inputfile = argv[2];
		int*totalAnalyzer=NULL;
		int*bagostuff = scanFile(inputfile);
		
		int FCFS_total=0,	SSTF_total=0, Scan_total=0,	C_Scan_total=0,	Look_total=0,	C_Look_total=0;	
		int FCFS_avg=0,	SSTF_avg=0, Scan_avg=0,	C_Scan_avg=0,	Look_avg=0,	C_Look_avg=0;			
		double FCFS_std=0,	SSTF_std=0, Scan_std=0,	C_Scan_std=0,	Look_std=0,	C_Look_std=0;			
			
		init_schedule(&headpos,bagostuff,0);
		
		//calculate total 
		for(int counter=0;counter<100;counter++){
			totalAnalyzer = init_schedule(&headpos,bagostuff,1);
			
			FCFS_total=FCFS_total + totalAnalyzer[0];			SSTF_total=SSTF_total + totalAnalyzer[1];
			Scan_total=Scan_total + totalAnalyzer[2];			C_Scan_total=C_Scan_total + totalAnalyzer[3];
			Look_total=Look_total + totalAnalyzer[4];			C_Look_total=C_Look_total + totalAnalyzer[5];			
							
		}
		
		//average calculation
		FCFS_avg=FCFS_total/100;	SSTF_avg=SSTF_total/100; 
		Scan_avg=Scan_total/100;	C_Scan_avg=C_Scan_total/100;
		Look_avg=Look_total/100;	C_Look_avg=C_Look_total/100;
					
		//average of schedules
		printf("\n\naverage FCFS: %i",FCFS_avg);
		printf("\naverage SSTF: %i",SSTF_avg);
		printf("\naverage SCAN: %i",Scan_avg);
		printf("\naverage C-SCAN: %i",C_Scan_avg);
		printf("\naverage LOOK: %i",Look_avg);
		printf("\naverage C-LOOK: %i",C_Look_avg);
		
		//std calculation
		for(int counter=0;counter<100;counter++){
			totalAnalyzer = init_schedule(&headpos,bagostuff,1);
			
			//std = sqrt( Σ |each movement - mean| / 100 trials)
			FCFS_std=FCFS_std + abs(totalAnalyzer[0] - FCFS_avg);
			SSTF_std=SSTF_std + abs(totalAnalyzer[1] - SSTF_avg);
			Scan_std=Scan_std + abs(totalAnalyzer[2] - Scan_avg);
			C_Scan_std=C_Scan_std + abs(totalAnalyzer[3] - C_Scan_avg);
			Look_std=Look_std + abs(totalAnalyzer[4] - Look_avg);
			C_Look_std=C_Look_std + abs(totalAnalyzer[5] - C_Look_avg);		
														
		}
		
		// Attention to clarify the situation //
		
		/*The program will be passed the initial position of the disk head (as a parameter on the
		 *command line) and REPORT THE TOTAL AMOUNT OF HEAD MOVEMENT required by each algorithm.(part A)
		 
		 *RUN THE PROGRAM 100 different times with random input(part B)
		
		 *the same program with same integers is executed for 100 trials and for each trial, the total head movmenent is same. This means mean is as same as total head movement
		 *as we can see that the standart deviation is zero because std = sqrt( ( Σ |each total head movement - mean| ) / 100 trials )
		 */
		 
		//print std of schedules 
		printf("\n\nstd FCFS: %lf",sqrt(FCFS_std)/100);
		printf("\nstd SSTF: %lf",sqrt(SSTF_std)/100);
		printf("\nstd SCAN: %lf",sqrt(Scan_std)/100);
		printf("\nstd C-SCAN: %lf",sqrt(C_Scan_std)/100);
		printf("\nstd LOOK: %lf",sqrt(Look_std)/100);
		printf("\nstd C-LOOK: %lf",sqrt(C_Look_std)/100);

	}
	
	printf("%s",lineSeparator);
	
	return 0;
	//END OF PROGRAM
}
