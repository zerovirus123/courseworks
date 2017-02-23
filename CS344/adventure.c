 /******************************************************************************
 adventure.c
 CS344-400
 Program 2: Adventure
 Text adventure game, akin to Colossal Cave Adventure. Introduction to
	programming in C on UNIX-based systems.
 Grading notes: I moved some variables outside of functions so that I could
	deallocate the memory later, since I was using valgrind to check for any
	potential memory leaks and memory not being freed.
	It looks like the path in the game example on Canvas doesn't include the
	starting Room, so I did not include it here (so for instance, if you start
	at Markarth then go to Solitude, Solitude will be the first element in the
	path you took to victory).
  
	I left in two debugging functions at the end (printCharArray() and
	printRoom()), but they're not called by anything.
	To ensure that there is a guaranteed path, I made it so that when the list
	of Room names is shuffled, the name that ends up first is the starting room
	and the generateSolution() function picks an end Room, then a path is
	created through connections from the start to the end using the shuffled
	list of names. After all the connections are made, the connections are
	shuffled around so that the solution isn't always created by entering the
	first name that shows up when you play.
	Names of Rooms are from The Elder Scrolls V: Skyrim. I may have played it
	a bit too much recently.
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>

struct Room {
    char roomName[25];				// Room name
    int connNum;					// number of connections this Room has
    struct Room* connections[6];	// array of connections
    int roomType;					// -1 = start; 0 = middle; 1 = end
};

void makeDir(char **argv, char *directory);
void createGame(char *directory, char *start, char *end);
struct Room *initRoom(char *name);
void freeRoom(struct Room *room);
void generateSolution(struct Room **roomList);
void connectRooms(struct Room *room1, struct Room *room2);
void createConnections(struct Room **roomList);
void shuffleRoomConnections(struct Room *room, int len);
void shuffleCharArray(char **array, int len);
void play(char *directory, char *start, char *end);
void readRoom(struct Room *room, FILE *input, char **array);
void printCharArray(char **array, int len);		// debugging function
void printRoom(struct Room *room);				// debugging function

int main(int argc, char **argv)
{
    // seed random number generator
    srand(time(NULL));
    
    // create directories
    char *dirName = (char *) malloc(128);	// to be freed later
    makeDir(argv, dirName);
    
    // create (initialize, generate solution path, set connections) rooms
    char *start = (char *) malloc(128);		// to be freed later
    char *end = (char *) malloc(128);		// to be freed later
    createGame(dirName, start, end);
    //exit(0);
    
    // start the game
    play(dirName, start, end);
    
    // free dynamically allocated memory
    free(start);
    free(end);
    free(dirName);
    
    return 0;	// exit with no errors
}

/* makeDir() creates the directory for the adventure game, using the format
	<username>.rooms.<process id>. Moved malloc and directory to outside of
	function so that main() can free the memory later. Changes the supplied
	directory char array to the generated name.
	Preconditions: directory is supplied as a character array outside of
 function
	Postconditions: changes directory to generated directory name
	Receives: array of char arrays (arguments to the program) for error
 messages; char array as directory name
	Returns: none */
void makeDir(char **argv, char *directory)
{
    snprintf(directory, 64, "rooms.%d", getpid());
    int mkdirResult = mkdir(directory, 0755);
    if (mkdirResult == -1)
    {
        if (errno == EEXIST)
        {
            // directory already exists
            fprintf(stderr, "%s: directory %s already exists\n",
                    *argv, directory);
            exit(1);
        }
        else
        {
            // some other error
            fprintf(stderr, "%s: error creating directory %s\n",
                    *argv, directory);
            exit(1);
        }
    }
}

/* createGame() creates 7 rooms from a hardcoded set of 10 room names. Begins
	by shuffling the array of 10 names, then choosing the first 7 as the names
	of the Rooms to be created. Next, generates a guaranteed solution path, and
	adds additional connections to the Rooms, then shuffles the Room
	connections for each Room. After Rooms are created, files are created for
	each Room, with the information being written in the specified format.
	Preconditions: directory is generated by makeDir(); start and end are
 allocated outside of the function
	Postconditions: start and end are changed to the generated start Room and
 end Room names, respectively
	Receives: char array as directory name; char array as start Room name; char
 array as end Room name
	Returns: none */
