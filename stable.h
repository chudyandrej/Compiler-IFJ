/* 
 * File: stable.h
 *
 * Description: header file for symbol table
 * Authors: Bayer Jan
 *
 * Created: 2015/10/6
 * Last time modified: 2015/10/13
 */

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
