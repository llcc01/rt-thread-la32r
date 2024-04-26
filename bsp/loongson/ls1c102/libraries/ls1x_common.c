#include "ls1x_common.h"
#include "ls1x.h"

void Wake_Set(INT32U sec)
{
    if(sec != 0)
    {
        INT32U time = sec * 256;
        INT32U compare = 0;
        compare = (PMU_COUNT & 0xfffff) + time;
        PMU_COMPARE  = compare;
        PMU_CMDSTS |= (1 << 7) | (1 << 8);      
    }
    else
    {
        PMU_CMDSTS &=  ~((1 << 7) | (1 << 8));  
    }
}
void Wake_256sec_Set(INT32U p256sec)
{
    if(p256sec != 0)
    {
        INT32U time = p256sec ;
        INT32U compare = 0;
        compare = (PMU_COUNT & 0xfffff) + time;
        PMU_COMPARE  = compare;
        PMU_CMDSTS |= (1 << 7) | (1 << 8);      
    }
    else
    {
        PMU_CMDSTS &=  ~((1 << 7) | (1 << 8));  
    }
}
