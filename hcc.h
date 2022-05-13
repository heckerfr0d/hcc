struct node
{
    int NODETYPE;
    char *NAME;
    int VALUE;
    struct node *left, *mid, *right;
    struct symbol *entry;
};

struct symbol
{
    char *NAME;
    int BINDING;
    struct symbol *NEXT;
};

struct symbol *lookup(char *);

void append(char *);

struct node *createNode(int, int, char *, struct node *, struct node *, struct node *);

int allocate_reg();

void free_reg();

int code_gen(struct node *);