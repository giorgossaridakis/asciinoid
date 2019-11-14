/* ASCIINoid - classical Arkanoid game */
#include <windows.h>
#include <constrea.h>

/* numeric assignments */
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define BORDER 'X'
#define INITIAL_racket 5
#define MAXBALLS 30
#define MAXTILES 770
#define MAXGIFTS 6

void draw_screen();
void showCursor(bool showFlag);
void message(int flag, char message[]);
void randomizer(long int depth);
int mod (double a, double b);
int check_rarity(int rarity);
void reset_ball(struct ball_prototype *ball);
void reset_tiles();
int check_coord_limits();
int check_ball(struct ball_prototype *ball, char c);
void reset_gifts();

/* structure definitions */
struct points {
  int x;
  int y; } ;
  
struct racket_prototype {
   struct points pt;
   char character;
   int color;
   int size;
   int gift;   } ;
  
struct ball_prototype {
   struct points pt;
   char character;
   int color;
   struct points direction;
   int sticky;
   int gift_hold;
   int playability; } ;

struct tile_prototype {
	struct points pt;
	char character;
	int color;
	int life; 
    int playability; } ;
	
struct gift_prototype {
	struct points pt;
	int identity;
	char character;
	int color;
    int life; 
	int playability;  } ;
 
int level=1;
int score=0;
int tiles_level;
int tiles_left;
int sticky_switch=0;

struct racket_prototype racket;
struct tile_prototype tiles[MAXTILES];
struct gift_prototype gifts[MAXGIFTS];

constream win1;
      
