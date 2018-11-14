#include<stdio.h>
#include<conio.h>
#define MAX 10
//MEGHANA ANVEKAR ROLL NO. 99 R4 BATCH 

int check_cycle(int a[][MAX],int n);
void visit(int a[][MAX],int i, int* found,int n);
int path[MAX];
int k;

int main()
{
    k = 0;
    int a[MAX][MAX],n;
    
    printf("\nEnter value of n");    scanf("%d",&n);

    int i,j;
    
    for(i = 0;i < n;++i)
    for(j = 0;j<n;++j)
    scanf("%d",&a[i][j]);
    
    if(check_cycle(a,n))
    printf("\nCycle exists");
    
    else
    printf("\nCycle does not exist");
    
    getch();
    return 0;
}



int check_cycle(int a[][MAX],int n)
{
    int i,found = 0;
    
    for(i = 0;i < n && !found;++i)
    {
          k=0;
          visit(a,i,&found,n);
    }
    return found;
}

void visit(int a[][MAX],int i, int* found,int n)
{
     int pos = k;
     path[pos] = i;
     
     int j;
     for(j = i+1; j%n < n && j%n != i&& k != n-1 && !(*found);++j)
     {
           if(a[i][j%n] == 1 && (j%n)!= path[pos - 1] && (j%n)!= i)
           {
                      path[++k] = (j%n);
                      pos = k;
                      if(path[pos] == path[0] && pos != 0)
                        *found  =1;
                      else 
                      visit(a,j%n,found,n);
                        
           }
     }
     
}
