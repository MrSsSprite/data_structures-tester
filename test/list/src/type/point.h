#ifndef LIST_TYPE_POINT_H
#define LIST_TYPE_POINT_H

#include <string.h>
#include "unity.h"
#include "Point_type.h"

// type/point.h — macros for struct Point

#define LIST_SUFFIX       p
#define LIST_TYPE         Point
#define LIST_EQ(a, b)     ({ Point _a_ = (a), _b_ = (b); memcmp(&_a_, &_b_, sizeof(Point)) == 0; })
#define LIST_ASSERT_EQ(a, b)  TEST_ASSERT_TRUE(LIST_EQ((a), (b)))
#define LIST_VAL_FROM_IDX(i)  ({ int _i_ = (i); (Point){ (int)(_i_), -(int)(_i_) }; })
#define LIST_RAND()           ((Point){ rand(), rand() }) /* rand() is not a macro arg; double eval is intentional */
#define LIST_SENTINEL_NEG     ((Point){ -1, -1 })
#define LIST_SENTINEL_A       ((Point){ 0xAB, 0xCD })
#define LIST_SENTINEL_B       ((Point){ 0xEF, 0x01 })
#define LIST_SENTINEL_MULTI   { { -3, 3 }, { -2, 2 }, { -1, 1 } }
#define LIST_SENTINEL_INSERT  { { 10, -10 }, { 20, -20 }, { 30, -30 } }
#define LIST_SENTINEL_SIZE    { { 100, -100 }, { 200, -200 }, { 300, -300 } }
#define LIST_VALUE_IDENTITY(a, b)  ({ Point _vi_a_ = (a), _vi_b_ = (b); memcmp(&_vi_a_, &_vi_b_, sizeof(Point)) == 0; })
#define LIST_SENTINEL_FROM_IDX(i)  ({ int _si_ = (i); (Point){ -(int)(_si_), (int)(_si_) }; })
#define LIST_CMP_GT(a, b)  (((a).x > (b).x) || ((a).x == (b).x && (a).y > (b).y))
#define LIST_CMP_LT(a, b)  (((a).x < (b).x) || ((a).x == (b).x && (a).y < (b).y))


#endif
