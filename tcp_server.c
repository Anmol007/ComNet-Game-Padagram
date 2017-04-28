/*
ANMOL K GUPTA 2013B4A7588P
PRYANSHU ARORA 2013B4A7601P
DEEPANSHU NAGPAL 2013B4A7834P
*/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> 
#include  <sys/wait.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <semaphore.h>
#include <sys/mman.h>  
sem_t* sem_arr[100];
sem_t* par_sem;
sem_t* wait_sem;
// sem_t* win_sem;
char* jumble(char* str){
    int length=strlen(str);
    char* str_=malloc(sizeof(char)* (length+1));
    strcpy(str_, str);
    for(int i=0; i<length; i++){
        int index = rand()%length;
        char temp = str_[index];
        str_[index]=str_[length - index- 1];
        str_[length - index- 1]=temp;
    }
    return str_;
}
char* get_string(char* mem,int idx_1,int idx_2)
{
    int p=idx_2-idx_1+1;
    char* ans=(char*)malloc(p);
    int i;
    for (i = 0; i < p; ++i)
    {
        // printf("%c",mem[idx_1+i]);
        ans[i]=mem[idx_1+i];
    }
    // printf("\n");
    // printf("giving%s\n",ans);
    return ans;
}
#define analength 34
int MAXCLIENTS = 1000;
int main()
{
    char* ana[]={"delhi",
"chennai",
"kolkata",
"surat",
"pune",
"jaipur",
"lucknow",
"kanpur",
"nagpur",
"indore",
"thane",
"bhopal",
"patna",
"vadodara",
"ludhiana",
"agra",
"nashik",
"meerut",
"rajkot",
"varanasi",
"srinagar",
"dhanbad",
"amritsar",
"ranchi",
"howrah",
"jabalpur",
"gwalior",
"jodhpur",
"raipur",
"kota",
"guwahati",
"mysore",
"gurgaon",
"aligarh"};


    int serverSocket = 0, newSocket[5];
    char *recvBuffer, *buffer;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddress;
    socklen_t addr_size;
    int recvBytes = 0, recvNumber = 0;
    recvBuffer = (char *)malloc(sizeof(char)*10);
    buffer = (char *)malloc(sizeof(char)*10);
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9011);
    serverAddr.sin_addr.s_addr = inet_addr ( "172.17.26.42" );
    memset ( serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero) );
    memset(recvBuffer, '\0', 10);
    memset(buffer, '\0', 10);
    if ( bind ( serverSocket, ( struct sockaddr *) &serverAddr, sizeof ( serverAddr ) )  < 0)
        printf("[ERROR] Socket binding Failed. \n");
    int cnt=0;
     int i;
    char turn_num[2];
    turn_num[1]='\0';
    while(cnt<MAXCLIENTS)
    {
        if ( listen ( serverSocket, 5 ) ==0 )
            printf ( "Server Scocket Initiated. Listening to its clients : \n" );
        else
            printf("[ERROR] More than limit.\n");
        newSocket[cnt] = accept ( serverSocket, (struct sockaddr *) &clientAddress, &addr_size);
        turn_num[0]=cnt+'0';
        // printf("Sending %s\n",turn_num);
        send  ( newSocket[cnt],turn_num , 10, 0 );  
        if(cnt==0){
            // printf("pappi in cnt\n"); 
            if ( ( recvBytes = recv ( newSocket[cnt], recvBuffer, 9, 0) != -1)){
                MAXCLIENTS=recvBuffer[0]-'0';
                printf("max = %d\n",MAXCLIENTS);
            }
        }
        else{
            turn_num[0]=MAXCLIENTS+'0';
            turn_num[1]='\0';
            send  ( newSocket[cnt], turn_num, 10, 0 ); 
        }
        cnt++;
        // printf("%d\n",cnt);
    }
    int POINT[MAXCLIENTS];
    memset(POINT, 0, sizeof(POINT));
    // printf("cooooooool\n");
    send(newSocket[0], "0", 9,0);
    if ( ( recvBytes = recv ( newSocket[0], recvBuffer, 9, 0) != -1)){
                if(strcmp(recvBuffer, "START")==0){
                    printf("GAME has started..\n");
                }
    
    }
    
    cnt=0;
    char point_table[MAXCLIENTS+1];
    memset(point_table, '0', sizeof(point_table) );
    point_table[MAXCLIENTS]='\0';
    int winner=-1;
    char points[]="points";
    char turn[]="Turn ";
    int num_round=0;
    while ( num_round < MAXCLIENTS*2)
    {
        addr_size = sizeof ( clientAddress );
        if ( newSocket[cnt] < 0)
            printf("[ERROR] Failed to accept the input.\n");
        for (i = 0; i < MAXCLIENTS; ++i)
        {
            // printf("Sending points to %d\n",i);
            send  ( newSocket[i], point_table, 10, 0 );  
            // send  ( newSocket[i], turn, 10, 0 );
            turn_num[0]=cnt+'0';
            send  ( newSocket[i],turn_num , 10, 0 );  
        }
        
            int rand_ana=rand()%analength;
            send  ( newSocket[cnt], jumble(ana[rand_ana]), 10, 0 );
        memset(recvBuffer, '\0', 10);
        if ( ( recvBytes = recv ( newSocket[cnt], recvBuffer, 9, 0) == -1))
            printf("[ERROR] No input received from client.\n");
        printf("Handling client:  %s : %d ", inet_ntoa(clientAddress.sin_addr),cnt);
        printf("Received Packet contains \"%s\"\n\n", recvBuffer);
        if(strcmp(recvBuffer, "DONE") == 0)
        {
            printf("   ===================== GAME OVER ========================  \n\n");
            break;
        }

        if ( strcmp(recvBuffer, ana[rand_ana])==0 )
        {
            
            strcpy( buffer, "1\0");
            POINT[cnt]++;
            point_table[cnt]=POINT[cnt]+'0';
            winner=cnt;
        } 
        else 
        {
            // printf("empty\n");
            strcpy( buffer, "2\0");
        }
        send  ( newSocket[cnt], buffer, 10, 0 );    
        sleep ( 1 );
        cnt=(cnt+1)%MAXCLIENTS;
        num_round++;
    }
    int count=MAXCLIENTS;
    int g;
    int fd=-1;
    int flag=1;
    int    ShmID;
     char    *ShmPTR;
     pid_t  pid;   
     int    status;

     ShmID = shmget(IPC_PRIVATE, 25*(MAXCLIENTS+2), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (char*) shmat(ShmID, NULL, 0);
     if ((int) ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     memset(ShmPTR,'\0',sizeof(ShmPTR));
     printf("Server has attached the shared memory...\n");
     for (g = 0; g < 100; ++g)
     {
         sem_arr[g] = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0 );
        if ((void*)sem_arr[g] == MAP_FAILED) { perror("mmap");  exit(1); }
        sem_init(sem_arr[g],1,0);
     }
     printf("start fork\n");
     int save_g;
    par_sem = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0 );
        if ((void*)par_sem == MAP_FAILED) { perror("mmap");  exit(1); }
        sem_init(par_sem,1,1);
        wait_sem = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0 );
        if ((void*)wait_sem == MAP_FAILED) { perror("mmap");  exit(1); }
        sem_init(wait_sem,1,0);
        // win_sem = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0 );
        // if ((void*)win_sem == MAP_FAILED) { perror("mmap");  exit(1); }
        // sem_init(win_sem,1,0);
        int y;
    for (y = (MAXCLIENTS+1)*25; y < (MAXCLIENTS+1)*25+MAXCLIENTS; ++y)
    {
        ShmPTR[y]='0';
    }
    for (g = 0; g < MAXCLIENTS; ++g)
    {
        if(fork()==0)
        {
            save_g=g;
            fd=newSocket[g];
            g=MAXCLIENTS;
        }
        else
        {
            // count--;
            flag=0;
        }
    }
    
    while(1)
    {
        if(flag==1)
        {
            sem_wait(sem_arr[save_g]);
            // printf("SENDING points\n");
            int idx_1=(MAXCLIENTS)*25,idx_2=((MAXCLIENTS+1)*25)-1;
            int p=idx_2-idx_1+1;
    char* ans=(char*)malloc(p);
    int i;
    printf("%s\n",ShmPTR );
    printf("%d\n",p );
    for (i = 0; i < p ; ++i)
    {
        printf("%c",ShmPTR[idx_1+i]);
        ans[i]=ShmPTR[idx_1+i];
    }
    printf("\n");
    printf("giving%s\n",ans);
            char* point_table=get_string(ShmPTR,(MAXCLIENTS+1)*25,(MAXCLIENTS+2)*25-1);
            send  ( newSocket[save_g], point_table, 10, 0 );    
            char* temp_buf=get_string(ShmPTR,25*MAXCLIENTS,25*(MAXCLIENTS+1)-1);
            printf("sending anagram %s\n",temp_buf);
            send  ( newSocket[save_g],temp_buf , strlen(temp_buf), 0 );  
            memset(recvBuffer, '\0', 10);
            recv(newSocket[save_g],recvBuffer,9,0);
            char time_rec[10];
            strcpy(time_rec,recvBuffer);
            if ( ( recvBytes = recv ( newSocket[save_g], recvBuffer, 9, 0) == -1))
                printf("[ERROR] No input received from client.\n");
            printf("Handling client:  %s : %d ", inet_ntoa(clientAddress.sin_addr),save_g);
            printf("Received Packet contains \"%s\"\n\n", recvBuffer);
            if(strcmp(recvBuffer, "DONE") == 0)
            {
                printf("   ===================== GAME OVER ========================  \n\n");
                break;
            }
            int r;
            for (r =0; r < 10; ++r)
            {
                ShmPTR[save_g*25+r]=recvBuffer[r];
            }
            for (r =0; r < 10; ++r)
            {
                ShmPTR[save_g*25+r+10]=time_rec[r];
            }
            sem_post(wait_sem);
        }
        else
        {
            sem_wait(par_sem);
            int rand_ana=rand()%analength;
            int idx;
            char* jum=jumble(ana[rand_ana]);
            // printf("PARENT jum %s\n",jum);
            for (idx = 0; idx <strlen(jum); ++idx)
            {
                ShmPTR[idx+MAXCLIENTS*25]=jum[idx];
            }
            ShmPTR[idx+MAXCLIENTS*25]='\0';
            // printf("pappppi\n");
            for (idx = MAXCLIENTS*25; idx <25+MAXCLIENTS*25; ++idx)
            {
                printf("%c",ShmPTR[idx] );
            }
            printf("\n");
            for(i=0;i<25*(MAXCLIENTS+2);i++)
            {
                ShmPTR[i]='p';
            }
            for (idx = 0; idx < MAXCLIENTS; ++idx)
            {
                sem_post(sem_arr[idx]); 
            }
            int check_client=MAXCLIENTS;
            while(check_client--)
            {
                sem_wait(wait_sem);
            }
            char min_time[10]="999999999";
            int win=-1;
            for ( idx = 0; idx < MAXCLIENTS; ++idx)
            {
                char* temp_time=get_string(ShmPTR,25*idx+10,25*idx+10+9);
                char* temp_ans=get_string(ShmPTR,25*idx,25*idx+9);
                if(strcmp(min_time,temp_time)>0)
                {
                    if(strcmp(temp_ans,ana[rand_ana])==0)
                    {
                        strcpy(min_time,temp_time);
                        win=idx;
                    }
                }
            }
            ShmPTR[(MAXCLIENTS+1)*25+win]++;
            sem_post(par_sem);
        }    
    }
    // printf("mai %d papa %d\n",getpid(),getppid());
    close ( serverSocket );
    return 0;
}
