#ifndef __DS_H
#define __DS_H

/*----------------------------- Public Includes ------------------------------*/
#include "List_temp.h"
/*--------------------------- Public Includes END ----------------------------*/

/*--------------------------- Public Declarations ----------------------------*/
List__i__declare();
/*------------------------- Public Declarations END --------------------------*/

/*--------------------------- Public Struct Reveal ---------------------------*/
struct list__i
{
   struct list_node__i *head;
   size_t size;
};
/*------------------------- Public Struct Reveal END -------------------------*/

#endif
