#include "csapp.h"
#define TERMINAL 0

char activecoin1 = 'o';
char activecoin2 = 'x';
char activecoin = ' ';
int win = 0;
char board[6][7];
int totalmove = 0;

void clearScreen()
{
  system("reset");
}


void drop(int n)
{
  int row = 0;
  n = n-1;
  for(row = 5; row > -1; row--)
    {
      if(board[row][n] == '-')
	break;
    }
  if (row == -1)
    {
      printf("ROW IS FILLED ALREADY, YOU LOSE YOUR TURN FOR CHEATING\n");
      fflush(stdout);
      totalmove--;
    }
    
  if(row != -1)
    {
      board[row][n] = activecoin;
    } 
}
  
int checkforwin(){
  int row = 0;
  int col = 0;
  char curr = board[row][col];
  //STRAIGHT DOWN OR UP WIN
  for (row = 0; row<6; row++) 
    {
      for (col = 0; col<4; col++) 
      {
	curr = board[row][col];
	if (curr != '-' && curr == board[row][col+1] && curr == board[row][col+2] && curr == board[row][col+3]) 
	  {
	    if(activecoin == activecoin1)
	      win = 1;
	    else if (activecoin == activecoin2)
	      win = 2;
	  }
      }
  }
  

  //RIGHT OR LEFT WIN
  for (col = 0; col<7; col++) 
    {
      for (row = 0; row<3; row++) 
	{
	  curr = board[row][col];
	  if ( curr != '-' && curr == board[row+1][col] && curr == board[row+2][col] && curr == board[row+3][col])
	    {
	      if(activecoin == activecoin1)
	      win=1;
	      else
		win = 2;
	    }
	}
    }
  
  //DIAGONAL
  for (row = 0; row<3; row++) 
    {
      for (col = 0; col<4; col++) 
	{
	  curr = board[row][col];
	  if (curr != '-' && curr == board[row+1][col+1] && curr == board[row+2][col+2] && curr == board[row+3][col+3])
	    {
	      if(activecoin == activecoin1)
		win=1;
	      else
		win = 2;
	    }
	}
    }
  

//REVERSE DIAGONAL
for (row = 5; row >= 3; row--) 
  {
    for (col = 0; col<4; col++) 
      {
	curr = board[row][col];
	if (curr != '-' && curr == board[row-1][col+1] && curr == board[row-2][col+2]&& curr == board[row-3][col+3])
	  {
	    if(activecoin == activecoin1)
	      win=1;
	    else
	      win = 2;
	  }
      }
  }
 return win;
} 

void printboard(){

  int col;
  int i;
  printf("CONNECT4\n\n");
  for(i = 0; i<6;i++)
    {
      for(col = 0; col < 7; col++)
	{
	  printf("| %c ", board[i][col]);
	}
      printf("|");
      printf("\n-----------------------------\n");
    }

}

void initializeboard()
{
  int row, col;
  for(row = 0; row < 6; row++)
    {
      for(col = 0; col < 7; col++)
	{
	  board[row][col] = '-';
	}
    }
}

int main(int argc, char **argv)
{
  char buf[MAXLINE]; //INPUTS FROM TERMINAL
  char serverbuf[MAXLINE]; //MESG FROM SERVER
  char chat[MAXLINE]; //CHAT
  char move[MAXLINE]; //MOVE
  int port; //PORT FOR CONNECTION
  char *serverip; //SERVER IP FOR CONNECTION
  int boardfd; //DESCRIPTOR OF CONNECTED SERVER
  activecoin = activecoin1;
  int pmove = 0;
 
 
  
  
  /* Check arguments */
  if (argc != 3)
    {
      fprintf(stderr, "Usage: %s <server ip address> <port number>\n", argv[0]);
      exit(0);
    }
  

  serverip = argv[1]; //GET SERVER IP FROM TERMINAL
  port =  atoi(argv[2]); //GET PORT FROM TERMINAL
  
  boardfd = Open_clientfd(serverip,port); //CONNECT TO SERVER
  
  printf ("CONNECTION ESTABLISHED!\nPlease wait for the other board\n"); 
  
  recv(boardfd, serverbuf, sizeof(serverbuf),0); //WAIT FOR MESSAGE FROM SERVER SAYING PLAYERS/BOARDS CONNECTED CONNECTED
  
  printf("%s",serverbuf); //MESSAGE "ALL PLAYERS/BOARDS CONNECTED, GAME MAY BEGIN"
  
  fflush(stdout);
  clearScreen();
  initializeboard();
  
  while(1)
    {
   
      bzero(serverbuf,sizeof(serverbuf)); //CLEAR SERVER BUFFER
      bzero(buf,sizeof(buf)); //CLEAR TERMINAL BUFFER
      bzero(move,sizeof(move)); //CLEAR MOVE BUFFER
      bzero(chat,sizeof(chat)); //CLEAR CHAT BUFFER
 
      if((Read(boardfd, serverbuf, sizeof(serverbuf)) > 0))
	{
	  totalmove++;
	  //MOVES CAME FROM SERVER
	  pmove = serverbuf[0] - '0';
	  
	  drop(pmove);
	  if (checkforwin() != 0)
	    {
	      if(win == 1)
		{
		  clearScreen();
		  printboard();
		  printf("PLAYER 1 HAS WON\n");
		  write(boardfd, "PLAYER 1 HAS WON\n", 18);
		  exit(0);
		}
	      else
		{
		  clearScreen();
		  printboard();
		  printf("PLAYER 2 HAS WON\n");
		  write(boardfd, "PLAYER 2 HAS WON\n", 18);
		  exit(0);
		}
	    }
	  else
	    {
	      clearScreen();
	      printboard();
	      printf("LAST MOVE MADE AT COLUMN %d\n",pmove);
	      fflush(stdout);
	    }    
	  if(totalmove == 42)
	    {
	      clearScreen();
	      printboard();
	      printf("GAME IS TIED\n");
	      write(boardfd, "GAME TIED\n", 11);
	      exit(0);
	    }  
	  if(activecoin == activecoin1)
	    activecoin = activecoin2;
	  else
	    activecoin = activecoin1;
	}
    }
  
  return 0;
}
  
