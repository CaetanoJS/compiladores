
#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 997
#define DATATYPE_CHAR 1
#define DATATYPE_INT 2
#define DATATYPE_FLOAT 3
#define DATATYPE_BOOL 4

typedef struct hash_node
{
    int type;
    int datatype;
    char *text;
    struct hash_node * next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
int hash_check_undeclared(void);

#endif
// END