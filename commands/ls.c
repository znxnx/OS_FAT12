#include <stdio.h>
#include "cmdSupport.h"

void displayDirectory(int);

/* ls implementation */
int main(int argc, char** argv) {

	/* Input Validation */
	if(argc < 1 || argc > 2){
		printf("command supports 0-1 arguments\n");
		return -1;
	}

	/* Setup Shared Memory */
	void *shPtr;
	accessShmem(&shPtr); //passing address of the pointer, int value
	memset(CPATH.path, '\0', MAX_PATH);
	memcpy(&CPATH, shPtr, SHMEMSIZE); //read in from shared memory

	/* Actual ls Implementation */
	char** entryNames;
	int offset = 0,
		i = 0,
		numSector = CPATH.sectorNum; //for relative path handling
	FileData* entry;


	//translate to physical sec num
	if(CPATH.sectorNum == 0)
		numSector = 19;
	else
		numSector += 31;

	printf("_____________________________________________\n");
	printf("%-13s %7s %12s %6s\n", "File Name", "Type", "File Size", "FLC");
	printf("_____________________________________________\n");


	if(argc == 2){
	
		if(strcmp(argv[1], "/") == 0 || strcmp(argv[1], ".") == 0){
			displayDirectory(numSector);
			return 0;
		}


		entryNames = parsePath(argv[1]);
		
				if((numSector = getSectorOffset(tmp[i], buffer)) == -1)
					return -1
					  
		while( i < getArgc(entryNames) ){

			if((entry = searchEntries(entryNames[i], numSector)) == NULL){
				printf("specified path does not exist\n");
				return -1;
			}

			if(entry->flc == 0)
				numSector = 19;
			else
				numSector = entry->flc + 31;

			i++;
		}

		if(isFile(entry)){

			/* WHY IS ENTRY->FILENAME SHOWING EXTENSIONS WHEN USING SEARCHENTRIES() */

			//wrap into char* getType() later
			char file[] = "file";
			char dir[] = "dir";

			char* type, *name;
			if(isFile(entry))
			  type = file;
			else
			  type = dir;

			for(int i = 0; i < strlen(entry->fileName); i++){
				entry->fileName[i] = toupper(entry->fileName[i]);
				if(entry->fileName[i] == ' ')
					entry->fileName[i] = '.'; //replace dots with spaces
			}


			printf("%-13s %7s %12d %6d\n", entry->fileName, type, entry->fileSize, entry->flc);
			return 0;

		}
		else {
			displayDirectory(numSector);
			return 0;
		}

	}
	else { 

		displayDirectory(numSector);
	}

	return 0;
}


/******************************************************************************
* displayDirectory
*
* displays the entries in the specified directory 
*
* numSector: the sector number that represents the directory
*
*****************************************************************************/
void displayDirectory(int numSector){

	/* Only read in floppy if argument = 1 or is directory */
	FILE_SYSTEM_ID = fopen("./floppies/floppy2", "r+");
	if (FILE_SYSTEM_ID == NULL) {
	  printf("Could not open the floppy drive or image.\n");
	  exit(1);
	}

	char *buffer = (char*)malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

	if (read_sector(numSector, buffer) == -1) {
		printf("Something has gone wrong -- could not read the sector\n");
		return;
	}


	int offset = 0;
	FileData *nEntry, *entry;
	
	for(int i = 0; i < 16; i++){ // 16 entries per sector

		entry = nEntry; //to reset values, spaghettiiiiiii

		if( (entry = readEntry(buffer, &offset)) == NULL )
			return;

		if( isEmpty(entry) || isReserved(entry)){ //isReserved checks 0xf6
			break; //fix this later
		}
		else if( isLongFile(entry) || isDeleted(entry) ){
			continue;

		}else{
			displayLs(entry);
		}

	}
}

