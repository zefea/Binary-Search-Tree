/*
 * Zeynep Ferah Akkurt
 * 150119824
 * Data Structures Project#1
 * 
 * Creates a BST using words from given input file 
 * and also creates a binary tree which has a better access time than BST.
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

/*Key*/
typedef struct{
	int no;
	char name[25];
	int frequency;  
	
}Key;

struct node { 
    Key key; 
    struct node *left, *right; 
}; 


/*converts a string's letters to lowercase*/
char* lower_string(char s[]) {
   int c = 0;
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
   return s;
}
/*create a new node*/
struct node* newNode(Key key) 
{ 
    struct node* temp = (struct node*)malloc(sizeof(struct node)); 
    if(temp == NULL){
		printf("No memory allocation.");
		exit(-1);
	}
    temp->key = key; 
    temp->left = NULL;
    temp->right = NULL; 
    return temp; 
}  

/*finds hights of the tree*/ 
int maxDepth(struct node * root)
{
  if (root == NULL)	// Root doesn't exist.
    return 0;
  
  int leftDepth = maxDepth(root->left);
  int rightDepth = maxDepth(root->right);

  /*bigger one + root*/
  if (leftDepth > rightDepth)
    return leftDepth + 1;
  else
    return rightDepth + 1;
}


/*Build a BST with the frequency*/
struct node* insertWithFrequencyToBST(struct node *root, Key key){
	
	if (root == NULL) 
        return newNode(key); 
        
    if (key.frequency < root->key.frequency) 
        root->left = insertWithFrequencyToBST(root->left, key); 
    else if (key.frequency >= root->key.frequency) 
        root->right = insertWithFrequencyToBST(root->right, key);  
        
    return root; 
}

/*Build a BST with the word*/
struct node* insertwithWordToBST(struct node *root, Key key){
	
	if (root == NULL) 
        return newNode(key); 
    
    char tempKeyName[25];
    char tempRootName[25];
    
    strcpy(tempKeyName,key.name);
    strcpy(tempRootName,root->key.name);
    
    lower_string(tempKeyName);
    lower_string(tempKeyName);
    
    if (strcmp(tempKeyName,tempRootName)<0) 
        root->left = insertwithWordToBST(root->left, key); 
    else if (strcmp(tempKeyName,tempRootName)>=0) 
        root->right = insertwithWordToBST(root->right, key);  
        
    return root;	
} 


/* returns level of the data in the tree if data is exist, otherwise returns 0.*/
int levelCheck(struct node *root, Key key, int level) 
{ 
    if (root == NULL) /*tree is empty*/
        return 0; 
  
    if (strcmp(root->key.name,key.name) == 0) /*node's key is found*/
        return level; 
  
    int nodeLevel = levelCheck(root->left, key, level+1); 
    if (nodeLevel != 0) 
        return nodeLevel; 
  
    nodeLevel = levelCheck(root->right, key, level+1); 
    return nodeLevel; 
} 
  
/* Returns access time of given node*/
int accessOfNode(struct node *root, Key key) 
{ 
	int access = 0;
	int level = levelCheck(root,key,1); 	/*find the level*/
    access = (key.frequency)* level;	
    
	//printf("%.20s , level: %d, freq: %d ,access:%.f\n", key.name,level,key.frequency,access); 
    return access; 
} 

/*returns total access time of tree*/
void calculateAccessTime(struct node* root,struct node *root2,int* totalAccessTime) 
{ 
    if (root != NULL) {    
        calculateAccessTime(root->right,root2,totalAccessTime);       
        *totalAccessTime += accessOfNode(root2,root->key); 
        calculateAccessTime(root->left,root2,totalAccessTime); 
    } 
} 

/*final function for access time calculation */
int totalAccessTime(struct node* root,struct node *root2){
	
	int totalAccessTime = 0;
	calculateAccessTime(root,root2,&totalAccessTime);
	return totalAccessTime;	
}


/*it stores the keys of BST in array as sorted from biggest to smallest*/
int i;
void treeMinimized(struct node* root,int size,Key array[]) 
{ 
    if (root != NULL) { 
		treeMinimized(root->right,size,array); 
        array[i] = root->key;
        i++;
        treeMinimized(root->left,size,array); 
    } 
} 

