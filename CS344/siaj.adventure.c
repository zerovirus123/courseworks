/*
 main.c
 CS344 program 2 that presents user with a text based interactive game, based on the Colossal Cave Adventure writen by William Crowter and Don Woods
 3rd July 2016
 
 Compile by typing in gcc -o siaj.adventure siaj.adventure.c
 Subdirectory of the format siaj.rooms.<PID> will be created. In it are the room files.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> //for formatting directories
#include <errno.h> //library with predefined C code errors
#include <sys/stat.h> //allows program to access inode information
#include <sys/types.h> //includes non-conventional data types
#include <unistd.h> //provide POSIX OS APIs (UNIX standard library)

//creates a room struct that holds some room properties, like name, number of connections, room type and array of connections
struct Room{
    
    char name[25];
    int numConnect;
    int type; //beginning=0, middle=1, end=2
    struct Room *connections[6]; //similar to how a linked list works. allows pointing to other Room struct
    
};

//subroutine declarations
void makeDir(char **argv, char *dirName);
void initialize(char *directory, char *begin, char *end);
struct Room *_makeRooms(char *roomName);
void createSolution(struct Room **pickedRooms);
int _isConnect(struct Room *room1, struct Room *room2);
void _connectRoom(struct Room *room1, struct Room *room2);
void addConnections(struct Room **pickedRooms);
void shuffleConnect(struct Room *room, int length);
void execute(char *directory, char *begin, char * end);
void roomInfo(struct Room *room, FILE *curRoom, char **roomConnections);
int isValid(char **roomConnections, int length, char *room);
void freeRoom(struct Room *room);
void printCharArray(char **array, int len);
void printRoom(struct Room *room);

//execute()
//the function that allows user to play the game
//enters a while loop that prompts the user for inputs
//each time user makes a choice, their choice will be stored and incremented
//exits the while loop if the user makes it to the end room. Prints out the path user took to get there.
//Preconditions: Accepts directory pointer, beginning and ending room pointer. The room structs, game subdirectory, and room files must have already been initialized
//Postconditions: Exits the routine when user reaches the ending room. In addition, the number of steps and path taken is also shown to the user.
//Alternatively, user may prematurely kill the program with the Ctrl+C hotkey.
void execute(char *directory, char *begin, char *end){
    
    printf("Dark Souls: Prepare to Die Edition\n\n");
    
    int index;
    int steps = 0;
    char *path[200];
    
    //allocate memory for each character of the room name
    for (index = 0; index < 200; index++){
        path[index] = (char*) malloc(64);
    }
    
    //allocate space for the current room file and next room file
    char *curFile = (char*) malloc(sizeof(char) * 64);
    char *nextFile = (char*) malloc(sizeof(char) * 64);
    
    //allocate space for the current room and next room pounter
    char *curRoom = (char*) malloc(sizeof(char) * 64);
    char *nextRoom = (char*) malloc(sizeof(char) * 64);
    
    struct Room *room = _makeRooms(begin); //needed later to read room information from supplied file. Holds the starting room at the beginning
    char *roomConnections[7]; //temporary array to store the room connections
    
    //allocate memory for each of the room
    for(index = 0; index < 7; index++){
        roomConnections[index] = (char*) malloc(64);
    }
    
    FILE *gameFile;
    
    //set the current room pointer as the starting room
    strcpy(curRoom, begin);
    
    //check the rooms (debug purposes)
    //printf("Current Room: %s, Beginning Room: %s, Ending Room: %s\n", curRoom, begin, end);
    
    //while loop that runs until current room is equal to ending room (reached the final room)
    while (strcmp(curRoom, end) != 0){ //if user reaches designated ending room, loop ends
        
        //sprintf stores the character array curRoom as a C string in buffer pointed by curFile
        sprintf(curFile, "%s/%s", directory, curRoom);
        
        gameFile = fopen(curFile, "r");
        
        if(gameFile == NULL){ //error check
            fprintf(stderr, "Game file not opened properly. Abort game.\n");
            exit(1);
        }
        
        //reads in the current room's information from the subdirectory
        roomInfo(room, gameFile, roomConnections);
        
        //close the file
        fclose(gameFile);
        
        do{
            //displays current room information
            printf("Current room: %s\n", room->name);
            printf("Branching paths: ");
            
            //loop through array to print out all possible branches
            for(index=0; index<room->numConnect; index++){
                printf("%s", roomConnections[index]); //print out the room names connected to current room
                if(index < room->numConnect - 1){
                    printf(", ");
                }
                else{
                    printf(".\n\n");
                }
            }
            printf("\n");
            
            printf("Where do you want to go? >");
            scanf("%s",nextRoom);
            printf("\n");
            sprintf(nextFile, "%s/%s", directory, nextRoom);
            gameFile = fopen(nextFile, "r");
            
            if( gameFile == NULL || !isValid(roomConnections, room->numConnect, nextRoom) )
            { //not a valid room connection, or named file does not exist
                printf("Input room does not exist. Please try again.\n");
            }
            
        } while(gameFile == NULL || isValid(roomConnections, room->numConnect, nextRoom) == 0);
        
        fclose(gameFile);
        
        //update current room with the next room
        strcpy(curRoom, nextRoom);
        
        //add next room to the path traversed. this path keeps getting longer until the user reaches the end
        strcpy(path[steps], nextRoom);
        steps++;
    }
    
    //once out of the while loop, user reached the destination room
    printf("Congratulations. You found the destination.\n");
    printf("It took you %d steps.\n", steps);
    printf("The path you took was: \n");
    //print out the path user traversed to destination
    for(index=0; index<steps; index++){
        printf("%s\n", path[index]);
    }
    
    //after game is completed, allocated memory must be deallocated to prevent resource leak
    
    for(index = 0; index < 7; index++){
        free(roomConnections[index]);
    }
    
    for(index = 0; index < 200; index++){
        free(path[index]);
    }
    
    free(curRoom);
    free(nextRoom);
    free(curFile);
    free(nextFile);
    free(room);
    
}

//initialize()
//randomly picks 7 rooms from a pre-determined 10 rooms
//also generated a solution path, room connections, and files corresponding to each room
//Preconditions: makeDir() successfully makes a directory, takes in directory, start and end room (represented as char array)
//Postconditions: creates 7 room objects, with connections, solution path, and files corresponding to each room
void initialize( char *directory, char *begin, char *end){
    
    //room list
    char *roomList[10] = {"FirelinkShrine", "UndeadParish", "BlightTown", "AnorLondo",
        "ThePit", "Majula", "LostIzalith", "Lothric",
        "Oolacile", "Drangleic"};
    
    //shuffles the room names up
    int index, randNum;
    for (index = 0; index < 10; index++){
        randNum = rand() % 10;
        char *tmp = roomList[index];
        roomList[index] = roomList[randNum];
        roomList[randNum] = tmp;
    }
    
    //pick the first seven rooms to be used for the rest of the game
    struct Room *pickedRooms[7];
    int j;
    for (j = 0; j < 7; j++){
        pickedRooms[j] = _makeRooms(roomList[j]); //populate each room struct with room data
    }
    
    //creates the solution path and connections for a game instance
    createSolution(pickedRooms);
    addConnections(pickedRooms);
    
    //shuffles the room connections
    for (index = 0; index < 7; index++){
        shuffleConnect(pickedRooms[index], pickedRooms[index]->numConnect);
    }
    
    //files that hold the information about the rooms
    char *fileName = (char*) malloc(64 * sizeof(char));
    
    //if file creation fails, print and exit
    if(fileName == 0){
        fprintf(stderr, "File creation fails. Game aborted\n");
        exit(1);
    }
    
    //write the files for each of the rooms
    for(index=0; index < 7; index++){
        
        //snprintf stores the file in a buffer, pointed by fileName
        snprintf(fileName, 64, "%s/%s", directory, pickedRooms[index]->name);
        FILE *file = fopen(fileName, "w");
        
        //if file opening fails, abort program
        if(file == NULL){
            fprintf(stderr, "Error opening file. Abort game.\n");
            exit(1);
        }
        
        else{ //if file is created successfully
            //room name written to file
            fprintf(file, "Room: %s\n", pickedRooms[index]->name);
            
            //room connections written to file
            for(j = 0; j < pickedRooms[index]->numConnect; j++){
                
                //first access the room name in array, then access the name of the connected rooms
                fprintf(file, "Connections %d: %s\n", j+1, pickedRooms[index]->connections[j]->name);
            }
            
            //write the room type to the file
            fprintf(file, "Room type: ");
            
            if(pickedRooms[index]->type == 0){
                fprintf(file, "Begin\n");
                strcpy(begin, pickedRooms[index]->name);
            }
            
            else if(pickedRooms[index]->type == 2){
                fprintf(file, "End\n");
                strcpy(end, pickedRooms[index]->name);
            }
            
            else
            {
                fprintf(file, "Middle\n");
            }
            
        }
        fclose(file);
    }
    
    //prints out the room structs for debug
    //int m;
    //for(m=0; m<7; m++){
    //    printRoom(pickedRooms[m]);
    //}
    
    //since all the file data have been written to necessary files, it is time to free up the unused memory
    for(index=0; index<7; index++){
        if(pickedRooms[index] != 0){
            freeRoom(pickedRooms[index]);
        }
    }
    free(fileName);
    
    char msg[] = "World created...\n";
    printf("%s\n", msg);
    
}

//freeRoom()
//free memory allocated for a room
//Preconditions: accept a room struct pointer that is called from looping through a room array
//Postconditions: deallocated memory associated with the room
void freeRoom(struct Room *room){

    if(room != 0){
        free(room);
    }

}

//isValid()
//sees if a room is is part of a connection
//Preconditions: an array of room connections, length of the connection array and the room struct is provided
//Postconditions: returns an integer. 1 for valid, 0 for not valid
int isValid(char **roomConnections, int length, char *room){
    
    int index;
    for(index = 0; index < length; index++){
        if( strcmp(roomConnections[index], room) == 0 ){
            return 1;
        }
    }
    return 0;
}

//roomInfo()
//from a supplied room file, reads in room information. Also writes the room connections
//in the roomConnections array
//Preconditions: accepts the current room object, current file pointer and room connection arrays.
//Room files must have been initialized previously.
//Postconditions: room information is written into room struct, to be used in the execute() method
void roomInfo(struct Room *room, FILE *curFile, char **roomConnections){
    
    if(curFile == NULL){
        fprintf(stderr, "Game file does not exist. Game abort.\n");
        exit(1);
    }
    
    int counter;
    char type[16];
    char line[128];
    char connection[64];
    
    //obtains the current room's name
    rewind(curFile); //sets file pointer to the beginning of the file.
    
    while( fgets(line, 128, curFile) != NULL )
    {
        if(sscanf(line, "Room: %[^\n]", room->name) == 1){ //sscanf reads the line
            //and writes it into room->name variable if it matches the format
            //printf("room name (%s) written successfully.\n", room->name);
            break;
        }
    }
    
    //obtain the current room's connections
    rewind(curFile);
    counter=0;
    room->numConnect = 0;
    while( fgets(line, 128, curFile) != NULL ){
        if(sscanf(line, "Connections %*d: %[^\n]", connection) )
        {
            strcpy(roomConnections[counter], connection);
            room->numConnect++;
            counter++;
            //printf("connections made(%s)\n", connection);
        }
    }
    
    //obtain the current room's room type
    rewind(curFile);
    while ( fgets(line, 128, curFile) != NULL ){ //while
        if( sscanf(line, "Room type: %[^\n]", type) ){
            if(strcmp(type, "End") == 0){
                room->type = 2;
            }
            else if(strcmp(type, "Begin") == 0){
                room->type = 0;
            }
            else{
                room->type = 1;
            }
        }
    }//endwhile
}

//createSolution()
//creates a solution path for a random instance of the game
//Preconditions: accept a pointer to the selected room list as input, with a total of 7 rooms
//Postconditions: a solution path is generated, and that there is only one path
void createSolution(struct Room **pickedRooms){
    
    int index;
    int solutionCount = rand() % 6 + 2; //this guarantees that solution path has a minimum length of 2, and max length of 7
    
    //creates the starting and ending room
    pickedRooms[0]->type = 0; //begin
    pickedRooms[ solutionCount-1 ]->type = 2; //ending room
    
    for (index = 0; index <= solutionCount-1; index++){ //comparing 2 adjacent room in size of 7, so six comparisons
        //if no connection exists between two contiguous room in array, make one
        if( !_isConnect(pickedRooms[index], pickedRooms[index+1]))
        {
            _connectRoom(pickedRooms[index], pickedRooms[index+1]);
        }
    }
}

//addConnections()
//adds the connections to each of the room struct connected
//Preconditions: accept a pointer to the list of rooms
//Postconditions: all the selected rooms have at least one mutual connection to another room. no duplicate or self connections
void addConnections(struct Room **pickedRooms){
    
    int index, totalConnection, randomRoom, compareName, isConnect;
    for(index=0; index<7; index++)
    {
        totalConnection = rand() % 4 + 3; //7 nodes, so a maximum of 6 connections
        
        while(pickedRooms[index]->numConnect < totalConnection){
            randomRoom = rand() % 7;
            compareName = strcmp(pickedRooms[index]->name, pickedRooms[randomRoom]->name);
            isConnect = _isConnect(pickedRooms[index], pickedRooms[randomRoom]);
            
            //if rooms are not connected and they have different names (ie. no self-connnection)
            //strcmp returns 0 if both string arguments are identical
            if( compareName!=0 && isConnect != 1){ //if not same room and no connection
                _connectRoom(pickedRooms[index], pickedRooms[randomRoom]);
            }
        }
    }
}

//shuffleConnect()
//shuffles array of room pointers for a given room, so solution path is randomized
//Preconditions: take in a room struct pointer
//Postconditions:connections in a room are shuffled
void shuffleConnect(struct Room *room, int length){
    
    int index, randomNumber;
    for(index = 0; index < length; index++){
        randomNumber = rand() % length; //pick between 0 to 6
        struct Room *tmp = room->connections[index];
        room->connections[index] = room->connections[randomNumber];
        room->connections[randomNumber] = tmp;
    }
}

//_connectRoom()
//a helper function that establishes a connection between two rooms. accept two room references, check for mutual connectivity
//and increments connection count for each room
//Preconditions: accept two room pointers
//Postconditions: the two room struct are connected, and their connectivity number is incremented
void _connectRoom(struct Room *room1, struct Room *room2){
    
    //debug statements
    if(room1 == 0){
        fprintf(stderr, "_connectRoom: Room 1 not found.\n");
        exit(1);
    }
    
    if(room2 == 0){
        fprintf(stderr, "_connectRoom: Room 2 not found.\n");
        exit(1);
    }
    
    if(room1->numConnect < 6 && room2->numConnect < 6)
    {
        //ensures that both rooms are mutually connected
        room1->connections[room1->numConnect] = room2;
        room1->numConnect++;
        room2->connections[room2->numConnect] = room1;
        room2->numConnect++;
        
        //statements to handle room connect error
        if (!_isConnect(room1, room2)){
            fprintf(stderr, "Error connecting %s and %s. \n", room1->name, room2->name);
            exit(1);
        }
        
        if (!_isConnect(room2, room1)){
            fprintf(stderr, "Error connecting %s and %s. \n",room2->name, room1->name);
            exit(1);
        }
    }
}

//_isConnect()
//helper function to determine if connection exists between two rooms
//Preconditions: accept two room struct from the array of seven room struct
//Postconditions: return an integer that determines connectivity. 0 for no connection, and 1 for an existing connection
int _isConnect(struct Room *room1, struct Room *room2){
    
    if(room1 == 0){
        fprintf(stderr, "_isConnect: Room 1 not found.\n");
        exit(1);
    }
    
    if(room2 == 0){
        fprintf(stderr, "_isConnect: Room 1 not found.\n");
        exit(1);
    }
    
    int index;
    for(index = 0; index < room1-> numConnect; index++){
        if (room1->connections[index] == room2){
            return 1;
        }
    }
    return 0;
}

//_makeRooms()
//helper function that creates the rooms with the hardcoded name
//allocates memory for room struct, as well as initialize room type and number of connections
//Preconditions: a room name string is provided
//Postconditions: room is initialized
struct Room *_makeRooms(char *roomName){
    
    struct Room *room = malloc(sizeof(struct Room));
    
    //if room struct fails to create, exit
    if(room == 0){
        fprintf(stderr, "%s: Error creating room.\n", roomName);
        exit(1);
    }
    
    //initialize the room types
    strcpy(room->name, roomName);
    room->numConnect = 0;
    room->type = 1; //all room types start as middle room, until user later changes beginning and end room
    return room;
}

//makeDir()
//creates a subdirectory in the format of <username>.rooms.<PID>
//directory name not initialized here. it is done in main function and will be deallocated there
//Preconditions: takes in directory name as character array
//Postconditions: subdirectory of generated directory name created
void makeDir(char **argv, char *dirName){
    char msg[] = "Making subdirectory...\n";
    printf("%s\n", msg);
    
    snprintf( dirName, 64, "siaj.rooms.%d", getpid() );
    int status = mkdir(dirName, 0755); //takes user input path. 755 refers to rwx for user, and r-x for groups and others
    if ( status == -1 ){
        
        //instead of using printf to directly print results to the stdout, fprintf redirects error message to stdout 
        //using printf is equivalent to using fprintf(stdout, ...)
        
        if (errno == EEXIST){ //if user generated directory already exists
            fprintf(stderr, "%s: %s already exists\n", *argv, dirName);
            exit(1);
        }
        
        else{ //some other error
            fprintf(stderr, "%s: Error creating directory %s\n", *argv, dirName);
            exit(1);
        }
    }
    
    char allGood[] = "Subdirectory successfully created.\n";
    printf("%s\n", allGood);
}

//Main function that is first called. This will call the subroutines
//argv takes in the PWD, so that a subdirectory can be created with the format <username>.rooms.<PID>
int main(int argc, char **argv){
    
    //seed random, to allow for random room connections later
    srand(time(NULL));
    
    char *directory = (char*) malloc(128);
    //create a directory in executing directory, that holds the room and connection data
    makeDir(argv, directory); 
    
    //holds the beginning and ending rooms. declared and allocated memory here so they can be used in other subroutine
    char *begin = (char*) malloc(128);
    char *end = (char*) malloc(128);
    
    //create the game world
    initialize(directory, begin, end);   
    
    //run the game
    execute(directory, begin, end);
    
    //deallocate memory to prevent resource leak
    free(begin);
    free(end);
    free(directory);
    
    return 0;
    
}