//Test module for Temp-Module while LibIIO is being ported
//
//acts as random number generator to generate fake temp vals

#include "Placeholder_TempInterface.h"

void getTempVals(char* Buff)
{
  float random = 0;
  
  random = rand()/(float)0x3fffffff*(float)2;
  
  sprintf(Buff, "%f \n", (float)22 + random);
}
  