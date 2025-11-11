/*
 * flash_072c8.c
 *
 *  Created on: 28-Jan-2023
 *      Author: Rohit N Ghosh
 */

#ifndef SRC_FLASH_072C8_C_
#define SRC_FLASH_072C8_C_

#include "main.h"
#include "flash_072c8.h"
#include <string.h>
#include "bq76952.h"
#include "bq76952_default_config.h"

extern RTC_HandleTypeDef hrtc;
extern uint32_t EEPROM_RAM_REG[512];

uint8_t flash_dump = 0;
uint8_t save_config = 0;
uint8_t update_config = 0;

/*Modified Portion*/
/*Time value in flash should be converted to hexadecimal to get the correct value
whereas voltages should be converted to binary for getting the correct value. That is because
the value of RTC clock is converted to binary format to be user friendly*/

/*Modified Portion*/
//Variables for storing time and date
RTC_TimeTypeDef Present_Time = { 0 };
RTC_DateTypeDef Present_Date = { 0 };

//Variables to store flash contents
//uint32_t SNAPSHOT[9]={0,0,0,0,0,0,0,0,0};
//uint32_t SNAPSHOT[3]={0,0,0};
uint32_t SNAPSHOT1=0;
uint32_t SNAPSHOT2=0;
uint32_t SNAPSHOT3=0;
uint32_t SNAPSHOT4=0;
uint32_t SNAPSHOT5=0;
uint32_t SNAPSHOT6=0;
uint32_t SNAPSHOT7=0;
uint32_t SNAPSHOT8=0;
uint32_t SNAPSHOT9=0;
//uint32_t temp=0;
uint16_t date_month=0;
uint16_t mins_seconds=0;
uint16_t day_hrs=0;

uint32_t U16x2toU32(uint16_t U16_high,uint16_t U16_low)
{
	uint32_t U32_a = 0;
	U32_a = (U16_high << 16) + U16_low;
	return U32_a;
}
uint32_t U8x4toU32(uint8_t U8_highest,uint8_t U8_high,uint8_t U8_low,uint8_t U8_lowest)
{
	uint32_t U32_a = 0;
	U32_a = (U8_highest << 24) + (U8_high <<16) + (U8_low <<8)+ (U8_lowest);
	return U32_a;
}
uint32_t U16x1U8x2toU32(uint16_t U16_high,uint8_t U8_high,uint8_t U8_low)
{
	uint32_t U32_a = 0;
	U32_a = (U16_high << 16) + (U8_high <<8)+ (U8_low);
	return U32_a;
}
uint16_t U8x2toU16(uint8_t U8_high,uint8_t U8_low)
{
	uint32_t U16_a = 0;
	U16_a = (U8_high <<16) + (U8_low);
	return U16_a;
}

void U32toU16x1U8x2(uint32_t U32_in,void *U16_high,void *U8_high,void *U8_low)
{
	{
		uint8_t temp = 0;
		temp = (U32_in & 0xFF);
		memcpy(U8_low, &temp,1);

		temp = (U32_in & 0xFF00) >> 8;
		memcpy(U8_high, &temp,1);
	}
	{
		uint16_t temp = 0;
		temp = (U32_in & 0xFFFF0000)>> 16;
		memcpy(U16_high, &temp,2);

	}
}
void U32toU8x4(uint32_t U32_in,void *U8_highest,void *U8_high,void *U8_low,void *U8_lowest)
{
	uint8_t temp = (U32_in & 0x000000FF);
	memcpy(U8_lowest, &temp,1);

	temp = (U32_in & 0x0000FF00) >> 8;
	memcpy(U8_low, &temp,1);

	temp = (U32_in & 0x00FF0000) >> 16;
	memcpy(U8_high, &temp,1);

	temp = (U32_in & 0xFF000000) >> 24;
	memcpy(U8_highest, &temp,1);

}
void U32toU16x2(uint32_t U32_in,void *U16_high,void *U16_low)
{

	uint16_t temp = 0;
	temp = (U32_in & 0x0000FFFF);
	memcpy(U16_low, &temp,2);

	temp = (U32_in & 0xFFFF0000)>> 16;
	memcpy(U16_high, &temp,2);
}


uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar=0;

	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();

	   /* Erase the user Flash area*/

	  uint32_t StartPage = GetPage(StartPageAddress);
	  uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
	  uint32_t EndPage = GetPage(EndPageAdress);

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	   EraseInitStruct.PageAddress = StartPage;
	   EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   {
	     /*Error occurred while page erase.*/
		  return HAL_FLASH_GetError ();
	   }

	   /* Program the user Flash area word by word*/

	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartPageAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	   /* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/
	   HAL_FLASH_Lock();

	   return 0;
}

void Flash_Read_Data (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{

		*RxBuf = *(__IO uint32_t *)StartPageAddress;
		StartPageAddress += 4;
		RxBuf++;
		if (!(numberofwords--)) break;
	}
}

uint8_t save_flash_reg()
{
	__disable_irq();
	if(!Flash_Write_Data (0x0800F800, EEPROM_RAM_REG, 512))
	{
		flash_save_config_reset();
		__enable_irq();
		return 0;
	}
	else
	{
		__enable_irq();
		return 1;
	}
}



uint32_t GetPage(uint32_t Address)
{
  for (int indx=0; indx<64; indx++)
  {
	  if((Address < (0x08000000 + (FLASH_PAGE_SIZE *(indx+1))) ) && (Address >= (0x08000000 + FLASH_PAGE_SIZE*indx)))
	  {
		  return (0x08000000 + FLASH_PAGE_SIZE*indx);
	  }
  }

  return 0;
}

void Flash_Data_init() //XXX
{
	Flash_Read_Data (0x0800F800, EEPROM_RAM_REG, 512);
	if(EEPROM_TEST_BYTE == (EEPROM_RAM_REG[511]&0xFF))
	{
		return;
	}
	else
	{

		EEPROM_RAM_REG[0] = U16x2toU32(CELL2GAIN,CELL1GAIN);
		EEPROM_RAM_REG[1] = U16x2toU32(CELL4GAIN,CELL3GAIN);
		EEPROM_RAM_REG[2] = U16x2toU32(CELL6GAIN,CELL5GAIN);
		EEPROM_RAM_REG[3] = U16x2toU32(CELL8GAIN,CELL7GAIN);
		EEPROM_RAM_REG[4] = U16x2toU32(CELL10GAIN,CELL9GAIN);
		EEPROM_RAM_REG[5] = U16x2toU32(CELL12GAIN,CELL11GAIN);
		EEPROM_RAM_REG[6] = U16x2toU32(CELL14GAIN,CELL13GAIN);
		EEPROM_RAM_REG[7] = U16x2toU32(CELL16GAIN,CELL15GAIN);
		EEPROM_RAM_REG[8] = U16x2toU32(TOSGAIN,PACKGAIN);
		EEPROM_RAM_REG[9] = U16x2toU32(LDGAIN,ADCGAIN);
		EEPROM_RAM_REG[10] = 0;
		float tempu = CCGAIN;
		memcpy(&EEPROM_RAM_REG[10],&tempu, 4);
		EEPROM_RAM_REG[11] = 0;
		tempu = CAPACITYGAIN;
		memcpy(&EEPROM_RAM_REG[11],&tempu, 4);
		EEPROM_RAM_REG[12] = U16x2toU32(VDIVOFFSET,VCELLOFFSET);
		EEPROM_RAM_REG[13] = U16x2toU32(BOARDOFFSET,COULOMBCOUNTEROFFSETSAMPLES);
		EEPROM_RAM_REG[14] = U8x4toU32(ALERTTEMPOFFSET,DFETOFFTEMPOFFSET,CFETOFFTEMPOFFSET,INTERNALTEMPOFFSET);
		EEPROM_RAM_REG[15] = U8x4toU32(HDQTEMPOFFSET,TS3TEMPOFFSET,TS2TEMPOFFSET,TS1TEMPOFFSET);
		EEPROM_RAM_REG[16] = U16x1U8x2toU32(INTGAIN,DCHGTEMPOFFSET,DCHGTEMPOFFSET);
		EEPROM_RAM_REG[17] = U16x2toU32(INTMAXIMUMAD,INTBASEOFFSET);
		EEPROM_RAM_REG[18] = U16x2toU32(T18KCOEFFA1,INTMAXIMUMTEMP);
		EEPROM_RAM_REG[19] = U16x2toU32(T18KCOEFFA3,T18KCOEFFA2);
		EEPROM_RAM_REG[20] = U16x2toU32(T18KCOEFFA5,T18KCOEFFA4);
		EEPROM_RAM_REG[21] = U16x2toU32(T18KCOEFFB2,T18KCOEFFB1);
		EEPROM_RAM_REG[22] = U16x2toU32(T18KCOEFFB4,T18KCOEFFB3);
		EEPROM_RAM_REG[23] = U16x2toU32(T180KCOEFFA1,T18KADC0);
		EEPROM_RAM_REG[24] = U16x2toU32(T180KCOEFFA3,T180KCOEFFA2);
		EEPROM_RAM_REG[25] = U16x2toU32(T180KCOEFFA5,T180KCOEFFA4);
		EEPROM_RAM_REG[26] = U16x2toU32(T180KCOEFFB2,T180KCOEFFB1);
		EEPROM_RAM_REG[27] = U16x2toU32(T180KCOEFFB4,T180KCOEFFB3);
		EEPROM_RAM_REG[28] = U16x2toU32(CUSTOMCOEFFA1,T180KADC0);
		EEPROM_RAM_REG[29] = U16x2toU32(CUSTOMCOEFFA3,CUSTOMCOEFFA2);
		EEPROM_RAM_REG[30] = U16x2toU32(CUSTOMCOEFFA5,CUSTOMCOEFFA4);
		EEPROM_RAM_REG[31] = U16x2toU32(CUSTOMCOEFFB2,CUSTOMCOEFFB1);
		EEPROM_RAM_REG[32] = U16x2toU32(CUSTOMCOEFFB4,CUSTOMCOEFFB3);
		EEPROM_RAM_REG[33] = U16x2toU32(CUSTOMADC0,CUSTOMRC0);

		EEPROM_RAM_REG[34] = U16x1U8x2toU32(CUVTHRESHOLDOVERRIDE,FUSEBLOWTIMEOUT,COULOMBCOUNTERDEADBAND);
		EEPROM_RAM_REG[35] = U16x2toU32(MINBLOWFUSEVOLTAGE,COVTHRESHOLDOVERRIDE);

		EEPROM_RAM_REG[36] = U16x1U8x2toU32(POWERCONFIG,REG12CONFIG,REG0CONFIG);
		EEPROM_RAM_REG[37] = U8x4toU32(SPICONFIGURATION,I2CADDRESS,COMMTYPE,HWDREGULATOROPTIONS);
		EEPROM_RAM_REG[38] = U8x4toU32(ALERTPINCONFIG,DFETOFFPINCONFIG,CFETOFFPINCONFIG,COMMIDLETIME);
		EEPROM_RAM_REG[39] = U8x4toU32(HDQPINCONFIG,TS3CONFIG,TS2CONFIG,TS1CONFIG);
		EEPROM_RAM_REG[40] = U8x4toU32(CC3SAMPLES,DACONFIGURATION,DDSGPINCONFIG,DCHGPINCONFIG);
		EEPROM_RAM_REG[41] = U16x2toU32(PROTECTIONCONFIGURATION,VCELLMODE);
		EEPROM_RAM_REG[42] = U8x4toU32(CHGFETPROTECTIONSA,ENABLEDPROTECTIONSC,ENABLEDPROTECTIONSB,ENABLEDPROTECTIONSA);
		EEPROM_RAM_REG[43] = U8x4toU32(DSGFETPROTECTIONSB,DSGFETPROTECTIONSA,CHGFETPROTECTIONSC,CHGFETPROTECTIONSB);
		EEPROM_RAM_REG[44] = U16x2toU32(DEFAULTALARMMASK,BODYDIODETHRESHOLD);
		EEPROM_RAM_REG[45] = U8x4toU32(SFALERTMASKC,SFALERTMASKB,SFALERTMASKA,DSGFETPROTECTIONSC);
		EEPROM_RAM_REG[46] = U8x4toU32(PFALERTMASKD,PFALERTMASKC,PFALERTMASKB,PFALERTMASKA);
		EEPROM_RAM_REG[47] = U8x4toU32(ENABLEDPFD,ENABLEDPFC,ENABLEDPFB,ENABLEDPFA);
		EEPROM_RAM_REG[48] = U16x1U8x2toU32(PRECHARGESTARTVOLTAGE,CHGPUMPCONTROL,FETOPTIONS);
		EEPROM_RAM_REG[49] = U16x1U8x2toU32(PRECHARGESTOPVOLTAGE,PREDISCHARGETIMEOUT,PREDISCHARGESTOPDELTA);
		EEPROM_RAM_REG[50] = U16x2toU32(CHGCURRENTTHRESHOLD,DSGCURRENTTHRESHOLD);
		EEPROM_RAM_REG[51] = U16x1U8x2toU32(MFGSTATUSINIT,BALANCINGCONFIGURATION,CHECKTIME);

		EEPROM_RAM_REG[52] = U16x2toU32(CELL2INTERCONNECT,CELL1INTERCONNECT);
		EEPROM_RAM_REG[53] = U16x2toU32(CELL4INTERCONNECT,CELL3INTERCONNECT);
		EEPROM_RAM_REG[54] = U16x2toU32(CELL6INTERCONNECT,CELL5INTERCONNECT);
		EEPROM_RAM_REG[55] = U16x2toU32(CELL8INTERCONNECT,CELL7INTERCONNECT);
		EEPROM_RAM_REG[56] = U16x2toU32(CELL10INTERCONNECT,CELL9INTERCONNECT);
		EEPROM_RAM_REG[57] = U16x2toU32(CELL12INTERCONNECT,CELL11INTERCONNECT);
		EEPROM_RAM_REG[58] = U16x2toU32(CELL14INTERCONNECT,CELL13INTERCONNECT);
		EEPROM_RAM_REG[59] = U16x2toU32(CELL16INTERCONNECT,CELL15INTERCONNECT);

		EEPROM_RAM_REG[60] = U8x4toU32(CELLBALANCEINTERVAL,MAXINTERNALTEMP,MAXCELLTEMP,MINCELLTEMP);
		EEPROM_RAM_REG[61] = U16x1U8x2toU32(CELLBALANCEMINCELLVCHARGE,CELLBALANCEMINDELTACHARGE,CELLBALANCEMAXCELLS);
		EEPROM_RAM_REG[62] = U16x1U8x2toU32(CELLBALANCEMINCELLVRELAX,CELLBALANCEMINDELTARELAX,CELLBALANCESTOPDELTACHARGE);
		EEPROM_RAM_REG[63] = U16x2toU32(SHUTDOWNSTACKVOLTAGE,SHUTDOWNCELLVOLTAGE);
		EEPROM_RAM_REG[64] = U8x4toU32(SHUTDOWNTEMPERATUREDELAY,SHUTDOWNTEMPERATURE,LOWVSHUTDOWNDELAY,CELLBALANCESTOPDELTARELAX);
		EEPROM_RAM_REG[65] = U8x4toU32(RAMFAILSHUTDOWNTIME,AUTOSHUTDOWNTIME,SHUTDOWNCOMMANDDELAY,FETOFFDELAY);
		EEPROM_RAM_REG[66] = U16x2toU32(WAKECOMPARATORCURRENT,SLEEPCURRENT);
		EEPROM_RAM_REG[67] = U16x1U8x2toU32(SLEEPCHARGERVOLTAGETHRESHOLD,VOLTAGETIME,SLEEPHYSTERESISTIME);
		EEPROM_RAM_REG[68] = U16x2toU32(CONFIGRAMSIGNATURE,SLEEPCHARGERPACKTOSDELTA);
		EEPROM_RAM_REG[69] = U16x1U8x2toU32(CUVDELAY,CUVTHRESHOLD,CUVRECOVERYHYSTERESIS);
		EEPROM_RAM_REG[70] = U16x1U8x2toU32(COVDELAY,COVTHRESHOLD,COVRECOVERYHYSTERESIS);
		EEPROM_RAM_REG[71] = U8x4toU32(OCCTHRESHOLD,COVLRECOVERYTIME,COVLCOUNTERDECDELAY,COVLLATCHLIMIT);
		EEPROM_RAM_REG[72] = U16x2toU32(OCCPACKTOSDELTA,OCCRECOVERYTHRESHOLD);
		EEPROM_RAM_REG[73] = U8x4toU32(OCD2THRESHOLD,OCD1DELAY,OCD1THRESHOLD,OCCDELAY);
		EEPROM_RAM_REG[74] = U8x4toU32(SCDRECOVERYTIME,SCDDELAY,SCDTHRESHOLD,OCD2DELAY);
		EEPROM_RAM_REG[75] = U16x2toU32(OCDRECOVERYTHRESHOLD,OCD3THRESHOLD);
		EEPROM_RAM_REG[76] = U8x4toU32(OCDLRECOVERYTIME,OCDLCOUNTERDECDELAY,OCDLLATCHLIMIT,OCD3DELAY);
		EEPROM_RAM_REG[77] = U16x1U8x2toU32(OCDLRECOVERYTHRESHOLD,SCDLCOUNTERDECDELAY,SCDLLATCHLIMIT);
		EEPROM_RAM_REG[78] = U16x1U8x2toU32(SCDLRECOVERYTHRESHOLD,OTCTHRESHOLD,SCDLRECOVERYTIME);
		EEPROM_RAM_REG[79] = U8x4toU32(OTDDELAY,OTDTHRESHOLD,OTCRECOVERY,OTCDELAY);
		EEPROM_RAM_REG[80] = U8x4toU32(OTFRECOVERY,OTFDELAY,OTFTHRESHOLD,OTDRECOVERY);
		EEPROM_RAM_REG[81] = U8x4toU32(UTCTHRESHOLD,OTINTRECOVERY,OTINTDELAY,OTINTTHRESHOLD);
		EEPROM_RAM_REG[82] = U8x4toU32(UTDDELAY,UTDTHRESHOLD,UTCRECOVERY,UTCDELAY);
		EEPROM_RAM_REG[83] = U8x4toU32(UTINTRECOVERY,UTINTDELAY,UTINTTHRESHOLD,UTDRECOVERY);
		EEPROM_RAM_REG[84] = U16x1U8x2toU32(HWDDELAY,LOADDETECTACTIVETIME,PROTECTIONSRECOVERYTIME);
		EEPROM_RAM_REG[85] = U16x2toU32(PTOCHARGETHRESHOLD,LOADDETECTTIMEOUT);
		EEPROM_RAM_REG[86] = U16x2toU32(PTORESET,PTODELAY);
		EEPROM_RAM_REG[87] = U16x1U8x2toU32(CUDEPTHRESHOLD,CUDEPDELAY,LOADDETECTRETRYDELAY);
		EEPROM_RAM_REG[88] = U16x2toU32(SOVTHRESHOLD,SUVTHRESHOLD);
		EEPROM_RAM_REG[89] = U16x1U8x2toU32(TOSSTHRESHOLD,SOVDELAY,SUVDELAY);
		EEPROM_RAM_REG[90] = U16x1U8x2toU32(SOCCTHRESHOLD,SOCCDELAY,TOSSDELAY);
		EEPROM_RAM_REG[91] = U16x1U8x2toU32(SOCDTHRESHOLD,SOTTHRESHOLD,SOCDDELAY);
		EEPROM_RAM_REG[92] = U8x4toU32(VIMRDELAY,SOTFDELAY,SOTFTHRESHOLD,SOTDELAY);
		EEPROM_RAM_REG[93] = U16x2toU32(VIMRMAXRELAXCURRENT,VIMRCHECKVOLTAGE);
		EEPROM_RAM_REG[94] = U16x2toU32(VIMRRELAXMINDURATION,VIMRTHRESHOLD);
		EEPROM_RAM_REG[95] = U16x2toU32(VIMAMINACTIVECURRENT,VIMACHECKVOLTAGE);
		EEPROM_RAM_REG[96] = U16x2toU32(CFETFOFFTHRESHOLD,VIMATHRESHOLD);
		EEPROM_RAM_REG[97] = U16x1U8x2toU32(DFETFOFFTHRESHOLD,CFETFOFFDELAY,VIMADELAY);
		EEPROM_RAM_REG[98] = U16x1U8x2toU32(VSSFFAILTHRESHOLD,VSSFDELAY,DFETFOFFDELAY);
		EEPROM_RAM_REG[99] = U8x4toU32(SECURITYSETTINGS,HWMXDELAY,LFOFDELAY,PF2LVLDELAY);
		EEPROM_RAM_REG[100] = U16x2toU32(UNSEALKEYSTEP2,UNSEALKEYSTEP1);
		EEPROM_RAM_REG[101] = U16x2toU32(FULLACCESSKEYSTEP2,FULLACCESSKEYSTEP1);

//All voltages set to 0 initially
		/*
		EEPROM_RAM_REG[497]=0x0;
		EEPROM_RAM_REG[498]=0x0;
		EEPROM_RAM_REG[499]=0x0;
		EEPROM_RAM_REG[500]=0x0;
		EEPROM_RAM_REG[501]=0x0;
		EEPROM_RAM_REG[502]=0x0;
		EEPROM_RAM_REG[503]=0x0;
		EEPROM_RAM_REG[504]=0x0;
		EEPROM_RAM_REG[505]=0x0;

		EEPROM_RAM_REG[506]=0x0;*/   //Time is set to 0 initially

		EEPROM_RAM_REG[507]=0x1;
		EEPROM_RAM_REG[508]=0x1;
		EEPROM_RAM_REG[509]=0x65D; //CAN RESPONSE ID (Transmission)
		EEPROM_RAM_REG[510]=0x1AA; // CAN REQUEST ID (Reception)
		//EEPROM_RAM_REG[510]=0x1AB;
		EEPROM_RAM_REG[511]=EEPROM_TEST_BYTE; // FLASH STATUS

		save_flash_reg();

	}

}