/*insert a node to the binary tree*/
void insertOptimalKey(struct node **root, Key *keys, int index, int n)
{
    if (index < n) {
		*root = newNode(keys[index]);
		
        insertOptimalKey(&(*root)->left, keys, 2*index + 1, n);
        insertOptimalKey(&(*root)->right, keys, 2*index + 2, n);
    }
}
/* Constructs a BT to keep these records in the main memory so as to minimize the total access time*/
void constructBTbyFrequency(struct node** root,struct node** root2, int size){
	
	i=0;
	Key array[size];
	treeMinimized(*root,size,array); 				/*bst to array*/
	insertOptimalKey(root2, array, 0, size);		/*array to bt*/
} 
 
/*in-order traversal for frequencies*/
void inorderFrequency(struct node* root) 
{ 
    if (root != NULL) { 
		inorderFrequency(root->left);
        printf("%d, ", root->key.frequency); 
        inorderFrequency(root->right); 
    } 
} 

/*in-order travelsal for words*/
void inorderWords(struct node* root) 
{ 
    if (root != NULL) { 
        inorderWords(root->left);
        printf("%.25s\n",root->key.name); 
        inorderWords(root->right); 
    } 
}

/*reads the keys from a given input file and add those keys to the binary search tree*/
int readAndBuild(struct node** root, struct node** root2, char* fileName,int *size){
	
	FILE *f; 
	f = fopen(fileName,"r");
	if(f == NULL){
		printf("file could not open!");
		return 0;
	} 
	
	char s[100];
	Key key;
	//read file create binary search tree

	/*Root*/
	fgets(s,100,f);
	char *noString= strtok(s,",");
	char *name = strtok(NULL,",");
	char *freqString = strtok(NULL,"\n");
	key.no = atoi(noString);
	strcpy(key.name,name);
	key.frequency = atoi(freqString);
	
	if(noString != NULL && name != NULL && freqString != NULL){
		*root = insertwithWordToBST(*root,key);
		*root2 = insertWithFrequencyToBST(*root2,key); 
		(*size)++;
		
	}
	/*Nodes*/
	while(fgets(s,100,f) != NULL){
		char *noString= strtok(s,",");
		char *name = strtok(NULL,",");
		char *freqString = strtok(NULL,"\n");
		
		if(noString != NULL && name != NULL && freqString != NULL){
			key.no = atoi(noString);
			strcpy(key.name,name);
			key.frequency = atoi(freqString);
			
			insertwithWordToBST(*root,key);
			insertWithFrequencyToBST(*root2,key);
			(*size)++;
				
		}
	}
	
	fclose(f);
	return 1;
}
 
/*free the all nodes in the list and the list*/
void free_nodes(struct node *root){
	
	if (root != NULL) { 
		free_nodes(root->left);
        free_nodes(root->right); 
        free(root);
    } 
} 


int main(int argc, char **argv)
{
	
	
	fcvcghbjklbhgfdxcvn b
	
	char fileName[25];		/*filename*/
	printf("Please enter file name with .text\n");
	scanf("%24s",fileName);
	
	/*roots of trees*/
	struct node *root = NULL;		/*root of bst with words*/
	struct node *root2 = NULL;		/*root of bst with frequencies of words*/
	struct node *root3 = NULL;		/*root of bt with minimized by their frequencies*/
	
	int size = 0; 					/*nnumber of nodes*/

	/*reads given input file and initialize the first binary search tree (part a)*/
	if(readAndBuild(&root,&root2,fileName,&size)){
		
		printf("Size: %d\n",size);
		
		/*prints the bst of words as in-order*/
		printf("\n******BST with the Word******\n");
		inorderWords(root);
		inorderFrequency(root);
		printf("\nAccess time: %d\n",totalAccessTime(root,root));
		
		
		/*initialzie the mimimize tree - from root2 --> root3 */
		/*printf("\n******BST with Frequency of The Words (in-order traversal)******\n");
		inorderFrequency(root2);
		*/
		
		/*minimize*/
		constructBTbyFrequency(&root2,&root3,size);
		printf("\n\n******BT with Minimize the Total Access Time******\n");
		inorderFrequency(root3);
		printf("\n");
		inorderWords(root3); 
		printf("\nMinimized Access Time: %d\n",totalAccessTime(root3,root3));
		
		/*Highst of trees: word bst,frequency bst, optimal bt*/
		/*printf("max depth of words bst : %d\n",maxDepth(root));
		printf("max depth of freq. bst : %d\n",maxDepth(root2));
		printf("max depth of minimized of bt: %d\n",maxDepth(root3));
		*/
		
		
	}else{
		printf("File reading has failed!.");
	}
	
	//free tree
	free_nodes(root);
	free_nodes(root2);
	free_nodes(root3);
	
	return 0;
}

