/*
 * flash_072c8.h
 *
 *  Created on: 28-Jan-2023
 *      Author: Rohit N Ghosh
 */

#ifndef INC_FLASH_072C8_H_
#define INC_FLASH_072C8_H_

uint32_t U16x2toU32(uint16_t U16_low,uint16_t U16_high);
uint32_t U8x4toU32(uint8_t U8_highest,uint8_t U8_high,uint8_t U8_low,uint8_t U8_lowest);
uint32_t U16x1U8x2toU32(uint16_t U16_high,uint8_t U8_high,uint8_t U8_low);
uint16_t U8x2toU16(uint8_t U8_high,uint8_t U8_low);

void U32toU8x4(uint32_t U32_in,void * U8_highest,void * U8_high,void * U8_low,void * U8_lowest);
void U32toU16x2(uint32_t U32_in,void *U16_high,void *U16_low);
void U32toU16x1U8x2(uint32_t U32_in,void *U16_high,void *U8_high,void *U8_low);

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords);

void Flash_Data_init();
void FLASH_COMMAND(uint8_t message_type,uint8_t data_b1,uint8_t data_b2,uint8_t data_b3,uint8_t data_b4,uint8_t data_chk);

uint8_t flash_dump_status();
uint8_t flash_update_config_status();
uint8_t flash_save_config_status();

void flash_dump_reset();
void flash_update_config_reset();
void flash_save_config_reset();
void flash_save_config_set();

uint8_t save_flash_reg();
void update_faults1_in_flash(uint8_t fault1, uint8_t fault2, uint8_t fault3, uint8_t fault4);
void update_faults2_in_flash(uint8_t fault1, uint8_t fault2, uint8_t fault3, uint8_t fault4);
void update_voltages_in_flash(uint8_t count,uint16_t v2, uint16_t v1);

void update_in_flash();
void time_in_flash();

uint32_t GetPage(uint32_t Address);

#endif /* INC_FLASH_072C8_H_ */
