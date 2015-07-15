#include "csapp.h"

int main(int argc, char *argv[])
{
  
  int port; //PORT OF SERVER				
  
  int maxfd; //MAX FDS FOR SELECT
  
  int clientfdSize = 2;	//MAX NUMBER OF CLIENTS
  int selectval; //NUMBER OF READY FDS
  int i,n=0,x,y,j=0,k=0;	//COUNTERS AND OTHER VARIABLES		
  int com; //BYTES READ
  int pnum; //PLAYER NUMBER
  char chatmover[MAXLINE]; //BUF RECEIVED FROM CLIENTS
  char chatmoves[MAXLINE]; //BUF TO SEND TO CLIENTS
  char boardbuf[MAXLINE]; //BUF TO SEND TO BOARDS
  char p1buf[MAXLINE]; //BUF TO SEND TO PLAYER 1
  char p2buf[MAXLINE]; //BUF TO SEND TO PLAYER 2
  char* turnbuf = "YOUR TURN\n"; //YOUR TURN
  char* oppbuf = "OPPONENTS TURN\n"; //OPPONENTS TURN
  
  
  
  int serverfd;
  socklen_t clientlen;
  struct sockaddr_in clientaddr;
  fd_set rset;
  
  
  //INITIALIZE FD ARRAYS
  int clientfd[clientfdSize];
  int boardfd[clientfdSize];
  
  //CHECK INPUT ARGUMENTS
  if (argc != 2) 
    {
      fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
      exit(0);
    }
  port = atoi(argv[1]);
  
  //SERVER FD
  serverfd = Open_listenfd(port);
  
  //INITIALIZE MAXFD
  maxfd = serverfd;
  
  //CLEARS SET
  FD_ZERO(&rset);
  
  // INFINITE WHILE LOOP FOR SERVER
  while(1)
    {
      
      clientlen = sizeof(clientaddr);
      
      //SET SERVER FDS IN THE FD
      FD_SET(serverfd, &rset);
      
      
      //SELECT READY DESCRIPTORS
      selectval = select(maxfd + 1, &rset, NULL, NULL, NULL);
      if (selectval == -1)
	{
	  perror("select failed");
	  exit(3);
	}
      
      //WAIT TILL ALL CONNECTIONS HAVE BEEN MADE
      if (k != 2) //DO NOT ACCESS THIS LOOP AFTER CONNECTIONS ARE SET
	{
	  
	  for(i = 0;i<2;i++)
	    {
	      if (FD_ISSET(serverfd, &rset))
		{
		  
		  clientfd[i] = Accept(serverfd,(SA *)&clientaddr, &clientlen);
		  FD_SET(clientfd[i], &rset);
		  
		  if(clientfd[i] > maxfd)
		    {
		      maxfd = clientfd[i] ;
		    }		
		  n = n+1;
		  printf("PLAYER %d HAS CONNECTED\n",n);
		}
	    }
	  
	  for(i = 0;i<2;i++)
	    {
	      if (FD_ISSET(serverfd, &rset))
		{
		  
		  boardfd[i] = Accept(serverfd,(SA *)&clientaddr, &clientlen);
		  FD_SET(boardfd[i], &rset);
		  
		  if(boardfd[i] > maxfd)
		    {
		      maxfd = boardfd[i] ;
		    }		
		  n = n+1;
		  printf("BOARD %d HAS CONNECTED\n",(i+1));
		}
	    }
	  
	  
	  //ONCE ALL CONNECTIONS HAVE BEEN MADE BEGIN GAME
	  if (n == 4)
	    {
	      
	      strncpy(p1buf,"ALL PLAYERS AND BOARDS CONNECTED, YOU ARE PLAYER 1\n",sizeof(p1buf));
	      strncpy(p2buf,"ALL PLAYERS AND BOARDS CONENCTED, YOU ARE PLAYER 2\n",sizeof(p2buf));
	      strncpy(boardbuf,"BOARD CONNECTED\n",sizeof(boardbuf));
	      send(clientfd[0], p1buf,sizeof(p1buf),0);
	      send(clientfd[1], p2buf,sizeof(p2buf),0);
	      send(boardfd[0],boardbuf,sizeof(boardbuf),0);
	      send(boardfd[1],boardbuf,sizeof(boardbuf),0);
	      Write(clientfd[0], turnbuf,11);
	      Write(clientfd[1], oppbuf,16);
	      
	      k=2;
	    }
	}
    
  
      if(j !=0)
	{
	  //CHECK CLIENTS
	  for( x = 0; x < clientfdSize; x++)
	    {
	      
	      //CHECK IF FD IS READY
	      if(FD_ISSET(clientfd[x], &rset))
		{
		  
		  bzero(chatmover,sizeof(chatmover));
		  bzero(chatmoves,sizeof(chatmoves));
		  
		  //MAKE Y THE OTHER CLIENT AND SET PLAYER NUMBERS
		  if (x == 0)
		    {
		      y = 1;
		      pnum = 1;
		    }
		  if (x == 1)
		    {
		      y = 0;
		      pnum = 2;
		    }
		  
		  
		  //READ FROM CLIENT
		  com = read(clientfd[x], chatmover,sizeof(chatmover));
		  
		  if(com == -1)
		    {
		      printf("READ ERROR FROM CLIENT %d\n",x);
		    }
		  else if (com != 0)
		    {
		      if(chatmover[0] != '-')
			{
			  sprintf(chatmoves,"Player %d: %s",pnum,chatmover);
			  Write(clientfd[y],chatmoves,sizeof(chatmoves)); 
			}
		      else 
			{
			  sprintf(chatmoves, "Player %d has added to this column:%c\n", pnum, chatmover[1]);
			  Write(clientfd[y],chatmoves, sizeof(chatmoves)); 
			  Write(clientfd[y],turnbuf, 11);
			  Write(clientfd[x],oppbuf, 16);
			  Write(boardfd[0],&chatmover[1],1);
			  Write(boardfd[1],&chatmover[1],1);
			}
		    }
		  
		}
	      
	    }
	  
	  
	  
	  //CHECK BOARDS
	  for( x = 0; x < clientfdSize; x++)
	    {
	      
	      //CHECK IF FD IS READY
	      if(FD_ISSET(boardfd[x], &rset))
		{
		  bzero(chatmover,sizeof(chatmover));
		  bzero(chatmoves,sizeof(chatmoves));
		  
		  //READ FROM BOARDS WHO WON
		  com = read(boardfd[x], chatmover,sizeof(chatmover));
		  
		  //SEND TO CLIENT WHO WON
		  Write(clientfd[x], chatmover,sizeof(chatmover));
		  Write(clientfd[x],"GAME OVER\n",11);
		}
	    }
	  
	}
      FD_SET(clientfd[0], &rset);
      FD_SET(clientfd[1], &rset);
      FD_SET(boardfd[0],&rset);
      FD_SET(boardfd[1],&rset);
      j=1;
      
}
exit(6);
}