/*Modified Portion*/


//Updating the COV,CUV,OCC,OCD1 counts in flash memory
/*
void update_faults1_in_flash(uint8_t fault1, uint8_t fault2, uint8_t fault3, uint8_t fault4)
{
	EEPROM_RAM_REG[501]=U8x4toU32(fault1,fault2,fault3,fault4);
	Flash_Write_Data (0x0800FFD4, EEPROM_RAM_REG+501, 1);
}

//Update the OTD,OTC,UTD,UTC counts in flash memory
void update_faults2_in_flash(uint8_t fault1, uint8_t fault2, uint8_t fault3, uint8_t fault4)
{
	EEPROM_RAM_REG[500]=U8x4toU32(fault1,fault2,fault3,fault4);
	Flash_Write_Data (0x0800FFD0, EEPROM_RAM_REG+500, 1);
}*/

//Update fault voltages in flash

/*
void update_voltages_in_flash(uint8_t count,uint16_t v2,uint16_t v1)
{
//	EEPROM_RAM_REG[505]=U16x2toU32(v2,v1);
//	Flash_Write_Data(0x0800FFE4,EEPROM_RAM_REG+505,1);
//	Flash_Read_Data (0x0800FFE4, &SNAPSHOT2, 1);


	EEPROM_RAM_REG[505-count]=U16x2toU32(v2,v1);


}*/

