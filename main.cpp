#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <time.h>

typedef struct celPos celPos;
struct celPos
{
    SDL_Rect pos;
    celPos *suiv;
    SDL_Surface *diglett;
};

typedef struct celnb celnb;
struct celnb
{
    SDL_Rect pos;
    SDL_Surface *img;
};

void crierCel(celPos **t, SDL_Rect positionCarre, SDL_Surface *digletts[], int y);
void supCel(celPos **t,SDL_Rect positionCarre);
int numPos(int y);
bool posExist(celPos *tete ,SDL_Rect posCarre);
void remplirTab(SDL_Surface *combinaison[],SDL_Surface *digletts[]);
bool checkWin(celPos * t, SDL_Surface * combinaison[]);
bool diglettExist(SDL_Surface * dig, SDL_Surface *combinaison[]);
void win(SDL_Surface *combinaison[], SDL_Surface *ecran);
SDL_Surface *chercher(celPos *t, int pos);
void  verifier(celPos *t, celnb nb[], int *n, SDL_Surface *combinaison[]);
void crierCelNB(celPos **nb, SDL_Rect posNb, SDL_Surface *boule);
bool ExistInList(SDL_Surface * dig, celPos *t);
bool numExistDsTab(int num, int tab[] , int n);



int main ( int argc, char** argv )
{
  SDL_Surface *ecran = NULL,
              *carre = NULL,
              *background=NULL,
              *digletts[6],
              *combinaison[4],
              *menu,
              *curseur,
              *gagner,
              *perdu,
              *bg2;



  SDL_Rect  positionCarre, positionZero, positionCurseur;
   positionZero.x=0;
   positionZero.y=0;
   positionCarre.x = 0;
   positionCarre.y = 0;
   positionCurseur.x=0;
   positionCurseur.y=0;

 celPos *t = NULL, *q = NULL;
 celnb nb[32];

 SDL_Event event;

 bool continuerPart1 = 0, continuerMenu = 1 ,continuerPart2=0 , ligneVide=1;
 int n=0,y=0,winb=3,ver=0 ,ctrCel=0;


    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode( 300, 500, 32, SDL_HWSURFACE | SDL_FULLSCREEN );

    digletts[0] = IMG_Load("rouge.gif");
    digletts[1] = IMG_Load("vert.gif");
    digletts[2] = IMG_Load("grie.gif");
    digletts[3] = IMG_Load("bleu.gif");
    digletts[4] = IMG_Load("jaune.gif");
    digletts[5] = IMG_Load("marron.gif");
    carre =IMG_Load("pokeball.gif");
    background =IMG_Load("background.jpg");
    menu=IMG_Load("menu.jpg");
    curseur=IMG_Load("curseur.gif");
    gagner=IMG_Load("gagner.jpg");
    perdu=IMG_Load("perdu.jpg");
    bg2=IMG_Load("bg2.jpg");

    srand(time(NULL));
    remplirTab(combinaison,digletts);



    while(continuerMenu)
    {
        SDL_BlitSurface(menu,NULL,ecran,&positionZero);
        SDL_WaitEvent(&event);
                switch (event.type)
                {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                             case SDLK_DOWN:
                                positionCurseur.y+=100;
                                if(positionCurseur.y>200)
                                {
                                    positionCurseur.y=200;
                                }
                             break;

                             case SDLK_UP:
                                positionCurseur.y-=100;
                                if(positionCurseur.y<0)
                                {
                                    positionCurseur.y=0;
                                }
                             break;

                             case SDLK_SPACE:
                                if(positionCurseur.y==0)
                                {
                                     continuerMenu=0;
                                     continuerPart1=1;
                                }
                                else if (positionCurseur.y==100)
                                {
                                    continuerMenu=0;
                                    continuerPart2=1;
                                }
                                else if (positionCurseur.y==200)
                                {
                                    continuerMenu=0;
                                }
                            break;
                        }
                    break;
                 }
                 SDL_BlitSurface(curseur,NULL,ecran,&positionCurseur);
                 SDL_Flip(ecran);
    }



        while(continuerPart1)
        {
                         SDL_BlitSurface(background,NULL,ecran,&positionZero);


            SDL_WaitEvent(&event);
                switch (event.type)
                {
                    case SDL_QUIT:
                       continuerPart1=0;
                    break;

                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                             case SDLK_ESCAPE:
                                continuerPart1=0;
                             break;

                             case SDLK_RIGHT:
                                positionCarre.x+=50;
                                if(positionCarre.x>150)
                                {
                                 positionCarre.x=150;
                                }
                             break;

                             case SDLK_LEFT:
                                positionCarre.x-=50;
                             break;
                        }
                    break;



                    case SDL_MOUSEBUTTONDOWN :                                               /***********CLICK SOURIS********/

                        if(event.button.x>250)
                        {
                            if(event.button.y<300)
                            {
                                if(posExist(t,positionCarre))
                                {
                                  supCel(&t,positionCarre);
                                  ctrCel--;
                                }
                                else
                                {
                                  y=numPos(event.button.y);
                                  crierCel(&t,positionCarre,digletts,y);
                                  ctrCel++;
                                }
                            }
                             else if(event.button.y>450 && ctrCel%4==0 && posExist(t,positionCarre))
                            {
                                ver++;
                                if(checkWin(t,combinaison))           /**checkwin**/
                                {
                                     winb=1;
                                }
                                else if(ver==8)
                                {
                                    winb=0;
                                }

                                 verifier(t,nb,&n,combinaison);


                                 positionCarre.y+=50;
                            }
                        }



                    break;
                }




                                                                                      /******************LES BLITES*/
            q=t;
            while(q != NULL)
            {
                SDL_BlitSurface(q->diglett,NULL,ecran,&q->pos);
                q=q->suiv;
            }

            for(int i=0;i<n;i++)
            {
                SDL_BlitSurface(nb[i].img,NULL,ecran,&nb[i].pos);
            }

            SDL_BlitSurface(carre,NULL,ecran,&positionCarre);

            if(winb==1)
            {
              SDL_BlitSurface(gagner,NULL,ecran,&positionZero);
              win(combinaison,ecran);
            }
            else if(winb==0)
            {
               SDL_BlitSurface(perdu,NULL,ecran,&positionZero);
               win(combinaison,ecran);
            }

            SDL_Flip(ecran);
        }


        while(continuerPart2)
        {
                SDL_BlitSurface(bg2,NULL,ecran,&positionZero);
                win(combinaison,ecran);

                SDL_WaitEvent(&event);
                if(event.key.keysym.sym==SDLK_ESCAPE)
                {
                    continuerPart2=0;
                }

                SDL_Flip(ecran);
        }



                                                                                                          /**SDL_FreeSurface**/
        SDL_FreeSurface(ecran);
        SDL_FreeSurface(carre);
        SDL_FreeSurface(background);
        SDL_FreeSurface(menu);
        SDL_FreeSurface(curseur);
        SDL_FreeSurface(gagner);
        SDL_FreeSurface(perdu);
        SDL_FreeSurface(bg2);
        for(int i=0;i<6;i++)
        {
                    SDL_FreeSurface(digletts[i]);
        }

        SDL_Quit();

    return 0;                                                                                                 /****main ends****/
}


