#include "csapp.h"
#define TERMINAL 0


int main(int argc, char **argv)
{
  char buf[MAXLINE]; //INPUTS FROM TERMINAL
  char serverbuf[MAXLINE]; //MESG FROM SERVER
  char chat[MAXLINE]; //CHAT
  char move[3]; //MOVE
  int turn = 0; //VARIABLE FOR YOUR TURN
  int port; //PORT FOR CONNECTION
  char *serverip; //SERVER IP FOR CONNECTION
  int clientfd; //DESCRIPTOR OF CONNECTED SERVER
 
  
  
/* Check arguments */
  if (argc != 3)
    {
      fprintf(stderr, "Usage: %s <server ip address> <port number>\n", argv[0]);
      exit(0);
    }
  

  serverip = argv[1]; //GET SERVER IP FROM TERMINAL
  port =  atoi(argv[2]); //GET PORT FROM TERMINAL
  
  clientfd = Open_clientfd(serverip,port); //CONNECT TO SERVER
 
  printf ("CONNECTION ESTABLISHED!\nPlease wait for the other player and boards to connect\n"); 
  
  recv(clientfd, serverbuf, sizeof(serverbuf),0); //WAIT FOR MESSAGE FROM SERVER SAYING PLAYERS CONNECTED
  
  printf("%s",serverbuf); //MESSAGE "ALL PLAYERS CONNECTED, YOU MAY BEGIN CHAT"
  
  fflush(stdout);
  
  //CREATE FD SET FOR SELECT
  int maxfd;
  fd_set fds;
  int selectval;
  maxfd = (clientfd > TERMINAL)?clientfd:TERMINAL;
  
  FD_ZERO(&fds);

  while(1)
    {
      
      FD_SET(clientfd, &fds); 
      FD_SET(TERMINAL, &fds); 
       
      //SELECT READY DESCRIPTORS
      selectval = select(maxfd + 1, &fds, NULL, NULL, NULL);
      if (selectval == -1)
	{
	  perror("select failed");
	  exit(3);
	}


      bzero(serverbuf,sizeof(serverbuf)); //CLEAR SERVER BUFFER
      bzero(buf,sizeof(buf)); //CLEAR TERMINAL BUFFER
      bzero(move,sizeof(move)); //CLEAR MOVE BUFFER
      bzero(chat,sizeof(chat)); //CLEAR CHAT BUFFER
 
     if (FD_ISSET(TERMINAL,&fds)) //CHECK IF TERMINAL IS READY TO BE READ
	{
	  if((Fgets(buf,sizeof(buf),stdin)!= 0)) //STORE TERMINAL INPUT 
	    {
	      //PARSE FOR EITHER MOVE OR CHAT
	      if (buf[0] == '-') //MOVE
		{
		  if (turn == 1) //DO STUFF IF YOUR TURN
		    {
		      if (isdigit(buf[1])) //CHECK IF VALID MOVE
			{
			  if (buf[1] == '8' || buf[1] == '9' || buf[1] =='0')
			    {
			      printf("INVALID COLUMN CHOICE PLEASE TRY AGAIN\n"); //INVALID COLUMN CHOICE
			    }
			  else
			    {
			      strncpy(move,buf,2);
			      Write(clientfd,move,2); //WRITE -d TO SERVER
			      turn = 0;
			    }
			}
		      else
			{
			  printf("INVALID MOVE, PLEASE INPUT -d WHERE d IS AN INTEGER 1-7\n"); //INVALID MOVE INPUT
			}
		    }
		  else //TURN = 0
		    {
		      printf("IT IS NOT YOUR TURN PLEASE WAIT OR CONTINUE CHAT\n");
		    }
		}
	      else
		{
		  strncpy(chat,buf,sizeof(buf));
		  Write(clientfd,chat,sizeof(chat)); //WRITE CHAT MESSAGE TO SERVER TO PASS TO OTHER CLIENT
		}
	    }
	}

     else if (FD_ISSET(clientfd,&fds)) //CHECK IF SOCKET IS READY TO BE READ
       {
	 if((Read(clientfd, serverbuf, sizeof(serverbuf)) > 0)) //CHECK IF MESSAGES WERE SENT TO CLIENT FROM SERVER
	   {
	     if (strcmp(serverbuf,"GAME OVER\n")==0) //IF GAME IS OVER EXIT PROGRAM
	       {
		 printf("%s1",serverbuf);
      		 exit(0);
	       }
	     else if (strcmp(serverbuf,"YOUR TURN\n")==0) //IF YOUR TURN SET VARIABLE TRUE
	       {
		 turn = 1;
		 printf("%s",serverbuf);
		
	       }
	     else if (strcmp(serverbuf,"OPPONENTS TURN\n")==0) //IF NOT YOUR TURN SET VARIABLE FALSE
	       {
		 turn = 0;
		 printf("%s",serverbuf);

      
	       }
	     else //IF NONE OF ABOVE MESSAGES WERE SENT PRINT THE SERVER MESSAGE
	       {
		 printf("%s",serverbuf);

	       }
	   }
       }
    }
  return 0;
}
  
