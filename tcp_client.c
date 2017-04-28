/*
ANMOL K GUPTA 2013B4A7588P
PRYANSHU ARORA 2013B4A7601P
DEEPANSHU NAGPAL 2013B4A7834P
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

// #include <unistd.h
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#define INPUT_LEN 10
#define WAIT 10


int main()
{
    // Important declaration
    int clientSocket;
    char *buffer;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // User Input
    char *userInput;

    userInput = (char *)malloc(sizeof(char)*10);
    buffer = (char *)malloc(sizeof(char)*10);

    /*  Create the network socket.   */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*  1) PR_INET Internet domain
        2) SOCK_STREAM represents Stream socket
        3) 0 represents Default protocol (TCP in this case)
        */

    // Configure settings of the server address
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;

    /* Set port number */
    serverAddr.sin_port = htons(9011);
    /*  In networking we prefer to use big-endian binary 
        sequencing.  Since in our PC we use small endian binary
        scheme, we use htons to convert little endian to big
        endian.  
        */

    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("172.17.26.42");

    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof
            serverAddr.sin_zero);

    /* Connect the socket to the server using the address*/
    addr_size = sizeof serverAddr;
    printf("Connection is being established\n");
    if (connect( clientSocket, (struct sockaddr *) &serverAddr,addr_size) < 0 )
    {
        printf("[ERROR] Unable to connect to server.\n");
        close( clientSocket );
        exit(0);
    }
    int player=0;
    // printf("ppai\n");
    if (recv ( clientSocket, buffer, 10, 0) < 0 )
    {
        printf("[ERROR] Connection closed prematurely.\n");
    }
    printf("%s\n",buffer );
    player=buffer[0]-'0';
    int tot_player;
    if(player==0)
    {
        printf("Enter total number of players:");
        
        scanf("%d",&tot_player);
        userInput[0]=tot_player+'0';
        send ( clientSocket, userInput,strlen(userInput), 0);
        printf("Room created for %d players\n",tot_player);
    }
    else
    {
        recv ( clientSocket, buffer, 10, 0);
        printf("Total number of players playing are: %d\n",buffer[0]-'0');
        tot_player=buffer[0]-'0';
    }
    printf("You have been assigned id number %d\n",player+1);
    if(player==0)
    {
        recv ( clientSocket, buffer, 10, 0);
        if(buffer[0]-'0'==0)
        {
            printf("All connections have been established\n");    
        }
        printf("Press Enter to start the game\n");
        int g=getchar();
        g=getchar();
        strcpy(userInput,"START");
        send ( clientSocket, userInput,strlen(userInput), 0);
        
    }
    memset(userInput,'\0',10);
    // printf("%s\n",buffer );
    // printf("ppai2\n");
    // int maxPoints = 1000;
    // int counter = 0;
    unsigned int inputLength = 0;

    // User input to guess a number
    // printf("Attempt [%d] : ", ( counter + 1 ) );
    // printf("Enter the number between 1 to 100 to send: ");
    char points[100];
    // int availablePoints = maxPoints;
    int hhuu=2;
    while (hhuu--)
    {
        // ++counter;
        // availablePoints = maxPoints - ( counter * 100 );
        if (1)
        {
            /* Send data to server*/
            while(1)
            {
                if ( recv ( clientSocket, points, 10, 0) < 0 )
                {
                    printf("[ERROR] Connection closed prematurely.\n");
                }
                int j;
                printf("-----POINTS TABLE------\n");
                printf("-----------------------\n");
                for (j = 0; j < tot_player; ++j)
                {
                    printf("-----------------------\n");
                    printf("PLAYER %d | %c |\n",j+1,points[j]);
                    printf("-----------------------\n");
                }
                // if ( recv ( clientSocket, buffer, 10, 0) < 0 )
                // {
                //     printf("[ERROR] Connection closed prematurely.\n");
                // }
                // printf("%s ",buffer);
                if ( recv ( clientSocket, buffer, 10, 0) < 0 )
                {
                    printf("[ERROR] Connection closed prematurely.\n");
                }
                printf("%s\n",buffer);
                if(buffer[0]-'0'==player)
                {
                    printf("It's your turn\n");
                }
                else
                {
                    printf("It's player %d's turn\n", buffer[0]-'0'+1);
                }
                if(buffer[0]-'0'==player)
                {
                    break;
                }
            }
            
            if ( recv ( clientSocket, buffer, 10, 0) < 0 )
            {
                printf("[ERROR] Connection closed prematurely.\n");
            }
            printf("%s\n",buffer );
           // scanf("%s", userInput);



    
    // fflush(stdout);

    time_t end = time(0) + 15; //5 seconds time limit.

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // char userInput[INPUT_LEN];
    int pos = 0;
    while(time(0) < end)
    {
        int c = getchar();

        /* 10 is new line */
        if (c != EOF && c != 10 && pos < INPUT_LEN - 1)
            userInput[pos++] = c;

        /* if new line entered we are ready */
        if (c == 10)
        {
            // printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy\n");
            break;
        }
            
    }

    userInput[pos] = '\0';

    if(pos > 0)
        printf("%s\n", userInput);
    else
    {
        puts("\nSorry, I got tired waiting for your input. Good bye!");
        strcpy(userInput,"empty");
    }
    
 // // in case of single character input
 //    fd_set          input_set;
 //    struct timeval  timeout;
 //    int             ready_for_reading = 0;
 //    int             read_bytes = 0;

 //    /* Empty the FD Set */
 //    FD_ZERO(&input_set );
 //    /* Listen to the input descriptor */
 //    FD_SET(0, &input_set);

 //    /* Waiting for some seconds */
 //    timeout.tv_sec = WAIT;    // WAIT seconds
 //    timeout.tv_usec = 0;    // 0 milliseconds

 //    /* Invitation for the user to write something */
 //    printf("Enter Username: (in %d seconds)\n", WAIT);
 //    printf("Time start now!!!\n");

 //    /* Listening for input stream for any activity */
 //    ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);
 //     Here, first parameter is number of FDs in the set, 
 //     * second is our FD set for reading,
 //     * third is the FD set in which any write activity needs to updated,
 //     * which is not required in this case. 
 //     * Fourth is timeout
     

 //    if (ready_for_reading == -1) {
 //        /* Some error has occured in input */
 //        printf("Unable to read your input\n");
 //        return -1;
 //    } 

 //    if (ready_for_reading) {
 //        read_bytes = read(0, userInput, 19);
 //        if(userInput[read_bytes-1]=='\n'){
 //        --read_bytes;
 //        userInput[read_bytes]='\0';
 //        }
 //        if(read_bytes==0){
 //            printf("You just hit enter\n");
 //        } else {
 //            printf("Read, %d bytes from input : %s \n", read_bytes, userInput);
 //        }
 //    } else {
 //        printf(" %d Seconds are over - no data input \n", WAIT);
 //        userInput[0]='\0';
 //    }





    // printf("ppppppppp1\n");
            inputLength = strlen(userInput);
            if ( send ( clientSocket, userInput,inputLength, 0) != inputLength )
            {
                 printf("[ERROR] Unable to send the data.\n");
            }
            // printf("pppppppppppp2\n");
            if(recv ( clientSocket, buffer, 10, 0) < 0 )
            {
                printf("[ERROR] Connection closed prematurely.\n");
            }
            printf("%s\n",buffer);
        } 
        // else {
        //     userInput = "DONE\0";
        //     send ( clientSocket, userInput, 5, 0);
        //     break;
        // }
    }
    // availablePoints = maxPoints - (counter * 100 );
    // if ( buffer[0] == '1' )
    // {
    //     // printf("CONGRATULATIONS ==> you were able to guess the correct number.FINAL SCORE IS: %d\n", availablePoints);
    //     userInput = "DONE\0";
    //     send ( clientSocket, userInput, 5, 0);
    // } else { 
    //     printf( "GAME OVER, TRY AGAIN WITH NEW TCP SOCKET CONNECTION.\n"); 
    // }
    //
    int iter=0;
    for(iter=0;iter<tot_player-player-1;iter++)
    {
         if ( recv ( clientSocket, points, 10, 0) < 0 )
                {
                    printf("[ERROR] Connection closed prematurely.\n");
                }

                int j;
                printf("-----POINTS TABLE------\n");
                printf("-----------------------\n");
                for (j = 0; j < tot_player; ++j)
                {
                    printf("-----------------------\n");
                    printf("PLAYER %d | %c |\n",j+1,points[j]);
                    printf("-----------------------\n");
                }
                if ( recv ( clientSocket, buffer, 10, 0) < 0 )
                {
                    printf("[ERROR] Connection closed prematurely.\n");
                }   
    }
    printf("phase 2 begins\n");
    int jhu=5;
    points[0]='u';
    points[1]='u';
    points[2]='u';
    getchar();
    char time_send[19];
    while(jhu--)
    {
        printf("%s\n", points);
            if ( recv ( clientSocket, points, 10, 0) < 0 )
                {
                    printf("[ERROR] Connection closed prematurely.\n");
                }
                printf("%s\n", points);
                int j;
                printf("-----POINTS TABLE------\n");
                printf("-----------------------\n");
                for (j = 0; j < tot_player; ++j)
                {
                    printf("-----------------------\n");
                    printf("PLAYER %d | %c |\n",j+1,points[j]);
                    printf("-----------------------\n");
                }
        if (recv ( clientSocket, buffer, 10, 0) < 0 )
        {
            printf("[ERROR] Connection closed prematurely.\n");
        }

//**************************************************************
        time_t start_t, end_t;
        double diff_t;
        char str[100];
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        scanf("%s",userInput);
        printf("%s\n",userInput);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        //DO WHATEVER WITH DELTA_US
        printf("Execution time = %lu\n", delta_us);

//*****************************************************************

        if ( send ( clientSocket, time_send,inputLength, 0) != inputLength )
        {
             printf("[ERROR] Unable to send the data.\n");
        }
        if ( send ( clientSocket, userInput,inputLength, 0) != inputLength )
        {
             printf("[ERROR] Unable to send the data.\n");
        }
    }
    close( clientSocket );
    return 0;
}
