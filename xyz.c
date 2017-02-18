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
  item_type address;
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
void LFU_update(node* cache,LFU *ptr, item_type d);
void MRU_update(node* cache,MRU *ptr, item_type d);
void LRU_update(node* cache,LRU *ptr, item_type d);
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
  LRU tag1;LFU tag2;MRU tag3;
  node* cache;

  init_LRU(&tag1);
  init_LFU(&tag2);
  init_MRU(&tag3);


  printf("**********************************************************************\n");
  printf("......................CACHE MANAGEMENT SYSTEM.........................\n");
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
            init_LRU(&tag1);
            for(i=0;i<n;++i)
            {
              printf("\nQuery %d ......",i+1);
              scanf("%d",&q);
              //add(&query_list,q);
            //  query_contents(&query_list);
              LRU_update(&tag1,q);
              LRU_cache_contents(&tag1);
            }
            break;

  case 2:

          init_LFU(&tag2);
          for(i=0;i<n;++i)
          {
            printf("\nQuery %d ......",i+1);
            scanf("%d",&q);
            //add(&query_list,q);
            LFU_update(&tag2,q);
            LFU_cache_contents(&tag2);
          }
          break;

  case 3:

          init_MRU(&tag3);
          for(i=0;i<n;++i)
          {
            printf("\nQuery %d ......",i+1);
            scanf("%d",&q);
            //add(&query_list,q);
            MRU_update(&tag3,q);
            MRU_cache_contents(&tag3);
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

void LFU_update(node* cache,LFU *ptr, item_type d)
{
  LFU_node *catch,*pre,*curr,*hold,*nptr;
  int found = 0;//flag to mark if reshuffle is required

  if(ptr->size == 0)
  {
    ptr->list = make_LFU_node(d);
    ptr->size += 1;
  }
  else
  {  //when cache is not empty
    if(ptr->list->data == d)
    {
      hold = ptr->list;
      curr = hold->next;
      hold->ctr += 1;

      //when there are more nodes, reshuffle of positions may be required
      if(curr != NULL)
      {  if(curr->ctr < hold->ctr)
        {
          ptr->list = curr;
          hold->next = NULL;
          found = 1;
        }
      }
    }
    else
    {
      // search for match in nodes other than 1st node
      catch = LFU_search(ptr->list,d);

      if(catch != NULL)
      {
        hold = catch->next;
        hold->ctr += 1;

        //if the match is not the last node
        if(hold->next != NULL)
        {
          curr = hold->next;
          //condition for reshuffle =>found is set to 1
          if(curr->ctr < hold->ctr)
          {
            catch->next = curr;
            hold->next = NULL;
            found = 1;
          }
        }

      }
      else
      {
        //when cache-miss occured and cache is full => replace the node at the beginning

        if(ptr->size == SIZE)
        {
          hold = ptr->list;
          nptr = make_LFU_node(d);
          nptr->next = hold->next;
          ptr->list = nptr;
          print(hold->data);
          free(hold);
        }
        else
        {
          //add the data in the cache when it is empty or partially filled
          nptr = make_LFU_node(d);
          nptr->next = ptr->list;
          ptr->list = nptr;
          ptr->size += 1;
        }

      }
    }
  }
  //insert data at the proper place in the list
  if(found)
  {
    //loop will be executed at least once
    while(curr != NULL && curr->ctr < hold->ctr)
    {
      pre = curr;
      curr = curr->next;
    }
    hold->next = curr;
    pre->next = hold;
  }

}

void MRU_update(node* cache,MRU *ptr, item_type d)
{
  node *hold,*catch;

  if(ptr->size == 0)
  {
    ptr->list = make_node(d);
    ptr->size = 1;

  }
  else
  {
    if((ptr->list)->data == d)
    {
      ;//do nothing as 1st node is most recently accessed
    }
    else
    {
      if(ptr->size >= 1)
      {
        //look for match in nodes other than first node
        catch = node_search(ptr->list,d);

        if(catch == NULL )
        {
          node* nptr = make_node(d);
          if(ptr->size == SIZE)
          {
            hold = ptr->list;
            nptr->next = hold->next;
            ptr->list = nptr;
            print(hold->data);
            free(hold);
          }
          else
          {
            //add a new node
            nptr->next = ptr->list;
            ptr->list = nptr;
            ptr->size += 1;
          }
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
}

void LRU_update(node* cache,LRU *ptr, item_type d)
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
        if(hold->next != NULL)
        {
          catch->next = hold->next;
          hold->next  = NULL;
          (ptr->rear)->next = hold;
          ptr->rear = hold;
        }
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