//Updating the date and time value in flash memory

/*
void time_in_flash()
{
		HAL_RTC_GetTime(&hrtc, &Present_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &Present_Date, RTC_FORMAT_BIN);



		EEPROM_RAM_REG[506]=U8x4toU32(Present_Date.WeekDay,Present_Time.Hours,Present_Time.Minutes,Present_Time.Seconds);

		EEPROM_RAM_REG[497]=U8x4toU32(Present_Date.Date,Present_Date.Month,0x14,Present_Date.Year);


}*/

/*
void update_in_flash()
{
	  Flash_Write_Data(0x0800FFC8,EEPROM_RAM_REG+497,10);
}*/


/*
void read_snapshot_from_flash()
{

	Flash_Read_Data (0x0800FFC8,&SNAPSHOT1,1);

}*/



void FLASH_COMMAND(uint8_t message_type,uint8_t data_b1,uint8_t data_b2,uint8_t data_b3,uint8_t data_b4,uint8_t data_chk)
{
	uint8_t test_data = data_b1 + data_b2 + data_b3 + data_b4;
	uint32_t temp_reg = 0;
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	if(data_chk == test_data)
	{
		switch (message_type)
		{
			case 0x00: //cell1gain
			{
				temp_reg = EEPROM_RAM_REG[0];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[0] = temp_reg;
				break;
			}
			case 0x01://cell2gain
			{
				temp_reg = EEPROM_RAM_REG[0];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[0] = temp_reg;
				break;
			}
			case 0x02://cell3gain
			{
				temp_reg = EEPROM_RAM_REG[1];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[1] = temp_reg;
				break;
			}
			case 0x03://cell4gain
			{
				temp_reg = EEPROM_RAM_REG[1];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[1] = temp_reg;
				break;
			}
			case 0x04://cell5gain
			{
				temp_reg = EEPROM_RAM_REG[2];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[2] = temp_reg;
				break;
			}
			case 0x05://cell6gain
			{
				temp_reg = EEPROM_RAM_REG[2];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[2] = temp_reg;
				break;
			}
			case 0x06://cell7gain
			{
				temp_reg = EEPROM_RAM_REG[3];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[3] = temp_reg;
				break;
			}
			case 0x07://cell8gain
			{
				temp_reg = EEPROM_RAM_REG[3];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[3] = temp_reg;
				break;
			}
			case 0x08://cell9gain
			{
				temp_reg = EEPROM_RAM_REG[4];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[4] = temp_reg;
				break;
			}
			case 0x09://cell10gain
			{
				temp_reg = EEPROM_RAM_REG[4];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[4] = temp_reg;
				break;
			}
			case 0x0A://cell11gain
			{
				temp_reg = EEPROM_RAM_REG[5];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[5] = temp_reg;
				break;
			}
			case 0x0B://cell12gain
			{
				temp_reg = EEPROM_RAM_REG[5];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[5] = temp_reg;
				break;
			}
			case 0x0C://cell13gain
			{
				temp_reg = EEPROM_RAM_REG[6];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[6] = temp_reg;
				break;
			}
			case 0x0D://cell14gain
			{
				temp_reg = EEPROM_RAM_REG[6];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[6] = temp_reg;
				break;
			}
			case 0x0E://cell15gain
			{
				temp_reg = EEPROM_RAM_REG[7];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[7] = temp_reg;
				break;
			}
			case 0x0F://cell16gain
			{
				temp_reg = EEPROM_RAM_REG[7];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[7] = temp_reg;
				break;
			}
			case 0x10://packgain
			{
				temp_reg = EEPROM_RAM_REG[8];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[8] = temp_reg;
				break;
			}
			case 0x11://tosgain
			{
				temp_reg = EEPROM_RAM_REG[8];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[8] = temp_reg;
				break;
			}
			case 0x12://adcgain
			{
				temp_reg = EEPROM_RAM_REG[9];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[9] = temp_reg;
				break;
			}
			case 0x13://ldgain
			{
				temp_reg = EEPROM_RAM_REG[9];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[9] = temp_reg;
				break;
			}
			case 0x14://ccgain
			{
				temp_reg = data_b1<<24|data_b2<<16|data_b3<<8|data_b4;
				EEPROM_RAM_REG[10] = temp_reg;
				break;
			}
			case 0x15://capacitygain
			{
				temp_reg = data_b1<<24|data_b2<<16|data_b3<<8|data_b4;
				EEPROM_RAM_REG[11] = temp_reg;
				break;
			}
			case 0x16://vcellmode
			{
				temp_reg = EEPROM_RAM_REG[41];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[41] = temp_reg;
				break;
			}
			case 0x17://mincelltemp
			{
				temp_reg = EEPROM_RAM_REG[60];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[60] = temp_reg;
				break;
			}
			case 0x18://maxcelltemp
			{
				temp_reg = EEPROM_RAM_REG[60];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[60] = temp_reg;
				break;
			}
			case 0x19://maxinternaltemp
			{
				temp_reg = EEPROM_RAM_REG[60];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[60] = temp_reg;
				break;
			}
			case 0x1A://cellbalanceinterval
			{
				temp_reg = EEPROM_RAM_REG[60];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[60] = temp_reg;
				break;
			}
			case 0x1B://cellbalancemincellvcharge
			{
				temp_reg = EEPROM_RAM_REG[61];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[61] = temp_reg;
				break;
			}
			case 0x1C://cellbalancemindeltacharge
			{
				temp_reg = EEPROM_RAM_REG[61];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[61] = temp_reg;
				break;
			}
			case 0x1D://cellbalancestopdeltacharge
			{
				temp_reg = EEPROM_RAM_REG[62];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[62] = temp_reg;
				break;
			}
			case 0x1E://cellbalancemincellvrelax
			{
				temp_reg = EEPROM_RAM_REG[62];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[62] = temp_reg;
				break;
			}
			case 0x1F://celbalancemindeltarelax
			{
				temp_reg = EEPROM_RAM_REG[62];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[62] = temp_reg;
				break;
			}
			case 0x20://cellbalancestopdeltarelax
			{
				temp_reg = EEPROM_RAM_REG[64];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[64] = temp_reg;
				break;
			}
			case 0x21://cuvthreshold
			{
				temp_reg = EEPROM_RAM_REG[69];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[69] = temp_reg;
				break;
			}
			case 0x22://cuvdelay
			{
				temp_reg = EEPROM_RAM_REG[69];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[69] = temp_reg;
				break;
			}
			case 0x23://cuvrecoveryhysteresis
			{
				temp_reg = EEPROM_RAM_REG[69];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[69] = temp_reg;
				break;
			}
			case 0x24://covthreshold
			{
				temp_reg = EEPROM_RAM_REG[70];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[70] = temp_reg;
				break;
			}
			case 0x25://covdelay
			{
				temp_reg = EEPROM_RAM_REG[70];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[70] = temp_reg;
				break;
			}
			case 0x26://covrecoveryhysteresis
			{
				temp_reg = EEPROM_RAM_REG[70];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[70] = temp_reg;
				break;
			}
			case 0x27://covllatchlimit
			{
				temp_reg = EEPROM_RAM_REG[71];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[71] = temp_reg;
				break;
			}
			case 0x28://covlcounterdecdelay
			{
				temp_reg = EEPROM_RAM_REG[71];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[71] = temp_reg;
				break;
			}
			case 0x29://covlrecoverytime
			{
				temp_reg = EEPROM_RAM_REG[71];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[71] = temp_reg;
				break;
			}
			case 0x2A://occthreshold
			{
				temp_reg = EEPROM_RAM_REG[71];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[71] = temp_reg;
				break;
			}
			case 0x2B://occdelay
			{
				temp_reg = EEPROM_RAM_REG[73];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[73] = temp_reg;
				break;
			}
			case 0x2C://occrecoverythreshold
			{
				temp_reg = EEPROM_RAM_REG[72];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[72] = temp_reg;
				break;
			}
			case 0x2D://occpacktosdelta
			{
				temp_reg = EEPROM_RAM_REG[72];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[72] = temp_reg;
				break;
			}
			case 0x2E://ocd1threshold
			{
				temp_reg = EEPROM_RAM_REG[73];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[73] = temp_reg;
				break;
			}
			case 0x2F://ocd1delay
			{
				temp_reg = EEPROM_RAM_REG[73];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[73] = temp_reg;
				break;
			}
			case 0x30://ocd2threshold
			{
				temp_reg = EEPROM_RAM_REG[73];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[73] = temp_reg;
				break;
			}
			case 0x31://ocd2delay
			{
				temp_reg = EEPROM_RAM_REG[74];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x32://scdthreshold
			{
				temp_reg = EEPROM_RAM_REG[74];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x33://scddelay
			{
				temp_reg = EEPROM_RAM_REG[74];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x34://scdrecoverytime
			{
				temp_reg = EEPROM_RAM_REG[74];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x35://ocd3threshold
			{
				temp_reg = EEPROM_RAM_REG[75];
				temp_reg = (temp_reg & 0xFFFF0000) | U8x2toU16(data_b1,data_b2);
				EEPROM_RAM_REG[75] = temp_reg;
				break;
			}
			case 0x36://ocd3delay
			{
				temp_reg = EEPROM_RAM_REG[76];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[76] = temp_reg;
				break;
			}
			case 0x37://ocdrecoverythreshold
			{
				temp_reg = EEPROM_RAM_REG[75];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[75] = temp_reg;
				break;
			}
			case 0x38://ocdlatchlimit
			{
				temp_reg = EEPROM_RAM_REG[76];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x39://ocdlcounterdecdelay
			{
				temp_reg = EEPROM_RAM_REG[76];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x3A://ocdlrecoverytime
			{
				temp_reg = EEPROM_RAM_REG[76];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[74] = temp_reg;
				break;
			}
			case 0x3B://ocdlrecoverythreshold
			{
				temp_reg = EEPROM_RAM_REG[77];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[77] = temp_reg;
				break;
			}
			case 0x3C://scdllatchlimit
			{
				temp_reg = EEPROM_RAM_REG[77];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[77] = temp_reg;
				break;
			}
			case 0x3D://scdlcounterdecdelay
			{
				temp_reg = EEPROM_RAM_REG[77];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[77] = temp_reg;
				break;
			}
			case 0x3E://scdlrecoverytime
			{
				temp_reg = EEPROM_RAM_REG[78];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[78] = temp_reg;
				break;
			}
			case 0x3F://scdlrecoverythreshold
			{
				temp_reg = EEPROM_RAM_REG[78];
				temp_reg = (temp_reg & 0x0000FFFF)| (U8x2toU16(data_b1,data_b2)<<16);
				EEPROM_RAM_REG[78] = temp_reg;
				break;
			}
			case 0x40://otcthreshold
			{
				temp_reg = EEPROM_RAM_REG[78];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[78] = temp_reg;
				break;
			}
			case 0x41://otcdelay
			{
				temp_reg = EEPROM_RAM_REG[79];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[79] = temp_reg;
				break;
			}
			case 0x42://otcrecovery
			{
				temp_reg = EEPROM_RAM_REG[79];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[79] = temp_reg;
				break;
			}
			case 0x43://otdthreshold
			{
				temp_reg = EEPROM_RAM_REG[79];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[79] = temp_reg;
				break;
			}
			case 0x44://otddelay
			{
				temp_reg = EEPROM_RAM_REG[79];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[79] = temp_reg;
				break;
			}
			case 0x45://otdrecovery
			{
				temp_reg = EEPROM_RAM_REG[80];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[80] = temp_reg;
				break;
			}
			case 0x46://otfthreshold
			{
				temp_reg = EEPROM_RAM_REG[80];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[80] = temp_reg;
				break;
			}
			case 0x47://otfdelay
			{
				temp_reg = EEPROM_RAM_REG[80];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[80] = temp_reg;
				break;
			}
			case 0x48://otfrecovery
			{
				temp_reg = EEPROM_RAM_REG[80];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[80] = temp_reg;
				break;
			}
			case 0x49://otintthreshold
			{
				temp_reg = EEPROM_RAM_REG[81];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[81] = temp_reg;
				break;
			}
			case 0x4A://otintdelay
			{
				temp_reg = EEPROM_RAM_REG[81];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[81] = temp_reg;
				break;
			}
			case 0x4B://otintrecovery
			{
				temp_reg = EEPROM_RAM_REG[81];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[81] = temp_reg;
				break;
			}
			case 0x4C://utcthreshold
			{
				temp_reg = EEPROM_RAM_REG[81];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[81] = temp_reg;
				break;
			}
			case 0x4D://utcdelay
			{
				temp_reg = EEPROM_RAM_REG[82];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[82] = temp_reg;
				break;
			}
			case 0x4E://utcrecovery
			{
				temp_reg = EEPROM_RAM_REG[82];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[82] = temp_reg;
				break;
			}
			case 0x4F://utdthreshold
			{
				temp_reg = EEPROM_RAM_REG[82];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[82] = temp_reg;
				break;
			}
			case 0x50://utddelay
			{
				temp_reg = EEPROM_RAM_REG[82];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[82] = temp_reg;
				break;
			}
			case 0x51://utdrecovery
			{
				temp_reg = EEPROM_RAM_REG[83];
				temp_reg = (temp_reg & 0xFFFFFF00)| (data_b1);
				EEPROM_RAM_REG[83] = temp_reg;
				break;
			}
			case 0x52://utintthreshold
			{
				temp_reg = EEPROM_RAM_REG[83];
				temp_reg = (temp_reg & 0xFFFF00FF)| (data_b1<<8);
				EEPROM_RAM_REG[83] = temp_reg;
				break;
			}
			case 0x53://utintdelay
			{
				temp_reg = EEPROM_RAM_REG[83];
				temp_reg = (temp_reg & 0xFF00FFFF)| (data_b1<<16);
				EEPROM_RAM_REG[83] = temp_reg;
				break;
			}
			case 0x54://utintrecovery
			{
				temp_reg = EEPROM_RAM_REG[83];
				temp_reg = (temp_reg & 0x00FFFFFF)| (data_b1<<24);
				EEPROM_RAM_REG[83] = temp_reg;
				break;
			}
			case 0xE0: //update time
			{
				sTime.Hours = data_b1;
				sTime.Minutes = data_b2;
				sTime.Seconds = data_b3;
				sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
				sTime.StoreOperation = RTC_STOREOPERATION_RESET;
				if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
				{
				    Error_Handler();
				}
				break;
			}
			case 0xE1:
			{
				 sDate.WeekDay = data_b1;
				 sDate.Month = data_b2;
				 sDate.Date = data_b3;
				 sDate.Year = data_b4;
				 if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
				 {
				   Error_Handler();
				 }
				break;
			}
			case 0xF0: //config update
			{
				update_config = 1;
				break;
			}
			case 0xF1: //config save to eeprom
			{
				flash_save_config_set();
				break;
			}
			case 0xF2://dump eeprom array

			{
				flash_dump = 1;
				break;
			}

			default:
			{
				break;
			}
		}
	}
}
uint8_t flash_dump_status()
{
	return flash_dump;
}
uint8_t flash_update_config_status()
{
	return update_config;
}
uint8_t flash_save_config_status()
{
	return save_config;
}

void flash_dump_reset()
{
	flash_dump = 0;
}
void flash_update_config_reset()
{
	update_config = 0;
}
void flash_save_config_reset()
{
	save_config = 0;
}
void flash_save_config_set()
{
	save_config = 1;
}

#endif /* SRC_FLASH_072C8_C_ */