void crierCel(celPos **t, SDL_Rect positionCarre, SDL_Surface *digletts[], int y)
{
    celPos *q = *t;

    *t = (celPos*) malloc(sizeof(celPos));
    (*t)->pos.x= positionCarre.x;
    (*t)->pos.y = positionCarre.y;
    (*t)->suiv=q;
    (*t)->diglett=digletts[y];
}


int numPos(int y)
{
    if(0<y && y<50)
   {
       return 0;
   }
   else if(50<y && y<100)
   {
        return 1;
   }
   else if(100<y && y<150)
   {
        return 2;
   }
   else if(150<y && y<200)
   {
        return 3;
   }
   else if(200<y && y<250)
   {
       return 4;
   }
   else if(250<y && y<300)
   {
       return 5;
   }
}

void supCel(celPos **t,SDL_Rect positionCarre)
{
    celPos *p=*t , *r=p;

    while(p->pos.x != positionCarre.x )
    {
        r=p;
        p=p->suiv;
    }

    if(*t==p)           /*** cas supression tete***/
    {
        (*t)=(*t)->suiv;
    }
    r->suiv=p->suiv;
    free(p);
}

 bool posExist(celPos *tete ,SDL_Rect posCarre)
 {
     while(tete != NULL)
     {
         if(tete->pos.x == posCarre.x  && tete->pos.y == posCarre.y)
         {
            return 1;
         }
         tete=tete->suiv;
     }
     return 0;
 }