int main()
{
  int i,n;
  char c=0;
  int lives=INITIAL_racket;
  int reset_flag=1;
  
  struct ball_prototype balls[MAXBALLS];
   
  randomizer(9);
  
   /* main routine */
   while (c!=27) {
  
   if (reset_flag) {
    draw_screen();
	tiles_level=int(level)*154;
	tiles_left=tiles_level;
    /* initialize parameters */
    racket.pt.x=2;
    racket.pt.y=22;
    racket.character=177;
    racket.color=MAGENTA;
    racket.size=7;
	racket.gift=0;
	sticky_switch=0;
  
    for (i=0;i<MAXBALLS;i++) 
	 reset_ball(&balls[i]);
     balls[0].playability=1;
	 balls[0].sticky=1;
	
    reset_tiles();	
	reset_gifts();
	c=0;

	 reset_flag=0; } 	 
	
    win1 << setxy(9,1);
    win1 <<setattr(RED)<< "   ";	
    win1 << setxy(9,1);
    win1 <<setattr(RED)<< lives;
	win1 << setxy(21,1);
	win1 << setattr(YELLOW) << "    ";
	win1 << setxy(21,1);
	win1 << setattr(YELLOW) << level;
	win1 << setxy(39,1);
    win1 << setattr(GREEN)	<< "    "; 
	win1 << setxy(39,1);
    win1 << setattr(GREEN)	<< tiles_left; 
	win1 << setxy(51,1); 
	win1 << setattr(BLUE) << "    ";
	win1 << setxy(51,1); 
	win1 << setattr(BLUE) << score;
	
	/* show the racket for better display 
	for (n=0;n<racket.size;n++) {	
	  win1 << setxy(racket.pt.x+n, racket.pt.y);
	 win1 << setattr(racket.color) << racket.character; } */
	 
	/* erase racket, tiles, gifts and balls */
	for (n=0;n<racket.size;n++) {	
	  win1 << setxy(racket.pt.x+n, racket.pt.y);
	 win1 << setattr(racket.color) << " "; } 
	for (i=0;i<tiles_level;i++) {
	if (!tiles[i].playability) {
	  win1 << setxy(tiles[i].pt.x, tiles[i].pt.y);
    win1 << setattr(tiles[i].color) << " "; } }	 
	for (i=0;i<MAXGIFTS;i++) {
	 if (gifts[i].playability) {
	  win1 << setxy(gifts[i].pt.x, gifts[i].pt.y-1);
	 win1 << setattr(gifts[i].color) << " "; } }
	for (i=0;i<MAXBALLS;i++) {
 	 if (balls[i].playability) {
	  win1 << setxy(balls[i].pt.x, balls[i].pt.y);
 	 win1 << setattr(balls[i].color) << " ";  } }
	 
	/* check for sticky balls */
	for (i=0;i<MAXBALLS;i++) 
	 if (balls[i].playability && balls[i].sticky && balls[i].pt.y==racket.pt.y-1 && c==32) {
	  balls[i].sticky=0;
	 c=0; }
   
   	/* check if balls hit on walls or tiles and move them */
	for (i=0;i<MAXBALLS;i++) {
	 if (check_ball(&balls[i], c)==-1)
	reset_ball(&balls[i]); }
	
	/* keyboard controls */
    if(kbhit()) 
	 c=getch();
   
    switch (c) {
	 case LEFT:
	 if (racket.pt.x>2)
	   racket.pt.x--;
     break;
	 case RIGHT:
	  if (racket.pt.x+racket.size <79)
	   racket.pt.x++;
     break;
	 case '+':
	  for (i=0;i<MAXBALLS;i++) {
	   if (!(balls[i].playability)) {
		balls[i].playability=1;
		balls[i].pt.x=(racket.pt.x+racket.size/2)+1;
		balls[i].pt.y=racket.pt.y-1;
		balls[i].sticky=sticky_switch;
	  i=MAXBALLS; } }
	 c=0;
     break;	  
	 case '*':
	  level+=1;
	  c=0;
	 break ;
     case '/':
	  level-=1;
	  c=0;
	 break;
	 case '.':
	  reset_flag=1;	 
     break;
	 case 'p':
	  for (n=0;n<racket.size;n++) {	
	   win1 << setxy(racket.pt.x+n, racket.pt.y);
	   win1 << setattr(racket.color) << racket.character; } 
	  message(1, "paused");
	  c=getch();
	  message(0, "paused");
	  c=0;
 	 break;
     case 27:
      message(1, "Killed by key");
	  showCursor(true);
	  exit(-1);
    break; }	
  
	/* show racket, tiles, gifts and balls */
	 for (n=0;n<racket.size;n++) {	
	  win1 << setxy(racket.pt.x+n, racket.pt.y);
	 win1 << setattr(racket.color) << racket.character; } 
	for (i=0;i<tiles_level;i++) {
	 if (tiles[i].playability) {
	  win1 << setxy(tiles[i].pt.x, tiles[i].pt.y);
     win1 << setattr(tiles[i].color) << tiles[i].character; } } 
	for (i=0;i<MAXGIFTS;i++) {
	 if (gifts[i].playability) {
	  win1 << setxy(gifts[i].pt.x, gifts[i].pt.y);
	 win1 << setattr(gifts[i].color) << gifts[i].character; } }
	for (i=0;i<MAXBALLS;i++) {
	if (balls[i].playability) {
	 win1 << setxy(balls[i].pt.x, balls[i].pt.y);
	win1 << setattr(balls[i].color) << balls[i].character;  } } 
	
	/* effects and controls of gifts */
	switch (racket.gift) {
	 case 10:
	  if (racket.size<16) {
	   if (racket.pt.x+racket.size>72) 
	    racket.pt.x-=2;
	   racket.size+=2; }
	  sticky_switch=0;
	  racket.gift=0;
	 break;
	 case 20:
	  if (racket.size<29) {
	   if (racket.pt.x+racket.size>58) 
	    racket.pt.x-=7;
	  racket.size+=7; } 
	  sticky_switch=0;
	  racket.gift=0;
	 break;
	 case 30:
      sticky_switch=1;
	/*  for (i=0;i<MAXBALLS;i++)
	   if (balls[i].playability)
		balls[i].sticky=1; */
	  racket.gift=0;
     break;	  
	 case 40:
	  ++lives;
	  sticky_switch=0;
	  racket.gift=0;
	 break;
	 case 50:
	  --lives;
	  sticky_switch=0;
	  racket.gift=0;
	break;
    case 60:
	 for (n=0;n<racket.size;n++) {	
	  win1 << setxy(racket.pt.x+n, racket.pt.y);
	 win1 << setattr(racket.color) << " "; } 
     if (racket.size>4)
      racket.size-=2;
     sticky_switch=0;
     racket.gift=0;
    break;	 }	
	/* move active gifts */
	for (i=0;i<MAXGIFTS;i++)
	 if (gifts[i].playability && (gifts[i].pt.y<racket.pt.y))
	  gifts[i].pt.y+=1; 
	
	/* check for balls, lives etc */	
    n=0;
	for (i=0;i<MAXBALLS;i++) 
	 if (balls[i].playability)	
      n++;	
	
	if (!n) {
	 --lives;
	 reset_ball(&balls[0]); 
	 balls[0].playability=1; 
	 balls[0].sticky=1; }
	
	if (!lives) { 
      message(1, "Classical Game Over!");
	  showCursor(true);
     exit(-1); }	
	 
	if (!tiles_left) {
     reset_flag=1;
	level+=1; }

     if (level>5)
      level=5;
     if (level<1) 
      level=1;		 
	  
	/* prepare for loop */
	for (i=1;i<80;i++) {
     win1 << setxy(i,2);
	win1 << setattr(CYAN | CYAN) << BORDER; }
	/* show the racket for better display */
	for (n=0;n<racket.size;n++) {	
	  win1 << setxy(racket.pt.x+n, racket.pt.y);
	 win1 << setattr(racket.color) << racket.character; }
	switch (level) {
	 case 1:
	  Sleep(100);
	 break;
	 case 2:
	  Sleep(50);
	 break;
	 case 3:
	  Sleep(25);
	 break;
	 case 4:
	  Sleep(12.5);
	break; }
     
   } 
   
   showCursor(true);	 
   
  return 0; 
}

