
#include "stm32f0xx.h"
#include "watchdog.h"

void iwdg_init()
{
/*1.Enable the watchdog by writing 0x0000CCCC to the IDWG_KR register
 * 2.Enable register access by writing 0x00005555 to the IDWG_KR register
 * 3. Set the IWDG prescaler
 * 4. Set the reload register(IWDG_RLR) to largest value 0x0FFF for max timeout
 * 5. Wait for the registers to be updated IWDG_SR = 0x0000
 * 6. Refresh the counter value with IWDG_RLR (IWDG_KR = 0xAAAA)
 */

	//Step 1
	IWDG->KR = IWDG_KEY_ENABLE;

	//Step 2
	IWDG->KR = IWDG_WRACCESS_ENABLE;

	//Step 3
	IWDG->PR = IWDG_PRESCALER;

	//Step 4
	IWDG->RLR = IWDG_RLR_VALUE;

	//Step 5
	while(IWDG->SR!=0x00000000);

	//Step 6
	IWDG->KR =IWDG_REFRESH_ENABLE;
}

