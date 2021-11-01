#include "spi.h"
#include "main.h"
#include "gpio.h"

#include "W25Q64_Flash.h"

#define LOADER_OK   0x1
#define LOADER_FAIL 0x0

extern void SystemClock_Config(void);

/**
 * @brief  System initialization.
 * @param  None
 * @retval  LOADER_OK = 1   : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int
Init(void) {

	HAL_StatusTypeDef status;

	SystemInit();
	SCB->VTOR = 0x20000000 | 0x200;

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();

    status = W25Q64_Init();

	if(HAL_OK == status)
		return LOADER_OK;
	else
		return LOADER_FAIL;
}

/**
 * @brief   Program memory.
 * @param   Address: page address
 * @param   Size   : size of data
 * @param   buffer : pointer to data buffer
 * @retval  LOADER_OK = 1       : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int
Write(uint32_t Address, uint32_t Size, uint8_t* buffer) {

	uint32_t i, number_of_pages, page_address;
	uint8_t *page_buffer;
	uint8_t modulo_flag;
	HAL_StatusTypeDef status;

	if((Size % PAGE_SIZE) == 0)
	{
		number_of_pages = Size/PAGE_SIZE;
		modulo_flag = 0;
	}
	else
	{
		number_of_pages = Size/PAGE_SIZE + 1;
		modulo_flag = 1;
	}

	page_address = Address;
	page_buffer = buffer;

	for(i=0; i<number_of_pages; i++)
	{
		status = W25Q64_PageProgram(page_address, page_buffer, PAGE_SIZE);

		page_address += PAGE_SIZE;
		page_buffer += PAGE_SIZE;

		if(HAL_OK != status)
			return LOADER_FAIL;
	}

	if(modulo_flag == 1)
	{
		status = W25Q64_PageProgram(page_address, page_buffer, Size % PAGE_SIZE);

		if(HAL_OK != status)
			return LOADER_FAIL;
	}

    return LOADER_OK;
}

/**
 * @brief   Sector erase.
 * @param   EraseStartAddress :  erase start address
 * @param   EraseEndAddress   :  erase end address
 * @retval  LOADER_OK = 1       : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int
SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress) {

	HAL_StatusTypeDef status;
	int i;

	uint32_t erase_start_sector_nbr, erase_sector_nbr;

	erase_start_sector_nbr = EraseStartAddress/SECTOR_SIZE;
	erase_sector_nbr = EraseEndAddress/SECTOR_SIZE - erase_start_sector_nbr;

	for(i = erase_start_sector_nbr; i<erase_sector_nbr; i++)
	{
		status = W25Q64_SectorErase(i);

		if(HAL_OK != status)
			return LOADER_FAIL;
	}

    return LOADER_OK;
}

/**
 * Description :
 * Mass erase of external flash area
 * Optional command - delete in case usage of mass erase is not planed
 * Inputs    :
 *      none
 * outputs   :
 *     none
 * Note: Optional for all types of device
 */
int
MassErase(void) {

	HAL_StatusTypeDef status;

	status = W25Q64_ChipErase();

	if(HAL_OK == status)
		return LOADER_OK;
	else
		return LOADER_FAIL;
}

/**
 * Description :
 * Calculates checksum value of the memory zone
 * Inputs    :
 *      StartAddress  : Flash start address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Checksum value
 * Note: Optional for all types of device
 */
uint32_t
CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal) {

    return LOADER_OK;
}

/**
 * Description :
 * Verify flash memory with RAM buffer and calculates checksum value of
 * the programmed memory
 * Inputs    :
 *      FlashAddr     : Flash address
 *      RAMBufferAddr : RAM buffer address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Operation failed (address of failure)
 *     R1             : Checksum value
 * Note: Optional for all types of device
 */
uint64_t
Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement) {


    return LOADER_OK;
}
