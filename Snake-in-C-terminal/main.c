#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "rlutil.h"

struct Tile
{
    char contents;
    short timeleft;
};

long GameLoop(short inspeed, short mapsize, short snakelen, short foodcount, char walltp, char modbad, char modshuffle, char modinput, char modramp);
void ConstructWalls(short mapsize, char walltp);
void PrecisionPrint(short x, short y, char type);
long IncreaseScore(long score, short gamespeed, float multiplier, short foodcount);
void ImprintIfHighScore(long score);
void BubbleSort(long arr[], int size);

int main()
{
    FILE* hsfile;
    hsfile = fopen("snake highscores.txt", "r");
    if(hsfile==NULL)
    {
        hsfile = fopen("snake highscores.txt", "w");
        fprintf(hsfile, "500\n450\n400\n350\n300\n250\n200\n150\n100\n50");
    }
    fclose(hsfile);
    char key=0, walltp='n', modbad='n', modshuffle='n', modinput='n', modramp='n';
    short inspeed=3, choice=0, mapsize=30, snakelen=2, foodcount=1;
    while(1)
    {
        printf("\x1b[?25l\x1b[1J\x1b[H");
        printf("\nWelcome to snake. Use w,a,s,d to to navigate the menu, k to confirm, p to pause during game.\n\n\n\n\x1b[0m");
        if(choice==0) printf("\x1b[47m\x1b[30m > ");
        printf("Start\n\n\x1b[0m");
        if(choice==1) printf("\x1b[47m\x1b[30m > ");
        printf("Initial speed < %hd >\n\n\x1b[0m", inspeed);
        if(choice==2) printf("\x1b[47m\x1b[30m > ");
        printf("Map Size < %hdx%hd > (make sure to properly zoom out your terminal on bigger maps)\n\n\x1b[0m", mapsize, mapsize);
        if(choice==3) printf("\x1b[47m\x1b[30m > ");
        printf("Initial length < %hd > \n\n\x1b[0m", snakelen-1);
        if(choice==4) printf("\x1b[47m\x1b[30m > ");
        printf("Food count < %hd > \n\n\x1b[0m", foodcount);
        if(choice==5) printf("\x1b[47m\x1b[30m > ");
        printf("Do walls teleport? < %c > \n\n\x1b[0m", walltp);
        if(choice==6) printf("\x1b[48;5;160m\x1b[38;5;189m > ");
        printf("Modifier: Bad spawns < %c > (food intentionally spawns at bad spots)\n\n\x1b[0m", modbad);
        if(choice==7) printf("\x1b[48;5;160m\x1b[38;5;189m > ");
        printf("Modifier: Controls shuffle < %c > (w, a, s, d keys get shuffled inbetween each other)\n\n\x1b[0m", modshuffle);
        if(choice==8) printf("\x1b[48;5;160m\x1b[38;5;189m > ");
        printf("Modifier: Misinput < %c > (small chance for snake to randomly change direction)\n\n\x1b[0m", modinput);
        if(choice==9) printf("\x1b[48;5;160m\x1b[38;5;189m > ");
        printf("Modifier: Ramp up < %c > (game keeps speeding up the longer it goes)\n\n\x1b[0m", modramp);
        if(choice==10) printf("\x1b[47m\x1b[30m > ");
        printf("Quit game\n\n\x1b[0m");
        key = getch();
        if(key=='w' && choice>0) choice--;
        if(key=='s' && choice<10) choice++;
    // speed change
        if(key=='d' && choice==1 && inspeed<10) inspeed++;
        if(key=='a' && choice==1 && inspeed>1) inspeed--;
    // map size change
        if(key=='d' && choice==2 && mapsize<60) mapsize+=2;
        if(key=='a' && choice==2 && mapsize>10) mapsize-=2;
    // snake length change
        if(key=='d' && choice==3 && snakelen<201) snakelen++;
        if(key=='a' && choice==3 && snakelen>2) snakelen--;
    // snake length change
        if(key=='d' && choice==4 && foodcount<10) foodcount++;
        if(key=='a' && choice==4 && foodcount>1) foodcount--;
    // wall teleport option
        if(key=='d' && choice==5 && walltp=='n') walltp='y';
        if(key=='a' && choice==5 && walltp=='y') walltp='n';
    // bad spawns
        if(key=='d' && choice==6 && modbad=='n') modbad='y';
        if(key=='a' && choice==6 && modbad=='y') modbad='n';
    // key shuffle
        if(key=='d' && choice==7 && modshuffle=='n') modshuffle='y';
        if(key=='a' && choice==7 && modshuffle=='y') modshuffle='n';
    // input failure
        if(key=='d' && choice==8 && modinput=='n') modinput='y';
        if(key=='a' && choice==8 && modinput=='y') modinput='n';
    // ramp up
        if(key=='d' && choice==9 && modramp=='n') modramp='y';
        if(key=='a' && choice==9 && modramp=='y') modramp='n';
        if(key=='k')
        {
            switch(choice)
            {
            case 0:
                ImprintIfHighScore(GameLoop(inspeed, mapsize, snakelen, foodcount, walltp, modbad, modshuffle, modinput, modramp));
                printf("\x1b[0m\x1b[2J");
                break;
            case 5:
                if(walltp=='n') walltp='y';
                else if(walltp=='y') walltp='n';
                break;
            case 6:
                if(modbad=='n') modbad='y';
                else if(modbad=='y') modbad='n';
                break;
            case 7:
                if(modshuffle=='n') modshuffle='y';
                else if(modshuffle=='y') modshuffle='n';
                break;
            case 8:
                if(modinput=='n') modinput='y';
                else if(modinput=='y') modinput='n';
                break;
            case 9:
                if(modramp=='n') modramp='y';
                else if(modramp=='y') modramp='n';
                break;
            case 10:
                printf("Quitting now!");
                return(0);
            }
        }
    }
    return 0;
}