void createGame(char *directory, char *start, char *end)
{
    int i, j;
    
    // create list of all possible Room names
    //char *allRooms[10];
    // capitals of Skyrim's holds + Solstheim
    //allRooms[0] = "Windhelm";
    //allRooms[1] = "Falkreath";
    //allRooms[2] = "Solitude";
    //allRooms[3] = "Morthal";
    //allRooms[4] = "Dawnstar";
    //allRooms[5] = "Markarth";
    //allRooms[6] = "Riften";
    //allRooms[7] = "Whiterun";
    //allRooms[8] = "Winterhold";
    //allRooms[9] = "Solstheim";
    
    char *allRooms[10] = {"Windhelm", "Falkreath", "Solitude",
                "Morthal", "Dawnstar", "Markarth", "Riften", "Whiterun",
                "Winterhold","Solstheim"
                };
    
    // shuffle room names for each new game
    shuffleCharArray(allRooms, 10);
    
    // use the first 7 rooms and copy them to a new array of rooms
    struct Room *selectedRooms[7];
    for (i = 0; i < 7; i++)
    {
        selectedRooms[i] = initRoom(allRooms[i]);
    }
    
    // generate a guaranteed solution to the maze from the shuffled rooms
    generateSolution(selectedRooms);
    
    // add connections to the generated rooms
    createConnections(selectedRooms);
    
    // shuffle Room connections prior to writing to file, so that the first
    //	connection isn't always the guaranteed solution path
    for (i = 0; i < 7; i++)
    {
        shuffleRoomConnections(selectedRooms[i], selectedRooms[i]->connNum);
    }
    
    // create files
    char *fileName = (char *) malloc(64 * sizeof(char));
    
    // assert(fileName != 0);
    for (i = 0; i < 7; i++)
    {
        snprintf(fileName, 64, "%s/%s", directory, selectedRooms[i]->roomName);
        FILE *f = fopen(fileName, "w");
        if (f == NULL)
        {
            // something went wrong
            fprintf(stderr, "Error opening file\n");
        }
        
        else
        {
            // print Room name to file
            fprintf(f, "ROOM NAME: %s\n", selectedRooms[i]->roomName);
            
            // print Room connections to file
            for (j = 0; j < selectedRooms[i]->connNum; j++)
            {
                fprintf(f, "CONNECTION %d: %s\n", j + 1,
                        selectedRooms[i]->connections[j]->roomName);
            }
            
            // print Room type to file
            fprintf(f, "ROOM TYPE: ");
            
            // determine room type
            if (selectedRooms[i]->roomType == -1)
            {
                fprintf(f, "START_ROOM\n");
                strcpy(start, selectedRooms[i]->roomName);
            }
            else if (selectedRooms[i]->roomType == 1)
            {
                fprintf(f, "END_ROOM\n");
                strcpy(end, selectedRooms[i]->roomName);
            }
            else
            {
                fprintf(f, "MID_ROOM\n");
            }
        }
        fclose(f);
    }
    
    // free Rooms in array of Rooms
    for (i = 0; i < 7; i++)
    {
        freeRoom(selectedRooms[i]);
    }
    free(fileName);
}

/* initRoom() creates a Room using a supplied name. Initializes the room and
	copies the supplied string to the Room name, then sets the current number
	of connections to 0 and the room type to the default MID_ROOM.
	Preconditions: none
	Postconditions: a Room is initialized
	Receives: char array representing the Room name
	Returns: initialized Room
 */
struct Room *initRoom(char *name)
{
    struct Room *temp = malloc(sizeof(struct Room));
    // assert(temp != 0);
    strcpy(temp->roomName, name);
    temp->connNum = 0;
    temp->roomType = 0;
    return temp;
}

/* freeRoom() frees a Room from memory (since it was dynamically allocated
	using malloc in initRoom().
	Preconditions: none
	Postconditions: given Room is freed from memory
	Receives: pointer to Room
	Returns: none */
void freeRoom(struct Room *room)
{
    if (room != 0)
    {
        free(room);
    }
}

/* generateSolution() guarantees that there is a solution to the generated
	maze. The supplied list of Rooms should be from a shuffled array of	names,
	so we can choose the first Room as the start and some room as the end,
	since the first Room should be random. (Essentially, the first Room is
	chosen during the name shuffling. Connects the rooms in the solution path
	using connectRooms().
	Preconditions: given array of Room pointers has 7 Rooms
	Postconditions: Rooms are connected to form a guaranteed solution
	Receives: array of Room pointers
	Returns: none
 */
void generateSolution(struct Room **roomList)
{
    int i;
    int solutionRoomCount = rand() % 6 + 2;	// solution length = 2-7 Rooms
    roomList[0]->roomType = -1; //begin
    roomList[solutionRoomCount - 1]->roomType = 1;//end
    
    for (i = 0; i < solutionRoomCount - 1; i++)
    {
        if (!connectionExists(roomList[i], roomList[i + 1]))
        {
            connectRooms(roomList[i], roomList[i + 1]);
        }
    }
}

