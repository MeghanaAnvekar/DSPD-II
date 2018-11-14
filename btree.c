#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 4
#define MIN 2
#define ORDER 5
#define SIZE 10
#define INT_MAX 1000

typedef struct
{
	char word[SIZE];
	int count;
}item_type;

typedef struct Btree_tag
{
	item_type key[MAX+1];
	struct Btree_tag* children[MAX+2];
	int isLeaf;
	int count;
	struct Btree_tag* parent;
}Btree;

int main()
{
	Btree* make_Btree_node();
	int search(Btree* curr, char* s);
	Btree* split(Btree* curr);
	char* calc_median(Btree* curr,char median[],int *median_count);
	Btree* Insert(Btree* curr, char s[]);
	Btree* Delete(Btree* root, char s[]);
	Btree* dummy();
	void Traverse(Btree* root);
	void RangeSearch(Btree* root, char min[],char max[]);
	void Find_k_time_words(Btree* root,int k);
	void Print_first_last_word(Btree* root);
	void Least_time_words(Btree* root);
	void level_visit(Btree* root);
	Btree* Insert_from_file(FILE* file);
	
	
	Btree* root = NULL;char s[20],min[SIZE],max[SIZE];int i = 0;FILE* file;
	
	int choice =-1;
	int set =0;
	printf("DICTIONARY.......\n\n");
	for(; i < 100 && choice != 9 ;++i)
	while(1 && choice != 9)
	{
		
		printf("\n\n0.Insert from file\n1.Insert \n2.Delete \n3.Traverse Dictionary \n4.Range Search \n5.Print words which appear more than k times \n6.Print First and Last word \n7.Print Words that appear least no.of times \n8.Print  Level wise\n9.Exit");
		printf("\n\nYour choice ? ...");
		scanf("%d",&choice);
		
		switch(choice)
		{
		    case 0: 
		    		file = fopen("word.txt","r");
		    		root = Insert_from_file(file);
		    		fclose(file);
		    		break;
			case 1: printf("\nEnter word ...");
					scanf("%s",s);
					
					root = Insert(root,s);
					//root = dummy();
					
					break;
					
			case 2: printf("\nEnter word ...");
					scanf("%s",s);
					root = Delete(root,s);
					break;
					
			case 3: if(root)
					{
						printf("\n\nPrinting the dictionary...\n\n");
						Traverse(root);
					}
					else
					{
						printf("\nEmpty dictionary!");
					}
					break;
			case 4: printf("\nenter min ... ");
					scanf("%s",min);
					printf("\nenter max....");
					scanf("%s",max);
					RangeSearch(root,min,max);
					break;
			case 5: printf("\nEnter the value of k....");
					int k;
					scanf("%d",&k);
					if(k >= 0)
						Find_k_time_words(root, k);
					break;
			case 6: Print_first_last_word(root);
					break;
			case 7: Least_time_words(root);
					break;
						
			case 8: level_visit(root);
					break;
			case 9: break;
			default: printf("\nError ! Invalid Choice");
		}
	

	}
}

Btree* make_Btree_node()
{
	Btree* nptr = (Btree*)malloc(sizeof(Btree));
	if(nptr)
	{
		int i=0;
		for(;i<= ORDER;++i)
		{
			nptr->children[i] = NULL;
		}
		nptr->isLeaf = 0;
		nptr->count = 0;
		nptr->parent = NULL;		
	}

return nptr;
}

/*int search(Btree* curr, char* s)
{
	int mid, lo = 0,hi = curr->count-1;
	int flag = 1,compare;
	while(lo <= hi && flag )
	{
		mid = (lo + hi)/2;
		compare = strcmp(curr->key[mid].word,s);
		if(compare < 0)
		{
			hi = mid-1;
		}
		else if(compare < 0)
		{
			lo = mid + 1;
		}
		else
		{
			flag = 0;
		}
		
	}
	return mid;
}*/

//returns -1 if word not found else returns the position in array where the word is less than or equal to given word  

