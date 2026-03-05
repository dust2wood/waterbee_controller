#ifndef MicroSd_H
#define MicroSd_H

#include "Main.h"
#include "sdio_sd.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>




int SD_TotalSize(void);	
FRESULT scan_files (char* path);
void MicroSD_Write (void);

extern void SD_LowLevel_Init(void);

#endif
