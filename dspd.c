#include<stdio.h>
#include<stdlib.h>
#define SIZE 5

typedef  int item_type;

typedef struct node_tag
{
  item_type data;
  struct node_tag * next;
}node;

typedef struct LFU_tag
{
  item_type data;
  int ctr;
  struct LFU_tag* next;
}LFU_node;

typedef struct
{
  node* front;
  node* rear;
  int size;
}LRU;

typedef struct
{
  LFU_node* list;
  int size;
}LFU;

typedef struct
{
  node* list;
  int size;
}MRU;

typedef LRU query;

void print(item_type d);
void init_query(query*);
void init_LRU(LRU* ptr);
void init_LFU(LFU* ptr);
void init_MRU(MRU* ptr);
node* make_node(item_type d);
LFU_node* make_LFU_node(item_type d);
node* node_search(node* list,item_type d);
LFU_node* LFU_search(LFU_node* list,item_type d);
void LFU_update(LFU *ptr, item_type d);
void MRU_update(MRU *ptr, item_type d);
void LRU_update(LRU *ptr, item_type d);
int pop(query *ptr);
void add(query *ptr, item_type d);
void LRU_cache_contents(LRU *ptr);
void LFU_cache_contents(LFU *ptr);
void MRU_cache_contents(MRU *ptr);
void query_contents(query *ptr);

int main()
{
  int i,n,choice,q;
  query query_list;
  init_query(&query_list);
  LRU cache1;LFU cache2;MRU cache3;

  init_LRU(&cache1);
  init_LFU(&cache2);
  init_MRU(&cache3);


  printf("**********************************************************************\n");
  printf("......................CACHE MANAGEMENT SYSTEM.........................");
  printf("**********************************************************************\n");

  printf("\n\n\nMENU :\n");
  printf("\n1. LRU \n2. LFU \n3. MRU\n");
  printf("Enter your choice ...........");
  scanf("%d",&choice);

  printf("\nEnter the no.of queries.....");
  scanf("%d",&n);

  printf("\nEnter queries ......\n");

  switch(choice)
  {
    case 1:
            init_LRU(&cache1);
            for(i=0;i<n;++i)
            {
              printf("\nQuery %d ......",i+1);
              scanf("%d",&q);
              add(&query_list,q);
            //  query_contents(&query_list);
              LRU_update(&cache1,pop(&query_list));
              LRU_cache_contents(&cache1);
            }
            break;

  case 2:

          init_LFU(&cache2);
          for(i=0;i<n;++i)
          {
            printf("\nQuery %d ......",i+1);
            scanf("%d",&q);
            add(&query_list,q);
            LFU_update(&cache2,pop(&query_list));
            LFU_cache_contents(&cache2);
          }
          break;

  case 3:

          init_MRU(&cache3);
          for(i=0;i<n;++i)
          {
            scanf("%d",&q);
            add(&query_list,q);
            MRU_update(&cache3,pop(&query_list));
            MRU_cache_contents(&cache3);
          }
          break;
  default : printf("\nINVALID CHOICE !!!. Enter choice(1-3) only\n");
  }


  return 0;
}
void print(item_type d)
{
  printf("\n\n%d is popped!. \n\n",d);
}
void init_LRU(LRU* ptr)
{
  ptr->size = 0;
  ptr->front = ptr->rear = NULL;

}

void init_LFU(LFU* ptr)
{
  ptr->size = 0;
  ptr->list = NULL;

}
void init_MRU(MRU* ptr)
{
  ptr->size = 0;
  ptr->list = NULL;
}

node* make_node(item_type d)
{
  node* nptr = (node*)malloc(sizeof(node));
  if(nptr)
  {
    nptr->data = d;
    nptr->next = NULL;
  }
  return nptr;
}

LFU_node* make_LFU_node(item_type d)
{
  LFU_node* nptr = (LFU_node*)malloc(sizeof(LFU_node));
  if(nptr)
  {
    nptr->data = d;
    nptr->next = NULL;
    nptr->ctr = 1;
  }
  return nptr;
}

node* node_search(node* list,item_type d)
{
  if(list != NULL)
  {
    node *prev, *ptr;
    int found = 0;
    prev = list;
    ptr = list->next;

    while(ptr != NULL && !found)
    {
      if(ptr->data == d)
      {
        found = 1;
      }
      else
      {
        prev = ptr;
        ptr = ptr->next;
      }

    }
    if(!found)
      prev = NULL;

    return prev;
  }

  return NULL;
}

LFU_node* LFU_search(LFU_node* list,item_type d)
{
  if(list != NULL)
  {
    LFU_node *prev, *ptr;
    int found = 0;
    prev = list;
    ptr = list->next;

    while(ptr != NULL && !found)
    {
      if(ptr->data == d)
      {
        found = 1;
      }
      else
      {
        prev = ptr;
        ptr = ptr->next;
      }

    }
    if(!found)
      prev = NULL;

    return prev;
  }

  return NULL;
}