int search(Btree* curr, char* s)
{
	int pos =-1;
	if(curr)
	{
		int count = curr->count,i,flag = 1;
		for(i = 0;i < count && flag;++i)
		{
			if(strcmp(curr->key[i].word, s) <= 0)
				pos = i;
			else
				flag = 0;
		}
	}
	
	return pos;
	
}

//returns the position where match occurs or the index which should be chased
int search_node(Btree* root, char s[])
{
	int pos = -1, compare;
	
	if(root)
	{
		int count = root->count,i,flag = 1;
		if(strcmp(s,root->key[0].word) < 0 )
		{
			return 0;
		}
		else
		{
			for(i = 0;i < count;++i)
			{
				compare = strcmp(root->key[i].word, s);
				if(compare < 0)
					pos = i+1;
				else if (compare == 0)
					pos = i;
			}
		}
	}
	return pos;
}
Btree* Insert(Btree* root,char s[])
{
	Btree* split(Btree* root);
	Btree* sort(Btree* root);
	Btree* final_root = root;
	void Traverse(Btree* root);
	
	//initial condtion
	if(root == NULL)
	{
		root = make_Btree_node();
		root->count =1;
		root->isLeaf = 1;
		root->parent = NULL;
		strcpy(root->key[0].word,s);
		root->key[0].count = 1;
		final_root = root;
	}
	else
	{
		int found = 0;int level = 1;
		
		//search internal nodes in B-Tree
		while(!root->isLeaf && !found)
		{
			int index = search(root,s);
			
			int compare = strcmp(root->key[index].word,s);
			
			if(compare == 0 && index >= 0 )
			{
				root->key[index].count += 1;
				found = 1;
			}
			else if(compare < 0)
			{
			
				root = root->children[index+1];
			}
			
			
		}
		
		
		if(root->isLeaf)
		{
			//search in leaf
			 int index = search(root,s);
			 int compare = strcmp(root->key[index].word,s);
			
			//if match found
			if(compare == 0 && index >= 0 )
			{
				root->key[index].count += 1;
				found = 1;
			}
			else //insertion at leaf level
			{
				strcpy(root->key[root->count].word,s);
				root->key[root->count].count = 1;
				root->count += 1;
				//printf("\nroot->count = %d",root->count);			
				root = sort(root);
				
				//splitting the node if limit exceeded
				if(root->count == MAX + 1)
				{
					final_root = split(root);
				
				} 
			}
			
			
		}
	}
	
	return final_root;
}	

//sort the keys and respective children accordingly
Btree* sort(Btree* root)
{
	if(root)
	{
		int i,j,count = root->count,flag = 0;item_type temp; Btree* ptr;
		
		for(i = 0;i < count -1 && !flag;++i)
		{
			flag = 1;
			
			for(j =0;j < count - 1;++j)
				if(strcmp(root->key[j].word,root->key[j+1].word) > 0)
				{
					flag = 0;
					temp = root->key[j];
					root->key[j] = root->key[j+1];
					root->key[j+1] = temp;
					
					ptr = root->children[j+1];
					root->children[j+1] = root->children[j+2];
					root->children[j+2] = ptr;
										
				}
		}
		return root;
	}
	return NULL;
}	

//calculate median of a node		
char* calc_median(Btree* curr,char* s,int *median_count)
{
	item_type * ptr = curr->key;int median_pos = ORDER/2;
	
	if(strcmp(s,ptr[median_pos].word) < 0)
	{
		if(strcmp(s,ptr[1].word) < 0)
		{
			median_pos = 1;
		}
		else
		{
			median_pos = -1;
		}
	}
	
	if(median_pos == -1)
	{
		*median_count = 0;
		return s;
	}
	else
	{
		*median_count = ptr[median_pos].count;
		return ptr[median_pos].word;
	}
	

}			

//recursive in-order traversal
void Traverse(Btree* root)
{
	if(root)
	{	
		if(root->isLeaf)
		{
			int j,count = root->count;
			for(j = 0;j < count;++j)
			{
				printf("\n%-10s   %-2d",root->key[j].word,root->key[j].count);
			}
			
		}
		else
		{
			int count = root->count;
			int i;
		
			for(i = 0; i < count;++i)
			{
			
				Traverse(root->children[i]);
				if(root->key[i].word);
				printf("\n%-10s   %-2d",root->key[i].word,root->key[i].count);
			}
			Traverse(root->children[i]);
			
		}
	}
	

}			

