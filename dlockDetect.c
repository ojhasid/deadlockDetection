/***************************************************************************
* File: dlockDetect.c
* Author: Sujeet Ojha
* Procedures:
* main   - function that reads input from process and resource data from
		   independent files and performs a Deadlock Detection algorithm
		   in order to determine if any processes are deadlocked.
***************************************************************************/

#include <stdio.h>
#include <stdbool.h>

/***************************************************************************
* int main( int argc, char *argv[] )
* Author: Sujeet Ojha
* Date: 24 February 2020
* Description: Initializes an allocation matrix, a request matrix, and a resource vector
* after reading data from a file.
* The function then implements a Deadlock Detection algorithm to check if any processes
* are deadlocked.
*
* Parameters:
* argc I/P int The number of arguments on the command line
* argv I/P char *[] The arguments on the command line
* main O/P int Status code (not currently used)
**************************************************************************/

int main()
{
	
	int numberProcesses;
	int numberResources;
	printf("\nPlease enter the total number of processes: ");
	scanf("%d",&numberProcesses); 												/* Gathering the number of processes */
	printf("\nPlease enter the number of resources for each process: ");
	scanf("%d",&numberResources); 												/* Gathering the number of resources */

	int allocationMatrix[numberProcesses][numberResources]; 					/* Dynamic allocation of the allocation matrix */
	int requestMatrix[numberProcesses][numberResources]; 						/* Dynamic allocation of the request matrix */
	int availableVector[numberResources]; 										/* Dynamic allocation of the available vector */
	int resourceVector[numberResources]; 										/* Dynamic allocation of the resource vector */
	int tempVector[numberResources]; 											/* Dynamic allocation of the temporary vector */
	int markedVector[numberProcesses]; 											/* Dynamic allocation of the vector used to mark processes */


	for (int i = 0; i < numberProcesses; i++){ 									/* Initializing all processes as unmarked */
    
    	markedVector[i] = 0;
    
	}
	
	int option = -1;
	
	printf("\n");																/* Letting the user select if file name is inputted or initialized automatically */
	printf("Selection Menu\n");
	printf("[0] - Type in file names\n");
	printf("[1] - Automatically initialize file names\n");
	printf("Please enter option 0 or 1: ");
	scanf("%d", &option);
	printf("\n");
	
	char rVectorText[100] = "resourceVector.txt";
	char rMatrixText[100] = "requestMatrix.txt";
	char aMatrixText[100] = "allocationMatrix.txt";
	
	if (option == 0){															/* If user chooses to type in file names */
		
		printf("Please enter the text file name for the Resource Vector: ");	/* Storing file name for Resource Vector */
		scanf("%s", rVectorText);
		printf("\n");
		
		printf("Please enter the text file name for the Request Matrix: ");		/* Storing file name for Request Matrix */
		scanf("%s", rMatrixText);
		printf("\n");
		
		printf("Please enter the text file name for the Allocation Matrix: ");	/* Storing file name for Allocation Matrix */
		scanf("%s", aMatrixText);
		printf("\n");
	}
	
	FILE *rVector = fopen(rVectorText, "r"); 									/* Opening file in read mode */
	FILE *rMatrix = fopen(rMatrixText, "r"); 									/* Opening file in read mode */
	FILE *aMatrix = fopen(aMatrixText, "r"); 									/* Opening file in read mode */

	int digit;

	
	printf("\nResource Vector Display:\n");

	for(int i = 0; i < numberResources; i++){ 									/* Gathering resource vector contents from the file */

		fscanf(rVector, "%d", &digit);
		resourceVector[i] = digit;
		printf("%d ", digit); 													/* Printing resource vector contents */
		
	}

	fclose(rVector);															/* Closing the file used to read resource vector contents */
	
	printf("\n");

	

	printf("\nRequest Matrix Display:\n");

	for (int i = 0; i < numberProcesses; i++){ 									/* Gathering request matrix contents from the file */

		for(int j = 0; j < numberResources; j++){
	
			fscanf(rMatrix, "%d", &digit);
			requestMatrix[i][j] = digit;
			printf("%d ", digit); 												/* Printing request matrix contents */
	
		}
	
		printf("\n");

	}

	fclose(rMatrix); 															/* Closing the file used to read request matrix contents */

	

	printf("\nAllocation Matrix Display:\n");

	for (int i = 0; i < numberProcesses; i++){ 									/* Gathering allocation matrix contents from the file */

		for(int j = 0; j < numberResources; j++){
	
			fscanf(aMatrix, "%d", &digit);
			allocationMatrix[i][j] = digit;
			printf("%d ", digit); 												/* Printing allocation matrix contents */
	
		}
	
		printf("\n");
	
	}

	fclose(aMatrix); 															/* Closing the file used to read allocation matrix contents */

	

	for(int j = 0; j < numberResources; j++){ 									/* Calculating the available vector contents */
	
		availableVector[j] = resourceVector[j];
	
		for(int i = 0; i < numberProcesses; i++){
	
			availableVector[j] = availableVector[j] - allocationMatrix[i][j];

		}
	}
	
	

	printf("\nAvailable Vector Display:\n");

	for(int i = 0; i < numberResources; i++){ 									/* Printing available vector contents */									
		
		printf("%d ", availableVector[i]);
												
	}
	
	printf("\n");

	int markedProcess = 0;
	int curr = 0;

	
	for(int i = 0; i < numberProcesses; i++){ 									/* Marking processes with 0 total allocation */
 	
 		for(int j = 0; j < numberResources; j++){

			curr++;
			
   	 		if(allocationMatrix[i][j] == 1){
    	
    			curr = 0;
    			break;
    	
  	  		}  
  	  		
  	  	}
    
   	 if (curr == numberResources){ 												/* If all resources in a process are 0, mark the process */
   	 
    		markedVector[markedProcess] = 1;
    		
  	  }
    
  	  markedProcess++;
    
	}

	for(int j = 0; j < numberResources; j++){ 									/* Initializing a temporary vector equal to the available vector */

		tempVector[j] = availableVector[j];
	
	}


	bool processThis = false;
	
	for (int i = 0; i < numberProcesses; i++){ 									/* Finding an unmarked process i where the ith row of requestMatrix is <= temporaryVector */
	
		if (markedVector[i] == 0){
	
			for (int j = 0; j < numberResources; j++){
	
		
				if (requestMatrix[i][j] <= tempVector[j]){
				
					processThis = true;
			
				}
			
				else{ 															/* Condition not satisfied, therefore, move on to next process */

					processThis = false;
					break;
			
			
				}
	
	
			}
		
			if (processThis == true){ 											/* If condition is satisfied, mark the process and add corresponding row to the temporary vector */
		
				markedVector[i] = 1;
				
				for (int j = 0; j < numberResources; j++){
			
					tempVector[j] = tempVector[j] + allocationMatrix[i][j];
			
				}
			
		
			}
	
	
		}

	}

	bool deadlock = false;

	printf("\n");

	for (int i = 0; i < numberProcesses; i++){ 									/* Checking if any process is deadlocked */

		if (markedVector[i] == 0){ 												/* If processes are deadlocked, state which process is deadlocked */
	
			printf("Process %d is deadlocked.\n", i+1);
			deadlock = true;
		
		}
	}

	printf("\n");

	if (deadlock == false){ 													/* If no deadlock exists, state that there are no deadlocks */

		printf("\nThere are no deadlocks in this system.");

	}
}

