#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hcc.h"
#include "y.tab.h"

#define STMTS_NODE 1

FILE *fp;


struct node *createNode(int NODETYPE, int VALUE, char *NAME, struct node *left, struct node *mid, struct node *right)
{
    struct node *t;
    t = (struct node *)malloc(sizeof(struct node));

    t->NODETYPE = NODETYPE;
    t->VALUE = VALUE;
    t->NAME = NAME;
    t->left = left;
    t->mid = mid;
    t->right = right;

    return t;
}

struct symbol *symbol_table_start = NULL;
struct symbol *symbol_table_end = NULL;

struct symbol *lookup(char *NAME)
{
    struct symbol *t = symbol_table_start;
    while (t)
    {
        if (!strcmp(t->NAME, NAME))
            return t;
        t = t->NEXT;
    }
    return t;
}

int addr = 5050;

void append(char *NAME)
{
    struct symbol *new_entry = lookup(NAME);
    if (new_entry)
        return;

    new_entry = (struct symbol *)malloc(sizeof(struct symbol));
    new_entry->NAME = NAME;
    new_entry->NEXT = NULL;

    new_entry->BINDING = addr;
    addr++;

    if (!symbol_table_end)
    {
        symbol_table_end = new_entry;
        symbol_table_start = new_entry;
    }
    else
    {
        symbol_table_end->NEXT = new_entry;
        symbol_table_end = new_entry;
    }
}

int curr_reg = -1;
int label_counter = -1;

int allocate_reg()
{
    if (curr_reg < 15)
        return ++curr_reg;
    printf("Not enough registers to allocate\n");
    exit(1);
}

void free_reg()
{
    if (curr_reg > -1)
        curr_reg--;
    else
        printf("Error freeing register\n");
}

int code_gen(struct node *ptr)
{
    int reg1, reg2, loop_start, loop_end;

    struct symbol *symbol_table_ptr;

    switch (ptr->NODETYPE)
    {
    case PLUS:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "ADD R%d, R%d\n", reg1, reg2);
        return reg1;
    case MINUS:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "SUB R%d, R%d\n", reg1, reg2);
        return reg1;
    case MUL:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "MUL R%d, R%d\n", reg1, reg2);
        return reg1;
    case LE:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "LE R%d, R%d\n", reg1, reg2);
        return reg1;
    case GE:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "GE R%d, R%d\n", reg1, reg2);
        return reg1;
    case LT:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "LT R%d, R%d\n", reg1, reg2);
        return reg1;
    case GT:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "GT R%d, R%d\n", reg1, reg2);
        return reg1;
    case EQ:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "EQ R%d, R%d\n", reg1, reg2);
        return reg1;
    case NE:
        reg1 = code_gen(ptr->left);
        reg2 = code_gen(ptr->mid);
        free_reg();
        fprintf(fp, "NE R%d, R%d\n", reg1, reg2);
        return reg1;
    case NUM:
        reg2 = allocate_reg();
        fprintf(fp, "MOV R%d, %d\n", reg2, ptr->VALUE);
        return reg2;
    case ASGN:
        reg2 = code_gen(ptr->right);
        free_reg();
        fprintf(fp, "MOV [%d], R%d\n", lookup(ptr->left->NAME)->BINDING, reg2);
        break;
    case WRITE:
        reg1 = code_gen(ptr->left);
        reg2 = allocate_reg();
        free_reg();
        fprintf(fp, "MOV R%d, 5\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", reg2, reg2, reg2, reg2, reg1, reg2, reg2);
        fprintf(fp, "INT 7\n");
        fprintf(fp, "POP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", reg2, reg2, reg2, reg2, reg2);
        break;
    case READ:
        reg1 = allocate_reg();
        free_reg();
        fprintf(fp, "MOV R%d, 7\nPUSH R%d\nMOV R%d, -1\nPUSH R%d\nMOV R%d, %d\nPUSH R%d\nPUSH R%d\nPUSH R%d\n", reg1, reg1, reg1, reg1, reg1, lookup(ptr->left->NAME)->BINDING, reg1, reg1, reg1);
        fprintf(fp, "INT 6\n");
        fprintf(fp, "POP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n", reg1, reg1, reg1, reg1, reg1);
        break;
    case VAR:
        reg1 = allocate_reg();
        fprintf(fp, "MOV R%d, [%d]\n", reg1, lookup(ptr->NAME)->BINDING);
        return reg1;
    case STMTS_NODE:
        code_gen(ptr->left);
        code_gen(ptr->right);
        return -1;
    case IF:
        reg1 = code_gen(ptr->left);
        loop_start = ++label_counter;
        fprintf(fp, "JZ R%d, L%d\n", reg1, loop_start);
        code_gen(ptr->mid);
        if (ptr->right)
        {
            loop_end = ++label_counter;
            fprintf(fp, "JMP L%d\nL%d:\n", loop_end, loop_start);
            code_gen(ptr->right);
            fprintf(fp, "L%d:\n", loop_end);
        }
        else
            fprintf(fp, "L%d:\n", loop_start);
        free_reg();
        return -1;
    case WHILE:
        loop_start = ++label_counter;
        fprintf(fp, "L%d:\n", loop_start);
        code_gen(ptr->left);
        reg1 = code_gen(ptr->mid);
        fprintf(fp, "JNZ R%d, L%d\n", reg1, loop_start);
        return -1;
    }
}