/* connectRooms() connects two rooms together. Takes two Room pointers as its
	arguments. Checks if the connection is mutual, and also increments the
	connection count for each Room.
	Preconditions: none
	Postconditions: two Rooms are connected, and their connNum values are
 incremented
	Receives: two Room pointers
	Returns: none
 */
void connectRooms(struct Room *room1, struct Room *room2)
{
    if (room1->connNum < 6 && room2->connNum < 6)
    {
        // ensures that if A is connected to B, then B also connects to A
        room1->connections[room1->connNum] = room2;
        room1->connNum++;
        room2->connections[room2->connNum] = room1;
        room2->connNum++;
        
        assert(connectionExists(room1, room2) == 1);
        assert(connectionExists(room2, room1) == 1);
    }
}

/* createConnections() creates connections between the Rooms in a given array
	of Rooms. Ensures that duplicate and self connections are not made.
	Preconditions: given array of Room pointers has 7 Rooms
	Postconditions: connections are generated between Rooms in the list of
 Rooms
	Receives: array of Room pointers
	Returns: none*/
void createConnections(struct Room **roomList)
{
    int i,  totalConnect, randRoom, compareName, connExists;
    for (i = 0; i < 7; i++)
    {
        totalConnect = rand() % 4 + 3;		// set total number of connections
        while (roomList[i]->connNum < totalConnect)
        {
            randRoom = rand() % 7;
            compareName = strcmp(roomList[i]->roomName,
                                 roomList[randRoom]->roomName);
            connExists = connectionExists(roomList[i], roomList[randRoom]);
            if (compareName != 0 && connExists != 1)
            {
                // if there is no existing connection and it's not a connection
                //	to itself, then connect the rooms
                connectRooms(roomList[i], roomList[randRoom]);
            }
        }
    }
}

/* connectionExists() checks if a connection exists between two Rooms.
	Preconditions: none
	Postconditions: none
	Receives: two pointers to Rooms
	Returns: 1 if connection exists; 0 if connection does not exist */
int connectionExists(struct Room *room1, struct Room *room2)
{
    int i;
    for (i = 0; i < room1->connNum; i++)
    {
        if (room1->connections[i] == room2)
        {
            return 1;
        }
    }
    return 0;
}

/* shuffleRoomConnections() shuffles an array of Room pointers, from a given
	Room. Called prior to writing Rooms to file, so that the solution path is
	not always the first connection.
	Preconditions: none
	Postconditions: connections in Room are shuffled
	Receives: pointer to Room
	Returns: none */
void shuffleRoomConnections(struct Room *room, int len)
{
    int i, randNum;
    for (i = 0; i < len; i++)
    {
        randNum = rand() % len;
        struct Room *temp = room->connections[i];
        room->connections[i] = room->connections[randNum];
        room->connections[randNum] = temp;
    }
}

/* shuffleCharArray() shuffles an array of char arrays.
	Preconditions: none
	Postconditions: char arrays in given array are shuffled
	Receives: pointer to array of character arrays; length of array
	Returns: none */
void shuffleCharArray(char **array, int len)
{
    int i, randNum;
    for (i = 0; i < len; i++)
    {
        randNum = rand() % len;
        char *temp = array[i];
        array[i] = array[randNum];
        array[randNum] = temp;
    }
}

/* play() is the main function for playing the adventure game. Begins by
	setting the start Room as the current Room, then enters a while loop that
	extracts the current Room information from the file, displays the Room
	information, then prompts the user to choose a path to take. Validates
	that the entered path is valid (name is valid and within the possible
	connections in the current Room), then sets the entered Room name as the
	current Room. Repeats until the current Room name is the same as the given
	end Room name.
	Preconditions: directory is generated by makeDir(); start and end are
 provided from createGame().
	Postconditions: game is played
	Receives: char array as directory name; char array as start Room name,
 char array as end Room name
	Returns: none */