Btree* split(Btree* root)
{
	Btree* sort(Btree* root);
	Btree* make_Btree_node();
	Btree* temp = make_Btree_node();
	
	temp->count = root->count = MIN;
	temp->isLeaf = root->isLeaf;
	temp->parent = root->parent;
	
	
	//splitting of root
	if(root->parent == NULL)
	{
		Btree* up = make_Btree_node();
		up->count = 1;
		up->parent = NULL;
		//new root key
		up->key[0] = root->key[MIN];
		root->parent = temp->parent = up;
		
		//transferring of keys and children
		temp->key[0] = root->key[3];
		temp->key[1] = root->key[4];
		
		temp->children[0] = root->children[3];
		temp->children[1] = root->children[4];
		temp->children[2] = root->children[5];
		
		//if children not null set new parent
		if(root->children[0])
		{
			root->children[3]->parent =temp;
			root->children[4]->parent =temp;
			root->children[5]->parent =temp;	
		}
		//deleting old children
		root->children[3] =NULL;
		root->children[4] =NULL;
		root->children[5] =NULL;
		
		//link new children and root
		up->children[0] = root;
		up->children[1] = temp;
	
		return up;
	}
	else
	{
		//splitting of internal node
		Btree* parent = root->parent;
		if(parent->count <= MAX)
		{
			int i,flag = 0;
			//insertion of median
			parent->key[parent->count] = root->key[MIN];
			parent->count +=1;
			parent = sort(parent);
			
			//transferring keys and children to right sibling			
			temp->key[0] = root->key[3];
		    temp->key[1] = root->key[4];
		    
		    temp->children[0] = root->children[3];
		    temp->children[1] = root->children[4];
		    temp->children[2] = root->children[5];
		    
		    //if children exist set their new parent
			if(root->children[0])
			{
				root->children[3]->parent =temp;
				root->children[4]->parent =temp;
				root->children[5]->parent =temp;	
			}
		    
		    //delete links with old children
		    root->children[3] =NULL;
			root->children[4] =NULL;
			root->children[5] =NULL;
		    
		    for(i = 0;i <= MAX+1 && !flag;++i)
		    if(parent->children[i] == NULL)
		    	{
		    		parent->children[i] = temp;
					flag = 1;
				}
				
			root = parent;		    
		}
		//if capacity of parent exceeds , parent is split again
		if(parent->count == MAX + 1)
		{
			root  = split(parent);	
		}
		else
		{	//traverse upwards to return root
			while(root->parent)
			{
				root = root->parent;
			}
		}
		
	}
	return root;
}		


void RangeSearch(Btree* root, char min[],char max[])
{
	void Selective_Traverse(Btree* root,char min[],char max[],int* found_min,int* found_max);
	
	char* Fetch_first_word(Btree* root);
	char* Fetch_last_word(Btree* root);
	
	if(root)
	{
		char * first = Fetch_first_word(root);
		char * last = Fetch_last_word(root);
		
		//check range
		if(strcmp(first, max) >0 || strcmp(last, min) < 0)
		{
			printf("\nNo words in the given range.");
		}
		else
		{
			int found_min = 0,found_max = 0;
			
			Selective_Traverse(root,min,max,&found_min,&found_max);
		}
	}
	else
	{
		printf("\nDictionary is empty .");
	}
}

