#ifndef STABLE_H_INCLUDED
#define STABLE_H_INCLUDED

typedef struct SymbolTableItem {
    char * value;
    int token_type;
}tSymbolTableItem;

typedef struct SymbolTable {
    tSymbolTableItem * first;
    tSymbolTableItem * rptr;
    tSymbolTableItem * lptr;
}tSymbolTable;

#endif // STABLE_H_INCLUDED
