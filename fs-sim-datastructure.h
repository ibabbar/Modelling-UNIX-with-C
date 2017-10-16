#include <stdio.h>

typedef struct Fs {
  struct Directory *root;
  struct Directory *current;
} Fs_sim;

typedef struct Directory {
  char *name;
  struct Directory *next;
  struct Directory *parent;
  struct Directory *subDir;
  struct cFile *cfiles;
} Directory;

typedef struct cFile {
  char* name;
  struct cFile* next;
 } cFile;

typedef struct Node {
  char *name;
  struct Node *next;
} Node;
