#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define NUM_CHARS 256

typedef struct trinode {
   struct trinode* children[NUM_CHARS];
   bool terminal;
} trinode;


extern trinode* createnode() {
   trinode* newnode = malloc(sizeof *newnode);
   for(int k = 0; k < NUM_CHARS; ++k) {
      newnode->children[k] = NULL;
   }
   newnode->terminal = false;
   return newnode;
}

extern bool trinodeInsert(trinode** root, char* signedtext)
{
   if(*root == NULL) *root = createnode();
   //unsigned char* text = (unsigned char*) signedtext;
   char* text = signedtext;
   trinode* spear = *root;
   int length = strlen(text);

   for(int k = 0; k < length; ++k)
   {
      if(spear->children[text[k]] == NULL)
      {
          spear->children[text[k]] = createnode();
      }

      spear = spear->children[text[k]];
   }
   if(spear->terminal) {
      return false;
   }
   else
   {
      spear->terminal = true;
      return true;
   }
}


static void f(trinode* node, char* prefix, int length) {
   char newPrefix[length+16];
   memcpy(newPrefix, prefix, length);
   newPrefix[length+1] = 0;

   if(node->terminal){
       printf("word: %s\n", prefix);
       return;
   }

   for(int k=0; k < NUM_CHARS-1; ++k)
   {
      //we should only execute f if node->children[k] is not nullptr
      if(node->children[k] != NULL) {
         newPrefix[length] = k;
         f(node->children[k], newPrefix, length+1);
      }
   }
}


extern void printTrinode(trinode* root)
{
   if(root == NULL)
   {
      return;
   }

   f(root, NULL, 0);
}

extern bool searchtrie(trinode* root, char* text)
{
   //unsigned char* text = (unsigned char*) signedtext;
   int length = strlen(text);
   trinode* head = root;
   for(int k = 0; k < length; ++k)
   {
      auto c = text[k];
      if(head->children[c] == NULL) continue;
      trinode* kids = head->children[c];
      if(kids == NULL) return false;
      head = head->children[text[k]];
   }
   return true;
}



int main()
{
   trinode* root = createnode();
   trinodeInsert(&root, "ASDIJ");
   trinodeInsert(&root, "ASDIK");
   printTrinode(root);
   int k = (int) searchtrie(&root, "A");
   printf("k:%d", k);
}