//prints words only when found_min = 1 and found_max = 0 , both limits inclusive
void Selective_Traverse(Btree* root,char min[],char max[],int* found_min,int* found_max)
{
	if(root)
	{
		if(root->isLeaf)
		{
		
				if(*found_max != 1)
				{
					int i,flag = 0;
					if(!(*found_min))
					{	
						for(i = 0;i < root->count && !flag;++i)
							if(strcmp(root->key[i].word,min) >= 0 && strcmp(root->key[i].word,max)<= 0)
							{
								*found_min = 1;
								flag = 1;
							}
						
						if(flag)
						{
							i = i-1;
						
							for(;i < root->count&& !(*found_max);++i)
							{
								if(strcmp(root->key[i].word,max) < 0)
								 printf("\n%s",root->key[i].word);
								else
								{
									if(strcmp(root->key[i].word,max) == 0)
									 printf("\n%s",root->key[i].word);
									 
									 *found_max =1;
								}
							}
						
						}
					}
					else
					{
						for(i = 0;i < root->count && !(*found_max);++i)
							if(strcmp(root->key[i].word,max) < 0 && (*found_min))
								printf("\n%s",root->key[i].word);
							else
							{
								if(strcmp(root->key[i].word,max) == 0)
									printf("\n%s",root->key[i].word);
								*found_max = 1;
							}
					}
				}
			
			
		}
		else
		{
			int i;
			for(i = 0; i < root->count && !(*found_max);++i)
			{	
					if(strcmp(root->key[i].word, min) > 0)
						Selective_Traverse(root->children[i],min,max,found_min,found_max);
					
					//if key is checked for match
					if(!(*found_min))
					{
						if(strcmp(root->key[i].word,min) >= 0 && strcmp(root->key[i].word, max) < 0)
						{
							*found_min = 1;
							printf("\n%s",root->key[i].word);
						}
						else if(strcmp(root->key[i].word, max) > 0)
						 { *found_max =1;
						 return;}
					
					}
					else //if found_min already set , continues to set found_max
					{
						if(strcmp(root->key[i].word, max) >= 0)
						{
							if(strcmp(root->key[i].word, max) == 0)
								printf("\n%s",root->key[i].word);
							*found_max = 1;
							return;
						}
						else
						{
							printf("\n%s",root->key[i].word);
						}
					}
				}
				if(*found_max != 1)
				{
					Selective_Traverse(root->children[i],min,max,found_min,found_max);
				}
					
			
		}
	}
	
}		
//my trial B-tree
Btree* dummy()
{
	Btree* root = make_Btree_node();
	
	strcpy(root->key[0].word,"ear");
	root->parent  = NULL;
	root->count = 1;
	root->isLeaf = 0;
	root->key[0].count = 5;
	Btree * temp = make_Btree_node();
	
	strcpy(temp->key[0].word,"bat");
	temp->key[0].count = 2;
	strcpy(temp->key[1].word,"cop"); 
	temp->key[1].count = 7;
	temp->isLeaf = 0;
	temp->count = 2;
	temp->parent = root;
	
	root->children[0] = temp;
	
	temp = make_Btree_node();
	
	strcpy(temp->key[0].word,"act");
	temp->key[0].count = 2;
	strcpy(temp->key[1].word,"apple"); 
	temp->key[1].count = 3;
	temp->isLeaf = 1;
	temp->count = 2;
	temp->parent = root->children[0];
	
	root->children[0]->children[0] = temp;
	
	temp = make_Btree_node();
	
	strcpy(temp->key[0].word,"bin");
	temp->key[0].count = 3;
	strcpy(temp->key[1].word,"cat"); 
	temp->key[1].count = 2;
	temp->isLeaf = 1;
	temp->count = 2;
	temp->parent = root->children[0];
	root->children[0]->children[1] = temp;
	
	
	temp = make_Btree_node();
	
	strcpy(temp->key[0].word,"dog");
	temp->key[0].count = 2;
	strcpy(temp->key[1].word,"dyne"); 
	temp->key[1].count = 2;
	strcpy(temp->key[2].word,"eagle"); 
	temp->key[2].count = 3;
	temp->isLeaf = 1;
	temp->count = 3;
	temp->parent = root->children[0]; 
	root->children[0]->children[2] = temp;
	
	temp = make_Btree_node();
	strcpy(temp->key[0].word,"gate");
	temp->key[0].count = 2;
	strcpy(temp->key[1].word,"kite");
	temp->key[1].count = 3;
	temp->count = 2;
	temp->isLeaf = 0;
	root->children[1] = temp;
	temp->parent = root;
	
	temp = make_Btree_node();
	
	strcpy(temp->key[0].word,"fish");
	temp->key[0].count = 2;
	strcpy(temp->key[1].word,"fund");
	temp->key[1].count = 3;
	temp->count = 2;
	temp->isLeaf = 1;
	root->children[1]->children[0] = temp;
	temp->parent = root->children[1];
	temp = make_Btree_node();
	
	strcpy(temp->key[0].word,"gun");
	temp->key[0].count = 1;
	strcpy(temp->key[1].word,"house");
	temp->key[1].count = 1;
	temp->count = 2;
	temp->isLeaf = 1;
	root->children[1]->children[1] = temp;
	temp->parent = root->children[1];	
	temp = make_Btree_node();

	strcpy(temp->key[0].word,"lamp");
	temp->key[0].count = 1;
	strcpy(temp->key[1].word,"owl");
	temp->key[1].count = 5;
	strcpy(temp->key[2].word,"park");
	temp->key[2].count = 1;
	strcpy(temp->key[3].word,"sad");
	temp->key[3].count = 4;
	temp->count = 4;
	temp->parent = root->children[1];
	temp->isLeaf = 1;
	root->children[1]->children[2] = temp;
	
	
	return root;
	
}

