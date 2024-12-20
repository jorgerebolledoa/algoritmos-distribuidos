/******************************************************************************
 *
 * go-021224.c
 * 
 * Programmer: Ruben Carvajal Schiaffino
 *
 * Santiago de Chile, 2/12/2024
 *
 ******************************************************************************/
 

#include <stdio.h>
#include <stdlib.h>


#define EMPTY     0
#define NORMAL    1
#define FULL      2
#define NOCHIP    0
#define NOTHING   0
#define CHIP1     1
#define CHIP2     2
#define NOOWNER   3
#define NOCHIPT   ' '
#define CHIP1T    '*'
#define CHIP2T    'O'
#define UNVISITED 1
#define VISITED   0
#define SIMBOLIC  'S'
#define TEXTUAL   'T'


struct GOCell {

   unsigned char element, statuscell, prop;
};       


struct Queue {

   unsigned int *memarea, begin, end, qsize;
};  


/*
 * Initializes a Queue
 */
struct Queue *InitQueue(unsigned int size) {

   struct Queue *q;

   q = calloc(1,sizeof(struct Queue));
   q->memarea = calloc(size,sizeof(unsigned int));
   q->qsize = size;
   return q;
}


/*
 * Verifies the queue status
 */
unsigned char StatusQueue(struct Queue *q) {

   if (q->begin == q->end)
      return EMPTY;
   if ((q->end + 1) % q->qsize == q->begin)
      return FULL;
   return NORMAL;
}


/*
 * Enqueue a data in q
 */
void EnQueue(struct Queue *q, unsigned int pos) {

   q->end = (q->end + 1) % q->qsize;
   q->memarea[q->end] = pos;
}


/*
 * Dequeue a data from q
 */
void DeQueue(struct Queue *q, unsigned int *pos) {
   
   q->begin = (q->begin + 1) % q->qsize;
   *pos = q->memarea[q->begin];
}


/*
 *
 */
unsigned int Owner(unsigned int e, unsigned int statusarea) {

   if ((e == CHIP1 || e == CHIP2) && statusarea == NOTHING)
      return e;
   if (e == CHIP1 && statusarea == CHIP2)
      return NOOWNER;
   if (e == CHIP2 && statusarea == CHIP1)
      return NOOWNER;
   return statusarea;
}


/*
 *
 */
unsigned int Analyze(struct GOCell **goboard, unsigned int size, unsigned int x, unsigned int y, struct Queue *q, unsigned int statusarea) {

   unsigned int pos;

   if (goboard[x][y].statuscell == UNVISITED)
      if (goboard[x][y].element == NOCHIP) {
         pos = x * size + y;
         EnQueue(q,pos);
      }
      else
         statusarea = Owner(goboard[x][y].element,statusarea);
   return statusarea;
}


/*
 *
 */
int AnNeighb(struct GOCell **goboard, unsigned int size, struct Queue *q, unsigned int *lregion, unsigned int *statusarea) {

   unsigned int a, x, y, pos;

   a = 0;
   while (StatusQueue(q) != EMPTY) {
      DeQueue(q,&pos);
      x = pos / size;
      y = pos % size;
      lregion[0] = lregion[0] + 1;
      lregion[lregion[0]] = pos;
      if (goboard[x][y].statuscell == UNVISITED) {
         a = a + 1;
         goboard[x][y].statuscell = VISITED;
         *statusarea = Analyze(goboard,size,x - 1,y,q,*statusarea);
         *statusarea = Analyze(goboard,size,x,y + 1,q,*statusarea);
         *statusarea = Analyze(goboard,size,x + 1,y,q,*statusarea);
         *statusarea = Analyze(goboard,size,x,y - 1,q,*statusarea);
      }
   }
   return a;
}


/*
 *
 */
void Process(struct GOCell **goboard, unsigned int n) {

   unsigned int i, j, a, pos, *lregion, x, y;
   unsigned int statusarea;
   struct QUEUE *q;

   q = InitQueue(n * n);
   lregion = calloc(n * n + 1,sizeof(unsigned int));
   statusarea = NOTHING;
   for (i = 1; i <= n - 2; i = i + 1)
      for (j = 1; j <= n - 2; j = j + 1) {
         if (goboard[i][j].element == NOCHIP && goboard[i][j].statuscell == UNVISITED) {
            pos = i * n + j;
	        EnQueue(q,pos);
	        a = AnNeighb(goboard,n,q,lregion,&statusarea);
	        printf("Area = %d Owner = %d\n",a,statusarea);
            for (pos = 1; pos <= lregion[0]; pos = pos + 1) {
               x = lregion[pos] / n;
               y = lregion[pos] % n;
               goboard[x][y].prop = statusarea;
            }
            lregion[0] = 0;
	     }
	     if (goboard[i][j].element != NOCHIP && goboard[i][j].statuscell == UNVISITED)
            statusarea = goboard[i][j].element;
      }
}


/*
 *
 */
void PrintGOBoard(struct GOCell **goboard, unsigned int n, char op) {

   unsigned int i, j;
  
   printf("\n\n");
   for (i = 1; i <= n; i = i + 1) {
     for (j = 1; j <= n; j = j + 1) {
        if (op == SIMBOLIC)
           printf(" %d,%d ",goboard[i][j].element,goboard[i][j].statuscell);
        if (op == TEXTUAL) {
           if (goboard[i][j].element == NOCHIP)
              printf("%1d",goboard[i][j].prop);
           if (goboard[i][j].element == CHIP1)
              printf("%c",CHIP1T);
           if (goboard[i][j].element == CHIP2)
              printf("%c",CHIP2T);
        }
     }
     printf("\n");
   }
}


/*
 *
 */
void ReadGOBoard(struct GOCell **goboard, unsigned int n) {

   unsigned int i, j;

   for (i = 1; i <= n; i = i + 1)
      for (j = 1; j <= n; j = j + 1) {
         scanf("%1d",&goboard[i][j].element);
	     goboard[i][j].statuscell = UNVISITED;
      }
}


/*
 *
 */
int main() {

   unsigned int n, i;
   struct Queue *q;
   struct GOCell **goboard;

   scanf("%d",&n);
   goboard = calloc(n + 2,sizeof(struct GOCell *));
   for (i = 0; i < n + 2; i = i + 1)
      goboard[i] = calloc(n + 2,sizeof(struct GOCell));
   ReadGOBoard(goboard,n);
   PrintGOBoard(goboard,n,SIMBOLIC);
   Process(goboard,n + 2);
   PrintGOBoard(goboard,n,TEXTUAL);
}
