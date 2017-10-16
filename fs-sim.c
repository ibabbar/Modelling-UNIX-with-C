#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs-sim.h"

/*
  Name: Ishan Babbar
  Terp ID: ibabbar
  UID: 113596834
  Section Number: 0102
  Description: To recreate the UNIX file system setup. We use functions like mkdir, mkfs, touch, ls, pwd, cd.
*/

/* Go through all files if files exists */
int exists (Directory *dir, const char *arg) {
  Directory *current = dir->subDir;
  cFile *curr = dir->cfiles;

  /*check directories*/
  while(current != NULL) {
    if(strcmp(current->name, arg) == 0)
      return 1;
    current = current->next;
  }
  /*check files*/
  while(curr != NULL) {
    if(strcmp(curr->name, arg) == 0)
      return 2;
    curr = curr->next;
    }
  return 0;
}
/*print all contents of directory */
void  printAllDir (Directory* dir) {
  Directory *curr = dir->subDir;
  cFile *currFile = dir->cfiles;
  Node *guy = NULL,*head = NULL, *temp = NULL, *prevTemp = NULL;

  /*print diretories*/
  while (curr != NULL) {
    guy = malloc(sizeof(Node));
    guy->name = malloc(strlen(curr->name) + 2);
    strcpy(guy->name, curr->name);
    guy->name[strlen(curr->name)] = '/';
    guy->name[strlen(curr->name) + 1] = '\0';

    if (head == NULL) {
      head = guy;
      temp = head;
      curr = curr->next;
      continue;
    } else {
      while(temp != NULL) {
	if (strcmp(temp->name, guy->name) > 0) {
	  if(prevTemp == NULL){
	    guy->next = temp;
	    head = guy;
	  } else {
	    prevTemp->next = guy;
	    guy -> next = temp;
	  }
	  break;
	}
	prevTemp = temp;
	temp = temp->next;
	if (temp == NULL) {
	  prevTemp -> next = guy;
	}
      }
      temp = head;
      prevTemp = NULL;
      curr = curr->next;
    }
  }
  /*print files*/
  while (currFile != NULL) {
    guy = malloc(sizeof(Node));
    guy->name = malloc(strlen(currFile->name) + 1);
    strcpy(guy->name, currFile->name);
    guy->next = NULL;

    if (head == NULL) {
      head = guy;
      temp = head;
      currFile = currFile->next;
      continue;
    } else {
      while(temp != NULL) {
	if (strcmp(temp->name, guy->name) > 0) {
	  if(prevTemp == NULL){
	    guy->next = temp;
	    head = guy;
	  } else {
	    prevTemp->next = guy;
	    guy -> next = temp;
	  }
	  break;
	}
	prevTemp = temp;
	temp = temp->next;
	if (temp == NULL) {
	  prevTemp -> next = guy;
	}
      }
      temp = head;
      prevTemp = NULL;
      currFile = currFile->next;
    }
  }
  while(temp != NULL) {
    printf("%s\n", temp->name);
    prevTemp = temp;
    temp = temp->next;
  }
}

/*initialize file system*/
void mkfs(Fs_sim *file) {
  Directory *direct = malloc(sizeof(Directory));
  file->root = direct;
  direct->name = malloc(2);
  strcpy(direct->name, "/");

  file-> current = direct;
  /* current */
  file->current->next = NULL;
  file->current->parent = NULL;
  file->current->subDir = NULL;
}

/*add files to file system*/
int touch(Fs_sim*files, const char arg[]){
  cFile *temp = NULL;
  cFile *new = NULL;
  
  if (files != NULL && arg != NULL) {
    if (strcmp(arg, "/") == 0 || strcmp(arg, ".") == 0 ||
	strcmp(arg, "\0") == 0 || exists(files->current, arg)
	|| strstr(arg, "/") != NULL) 
      return 0;
    /*if there are files*/
    if (files->current->cfiles != NULL) {
      temp = malloc(sizeof(cFile));
      temp->next = files->current->cfiles;
      temp->name = malloc(strlen(arg) + 1);
      strcpy(temp->name, arg);
      files->current->cfiles = temp;
    }
    else {
      new = malloc(sizeof(cFile));
      new->name = malloc(strlen(arg) + 1);
      strcpy(new->name, arg);
      files->current->cfiles = new;
    }
    return 1;
  }
  return 0;
}

/*creates new directory */
int mkdir(Fs_sim *files, const char arg[]){
  Directory *newDir, *temp;

  if (files != NULL && arg != NULL) {
    if (!exists(files->current, arg)) {
      if (files->current->subDir != NULL) {
	newDir = malloc(sizeof(Directory));
	newDir->name = malloc(strlen(arg) + 1);
	strcpy(newDir->name, arg);
	newDir->parent = files->current;
	newDir->next = files->current->subDir;
	files->current->subDir = newDir;
	newDir->subDir = NULL;
	newDir->cfiles = NULL;
      } else {
	temp = malloc(sizeof(Directory));
	files->current->subDir = temp;
	temp->name = malloc(strlen(arg) + 1);
	strcpy(temp->name, arg);
	temp->parent = files->current;
	temp->next = NULL;
	temp->subDir = NULL;
	temp->cfiles = NULL;
      }
    }
  }
  return 0;
}

/*changes directory to arg*/
int cd(Fs_sim *files, const char arg[]){
  Directory *curr = files->current->subDir;
  
  if (files != NULL && arg != NULL) {
    if (strcmp(arg, "/") == 0 || strlen(arg) == 0) {
      files->current = files->root;
    }
    if (strcmp(arg, ".") == 0 || strstr(arg, "/") != NULL)
	return 0;
    if (strcmp(arg, "..") == 0) {
      if( files->current->parent != NULL) {
	files->current = files-> current->parent;
      }
      return 1;
    }
    if (!exists(files->current, arg))
      return 0;
    while (curr != NULL) {
      if (strcmp(curr->name, arg) == 0) {
	/*setting that directory to current*/
	files->current = curr;
	return 1;
      }
      curr = curr->next;
    }
  }
  return 0;
}
  
/*lists all files and subdirectories */
int ls(Fs_sim *files, const char arg[]) {
  
  if (files != NULL && arg != NULL) {
    if (exists(files->current, arg) == 1) {
	cd(files, arg);
	printAllDir(files->current);
	files->current = files->current->parent;
    } else if (exists(files->current, arg) == 2) {
      printf("%s\n", arg);
    } else if (strcmp(arg, "\0") == 0 || strcmp(arg, ".") == 0)
      printAllDir(files->current);
    else if (strcmp(arg, "..") == 0)
      printAllDir(files->current->parent);
    else if (strcmp(arg, "/") == 0)
      printAllDir(files->root);
    else if (strcmp(arg, "/") != 0 && strstr(arg, "/") != NULL)
      return 0;
    else if (!exists(files->current, arg))
      return 0;
  }
  return 0;
}

/*recursive helper for pwd*/
void pwd_aux(Directory *dir){
   if(dir == NULL){
    return;
  }
   
  pwd_aux(dir->parent);
  printf("%s", dir->name);
}
/*creating path for a file system */
void pwd(Fs_sim *files) {
  pwd_aux(files->current);
  printf("\n");
}