/* draw the screen */
void draw_screen()
{
   int i;	
 
    win1.window(1, 1, 80, 25); 
    win1.clrscr();
	showCursor(false);
	win1 << setxy(1,1);
	win1 <<setattr(RED)<< "rackets:  ";
	win1 << setxy(15,1);
	win1 << setattr(YELLOW) << "level:  ";
	win1 << setxy(28,1);
    win1 << setattr(GREEN)	<< "tiles left:   "; 
	win1 << setxy(45,1); 
	win1 << setattr(BLUE) << "score:   ";
    win1 << setxy(67,1);
    win1 <<setattr(BLUE<<4 | YELLOW)	<< "C++ ASCIINoid";
	win1 << setxy(2,24);
	win1 << setattr(GREEN)	<< " | left/right arrow|+ to add balls|* level up|/ level down|. redraw|p pause | ";
 
    for (i=1;i<80;i++) {
     win1 << setxy(i,2);
      win1 << setattr(CYAN | CYAN) << BORDER; 
 	win1 << setxy(i,23);
      win1 << setattr(CYAN | CYAN) << BORDER;  }
    for (i=2;i<23;i++) {
     win1 << setxy(1, i);  
	 win1 << setattr(CYAN | CYAN) << BORDER; 
 	win1 << setxy(79,i);
	 win1 << setattr(CYAN | CYAN) << BORDER;  }
}
   
