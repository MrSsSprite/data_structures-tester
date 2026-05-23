#ifndef __DS_H
#define __DS_H

/*----------------------------- Public Includes ------------------------------*/
#include "List_temp.h"
#include "type/Point_type.h"
/*--------------------------- Public Includes END ----------------------------*/

/*--------------------------- Public Declarations ----------------------------*/
List__declare(int, i);
List__declare(float, f);
List__declare(double, d);
List__declare(unsigned char, c);
List__declare(unsigned int, u);
List__declare(long, l);
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
_DS_STRUCT_REVEAL(d);
_DS_STRUCT_REVEAL(c);
_DS_STRUCT_REVEAL(u);
_DS_STRUCT_REVEAL(l);

#undef _DS_STRUCT_REVEAL
/*------------------------- Public Struct Reveal END -------------------------*/

#endif