void play(char *directory, char *start, char *end)
{
    int i;
    char *path[100];
    // need to malloc each element for later
    for (i = 0; i < 100; i++)
    {
        path[i] = (char *) malloc(64);
    }
    int steps = 0;
    char *current = (char *) malloc(64 * sizeof(char));
    char *currentFile = (char *) malloc(64 * sizeof(char));
    char *next = (char *) malloc(64 * sizeof(char));
    char *nextFile = (char *) malloc(64 * sizeof(char));
    struct Room *temp = initRoom(start);
    
    /*	for saving Room connections as array of char arrays, instead of array
     of Room pointers*/
    char *tempConnections[7];
    // need to malloc each element for readRoom()
    for (i = 0; i < 7; i++)
    {
        tempConnections[i] = (char *) malloc(64);
    }
    FILE *f;
    
    printf("WELCOME TO ADVENTURE!!\n\n");
    
    // start game at start file
    strcpy(current, start);
    // printf("%s, %s, %s\n", current, start, end);
    
    // read in files of Rooms
    while (strcmp(current, end) != 0)
    {
        sprintf(currentFile, "%s/%s", directory, current);
        // open the current Room file
        f = fopen(currentFile, "r");
        if (f == NULL)
        {
            fprintf(stderr, "Error opening file\n");
        }
        // read current Room info
        readRoom(temp, f, tempConnections);
        fclose(f);
        // strcpy(current, temp->roomName);
        
        do
        {
            // print out current Room info
            printf("CURRENT LOCATION: %s\n", temp->roomName);
            printf("POSSIBLE CONNECTIONS: ");
            for (i = 0; i < temp->connNum; i++)
            {
                printf("%s", tempConnections[i]);
                if (i < temp->connNum - 1)
                {
                    printf(", ");
                }
                else
                {
                    printf(".\n\n");
                }
            }
            
            // prompt user for next Room
            printf("WHERE TO? >");
            // strcpy(next, end);			// for debugging & valgrind use
            scanf("%s", next);				// get user input to next room
            printf("\n");
            // attempt to open file with the supplied name
            sprintf(nextFile, "%s/%s", directory, next);
            f = fopen(nextFile, "r");
            
            if (f == NULL || validStep(tempConnections, temp->connNum, next) == 0)
            {
                // if there is no file with that name or the name is not in the
                //	list of valid steps to take from the current Room
                printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
            }
        }
        while (f == NULL ||
               validStep(tempConnections, temp->connNum, next) == 0);
        fclose(f);
        
        // copy entered name to current
        strcpy(current, next);
        // add entered name to path
        strcpy(path[steps], next);
        steps++;
    }
    
    // end room is reached
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEP", steps);
    if (steps > 1)
    {
        // plural
        printf("S");
    }
    printf(". YOUR PATH TO VICTORY WAS:\n");
    for (i = 0; i < steps; i++)
    {
        printf("%s\n", path[i]);
    }
    
    // free dynamically allocated memory
    for (i = 0; i < 7; i++)
    {
        free(tempConnections[i]);
    }
    for (i = 0; i < 100; i++)
    {
        free(path[i]);
    }
    free(current);
    free(currentFile);
    free(next);
    free(nextFile);
    free(temp);
}

/* readRoom() reads in Room information from a supplied file. Saves names of
	Room connections to a given array.
	Preconditions: supplied input file is opened, array of char arrays is
 allocated
	Postconditions: Room information is read to temporary Room variable for
 use by play()
	Receives: pointer to Room, pointer to input file, array of char arrays */
void readRoom(struct Room *room, FILE *input, char **array)
{
    assert(input != NULL);
    char line[128];			// store line in file
    char connection[64];	// store connection name
    int count;
    char roomType[16];
    
    // get Room name
    rewind(input);			// start from beginning of file
    while (fgets(line, 128, input) != NULL)
    {
        if (sscanf(line, "ROOM NAME: %[^\n]", room->roomName) == 1)
        {
            break;
        }
    }
    
    // get Room connections and number of connections
    rewind(input);
    count = 0;
    room->connNum = 0;
    while (fgets(line, 128, input) != NULL)
    {
        if (sscanf(line, "CONNECTION %*d: %[^\n]", connection) == 1)
        {
            strcpy(array[count], connection);
            room->connNum++;
            count++;
        }
    }
    
    /* The Room type is sort of circumvented by the use of the end char array
     in other functions, but it prevents the program from having to break
     out of a while loop in play(), which I try to keep to a minimum. */
    // get Room type
    rewind(input);
    while (fgets(line, 128, input) != NULL)
    {
        if (sscanf(line, "ROOM TYPE: %[^\n]", roomType) == 1)
        {
            if (strcmp(roomType, "END_ROOM") == 0)
            {
                room->roomType = 1;
            }
            else if (strcmp(roomType, "START_ROOM") == 0)
            {
                room->roomType = -1;
            }
            else
            {
                room->roomType = 0;
            }
        }
    }
}

/* validStep() checks if a supplied Room name is within a supplied array of
	char arrays; that is, it checks if the name is in a list of valid names.
	This function is used to prevent users from simply typing in the name of
	the end room to get to the end of the game even though it might not be a
	valid connection.
	Preconditions: array of char arrays has names in it
	Postconditions: none
	Receives: array of char arrays as list of valid names, length of list of
 valid names as int, char array as name to validate
	Returns: 1 if supplied name is valid; 0 if invalid */
int validStep(char **array, int len, char *roomName)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (strcmp(roomName, array[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