void remplirTab(SDL_Surface  *combinaison[],SDL_Surface *digletts[])
 {
     int randomIndex;
     int sauvgardeNum[4];

     for(int i=0;i<4;i++)
     {
         do
         {
             randomIndex=(rand() % (5 - 0 + 1)) + 0;
         }while(numExistDsTab(randomIndex,sauvgardeNum,i));
          sauvgardeNum[i]=randomIndex;

          combinaison[i]=digletts[randomIndex];
     }
 }


 bool checkWin(celPos * t, SDL_Surface * combinaison[])
 {
     int pos =0;
     for(int i=0;i<4;i++)
     {
         if(chercher(t,pos)!=combinaison[i])
        {
            return 0;
        }
        pos+=50;
     }

     return 1;
 }

bool diglettExist(SDL_Surface * dig, SDL_Surface *combinaison[])
{
    for(int i=0;i<4;i++)
    {
        if(dig == combinaison[i])
        {
            return 1;
        }
    }
    return 0;
}


void win(SDL_Surface *combinaison[], SDL_Surface *ecran)
{
    SDL_Rect positionHidden;
    positionHidden.x=0;
    positionHidden.y=450;
    for(int i=0;i<4;i++)
    {
        SDL_BlitSurface(combinaison[i],NULL,ecran,&positionHidden);
        positionHidden.x+=50;
    }
}


SDL_Surface *chercher(celPos *t, int pos)
{
    for(int i =1; i<=4; i++)
    {
        if(t->pos.x == pos)
        return t->diglett;
        t=t->suiv;
    }
}


void  verifier(celPos *t, celnb nb[], int *n, SDL_Surface *combinaison[])
{

    int pos=0, xpos=0;
    SDL_Surface *digActu,
                *boule,
                *blanc = IMG_Load("blanc.gif"),
                *noire =  IMG_Load("noire.gif");


    for(int i=0;i<4;i++)
    {
        digActu=chercher(t,pos);

        if(combinaison[i]==digActu)
        {
            nb[*n].img=noire;
            nb[*n].pos.y=t->pos.y;
            nb[*n].pos.x=xpos;
            *n=*n+1;
            xpos+=10;
        }
        else if(ExistInList(combinaison[i],t))
        {
            nb[*n].img=blanc;
            nb[*n].pos.y=t->pos.y;
            nb[*n].pos.x=xpos;
            *n=*n+1;
            xpos+=10;
        }
        pos+=50;

    }

}


bool ExistInList(SDL_Surface * dig, celPos *t)
{
    for(int i=0;i<4;i++)
    {
        if(dig == t->diglett)
        {
            return 1;
        }
        t=t->suiv;
    }
    return 0;
}


bool numExistDsTab(int num, int tab[] , int n)
{
    for(int i=0;i<n;i++)
    {
        if(tab[i]==num)
        {
            return 1;
        }
    }
    return 0;
}
