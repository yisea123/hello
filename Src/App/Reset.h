#ifndef __RESET__H
#define __RESET__H

#include "logic.h"
#include "fsm.h"

#define IFHOME (GSS.AxIOconfig[i].Orglev == InPut_GetSta(GSS.AxIOconfig[i].OrgNum))

void Reset(void);

#endif