//fetch first word
char* Fetch_first_word(Btree* root)
{
	while(root)
	{
		if(root->isLeaf)
		{
			return root->key[0].word;
		}
		else
		{
			root = root->children[0];
		}
	}
	
	return NULL; 
}

//fetch first word
char* Fetch_last_word(Btree* root)
{
	while(root)
	{
		if(root->isLeaf)
		{
			return root->key[root->count -1].word;
		}
		else
		{
			root = root->children[root->count];
		}
	}
	return NULL;
}

void Print_first_last_word(Btree* root)
{
	char* Fetch_first_word(Btree* root);
	char* Fetch_last_word(Btree* root);
	
	printf("\nFirst Word....%s",Fetch_first_word(root));
	printf("\nLast Word.....%s",Fetch_last_word(root));
	
}

//recursively calculate the least count of appearance
int Calc_least(Btree* root)
{
	int least = INT_MAX;
	int temp = INT_MAX;
	
	if(root)
	{
		if(root->isLeaf)
		{
			int i;
			
			for(i = 0;i < root->count;++i)
			{
				if(root->key[i].count < least && root->key[i].count>0 )
					least = root->key[i].count;
			}
			//printf("\nleast = %d",least);
		}
		else
		{
			int i;
			
			for(i = 0; i < root->count;++i)
			{
			 temp = Calc_least(root->children[i]);
				
				if(root->key[i].count < least && root->key[i].count>0)
					least = root->key[i].count;
			}
			 temp = Calc_least(root->children[i]);
			// printf("\nleast = %d",least);
		}
		
		if(temp < least)
		least = temp;
	}
	else
	{
		return 0;
	}
	return least;
}

void Least_time_words(Btree* root)
{
	void Print_least_time_words(Btree* root,int least);
	int least = Calc_least(root);
	printf("\nLEAST = %d",least);
	Print_least_time_words(root,least);
	
}

void Print_least_time_words(Btree* root,int k)
{
	if(root)
	{
		if(root->isLeaf)
		{	
			int i;
			for(i = 0;i < root->count;++i)
			{
				if(root->key[i].count == k)
				{
					printf("\n%s",root->key[i].word);
				}
			}
		}
		else
		{
			int i;
			
			for(i = 0;i < root->count;++i)
			{
				Print_least_time_words(root->children[i],k);
				
				if(root->key[i].count == k)
				{
					printf("\n%s",root->key[i].word);
				}
			}
			
			Print_least_time_words(root->children[i],k);
		}
	}
	else
	{
		printf("\nEmpty dictionary");
	}
}