long GameLoop(short inspeed, short mapsize, short snakelen, short foodcount, char walltp, char modbad, char modshuffle, char modinput, char modramp)
{
///////////////// Initialization of Variables ////////////////////
    FILE* hsfile=0;
    float multiplier=1;
    long score=0, highscorescan=0;
    int gamestartingtime = (clock()/CLOCKS_PER_SEC), currentgametime=0;
    short snakex=mapsize/2, snakey=mapsize/2, snakedir=0, foodgenx=(rand()%mapsize), foodgeny=(rand()%mapsize), gamespeed=inspeed; // 0=right, 1=down, 2=left, 3=up for snakedir
    short buton0=0, buton1=1, buton2=2, buton3=3, increment=0;
    char key=0;
    struct Tile map[mapsize*mapsize];
    for(short i=0; i<mapsize*mapsize ;i++)
    { // wipe the map[] array
        map[i].contents = 0;
        map[i].timeleft = 0;
    }
    if(modbad=='y') multiplier+=0.5;
    if(modinput=='y') multiplier+=3.5;
    if(modshuffle=='y') multiplier+=2.0;
    if(modramp=='y') multiplier+=0.2;
///////// First Foods Spawn //////////
    for(short i=0; i<foodcount ;i++)
    {
        while(map[foodgeny*mapsize+foodgenx].contents!=0)
        {
            foodgenx=(rand()%mapsize);
            foodgeny=(rand()%mapsize);
        }
        map[foodgeny*mapsize+foodgenx].contents = 'a';
    }
//////////// Print Walls /////////////
    printf("\x1b[2J");
    ConstructWalls(mapsize, walltp);
//////////// Print Highscores //////////////
    hsfile = fopen("snake highscores.txt", "r");
    for(short i=0; i<10 ;i++)
    {
        printf("\x1b[0m");
        fscanf(hsfile, "%ld\n", &highscorescan);
        if(i==0) printf("\x1b[38;5;220m");
        else if(i==1) printf("\x1b[38;5;250m");
        else if(i==2) printf("\x1b[38;5;202m");
        else printf("\x1b[38;5;242m");
        printf("\x1b[%hd;%hdH%hd. %ld", 8+2*(i+1) ,mapsize*2+6, i+1, highscorescan);
    }
    fclose(hsfile);
///////////// wasd shuffle /////////////
    if(modshuffle=='y')
    {
        increment = 1+rand()%3;
        buton0+=increment;
        buton1+=increment;
        buton2+=increment;
        buton3+=increment;
        buton0=buton0%4;
        buton1=buton1%4;
        buton2=buton2%4;
        buton3=buton3%4;
    }

/////////////// GAME LOOP ////////////////

    while(1)
    {
        clock_t begin = clock();
        currentgametime = ((clock()/CLOCKS_PER_SEC)-gamestartingtime);
    ///////////// Taking Input //////////////
        if(modinput=='n')
        {
            if(kbhit() == 1) key = getch();
            switch(key)
            {
            case 'w': if(snakedir!=buton3) snakedir=buton1; break;
            case 'a': if(snakedir!=buton0) snakedir=buton2; break;
            case 's': if(snakedir!=buton1) snakedir=buton3; break;
            case 'd': if(snakedir!=buton2) snakedir=buton0; break;
            case 'p': getch(); key=0; begin = clock(); break;
            }
        }
        else
        {
            if(kbhit() == 1) key = getch();
            switch(key)
            {
            case 'w': if(snakedir!=buton3) snakedir=buton1; break;
            case 'a': if(snakedir!=buton0) snakedir=buton2; break;
            case 's': if(snakedir!=buton1) snakedir=buton3; break;
            case 'd': if(snakedir!=buton2) snakedir=buton0; break;
            case 'p': getch(); key=0; begin = clock(); break;
            }
            if(rand()%100<3)
            {
                switch(1+rand()%4)
                {
                case 1: key='w'; break;
                case 2: key='a'; break;
                case 3: key='s'; break;
                case 4: key='d'; break;
                }
            }
        }
    ////////////// Moving The Snake ////////////////
        map[snakey*mapsize+snakex].contents='s';
        map[snakey*mapsize+snakex].timeleft=snakelen;
        switch(snakedir)
        {
        case 0: snakex++; break;
        case 1: snakey--; break;
        case 2: snakex--; break;
        case 3: snakey++; break;
        }
        PrecisionPrint(snakex, snakey, 's');
        if(walltp=='n') // wall collision check
        {
            if(snakex>mapsize-1 || snakex<0) return score;
            if(snakey>mapsize-1 || snakey<0) return score;
        }
        else
        { // wall teleportation    // unreadable as fuck but it basically reprints the wall before teleporting the snake so wall doesnt green
            if(snakex>mapsize-1){PrecisionPrint(snakex, snakey, 'w');snakex=0;}
            if(snakex<0){PrecisionPrint(snakex, snakey, 'w');snakex=mapsize-1;}
            if(snakey>mapsize-1){PrecisionPrint(snakex, snakey, 'w');snakey=0;}
            if(snakey<0){PrecisionPrint(snakex, snakey, 'w');snakey=mapsize-1;}
        }
    // collision check for snakes body
        if(map[snakey*mapsize+snakex].contents == 's') return score;
    // collision check for food
        if(map[snakey*mapsize+snakex].contents == 'a')
        {
            snakelen++;
            score = IncreaseScore(score, gamespeed, multiplier, foodcount);
            map[snakey*mapsize+snakex].contents = 's';
    ////////////////// Food Generation ////////////////////
            if(modbad=='n')
            {
                while(map[foodgeny*mapsize+foodgenx].contents!=0)
                {
                    foodgenx=(rand()%mapsize);
                    foodgeny=(rand()%mapsize);
                }
                map[foodgeny*mapsize+foodgenx].contents = 'a';
            }
            else
            { // this whole chunk only for the bad spawns modifier
                foodgenx=(rand()%mapsize);
                foodgeny=(rand()%mapsize);
                if(foodgenx==0 || foodgenx==mapsize-1 || foodgeny==0 || foodgeny==mapsize-1) goto CORNERSPAWN;
                BADSPAWNSRESTART:
                while(map[foodgeny*mapsize+foodgenx].contents!='s')
                {
                    foodgenx=(rand()%mapsize);
                    foodgeny=(rand()%mapsize);
                }
                switch(rand()%4)
                {
                    case 0: foodgenx++;
                    case 1: foodgeny++; break;
                    case 2: foodgenx--;
                    case 3: foodgeny--; break;
                }
                if(map[foodgeny*mapsize+foodgenx].contents=='s')
                    goto BADSPAWNSRESTART;
                if(foodgenx<0 || foodgeny<0 || foodgenx>mapsize-1 ||foodgeny>mapsize-1)
                    goto BADSPAWNSRESTART;
                CORNERSPAWN:
                map[foodgeny*mapsize+foodgenx].contents = 'a';
            }
        }
    ////////////////// Printing The Screen //////////////////
        for(short y=0; y<mapsize ;y++)
        {
            for(short x=0; x<mapsize ;x++)
            {
                if(map[y*mapsize+x].timeleft > 0) map[y*mapsize+x].timeleft--;
                if(map[y*mapsize+x].timeleft == 0 && map[y*mapsize+x].contents == 's')
                {
                    map[y*mapsize+x].contents = 0;
                    PrecisionPrint(x,y,map[y*mapsize+x].contents);
                }
                if(map[y*mapsize+x].contents != 0) PrecisionPrint(x,y,map[y*mapsize+x].contents);
            }
        }
    /////////////// Setting the game speed and some technical stuff ////////////////
        if(modramp == 'y') gamespeed = inspeed + currentgametime/60;
        clock_t end = clock();
        usleep(abs((300000/gamespeed)-(end - begin)*1000));
        printf("\x1b[0m\x1b[2;%hdHTime: %hd\x1b[4;%hdHScore: %ld", mapsize*2+6, currentgametime, mapsize*2+6 ,score);
    }
}

