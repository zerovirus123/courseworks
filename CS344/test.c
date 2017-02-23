//
//  test.c
//  
//
//  Created by Brian Sia on 8/11/16.
//
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char**argv){

          char *self = NULL;
        pid_t spawnpid = fork();
        int pid = getpid();
        int pgid = getpgid(0);
    
        switch (spawnpid)
        {
            case -1: exit(1); break;
            case 0: exit(0); break;
            default:  break;
        }
    
    if (pid == pgid)
        self = "Parent";
    else
        self = "Child";
    
    printf("%s pid = %d pgid = %d\n", self, pid, pgid);
        printf("XYZZY\n");
    

}
