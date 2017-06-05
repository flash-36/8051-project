#include<reg51.h>
#include<string.h>
#include<stdlib.h>
//port 0-led 1,2-lcd ,3-keypad
sbit rs=P2^0;
sbit rw=P2^1;
sbit en=P2^2;
sbit r0=P0^7;
sbit r1=P0^6;
sbit r2=P0^5;
sbit r3=P0^4;
sbit c0=P0^3;
sbit c1=P0^2;
sbit c2=P0^1;
sbit c3=P0^0;
int readkey(int m, int n);
void simonsays();
void reactiontime();
void displayv(unsigned long z);
void lcmd(char z);
void display(char message[]);
void ldsp(char z);
void turnon(int m,int n);
void linit();
void waitfor(int time);
void starttimer();
unsigned long iter;
char* strrev(char *str);
int readtimer();
void turnoff();
void timer0() interrupt 1
{ iter++; }

void starttimer()
{ iter=0;
  TH0=TL0=0x00;
  TMOD=0x01;
  IE=0x82;
  TR0=1;
  
  
}

char* strrev(char *str)
{
      char *p1,*p2,temp;

      if (!str||!*str)
          return str;
      for (p1=str,p2=str+strlen(str)-1;p2>p1;++p1,--p2)
      {
            temp=*p1;
            *p1=*p2;
            *p2=temp;
      }
      return str;
}

int readtimer()//make int for efficiency
{ int t;
   t=(TL0+(TH0*16*16));
   TR0=0;
   IE=0x00;//Not really necessary
   return t;
}
void turnoff(){P0=0x0F;}
void turnon(int m,int n)//on led m,n
{
  P0=0x0F;//turn everything off
  switch(m){case 0:r0=1;break;
            case 1:r1=1;break;
            case 2:r2=1;break;
            case 3:r3=1;break;}

  switch(n){case 0:c0=0;break;
            case 1:c1=0;break;
            case 2:c2=0;break;
            case 3:c3=0;break;}
	}



int readkey(int m, int n)//read key and return 0 if its m,n
{ unsigned char c;
	int x,y;
  do{ P3=0x0F;
      c=P3;
      c&=0x0F;
    } while(c!=0x0F);
  do{ 
      do{c=P3;
         c&=0x0F;
        }while(c==0x0F);
       waitfor(20);
       c=P3;
       c&=0x0F;
       
    }while(c==0x0F);

  while(1)
  {P3=0xEF;
   c=P3;
   c&=0x0F;
   if(c!=0x0F) {x=3;break;}
   P3=0xDF;
   c=P3;
   c&=0x0F;
   if(c!=0x0F) {x=2;break;}
   P3=0xBF;
   c=P3;
   c&=0x0F;
   if(c!=0x0F) {x=1;break;}
   P3=0x7F;
   c=P3;
   c&=0x0F;
   if(c!=0x0F) {x=0;break;}
  }

  switch(c){ case 0x0E:y=3;break;
             case 0x0D:y=2;break;
             case 0x0B:y=1;break;
             case 0x07:y=0;break;
           }
if(x==m&&y==n) return 0;
else return 1;
}

void lcmd(char z)
{ P1=z;
  rs=0;
  rw=0;
  en=1;
  en=0;
  waitfor(10);
}
void ldsp(char z)
{ P1=z;
  rs=1;
  rw=0;
  en=1;
  en=0;
  waitfor(100);
}
void linit(){lcmd(0x38);lcmd(0x0E);lcmd(0x01);lcmd(0x80);}
void display(char message[])//LCD display string
{  int i;
   for(i=0;message[i]!='\0';i++) ldsp(message[i]);
}
void displayv(unsigned long z)//LCD display value
{  int i,k=0;char message[7];
   while(z)
   { i=z%10;
     z/=10;
     message[k++]=(i+48);
   } message[k]='\0';
   strrev(message);
   display(message);
}
void waitfor(int time)//appx delay of 'time' millieseconds
{ int i,j;
  for(i=0;i<time;i++)
  for(j=0;j<1275;j++); 

}
void simonsays()
{ int gameover=0;int level=0,i,m,n,am[15],an[15];
  

  while(!gameover)
   { level++;
     
      for(i=0;i<level;i++)
       { 
         if(!am[i])
         {m=(rand()%4);
          am[i]=m;}
         if(!an[i])
         {n=(rand()%4);
          an[i]=n;}
         turnon(am[i],an[i]);
         waitfor(800);
         
       }turnoff();
      for(i=0;i<level;i++)
       { gameover=readkey(am[i],an[i]);
         if(gameover) break;
       } 

     
   } linit();display("Game over");
     waitfor(3000);
     lcmd(0xC0);display("Your score:");
     displayv(level);
     waitfor(5000);
}
void reactiontime()
{ int gameover=0;int i,m,n;unsigned long ticks=0;double clock;
  for(i=0;i<25&&!gameover;i++)
       { 
         m=(rand()%4);
         n=(rand()%4);
         linit();display("Go!Go!Go!");
         starttimer();
         gameover=readkey(m,n);
         if(gameover) break;
         else {
                ticks +=(readtimer()+(iter*65536));
              }
       }  
   linit();display("Game over");
   waitfor(3000);
   lcmd(0xC0);display("Time:");
   if(gameover) {display("N/A");}
   else { clock=ticks*0.001085; displayv(clock);display("msec"); }    
   waitfor(5000);

}

void main()
{ int noob=1;
  while(1)
  { starttimer();
  if(noob){linit();display("Welcome");
           waitfor(3500);noob=0;}
  linit();
  display("Choose game mode");lcmd(0xC0);
  display("1.Memory 2.Reflex");
  if(!readkey(0,0)){ srand(readtimer());simonsays();}
  else{ srand(readtimer());reactiontime();}
  }
} 