//print words having count greater than k
void Find_k_time_words(Btree* root,int k)
{
	if(root)
	{
		if(root->isLeaf)
		{	
			int i;
			for(i = 0;i < root->count;++i)
			{
				if(root->key[i].count > k)
				{
					printf("\n%s",root->key[i].word);
				}
			}
		}
		else
		{
			int i;
			
			for(i = 0;i < root->count;++i)
			{
				Find_k_time_words(root->children[i],k);
				
				if(root->key[i].count > k)
				{
					printf("\n%s",root->key[i].word);
				}
			}
			
			Find_k_time_words(root->children[i],k);
		}
	}
	else
	{
		printf("\nEmpty dictionary");
	}
}

int height(Btree * root)
{
	int ht = 0,flag = 1;
	
	while(root && flag)	
	{
		ht++;
		if(root->isLeaf)
			flag =0;
			
		root = root->children[0];
	}
	return ht;
}

void level_visit(Btree* root)
{
	void visit(Btree* root, int level);
	int height(Btree * root);
	int Height = height(root), i;
	
	printf("\n HEight = %d\n\n",Height);
	
	for(i = Height -1;i >= 0 ;--i)
	{	printf("\n");
		visit(root,i);
	}
}


void visit(Btree* root, int level)
{
	if(root != NULL)
	{
		if(level == 0)
		{
			int i;
			
			for(i = 0; i < root->count; ++i)
				printf("  %s",root->key[i].word);
				printf("|");
		}
		else
		{
			int i;
			
			for(i = 0;i <= root->count;++i)
			{

				visit(root->children[i],level-1);
			}
		}
	}
}

Btree* Delete(Btree* root, char s[])
{
	int remove_word(Btree* root,char s[]);
	Btree* temp;
	if(root)
	{if(!remove_word(root,s))
	{
		printf("\nWord not found !");
	}
	else if(root->count == 0)
	{
		temp = root;
		root = root->children[0];
		free(temp);
	}}
	else
	{
		printf("\nEmpty dictionary!");
	}
	
	return root;
}

int remove_word(Btree* root, char s[])
{
	int search_node(Btree* curr, char s[]);
	void remove_from_leaf(Btree* root, int index);
	void successor(Btree* root, int index);
	void restore_child(Btree* root, int index);
	
	
	int retval = 0;
	//root should exist to remove a key
	if(root)
	{
		int index = search_node(root,s);
		
		int compare = strcmp(root->key[index].word,s);
		
		
		// if match was found
		if(compare == 0)
		{
			//if internal node
			if(!root->isLeaf)
			{
				//replaces key at index with key in leaf in right child 
				successor(root,index+1);
				
				//delete key from root
				if(!remove_word(root->children[index+1],root->key[index].word))
					{printf("\nCould not delete key from leaf");
					retval = 0;}
				
			}
			else
			{
				remove_from_leaf(root,index);
			}
			retval = 1; //successfully removed key from leaf
		}
		else
		{
			//move to next child
			retval = remove_word(root->children[index],s);
		}
		
		//after returning from recursive call , check if subtree at right is deficient at any level
		
		if(root->children[index+1] != NULL)
			{
				if(root->children[index+1]->count < MIN)
			 		 restore_child(root,index+1);
			 		 }
		 if(root->children[index] != NULL)
			if(root->children[index]->count < MIN)
			 		 restore_child(root,index);
	}
	
	return retval;
}


void remove_from_leaf(Btree* root, int index)
{
	int i;
	
	for(i = index;i < root->count-1;++i)
	{
		root->key[i] = root->key[i+1];
	}
	
	(root->count)--;
}

void successor(Btree* root, int index)
{
	Btree* temp = root->children[index];
	
	while(!temp->isLeaf)
	{
		temp = temp->children[0]; 
	}
	
	root->key[index-1] = temp->key[0];
}