void LFU_update(LFU *ptr, item_type d)
{
  LFU_node *catch,*pre,*curr,*hold;
  int found = 0;//if found is 1 ctr is incremented and the node is suitably placed

  if(ptr->size > 1)
  {
    //if 1st element matches with data it is moved to the end.
    if((ptr->list)->data == d)
    {
      hold = ptr->list;
      ptr->list = hold->next;
      curr = ptr->list;
      found = 1;
    }
    else
    {
      catch = LFU_search(ptr->list,d);

      if(catch != NULL )
      {
        hold = catch->next;
        hold->ctr += 1;
        //if required node is not the last node
        if(hold->next !=NULL)
        {
          catch->next = hold->next;
          pre  = catch;
          //the search for appropriate place in the list begins from curr
          curr = catch->next;
          found = 1;
        }
      }
      else
      {
        //replace or add
        if(ptr->size == SIZE)
        {
          hold = ptr->list;
          ptr->list = hold->next;
          print(hold->data);
          free(hold);
          ptr->size -=1;
        }
        LFU_node* nptr = make_LFU_node(d);
        nptr->next = ptr->list;
        ptr->list = nptr;
        ptr->size += 1;
      }
    }
    if(found)
    {

      while (curr != NULL && curr->ctr < hold->ctr)
      {
        /* code */
        pre = curr;
        curr = curr->next;

      }

      hold->next = curr;
      pre->next = hold;

    }


  }
  else
  {
    if(ptr->size == 1)
    {
      if((ptr->list)->data == d )
      {
        (ptr->list)->ctr += 1;
        found =1;
      }
    }
    //when the cache is empty or a cache-miss occured with only 1 node in the list
    if(!found)
    {
      LFU_node *nptr = make_LFU_node(d);
      nptr->next = ptr->list;
      ptr->list = nptr;
      ptr->size +=1;
    }
  }

}

void MRU_update(MRU *ptr, item_type d)
{
  node *hold,*catch;

  if(ptr->size == 0)
  {
    ptr->list = make_node(d);
    ptr->size = 1;

  }
  else
  {
    if((ptr->list)->data != d)
    {
      //look for match in nodes other than first node
      catch = node_search(ptr->list,d);

      if(catch == NULL && ptr->size == SIZE)
      {
        hold = ptr->list;
        node* nptr = make_node(d);
        nptr->next = hold->next;

        ptr->list = nptr;
        print(hold->data);
        free(hold);
      }
    }
    else
    {
      if(catch == NULL)
      {
        //add new node
        node *nptr = make_node(d);
        nptr->next = ptr->list;
        ptr->list =  nptr;
        ptr->size += 1;
      }
      else
      {
        //bring the most recently accessed node to front
        hold = catch->next;
        catch->next = hold->next;
        hold->next = ptr->list;
        ptr->list = hold;
      }
    }
  }
}

void LRU_update(LRU *ptr, item_type d)
{
  node *catch,*hold;
  if(ptr->size == 0)
  {
    ptr->rear = ptr->front = make_node(d);
    ptr->size += 1;
  }
  else
  {
    if((ptr->front)->data == d)
    {
      if( ptr->size >1)
      {
        hold = ptr->front;
        ptr->front = hold->next;
        hold->next = NULL;
        ptr->rear->next = hold;
        ptr->rear = hold;
      }
    }
    else
    {
      //catch points to the node previous to the node where the data is matched
      node *catch = node_search(ptr->front,d);

      if(catch != NULL)
      {
        hold = catch->next;
        catch->next = hold->next;
        hold->next  = NULL;
        (ptr->rear)->next = hold;
        ptr->rear = hold;
      }
      else
      {
        //when no match
        (ptr->rear)->next = make_node(d);
        ptr->rear = (ptr->rear)->next;
        ptr->size += 1;

        if(ptr->size > SIZE)
        {
          hold = ptr->front;
          ptr->front = (ptr->front)->next;
          print(hold->data);
          free(hold);
          ptr->size -= 1;
        }
      }
    }

  }
}

int pop(query *ptr)
{
  node* hold;item_type d;

  if(ptr->front!=NULL)
  {
    hold = ptr->front;
    ptr->front = hold->next;
    if(ptr->front == NULL)
      ptr->rear = NULL;
    d = hold->data;
    free(hold);
  }
  return d;
}
void add(query *ptr,item_type d)
{
  if(ptr->front == NULL && ptr->rear == NULL)
  {  ptr->front =  make_node(d);
    ptr->rear = ptr->front ;
  }
  else
  {
    node* nptr = make_node(d);
    (ptr->rear)->next = nptr;
    ptr->rear = nptr;
  }
}
void LRU_cache_contents(LRU *ptr)
{
  node* temp =ptr->front;
  int i = 1;
  printf("\nUpdated Cache...........\n\n");
  if(temp != NULL)
  {
    while(temp != NULL)
    {
      printf("%d => %d\n",i,temp->data);
      temp = temp->next;
      ++i;
    }

  }

}
void LFU_cache_contents(LFU *ptr)
{
  LFU_node* temp =ptr->list;
  int i = 1;
  printf("\nUpdated Cache...........\n\n");
  if(temp != NULL)
  {
    while(temp != NULL)
    {
      printf("%d => %d\n",i,temp->data);
      temp = temp->next;
      ++i;
    }

  }
}

void MRU_cache_contents(MRU *ptr)
{
  node* temp =ptr->list;
  int i = 1;
  printf("\nUpdated Cache...........\n\n");
  if(temp != NULL)
  {
    while(temp != NULL)
    {
      printf("%d => %d\n",i,temp->data);
      temp = temp->next;
      ++i;
    }

  }
}
void init_query(query* ptr)
{
  ptr->size = 0;
  ptr->front =NULL;
  ptr->rear = NULL;
}
void query_contents(query *ptr)
{
  node* temp =ptr->front;
  int i = 1;
  printf("\nQuery List...........\n\n");
  if(temp != NULL)
  {
    while(temp != NULL)
    {
      printf("%d => %d\n",i,temp->data);
      temp = temp->next;
      ++i;
    }

  }

}