void ConstructWalls(short mapsize, char walltp)
{
    if(walltp=='y') printf("\x1b[H\x1b[48;5;51m");
    else printf("\x1b[H\x1b[48;5;255m");
    for(short i=0; i<mapsize+2 ;i++) printf("  ");
    for(short i=0; i<mapsize ;i++) printf("\n  \x1b[%hdC  ", mapsize*2);
    printf("\n");
    for(short i=0; i<mapsize+2 ;i++) printf("  ");
}

void PrecisionPrint(short x, short y, char type)
{
    if(type=='a')printf("\x1b[48;5;196m");
    else if(type=='s')printf("\x1b[48;5;46m");
    else if(type=='w')printf("\x1b[48;5;51m");
    else printf("\x1b[48;5;0m");
    printf("\x1b[%hd;%hdH", y+2, x*2+3); printf("  ");
    return;
}

long IncreaseScore(long score, short gamespeed, float multiplier, short foodcount)
{
    int baseaddedscore=(10*gamespeed)-(foodcount*5);
    if(baseaddedscore<10) baseaddedscore=10;
    return (score + baseaddedscore*multiplier);
}

void ImprintIfHighScore(long score)
{
    short i=0;
    long scores[11] = {0};
    FILE* hsfile;
    hsfile = fopen("snake highscores.txt", "r");
    for(i=0; i<10 ;i++) fscanf(hsfile, "%ld\n", &scores[i]);
    fclose(hsfile);
    scores[10] = score;
    BubbleSort(scores, 11);
    hsfile = fopen("snake highscores.txt", "w");
    for(i=0; i<10 ;i++) fprintf(hsfile, "%ld\n", scores[i]);
    fclose(hsfile);
    return;
}

void BubbleSort(long arr[], int size)
{
    int i, j, temp;
    for(i=0; i<size-1 ;i++)
        for(j=0; j<size-i-1 ;j++)
            if(arr[j] < arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}
