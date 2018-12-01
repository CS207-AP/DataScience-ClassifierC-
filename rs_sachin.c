#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node *nptr;

typedef struct node{
    nptr next;
    nptr prev;
    float* f;
} dll;

nptr listify(char* filename, int nattr, nptr head);
nptr insertAtEnd(nptr head, float *f, int nattr);
void display(nptr head, int nattr);

float mean(nptr head, int i);
float SS(float avg, nptr head, int i, float* deviation); //sum_of_squares
float SP(float* dev1, float* dev2, int n); //sum_of_products

int NUM;

int main(void)
{
  float *dx, *dy;
  float mean_x, mean_y, ss_x, ss_y, sp, b, a;
  int i;

  nptr head;
  head = NULL;
  char* filename = "test.csv";
  head = listify(filename, 2, head);
  display(head, 2);

  mean_x = mean(head, 0); //2 argument is column
  mean_y = mean(head, 1);

  dx = malloc(NUM*sizeof(float));
  dy = malloc(NUM*sizeof(float));

  printf("Mean X: %lg\n", mean_x);
  printf("Mean Y: %lg\n", mean_y);

  ss_x = SS(mean_x, head, 0, dx);
  ss_y = SS(mean_y, head, 1, dy);
  sp = SP(dx, dy, NUM);

  // y = bX + a

  b = sp/ss_x;
  a = mean_y - b*mean_x;

  printf("y = %lgX-%lg\n", b, a);

 //printf("(X-Mx)^2 = %lf\n", s_x);

  return 0;

}

float mean(nptr head, int i)
{
  float sum = 0;
  nptr curr = head;
  int n = 0;

  do{
    sum+=curr->f[i];
    ++n;
    curr = curr->next;
  }while(curr != head);
  NUM = n;
  return (sum/n);
}

float SS(float avg, nptr head, int i, float* deviation)
{
  int j = 0;
  float sum_of_squares;
  sum_of_squares = 0;
  nptr curr = head;

   do{
      deviation[j] = curr->f[i] - avg;
      sum_of_squares+=pow(deviation[j], 2);
      ++j;
      curr = curr->next;
    }while(curr != head);

  return sum_of_squares;
}

float SP(float* dev1, float* dev2, int n)
{
  int i;
  float sum_of_products;
  sum_of_products = 0;
  for(int i = 0; i < n; ++i)
    sum_of_products+= (dev1[i]*dev2[i]);

  printf("Sum of Products: %lg\n", sum_of_products);
  return sum_of_products;
}

nptr listify(char* filename, int nattr, nptr head)
{
    int flag = 0;
    FILE *dataset = fopen(filename, "r");
    while(1)
    {
        int c = 0;
        float* f = malloc(sizeof(float)*nattr);
        while(c!=nattr)
        {
            fscanf(dataset, "%g%*c", &f[c]);
            if(feof(dataset))
                flag = 1;
            c++;
        }
        head = insertAtEnd(head, f, nattr);
        if(flag)
            break;
    }
    return(head);
}

nptr insertAtEnd(nptr head, float *f, int nattr)
{
    nptr curr = head;
    nptr p = malloc(sizeof(dll));
    p->f = malloc(sizeof(float)*nattr);
    p->f = f;
    if(curr==NULL)
    {
        head = p;
        p->next = p;
        p->prev = p;
        return(head);
    }
    else
    {
        p->next = curr;
        p->prev = curr->prev;
        (curr->prev)->next = p;
        curr->prev = p;
        return(head);
    }
}

void display(nptr head, int nattr)
{
    if(head == NULL)
    {
        printf("Empty.\n");
        return;
    }

    else
    {
      nptr curr = head;

        do
        {
          for(int i=0; i<nattr; i++) printf("%g, ", curr->f[i]);
          printf("\n");
          curr = curr->next;
        }while(curr != head);
    }
}
