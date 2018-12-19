//  test_me.c

#include "global.h"
// #include "GALX_1132_TEST.h"
#include "SD_TEST.h"

void NothingFunction(void); // function prototype
void DoAnotherNothing(void);

int16 x = 1, y = 2, z[10];
int16 *ip;

struct point {
    int x;
    int y;
    void *Service;
} origin;

struct point OriginArray[7];
struct point AnotherArray[] = {
    {1, 2, DoAnotherNothing},
    {3, 4, DoAnotherNothing},
    {5, 6, DoAnotherNothing},
    {7, 8, DoAnotherNothing}
};

int16 rr;
int16 *xy;

struct point *pp;
int16 z23;

//////////////////////////////////////
void NothingFunction(void)
{
    ip = &x;
    xy = NULL;
    xy = &rr;
    xy = &z23;
    pp = &origin;
    pp = OriginArray;
    pp = AnotherArray;
    ;
}

void DoAnotherNothing(void)
{
    ;
}