/* show / hide cursor*/
void showCursor(bool showFlag)
{   //enable/disable cursor visibility
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

/* show message in middle of screen, flag 1 to show, 0 to recover border */
void message(int flag, char message[])
{
  int i;	
	
   if (flag) {
    win1 << setxy(35,12);	
    win1 <<setattr(YELLOW<<4 | RED) << message; }
   else { 
    win1 << setxy(35,12);	
    for (i=0;i<strlen(message);i++)
     win1 << setattr(BLACK | BLACK) << ' '; }
}
  
/* randomize numbers better */
void randomizer(long int depth)
{
  time_t timer;
  int seed_number;
  int i,n,l;

  randomize();

  time(&timer);
  seed_number=mod(timer,10000);
 
   for (n=0;n<seed_number;n++) {

    for (i=0;i<depth;i++)
     l=(rand () % 1111);

  }
}

/* remaining from division */
int mod (double a, double b)
{
int tmp =a/b;

return a-(b*tmp);
}

/* rarity check routine, return 1 if no luck */
int check_rarity(int rarity)
{
  int i,n,l;
  int success[3];
  
    if (rarity<3)
	 return 1;
  
	i=rand()% rarity;
    n=int(rarity/3);
    
    for (l=1;l<4;l++) 
     success[l-1]=n*l;
	for (l=0;l<3;l++) {
	 if (i==success[l])
	  return 1; }
 
  return 0;
}

/* reset ball structure variables */
void reset_ball(struct ball_prototype *ball)
{	
  int r=rand() % 2;
  
     ball->pt.x=(racket.pt.x+racket.size/2)+1;
     ball->pt.y=racket.pt.y-1;
     ball->character=254;
     ball->color=RED;	
     ball->sticky=sticky_switch;	 
     ball->direction.x=(!r) ? -1 : 1;
     ball->direction.y=-1;
     ball->playability=0; 	 	 
}

/* reset tile parameters */
void reset_tiles()
{ 
 int i;
 
 int x=2;
 int y=3;
  
  for (i=0;i<MAXTILES;i++) {
   tiles[i].pt.x=x;
   tiles[i].pt.y=y;
   tiles[i].character=220;
   tiles[i].color=rand() % 7 + 1;
   tiles[i].life=rand() % level + 1;
   tiles[i].playability=1;
   
    if (x==78) {
	 y++;
	x=2; }
    else x++;
	
  }
}

/* check balls, gifts and treat tiles */
int check_ball(struct ball_prototype *ball, char c)
{
  int i,n;
  int playable_gifts=0;
  int r;
   
   /* has the ball fallen off ? */   
   if ((ball->pt.y+1>racket.pt.y) && (ball->playability))
    return -1; 

   /* ball hit walls or ceiling ? */
   if (ball->pt.x==2 && ball->direction.x==-1) 
    ball->direction.x=1;
   if (ball->pt.x==78 && ball->direction.x==1) 
    ball->direction.x=-1;
   if (ball->pt.y==2 && ball->direction.y==-1) 
    ball->direction.y=1;

   /* what happens when the ball hits the racket */
   if (!ball->sticky) {
    for (i=0;i<racket.size;i++) {
	 if (ball->pt.x==racket.pt.x+i && ball->pt.y==racket.pt.y-1) {
	  ball->direction.y=-1;
	  if (c==LEFT && ball->direction.x==-1) 
	   ball->direction.x=-1;
      if (c==LEFT && ball->direction.x==1) 
	   ball->direction.x=-1;
	  if (c==RIGHT && ball->direction.x==1) 
	   ball->direction.x=1; 
      if (c==RIGHT && ball->direction.x==-1) 
   ball->direction.x=1;  } } } 

    /* tile contact and gift generation */
    for (i=0;i<tiles_level;i++) {
	  if (tiles[i].playability && (!(ball->sticky))) {
	   if (ball->pt.x==tiles[i].pt.x && ball->pt.y==tiles[i].pt.y) {
	    ball->direction.y*=-1;
	   tiles[i].life-=1; }
	  if (!tiles[i].life) {
	   tiles[i].playability=0;
      --tiles_left;
	  score+=1; 
	  playable_gifts=0;
	  r=0;
	  for (n=0;n<MAXGIFTS;n++)
	   if (gifts[n].playability)
		++playable_gifts;
	  if (!n)
	   reset_gifts();
	  if (check_rarity(level*10) && playable_gifts<MAXGIFTS) {
      while (gifts[r].playability)	
	    r=rand() % MAXGIFTS;
	   gifts[r].pt.x=tiles[i].pt.x;
	   gifts[r].pt.y=tiles[i].pt.y+1;
	  gifts[r].playability=1; 	 } } } }
     
   /* move the ball */
   if (ball->playability) {
   if (!(ball->sticky)) {
	 ball->pt.x+=ball->direction.x;
   ball->pt.y+=ball->direction.y; }
	if (ball->sticky && ball->pt.y==racket.pt.y-1) 
    ball->pt.x=(racket.pt.x+racket.size/2)+1;
     win1 << setxy(ball->pt.x, ball->pt.y);
	win1 << setattr(ball->color) << ball->character; }
	
	/* gift contact with racket and reshow for clarity */
	for (i=0;i<MAXGIFTS;i++) 
	 if (gifts[i].playability && gifts[i].pt.y==racket.pt.y) 
	  gifts[i].playability=0;
    for (n=0;n<MAXGIFTS;n++) {
	 if (gifts[n].playability) {
	  for (i=0;i<racket.size;i++) 
	   if (gifts[n].pt.x==racket.pt.x+i && gifts[n].pt.y==racket.pt.y-1) {
 	    racket.gift=gifts[n].identity; 
	   gifts[n].playability=0; } } }
	for (i=0;i<MAXGIFTS;i++) {
	 if (gifts[i].playability) {
	  win1 << setxy(gifts[i].pt.x, gifts[i].pt.y);
	 win1 << setattr(gifts[i].color) << gifts[i].character; } }
     
 return 0;
}

/* reset gifts */
void reset_gifts()
{
  int i;
  
   for (i=0;i<MAXGIFTS;i++) {
	gifts[i].pt.x=78;
	gifts[i].pt.y=2;
	gifts[i].color=rand() % 7 +1 ;
	gifts[i].life=50 - rand() % 20;
   gifts[i].playability=0; }
   
   /* lets set up our gift behavior */
   gifts[0].identity=10; /* add two characters to racket width */
   gifts[0].character='O';
   gifts[1].identity=20; /* add seven characters to racket size */
   gifts[1].character='o'; 
   gifts[2].identity=30; /* make ball sticky */
   gifts[2].character='+';
   gifts[3].identity=40; /* add a life */
   gifts[3].character='@';
   gifts[4].identity=50; /* remove a life */
   gifts[4].character='%';
   gifts[5].identity=60; /* cut down the racket size by two characters */
   gifts[5].character='i';
 
}
   