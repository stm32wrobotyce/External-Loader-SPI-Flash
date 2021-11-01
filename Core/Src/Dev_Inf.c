/*
 * Dev_Inf.c
 *
 */
#include "Dev_Inf.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo const StorageInfo = {
#endif
    "W25Q64FV_STM32L476RG_SPI",                  // Device Name + version number
	SPI_FLASH,                           // Device Type
    0x00000000,                          // Device Start Address
	0x00800000,                   		// Device Size in Bytes
	0x00000100,                   		 // Programming Page Size
    0xFF,                                // Initial Content of Erased Memory

    // Specify Size and Address of Sectors
    {   {
    		0x800,						// Number of Sectors
    		0x1000						// Sector Size in Bytes
        },       //Sector Size

        { 0x00000000, 0x00000000 }
    }
};
