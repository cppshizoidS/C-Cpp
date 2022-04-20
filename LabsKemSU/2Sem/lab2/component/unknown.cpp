#include "unknown.h"

U_LONG I_ClassFactory::AddRef() 
{
    countReference++;
    return countReference;
}

U_LONG I_ClassFactory::Release() 
{
    countReference--;
    if(countReference == 0)
    {
        delete this;
    }
    return countReference;
}