void restore_child(Btree* root, int index)
{
	void borrow_from_left_sibling(Btree* root, int index);
	void borrow_from_right_sibling(Btree* root, int index);
	void combine(Btree* root, int index);
	
	//if deficiency in left most child
	if(index == 0)
	{	
		if(root->children[1]->count > MIN)
		{
			borrow_from_right_sibling(root, 0);
		}
		else
		{
			combine(root,0);
		}
	}//deficiency in right most child
	else if(index == root->count)
	{
		if(root->children[index - 1]-> count > MIN)
		{
			borrow_from_left_sibling(root, index);
		}
		else
		{
			combine(root,index);
		}
	}//deficiency in other nodes
	else if(root->children[index-1]->count > MIN)
	{
		borrow_from_left_sibling(root,index);
	}
	else if(root->children[index+1]->count > MIN)
	{
		borrow_from_right_sibling(root,index);
	}
	else
	{
		combine(root, index);
	}
}


void borrow_from_left_sibling(Btree* root, int index)
{
	//min value that index can take is 1
	int i;
	Btree* temp;
	
	temp = root->children[index];
	
	for(i = temp->count; i>0;--i)
	{
		temp->key[i] = temp->key[i-1];
		temp->children[i+1] = temp->children[i];
	}
	
	temp->children[i+1] = temp->children[i];
	(temp->count)++;
	
	//shifting value from root
	temp->key[0] = root->key[index-1];
	
	//temp now points to sibling
	temp = root->children[index-1];
	root->key[index-1] = temp->key[temp->count - 1];
	
	//last child shifted to deficient node
	if(temp->children[temp->count])
	 temp->children[temp->count]->parent = root->children[index];
	root->children[index]->children[0] = temp->children[temp->count];
	temp->children[temp->count] = NULL;
	
	(temp->count)--;
	
	
}

void borrow_from_right_sibling(Btree* root, int index)
{
	//min valu that index can take is 0
	int i;
	Btree* temp;
	
	temp = root->children[index];
	
	//shifting value from root
	temp->key[temp->count] = root->key[index];
	(temp->count)++;
	
	//temp now points to sibling
	temp = root->children[index+1];
	root->key[index] = temp->key[0];
	
	//first child shifted to deficient node
	if(temp->children[0])
	 temp->children[0]->parent = root->children[index];
	root->children[index]->children[root->children[index]->count] = temp->children[0];
	temp->children[0] = NULL;
	
	//shifting keys and children to the left;
	for(i = 1; i < temp->count;++i)
	{
		temp->key[i-1] = temp->key[i];
		temp->children[i-1] = temp->children[i];
	}
	temp->children[i-1] = temp->children[i];
	(temp->count)--;
	
	
}

void combine(Btree* root, int index)
{
	int i;
	
	//points to right child
	Btree* right;
	
	//points to left child
	Btree* left;
	
	if(index == 0)
	{
		left = root->children[0];
		right = root->children[1];
		
		left->key[1] = root->key[0];
		left->key[2] = right->key[0];
		left->key[3] = right->key[1];
		left->children[2] = right->children[0];
		left->children[3] = right->children[1];
		left->children[4] = right->children[2];
		
		left->count = MAX;
		root->children[1] = NULL;
	}
	else if(index == root->count)
	{
		left = root->children[index-1];
		right = root->children[index];
		
		left->key[2] = root->key[index-1];
		left->key[3] = right->key[0];
		left->children[3] = right->children[0];
		left->children[4] = right->children[1];
		left->count = MAX;
		root->children[index] = NULL;
	}
	else
	{
		left = root->children[index];
		right = root->children[index+1];
		
		left->key[1] = root->key[index];
		left->key[2] = right->key[0];
		left->key[3] = right->key[1];
		left->children[3] = right->children[0];
		left->children[4] = right->children[1];
		left->children[5] = right->children[2];
		root->children[index+1] = NULL;
		left->count =MAX;
				
	}
	
	//left shifting the  elements in the root index onwards
	for(i = index;i < root->count-1;++i)
	{
		root->key[i] = root->key[i+1];
		root->children[i+1] = root->children[i+2];
		
	}
	(root->count)--;
	free(right);
	
}

Btree* Insert_from_file(FILE* file)
{
	Btree* root = NULL;
	char str[SIZE];
	if(file)
	{
		while(!feof(file))
		{
			fscanf(file,"%s",str);
			root = Insert(root,str);
		}
	}
	
	return root;
}
