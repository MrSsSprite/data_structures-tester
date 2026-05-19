#ifndef __DS_H
#define __DS_H

/*----------------------------- Public Includes ------------------------------*/
#include "List_temp.h"
/*--------------------------- Public Includes END ----------------------------*/

/*--------------------------- Public Declarations ----------------------------*/
List__declare(int, i);
List__declare(float, f);
/*------------------------- Public Declarations END --------------------------*/

/*--------------------------- Public Struct Reveal ---------------------------*/
#define _DS_STRUCT_REVEAL(S) \
struct list__##S \
{ \
   struct list_node__##S *head; \
   size_t size; \
};

_DS_STRUCT_REVEAL(i);
_DS_STRUCT_REVEAL(f);

#undef _DS_STRUCT_REVEAL
/*------------------------- Public Struct Reveal END -------------------------*/

#endif
