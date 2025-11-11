/*
 * bq76952.c
 *
 *  Created on: Aug 13, 2022
 *      Author: Rohit N Ghosh
 */


#include "bq76952.h"
#include "main.h"
#include "BQ76952_default_config.h"
#include "flash_072c8.h"

extern I2C_HandleTypeDef hi2c1;
extern uint32_t EEPROM_RAM_REG[512];

/*Configuration storage registers*/

volatile int16_t cell1gain = CELL1GAIN;
volatile int16_t cell2gain = CELL2GAIN;
volatile int16_t cell3gain = CELL3GAIN;
volatile int16_t cell4gain = CELL4GAIN;
volatile int16_t cell5gain = CELL5GAIN;
volatile int16_t cell6gain = CELL6GAIN;
volatile int16_t cell7gain = CELL7GAIN;
volatile int16_t cell8gain = CELL8GAIN;
volatile int16_t cell9gain = CELL9GAIN;
volatile int16_t cell10gain = CELL10GAIN;
volatile int16_t cell11gain = CELL11GAIN;
volatile int16_t cell12gain = CELL12GAIN;
volatile int16_t cell13gain = CELL13GAIN;
volatile int16_t cell14gain = CELL14GAIN;
volatile int16_t cell15gain = CELL15GAIN;
volatile int16_t cell16gain = CELL16GAIN;
volatile uint16_t packgain = PACKGAIN;
volatile uint16_t tosgain = TOSGAIN;
volatile uint16_t ldgain = LDGAIN;
volatile int16_t adcgain = ADCGAIN;
volatile float ccgain = CCGAIN;
volatile float capacitygain = CAPACITYGAIN;
volatile int16_t vcelloffset = VCELLOFFSET;
volatile int16_t vdivoffset = VDIVOFFSET;
volatile uint16_t coulombcounteroffsetsamples = COULOMBCOUNTEROFFSETSAMPLES;
volatile int16_t boardoffset = BOARDOFFSET;
volatile int8_t internaltempoffset = INTERNALTEMPOFFSET;
volatile int8_t cfetofftempoffset = CFETOFFTEMPOFFSET;
volatile int8_t dfetofftempoffset = DFETOFFTEMPOFFSET;
volatile int8_t alerttempoffset = ALERTTEMPOFFSET;
volatile int8_t ts1tempoffset = TS1TEMPOFFSET;
volatile int8_t ts2tempoffset = TS2TEMPOFFSET;
volatile int8_t ts3tempoffset = TS3TEMPOFFSET;
volatile int8_t hdqtempoffset = HDQTEMPOFFSET;
volatile int8_t dchgtempoffset = DCHGTEMPOFFSET;
volatile int8_t ddsgtempoffset = DDSGTEMPOFFSET;
volatile int16_t intgain = INTGAIN;
volatile int16_t intbaseoffset = INTBASEOFFSET;
volatile int16_t intmaximumad = INTMAXIMUMAD;
volatile int16_t intmaximumtemp = INTMAXIMUMTEMP;
volatile int16_t t18kcoeffa1 = T18KCOEFFA1;
volatile int16_t t18kcoeffa2 = T18KCOEFFA2;
volatile int16_t t18kcoeffa3 = T18KCOEFFA3;
volatile int16_t t18kcoeffa4 = T18KCOEFFA4;
volatile int16_t t18kcoeffa5 = T18KCOEFFA5;
volatile int16_t t18kcoeffb1 = T18KCOEFFB1;
volatile int16_t t18kcoeffb2 = T18KCOEFFB2;
volatile int16_t t18kcoeffb3 = T18KCOEFFB3;
volatile int16_t t18kcoeffb4 = T18KCOEFFB4;
volatile int16_t t18kadc0 = T18KADC0;
volatile int16_t t180kcoeffa1 = T180KCOEFFA1;
volatile int16_t t180kcoeffa2 = T180KCOEFFA2;
volatile int16_t t180kcoeffa3 = T180KCOEFFA3;
volatile int16_t t180kcoeffa4 = T180KCOEFFA4;
volatile int16_t t180kcoeffa5 = T180KCOEFFA5;
volatile int16_t t180kcoeffb1 = T180KCOEFFB1;
volatile int16_t t180kcoeffb2 = T180KCOEFFB2;
volatile int16_t t180kcoeffb3 = T180KCOEFFB3;
volatile int16_t t180kcoeffb4 = T180KCOEFFB4;
volatile int16_t t180kadc0 = T180KADC0;
volatile int16_t customcoeffa1 = CUSTOMCOEFFA1;
volatile int16_t customcoeffa2 = CUSTOMCOEFFA2;
volatile int16_t customcoeffa3 = CUSTOMCOEFFA3;
volatile int16_t customcoeffa4 = CUSTOMCOEFFA4;
volatile int16_t customcoeffa5 = CUSTOMCOEFFA5;
volatile int16_t customcoeffb1 = CUSTOMCOEFFB1;
volatile int16_t customcoeffb2 = CUSTOMCOEFFB2;
volatile int16_t customcoeffb3 = CUSTOMCOEFFB3;
volatile int16_t customcoeffb4 = CUSTOMCOEFFB4;
volatile int16_t customrc0 = CUSTOMRC0;
volatile int16_t customadc0 = CUSTOMADC0;
volatile uint8_t coulombcounterdeadband = COULOMBCOUNTERDEADBAND;
volatile uint16_t cuvthresholdoverride = CUVTHRESHOLDOVERRIDE;
volatile uint16_t covthresholdoverride = COVTHRESHOLDOVERRIDE;
volatile int16_t minblowfusevoltage = MINBLOWFUSEVOLTAGE;
volatile uint8_t fuseblowtimeout = FUSEBLOWTIMEOUT;
volatile uint16_t powerconfig = POWERCONFIG;
volatile uint16_t powerconfig_nosleep = POWERCONFIG_NOSLEEP;
volatile uint8_t reg12config = REG12CONFIG;
volatile uint8_t reg0config = REG0CONFIG;
volatile uint8_t hwdregulatoroptions = HWDREGULATOROPTIONS;
volatile uint8_t commtype = COMMTYPE;
volatile uint8_t i2caddress = I2CADDRESS;
volatile uint8_t spiconfiguration = SPICONFIGURATION;
volatile uint8_t commidletime = COMMIDLETIME;
volatile uint8_t cfetoffpinconfig = CFETOFFPINCONFIG;
volatile uint8_t dfetoffpinconfig = DFETOFFPINCONFIG;
volatile uint8_t alertpinconfig = ALERTPINCONFIG;
volatile uint8_t ts1config = TS1CONFIG;
volatile uint8_t ts2config = TS2CONFIG;
volatile uint8_t ts3config = TS3CONFIG;
volatile uint8_t hdqpinconfig = HDQPINCONFIG;
volatile uint8_t dchgpinconfig = DCHGPINCONFIG;
volatile uint8_t ddsgpinconfig = DDSGPINCONFIG;
volatile uint8_t daconfiguration = DACONFIGURATION;
volatile uint16_t vcellmode = VCELLMODE;
volatile uint8_t cc3samples = CC3SAMPLES;
volatile uint16_t protectionconfiguration = PROTECTIONCONFIGURATION;
volatile uint8_t enabledprotectionsa = ENABLEDPROTECTIONSA;
volatile uint8_t enabledprotectionsb = ENABLEDPROTECTIONSB;
volatile uint8_t enabledprotectionsc = ENABLEDPROTECTIONSC;
volatile uint8_t chgfetprotectionsa = CHGFETPROTECTIONSA;
volatile uint8_t chgfetprotectionsb = CHGFETPROTECTIONSB;
volatile uint8_t chgfetprotectionsc = CHGFETPROTECTIONSC;
volatile uint8_t dsgfetprotectionsa = DSGFETPROTECTIONSA;
volatile uint8_t dsgfetprotectionsb = DSGFETPROTECTIONSB;
volatile uint8_t dsgfetprotectionsc = DSGFETPROTECTIONSC;
volatile int16_t bodydiodethreshold = BODYDIODETHRESHOLD;
volatile uint16_t defaultalarmmask = DEFAULTALARMMASK;
volatile uint8_t sfalertmaska = SFALERTMASKA;
volatile uint8_t sfalertmaskb = SFALERTMASKB;
volatile uint8_t sfalertmaskc = SFALERTMASKC;
volatile uint8_t pfalertmaska = PFALERTMASKA;
volatile uint8_t pfalertmaskb = PFALERTMASKB;
volatile uint8_t pfalertmaskc = PFALERTMASKC;
volatile uint8_t pfalertmaskd = PFALERTMASKD;
volatile uint8_t enabledpfa = ENABLEDPFA;
volatile uint8_t enabledpfb = ENABLEDPFB;
volatile uint8_t enabledpfc = ENABLEDPFC;
volatile uint8_t enabledpfd = ENABLEDPFD;
volatile uint8_t fetoptions = FETOPTIONS;
volatile uint8_t chgpumpcontrol = CHGPUMPCONTROL;
volatile int16_t prechargestartvoltage = PRECHARGESTARTVOLTAGE;
volatile int16_t prechargestopvoltage = PRECHARGESTOPVOLTAGE;
volatile uint8_t predischargetimeout = PREDISCHARGETIMEOUT;
volatile uint8_t predischargestopdelta = PREDISCHARGESTOPDELTA;
volatile int16_t dsgcurrentthreshold = DSGCURRENTTHRESHOLD;
volatile int16_t chgcurrentthreshold = CHGCURRENTTHRESHOLD;
volatile uint8_t checktime = CHECKTIME;
volatile int16_t cell1interconnect = CELL1INTERCONNECT;
volatile int16_t cell2interconnect = CELL2INTERCONNECT;
volatile int16_t cell3interconnect = CELL3INTERCONNECT;
volatile int16_t cell4interconnect = CELL4INTERCONNECT;
volatile int16_t cell5interconnect = CELL5INTERCONNECT;
volatile int16_t cell6interconnect = CELL6INTERCONNECT;
volatile int16_t cell7interconnect = CELL7INTERCONNECT;
volatile int16_t cell8interconnect = CELL8INTERCONNECT;
volatile int16_t cell9interconnect = CELL9INTERCONNECT;
volatile int16_t cell10interconnect = CELL10INTERCONNECT;
volatile int16_t cell11interconnect = CELL11INTERCONNECT;
volatile int16_t cell12interconnect = CELL12INTERCONNECT;
volatile int16_t cell13interconnect = CELL13INTERCONNECT;
volatile int16_t cell14interconnect = CELL14INTERCONNECT;
volatile int16_t cell15interconnect = CELL15INTERCONNECT;
volatile int16_t cell16interconnect = CELL16INTERCONNECT;
volatile uint16_t mfgstatusinit = MFGSTATUSINIT;
volatile uint8_t balancingconfiguration = BALANCINGCONFIGURATION;
volatile int8_t mincelltemp = MINCELLTEMP;
volatile int8_t maxcelltemp = MAXCELLTEMP;
volatile int8_t maxinternaltemp = MAXINTERNALTEMP;
volatile uint8_t cellbalanceinterval = CELLBALANCEINTERVAL;
volatile uint8_t cellbalancemaxcells = CELLBALANCEMAXCELLS;
volatile int16_t cellbalancemincellvcharge = CELLBALANCEMINCELLVCHARGE;
volatile uint8_t cellbalancemindeltacharge = CELLBALANCEMINDELTACHARGE;
volatile uint8_t cellbalancestopdeltacharge = CELLBALANCESTOPDELTACHARGE;
volatile int16_t cellbalancemincellvrelax = CELLBALANCEMINCELLVRELAX;
volatile uint8_t cellbalancemindeltarelax = CELLBALANCEMINDELTARELAX;
volatile uint8_t cellbalancestopdeltarelax = CELLBALANCESTOPDELTARELAX;
volatile int16_t shutdowncellvoltage = SHUTDOWNCELLVOLTAGE;
volatile int16_t shutdownstackvoltage = SHUTDOWNSTACKVOLTAGE;
volatile uint8_t lowvshutdowndelay = LOWVSHUTDOWNDELAY;
volatile uint8_t shutdowntemperature = SHUTDOWNTEMPERATURE;
volatile uint8_t shutdowntemperaturedelay = SHUTDOWNTEMPERATUREDELAY;
volatile uint8_t fetoffdelay = FETOFFDELAY;
volatile uint8_t shutdowncommanddelay = SHUTDOWNCOMMANDDELAY;
volatile uint8_t autoshutdowntime = AUTOSHUTDOWNTIME;
volatile uint8_t ramfailshutdowntime = RAMFAILSHUTDOWNTIME;
volatile int16_t sleepcurrent = SLEEPCURRENT;
volatile uint8_t voltagetime = VOLTAGETIME;
volatile int16_t wakecomparatorcurrent = WAKECOMPARATORCURRENT;
volatile uint8_t sleephysteresistime = SLEEPHYSTERESISTIME;
volatile int16_t sleepchargervoltagethreshold = SLEEPCHARGERVOLTAGETHRESHOLD;
volatile int16_t sleepchargerpacktosdelta = SLEEPCHARGERPACKTOSDELTA;
volatile uint16_t configramsignature = CONFIGRAMSIGNATURE;
volatile uint8_t cuvthreshold = CUVTHRESHOLD;
volatile uint16_t cuvdelay = CUVDELAY;
volatile uint8_t cuvrecoveryhysteresis = CUVRECOVERYHYSTERESIS;
volatile uint8_t covthreshold = COVTHRESHOLD;
volatile uint16_t covdelay = COVDELAY;
volatile uint8_t covrecoveryhysteresis = COVRECOVERYHYSTERESIS;
volatile uint8_t covllatchlimit = COVLLATCHLIMIT;
volatile uint8_t covlcounterdecdelay = COVLCOUNTERDECDELAY;
volatile uint8_t covlrecoverytime = COVLRECOVERYTIME;
volatile uint8_t occthreshold = OCCTHRESHOLD;
volatile uint8_t occdelay = OCCDELAY;
volatile int16_t occrecoverythreshold = OCCRECOVERYTHRESHOLD;
volatile int16_t occpacktosdelta = OCCPACKTOSDELTA;
volatile uint8_t ocd1threshold = OCD1THRESHOLD;
volatile uint8_t ocd1delay = OCD1DELAY;
volatile uint8_t ocd2threshold = OCD2THRESHOLD;
volatile uint8_t ocd2delay = OCD2DELAY;
volatile uint8_t scdthreshold = SCDTHRESHOLD;
volatile uint8_t scddelay = SCDDELAY;
volatile uint8_t scdrecoverytime = SCDRECOVERYTIME;
volatile int16_t ocd3threshold = OCD3THRESHOLD;
volatile uint8_t ocd3delay = OCD3DELAY;
volatile int16_t ocdrecoverythreshold = OCDRECOVERYTHRESHOLD;
volatile uint8_t ocdllatchlimit = OCDLLATCHLIMIT;
volatile uint8_t ocdlcounterdecdelay = OCDLCOUNTERDECDELAY;
volatile uint8_t ocdlrecoverytime = OCDLRECOVERYTIME;
volatile int16_t ocdlrecoverythreshold = OCDLRECOVERYTHRESHOLD;
volatile uint8_t scdllatchlimit = SCDLLATCHLIMIT;
volatile uint8_t scdlcounterdecdelay = SCDLCOUNTERDECDELAY;
volatile uint8_t scdlrecoverytime = SCDLRECOVERYTIME;
volatile int16_t scdlrecoverythreshold = SCDLRECOVERYTHRESHOLD;
volatile int8_t otcthreshold = OTCTHRESHOLD;
volatile uint8_t otcdelay = OTCDELAY;
volatile int8_t otcrecovery = OTCRECOVERY;
volatile int8_t otdthreshold = OTDTHRESHOLD;
volatile uint8_t otddelay = OTDDELAY;
volatile int8_t otdrecovery = OTDRECOVERY;
volatile uint8_t otfthreshold = OTFTHRESHOLD;
volatile uint8_t otfdelay = OTFDELAY;
volatile uint8_t otfrecovery = OTFRECOVERY;
volatile int8_t otintthreshold = OTINTTHRESHOLD;
volatile uint8_t otintdelay = OTINTDELAY;
volatile int8_t otintrecovery = OTINTRECOVERY;
volatile int8_t utcthreshold = UTCTHRESHOLD;
volatile uint8_t utcdelay = UTCDELAY;
volatile int8_t utcrecovery = UTCRECOVERY;
volatile int8_t utdthreshold = UTDTHRESHOLD;
volatile uint8_t utddelay = UTDDELAY;
volatile int8_t utdrecovery = UTDRECOVERY;
volatile int8_t utintthreshold = UTINTTHRESHOLD;
volatile uint8_t utintdelay = UTINTDELAY;
volatile int8_t utintrecovery = UTINTRECOVERY;
volatile uint8_t protectionsrecoverytime = PROTECTIONSRECOVERYTIME;
volatile uint16_t hwddelay = HWDDELAY;
volatile uint8_t loaddetectactivetime = LOADDETECTACTIVETIME;
volatile uint8_t loaddetectretrydelay = LOADDETECTRETRYDELAY;
volatile uint16_t loaddetecttimeout = LOADDETECTTIMEOUT;
volatile int16_t ptochargethreshold = PTOCHARGETHRESHOLD;
volatile uint16_t ptodelay = PTODELAY;
volatile int16_t ptoreset = PTORESET;
volatile int16_t cudepthreshold = CUDEPTHRESHOLD;
volatile uint8_t cudepdelay = CUDEPDELAY;
volatile int16_t suvthreshold = SUVTHRESHOLD;
volatile uint8_t suvdelay = SUVDELAY;
volatile int16_t sovthreshold = SOVTHRESHOLD;
volatile uint8_t sovdelay = SOVDELAY;
volatile int16_t tossthreshold = TOSSTHRESHOLD;
volatile uint8_t tossdelay = TOSSDELAY;
volatile int16_t soccthreshold = SOCCTHRESHOLD;
volatile uint8_t soccdelay = SOCCDELAY;
volatile int16_t socdthreshold = SOCDTHRESHOLD;
volatile uint8_t socddelay = SOCDDELAY;
volatile int8_t sotthreshold = SOTTHRESHOLD;
volatile uint8_t sotdelay = SOTDELAY;
volatile uint8_t sotfthreshold = SOTFTHRESHOLD;
volatile uint8_t sotfdelay = SOTFDELAY;
volatile int16_t vimrcheckvoltage = VIMRCHECKVOLTAGE;
volatile int16_t vimrmaxrelaxcurrent = VIMRMAXRELAXCURRENT;
volatile int16_t vimrthreshold = VIMRTHRESHOLD;
volatile uint8_t vimrdelay = VIMRDELAY;
volatile uint16_t vimrrelaxminduration = VIMRRELAXMINDURATION;
volatile int16_t vimacheckvoltage = VIMACHECKVOLTAGE;
volatile int16_t vimaminactivecurrent = VIMAMINACTIVECURRENT;
volatile int16_t vimathreshold = VIMATHRESHOLD;
volatile uint8_t vimadelay = VIMADELAY;
volatile int16_t cfetfoffthreshold = CFETFOFFTHRESHOLD;
volatile uint8_t cfetfoffdelay = CFETFOFFDELAY;
volatile int16_t dfetfoffthreshold = DFETFOFFTHRESHOLD;
volatile uint8_t dfetfoffdelay = DFETFOFFDELAY;
volatile int16_t vssffailthreshold = VSSFFAILTHRESHOLD;
volatile uint8_t vssfdelay = VSSFDELAY;
volatile uint8_t pf2lvldelay = PF2LVLDELAY;
volatile uint8_t lfofdelay = LFOFDELAY;
volatile uint8_t hwmxdelay = HWMXDELAY;
volatile uint8_t securitysettings = SECURITYSETTINGS;
volatile uint16_t unsealkeystep1 = UNSEALKEYSTEP1;
volatile uint16_t unsealkeystep2 = UNSEALKEYSTEP2;
volatile uint16_t fullaccesskeystep1 = FULLACCESSKEYSTEP1;
volatile uint16_t fullaccesskeystep2 = FULLACCESSKEYSTEP2;


uint8_t RX_data [2] = {0x00, 0x00}; // used in several functions to store data read from BQ769x2

/*Modified Portion*/
/*
uint8_t CellVoltage1_temp;
uint8_t CellVoltage2_temp;
uint8_t CellVoltage3_temp;
uint8_t CellVoltage4_temp;
uint8_t CellVoltage5_temp;
uint8_t CellVoltage6_temp;
uint32_t Cell1_ADC_Count_V;
uint32_t Cell2_ADC_Count_V;
uint32_t Cell3_ADC_Count_V;
uint32_t Cell4_ADC_Count_V;*/


int CellV1_temp;
int CellV2_temp;

uint8_t RX_32Byte [32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//used in Subcommands read function
// Global Variables for cell voltages, temperatures, Stack voltage, PACK Pin voltage, LD Pin voltage, CC2 current

uint16_t CellVoltage [16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint16_t FaultCellVoltage [16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
float Temperature [9] = {0};
uint16_t Stack_Voltage = 0x00;
uint16_t Pack_Voltage = 0x00;
uint16_t LD_Voltage = 0x00;
int16_t Pack_CC2_Current = 0x00;
int16_t Pack_CC3_Current = 0x00;

/*Modified Portion*/
int16_t Pack_CC1_Current = 0x00;
//int16_t acc_charge = 0x00;

uint16_t AlarmBits = 0x00;
uint8_t value_SafetyStatusA;  // Safety Status Register A
uint8_t value_SafetyStatusB;  // Safety Status Register B
uint8_t value_SafetyStatusC;  // Safety Status Register C
uint8_t value_PFStatusA;   // Permanent Fail Status Register A
uint8_t value_PFStatusB;   // Permanent Fail Status Register B
uint8_t value_PFStatusC;   // Permanent Fail Status Register C

uint16_t bat_status = 0x00;
uint8_t FET_Status;  // FET Status register contents  - Shows states of FETs
uint16_t CB_ActiveCells;  // Cell Balancing Active Cells

uint8_t	UV_Fault = 0;   // under-voltage fault state
uint8_t	OV_Fault = 0;   // over-voltage fault state
uint8_t	SCD_Fault = 0;  // short-circuit fault state
uint8_t	OCD_Fault = 0;  // over-current fault state
uint8_t ProtectionsTriggered = 0; // Set to 1 if any protection triggers
uint8_t FaultProtectionsTriggered =0; // Set to 1 if any fault protection triggers

uint8_t LD_ON = 0;	// Load Detect status bit
uint8_t DSG = 0;   // discharge FET state
uint8_t CHG = 0;   // charge FET state
uint8_t PCHG = 0;  // pre-charge FET state
uint8_t PDSG = 0;  // pre-discharge FET state

int32_t AccumulatedCharge_Int; // in BQ769x2_READPASSQ func
int32_t AccumulatedCharge_Frac;// in BQ769x2_READPASSQ func
int32_t AccumulatedCharge_Time;// in BQ769x2_READPASSQ func

/*Modified Portion*/
//For extracting the accumulated charge in parts
uint8_t chg_lowestbyte;
uint8_t chg_lowbyte;
uint8_t chg_highbyte;
uint8_t chg_highestbyte;

uint16_t value_fetstatus = 0;

/*Fault Counts of SafetyStatusA Register*/
/*uint8_t COVFault_Count=0;
uint8_t CUVFault_Count=0;
uint8_t OCCFault_Count=0;
uint8_t OCD1Fault_Count=0;*/
//uint8_t SCDFault_Count=0;

/*Fault Counts of SafetyStatusB Register*/
/*uint8_t OTDFault_Count=0;
uint8_t OTCFault_Count=0;
uint8_t UTDFault_Count=0;
uint8_t UTCFault_Count=0;*/

/*Fault Counts of SafetyStatusC Register*/
//uint8_t SCLatch_Count=0;

/*Modified Portion*/
//extern RTC_HandleTypeDef hrtc;

//Variables for storing time and date
//RTC_TimeTypeDef Present_Time = { 0 };
//RTC_DateTypeDef Present_Date = { 0 };

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++)
    {
        dest[copyIndex] = source[copyIndex];
    }
}

unsigned char Checksum(unsigned char *ptr, unsigned char len)
// Calculates the checksum when writing to a RAM register. The checksum is the inverse of the sum of the bytes.
{
	unsigned char i;
	unsigned char checksum = 0;

	for(i=0; i<len; i++)
		checksum += ptr[i];

	checksum = 0xff & ~checksum;

	return(checksum);
}

unsigned char CRC8(unsigned char *ptr, unsigned char len)
//Calculates CRC8 for passed bytes. Used in i2c read and write functions
{
	unsigned char i;
	unsigned char crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= 0x107;
			}
			else
				crc *= 2;

			if((*ptr & i)!=0)
				crc ^= 0x107;
		}
		ptr++;
	}
	return(crc);
}

void I2C_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
	uint8_t TX_Buffer [MAX_BUFFER_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
#if CRC_Mode
	{
		uint8_t crc_count = 0;
		crc_count = count * 2;
		uint8_t crc1stByteBuffer [3] = {0x10, reg_addr, reg_data[0]};
		unsigned int j;
		unsigned int i;
		uint8_t temp_crc_buffer [3];

		TX_Buffer[0] = reg_data[0];
		TX_Buffer[1] = CRC8(crc1stByteBuffer,3);

		j = 2;
		for(i=1; i<count; i++)
		{
			TX_Buffer[j] = reg_data[i];
			j = j + 1;
			temp_crc_buffer[0] = reg_data[i];
			TX_Buffer[j] = CRC8(temp_crc_buffer,1);
			j = j + 1;
		}
		HAL_I2C_Mem_Write(&hi2c1, DEV_ADDR, reg_addr, 1, TX_Buffer, crc_count, 1000);
	}
#else
	HAL_I2C_Mem_Write(&hi2c1, DEV_ADDR, reg_addr, 1, reg_data, count, 1000);
#endif
}

int I2C_ReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
	unsigned int RX_CRC_Fail = 0;  // reset to 0. If in CRC Mode and CRC fails, this will be incremented.
	uint8_t RX_Buffer [MAX_BUFFER_SIZE] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
#if CRC_Mode
	{
		uint8_t crc_count = 0;
		uint8_t ReceiveBuffer [10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		crc_count = count * 2;
		unsigned int j;
		unsigned int i;
		unsigned char CRCc = 0;
		uint8_t temp_crc_buffer [3];

		HAL_I2C_Mem_Read(&hi2c1, DEV_ADDR, reg_addr, 1, ReceiveBuffer, crc_count, 1000);
		uint8_t crc1stByteBuffer [4] = {0x10, reg_addr, 0x11, ReceiveBuffer[0]};
		CRCc = CRC8(crc1stByteBuffer,4);
		if (CRCc != ReceiveBuffer[1])
		{
			RX_CRC_Fail += 1;
		}
		RX_Buffer[0] = ReceiveBuffer[0];

		j = 2;
		for (i=1; i<count; i++)
		{
			RX_Buffer[i] = ReceiveBuffer[j];
			temp_crc_buffer[0] = ReceiveBuffer[j];
			j = j + 1;
			CRCc = CRC8(temp_crc_buffer,1);
			if (CRCc != ReceiveBuffer[j])
				RX_CRC_Fail += 1;
			j = j + 1;
		}
		CopyArray(RX_Buffer, reg_data, crc_count);
	}
#else
	HAL_I2C_Mem_Read(&hi2c1, DEV_ADDR, reg_addr, 1, reg_data, count, 1000);
#endif
	return 0;
}

void BQ769x2_SetRegister(uint16_t reg_addr, uint32_t reg_data, uint8_t datalen)
{
	uint8_t TX_Buffer[2] = {0x00, 0x00};
	uint8_t TX_RegData[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	//TX_RegData in little endian format
	TX_RegData[0] = reg_addr & 0xff;
	TX_RegData[1] = (reg_addr >> 8) & 0xff;
	TX_RegData[2] = reg_data & 0xff; //1st byte of data

	switch(datalen)
    {
		case 1: //1 byte datalength
      		I2C_WriteReg(0x3E, TX_RegData, 3);
			HAL_Delay(2);
			TX_Buffer[0] = Checksum(TX_RegData, 3);
			TX_Buffer[1] = 0x05; //combined length of register address and data
      		I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
			HAL_Delay(10);
			break;
		case 2: //2 byte datalength
			TX_RegData[3] = (reg_data >> 8) & 0xff;
			I2C_WriteReg(0x3E, TX_RegData, 4);
			HAL_Delay(2);
			TX_Buffer[0] = Checksum(TX_RegData, 4);
			TX_Buffer[1] = 0x06; //combined length of register address and data
      		I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
			HAL_Delay(10);
			break;
		case 4: //4 byte datalength, Only used for CCGain and Capacity Gain
			TX_RegData[3] = (reg_data >> 8) & 0xff;
			TX_RegData[4] = (reg_data >> 16) & 0xff;
			TX_RegData[5] = (reg_data >> 24) & 0xff;
			I2C_WriteReg(0x3E, TX_RegData, 6);
			HAL_Delay(2);
			TX_Buffer[0] = Checksum(TX_RegData, 6);
			TX_Buffer[1] = 0x08; //combined length of register address and data
      		I2C_WriteReg(0x60, TX_Buffer, 2); // Write the checksum and length
			HAL_Delay(10);
			break;
    }
}

void CommandSubcommands(uint16_t command) //For Command only Subcommands
// See the TRM or the BQ76952 header file for a full list of Command-only subcommands
{	//For DEEPSLEEP/SHUTDOWN subcommand you will need to call this function twice consecutively

	uint8_t TX_Reg[2] = {0x00, 0x00};

	//TX_Reg in little endian format
	TX_Reg[0] = command & 0xff;
	TX_Reg[1] = (command >> 8) & 0xff;

	I2C_WriteReg(0x3E,TX_Reg,2);
	HAL_Delay(2);
}
uint32_t BQ769x2_ReadRegister(uint16_t reg_addr)
{
	uint32_t output = 0;
	uint8_t TX_Reg[4] = {0x00, 0x00, 0x00, 0x00};

	//TX_Reg in little endian format
	TX_Reg[0] = reg_addr & 0xff;
	TX_Reg[1] = (reg_addr >> 8) & 0xff;
	I2C_WriteReg(0x3E,TX_Reg,2);
	HAL_Delay(2);
	I2C_ReadReg(0x40, RX_32Byte, 32); //RX_32Byte is a global variable
	output=RX_32Byte[1]*256 + RX_32Byte[0];
	return output;
}
void Subcommands(uint16_t command, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Subcommands
{
	//security keys and Manu_data writes dont work with this function (reading these commands works)
	//max readback size is 32 bytes i.e. DASTATUS, CUV/COV snapshot
	uint8_t TX_Reg[4] = {0x00, 0x00, 0x00, 0x00};
	uint8_t TX_Buffer[2] = {0x00, 0x00};

	//TX_Reg in little endian format
	TX_Reg[0] = command & 0xff;
	TX_Reg[1] = (command >> 8) & 0xff;

	if (type == R)
	{//read
		I2C_WriteReg(0x3E,TX_Reg,2);
		HAL_Delay(2);
		I2C_ReadReg(0x40, RX_32Byte, 32); //RX_32Byte is a global variable
	}

	else if (type == W)
	{
		//FET_Control, REG12_Control
		TX_Reg[2] = data & 0xff;
		I2C_WriteReg(0x3E,TX_Reg,3);
		HAL_Delay(1);
		TX_Buffer[0] = Checksum(TX_Reg, 3);
		TX_Buffer[1] = 0x05; //combined length of registers address and data
		I2C_WriteReg(0x60, TX_Buffer, 2);
		HAL_Delay(1);
	}
	else if (type == W2)
	{ //write data with 2 bytes
		//CB_Active_Cells, CB_SET_LVL
		TX_Reg[2] = data & 0xff;
		TX_Reg[3] = (data >> 8) & 0xff;
		I2C_WriteReg(0x3E,TX_Reg,4);
		HAL_Delay(1);
		TX_Buffer[0] = Checksum(TX_Reg, 4);
		TX_Buffer[1] = 0x06; //combined length of registers address and data
		I2C_WriteReg(0x60, TX_Buffer, 2);
		HAL_Delay(1);
	}
}

void DirectCommands(uint8_t command, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Direct Commands
{	//type: R = read, W = write
	uint8_t TX_data[2] = {0x00, 0x00};

	//little endian format
	TX_data[0] = data & 0xff;
	TX_data[1] = (data >> 8) & 0xff;

	if (type == R)
	{//Read
		I2C_ReadReg(command, RX_data, 2); //RX_data is a global variable
		HAL_Delay(2);
	}
	if (type == W)
	{//write
    //Control_status, alarm_status, alarm_enable all 2 bytes long
		I2C_WriteReg(command,TX_data,2);
		HAL_Delay(2);
	}
}

void BQ769x2_Init()  //XXX
{

	// Configures all parameters in device RAM

	// Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
	// See TRM for full description of CONFIG_UPDATE mode
	CommandSubcommands(SET_CFGUPDATE);

	HAL_Delay(10);
	// After entering CONFIG_UPDATE mode, RAM registers can be programmed. When programming RAM, checksum and length must also be
	// programmed for the change to take effect. All of the RAM registers are described in detail in the BQ769x2 TRM.
	// An easier way to find the descriptions is in the BQStudio Data Memory screen. When you move the mouse over the register name,
	// a full description of the register and the bits will pop up on the screen.

	U32toU16x2(EEPROM_RAM_REG[0],&cell2gain,&cell1gain); ////read cell1gain & cell2gain from eeprom
	BQ769x2_SetRegister(Cell1Gain,cell1gain,2);
	BQ769x2_SetRegister(Cell2Gain,cell2gain,2);

	U32toU16x2(EEPROM_RAM_REG[1],&cell4gain,&cell3gain); ////read cell4gain & cell3gain from eeprom
	BQ769x2_SetRegister(Cell3Gain,cell3gain,2);
	BQ769x2_SetRegister(Cell4Gain,cell4gain,2);

	U32toU16x2(EEPROM_RAM_REG[2],&cell6gain,&cell5gain); ////read cell6gain & cell5gain from eeprom
	BQ769x2_SetRegister(Cell5Gain,cell5gain,2);
	BQ769x2_SetRegister(Cell6Gain,cell6gain,2);

	U32toU16x2(EEPROM_RAM_REG[3],&cell8gain,&cell7gain); ////read cell8gain & cell7gain from eeprom
	BQ769x2_SetRegister(Cell7Gain,cell7gain,2);
	BQ769x2_SetRegister(Cell8Gain,cell8gain,2);

	U32toU16x2(EEPROM_RAM_REG[4],&cell10gain,&cell9gain); ////read cell10gain & cell9gain from eeprom
	BQ769x2_SetRegister(Cell9Gain,cell9gain,2);
	BQ769x2_SetRegister(Cell10Gain,cell10gain,2);

	U32toU16x2(EEPROM_RAM_REG[5],&cell12gain,&cell11gain); ////read cell12gain & cell11gain from eeprom
	BQ769x2_SetRegister(Cell11Gain,cell11gain,2);
	BQ769x2_SetRegister(Cell12Gain,cell12gain,2);

	U32toU16x2(EEPROM_RAM_REG[6],&cell14gain,&cell13gain); ////read cell14gain & cell13gain from eeprom
	BQ769x2_SetRegister(Cell13Gain,cell13gain,2);
	BQ769x2_SetRegister(Cell14Gain,cell14gain,2);

	U32toU16x2(EEPROM_RAM_REG[7],&cell16gain,&cell15gain); ////read cell16gain & cell15gain from eeprom
	BQ769x2_SetRegister(Cell15Gain,cell15gain,2);
	BQ769x2_SetRegister(Cell16Gain,cell16gain,2);

	U32toU16x2(EEPROM_RAM_REG[8],&tosgain,&packgain); ////read tosgain & packgain from eeprom
	BQ769x2_SetRegister(PackGain,packgain,2);
	BQ769x2_SetRegister(TOSGain,tosgain,2);

	U32toU16x2(EEPROM_RAM_REG[9],&ldgain,&adcgain); ////read tosgain & packgain from eeprom
	BQ769x2_SetRegister(LDGain,ldgain,2);
	BQ769x2_SetRegister(ADCGain,adcgain,2);


	float tempu = 0.0;
	uint32_t tempfreg = 0;
	memcpy(&ccgain,&EEPROM_RAM_REG[10], 4);
	memcpy(&tempfreg,&ccgain, 4);
	BQ769x2_SetRegister(CCGain,tempfreg,4);

	memcpy(&capacitygain,&EEPROM_RAM_REG[11], 4);
	memcpy(&tempfreg,&capacitygain, 4);
	BQ769x2_SetRegister(CapacityGain,tempfreg,4);

	BQ769x2_SetRegister(VcellOffset,vcelloffset,2);
	BQ769x2_SetRegister(VdivOffset,vdivoffset,2);
	BQ769x2_SetRegister(CoulombCounterOffsetSamples,coulombcounteroffsetsamples,2);
	BQ769x2_SetRegister(BoardOffset,boardoffset,2);
	BQ769x2_SetRegister(InternalTempOffset,internaltempoffset,1);
	BQ769x2_SetRegister(CFETOFFTempOffset,cfetofftempoffset,1);
	BQ769x2_SetRegister(DFETOFFTempOffset,dfetofftempoffset,1);
	BQ769x2_SetRegister(ALERTTempOffset,alerttempoffset,1);
	BQ769x2_SetRegister(TS1TempOffset,ts1tempoffset,1);
	BQ769x2_SetRegister(TS2TempOffset,ts2tempoffset,1);
	BQ769x2_SetRegister(TS3TempOffset,ts3tempoffset,1);
	BQ769x2_SetRegister(HDQTempOffset,hdqtempoffset,1);
	BQ769x2_SetRegister(DCHGTempOffset,dchgtempoffset,1);
	BQ769x2_SetRegister(DDSGTempOffset,ddsgtempoffset,1);
	BQ769x2_SetRegister(IntGain,intgain,2);
	BQ769x2_SetRegister(Intbaseoffset,intbaseoffset,2);
	BQ769x2_SetRegister(IntMaximumAD,intmaximumad,2);
	BQ769x2_SetRegister(IntMaximumTemp,intmaximumtemp,2);
	BQ769x2_SetRegister(T18kCoeffa1,t18kcoeffa1,2);
	BQ769x2_SetRegister(T18kCoeffa2,t18kcoeffa2,2);
	BQ769x2_SetRegister(T18kCoeffa3,t18kcoeffa3,2);
	BQ769x2_SetRegister(T18kCoeffa4,t18kcoeffa4,2);
	BQ769x2_SetRegister(T18kCoeffa5,t18kcoeffa5,2);
	BQ769x2_SetRegister(T18kCoeffb1,t18kcoeffb1,2);
	BQ769x2_SetRegister(T18kCoeffb2,t18kcoeffb2,2);
	BQ769x2_SetRegister(T18kCoeffb3,t18kcoeffb3,2);
	BQ769x2_SetRegister(T18kCoeffb4,t18kcoeffb4,2);
	BQ769x2_SetRegister(T18kAdc0,t18kadc0,2);
	BQ769x2_SetRegister(T180kCoeffa1,t180kcoeffa1,2);
	BQ769x2_SetRegister(T180kCoeffa2,t180kcoeffa2,2);
	BQ769x2_SetRegister(T180kCoeffa3,t180kcoeffa3,2);
	BQ769x2_SetRegister(T180kCoeffa4,t180kcoeffa4,2);
	BQ769x2_SetRegister(T180kCoeffa5,t180kcoeffa5,2);
	BQ769x2_SetRegister(T180kCoeffb1,t180kcoeffb1,2);
	BQ769x2_SetRegister(T180kCoeffb2,t180kcoeffb2,2);
	BQ769x2_SetRegister(T180kCoeffb3,t180kcoeffb3,2);
	BQ769x2_SetRegister(T180kCoeffb4,t180kcoeffb4,2);
	BQ769x2_SetRegister(T180kAdc0,t180kadc0,2);
	BQ769x2_SetRegister(CustomCoeffa1,customcoeffa1,2);
	BQ769x2_SetRegister(CustomCoeffa2,customcoeffa2,2);
	BQ769x2_SetRegister(CustomCoeffa3,customcoeffa3,2);
	BQ769x2_SetRegister(CustomCoeffa4,customcoeffa4,2);
	BQ769x2_SetRegister(CustomCoeffa5,customcoeffa5,2);
	BQ769x2_SetRegister(CustomCoeffb1,customcoeffb1,2);
	BQ769x2_SetRegister(CustomCoeffb2,customcoeffb2,2);
	BQ769x2_SetRegister(CustomCoeffb3,customcoeffb3,2);
	BQ769x2_SetRegister(CustomCoeffb4,customcoeffb4,2);
	BQ769x2_SetRegister(CustomRc0,customrc0,2);
	BQ769x2_SetRegister(CustomAdc0,customadc0,2);
	BQ769x2_SetRegister(CoulombCounterDeadband,coulombcounterdeadband,1);
	BQ769x2_SetRegister(CUVThresholdOverride,cuvthresholdoverride,2);
	BQ769x2_SetRegister(COVThresholdOverride,covthresholdoverride,2);

	BQ769x2_SetRegister(MinBlowFuseVoltage,minblowfusevoltage,2);
	BQ769x2_SetRegister(FuseBlowTimeout,fuseblowtimeout,1);
	BQ769x2_SetRegister(PowerConfig,powerconfig,2);
	BQ769x2_SetRegister(REG12Config,reg12config,1);
	BQ769x2_SetRegister(REG0Config,reg0config,1);
	BQ769x2_SetRegister(HWDRegulatorOptions,hwdregulatoroptions,1);
	BQ769x2_SetRegister(CommType,commtype,1);
	BQ769x2_SetRegister(I2CAddress,i2caddress,1);
	BQ769x2_SetRegister(SPIConfiguration,spiconfiguration,1);
	BQ769x2_SetRegister(CommIdleTime,commidletime,1);
	BQ769x2_SetRegister(CFETOFFPinConfig,cfetoffpinconfig,1);

	BQ769x2_SetRegister(DFETOFFPinConfig,dfetoffpinconfig,1);

	BQ769x2_SetRegister(ALERTPinConfig,alertpinconfig,1);
	BQ769x2_SetRegister(TS1Config,ts1config,1);
	BQ769x2_SetRegister(TS2Config,ts2config,1);
	BQ769x2_SetRegister(TS3Config,ts3config,1);

	BQ769x2_SetRegister(HDQPinConfig,hdqpinconfig,1);

	BQ769x2_SetRegister(DCHGPinConfig,dchgpinconfig,1);
	BQ769x2_SetRegister(DDSGPinConfig,ddsgpinconfig,1);
	BQ769x2_SetRegister(DAConfiguration,daconfiguration,1);

//XXX
	U32toU16x2(EEPROM_RAM_REG[41],&protectionconfiguration,&vcellmode); //read protectionconfig & cellmode from eeprom
	BQ769x2_SetRegister(VCellMode,vcellmode,2);
	BQ769x2_SetRegister(ProtectionConfiguration,protectionconfiguration,2);

	BQ769x2_SetRegister(CC3Samples,cc3samples,1);

	BQ769x2_SetRegister(EnabledProtectionsA,enabledprotectionsa,1);
	BQ769x2_SetRegister(EnabledProtectionsB,enabledprotectionsb,1);
	BQ769x2_SetRegister(EnabledProtectionsC,enabledprotectionsc,1);
	BQ769x2_SetRegister(CHGFETProtectionsA,chgfetprotectionsa,1);
	BQ769x2_SetRegister(CHGFETProtectionsB,chgfetprotectionsb,1);
	BQ769x2_SetRegister(CHGFETProtectionsC,chgfetprotectionsc,1);
	BQ769x2_SetRegister(DSGFETProtectionsA,dsgfetprotectionsa,1);
	BQ769x2_SetRegister(DSGFETProtectionsB,dsgfetprotectionsb,1);
	BQ769x2_SetRegister(DSGFETProtectionsC,dsgfetprotectionsc,1);
	BQ769x2_SetRegister(BodyDiodeThreshold,bodydiodethreshold,2);
	BQ769x2_SetRegister(DefaultAlarmMask,defaultalarmmask,2);
	BQ769x2_SetRegister(SFAlertMaskA,sfalertmaska,1);
	BQ769x2_SetRegister(SFAlertMaskB,sfalertmaskb,1);
	BQ769x2_SetRegister(SFAlertMaskC,sfalertmaskc,1);
	BQ769x2_SetRegister(PFAlertMaskA,pfalertmaska,1);
	BQ769x2_SetRegister(PFAlertMaskB,pfalertmaskb,1);
	BQ769x2_SetRegister(PFAlertMaskC,pfalertmaskc,1);
	BQ769x2_SetRegister(PFAlertMaskD,pfalertmaskd,1);
	BQ769x2_SetRegister(EnabledPFA,enabledpfa,1);
	BQ769x2_SetRegister(EnabledPFB,enabledpfb,1);
	BQ769x2_SetRegister(EnabledPFC,enabledpfc,1);
	BQ769x2_SetRegister(EnabledPFD,enabledpfd,1);
	BQ769x2_SetRegister(FETOptions,fetoptions,1);
	BQ769x2_SetRegister(ChgPumpControl,chgpumpcontrol,1);
	BQ769x2_SetRegister(PrechargeStartVoltage,prechargestartvoltage,2);
	BQ769x2_SetRegister(PrechargeStopVoltage,prechargestopvoltage,2);
	BQ769x2_SetRegister(PredischargeTimeout,predischargetimeout,1);
	BQ769x2_SetRegister(PredischargeStopDelta,predischargestopdelta,1);
	BQ769x2_SetRegister(DsgCurrentThreshold,dsgcurrentthreshold,2);
	BQ769x2_SetRegister(ChgCurrentThreshold,chgcurrentthreshold,2);
	BQ769x2_SetRegister(CheckTime,checktime,1);
	BQ769x2_SetRegister(Cell1Interconnect,cell1interconnect,2);
	BQ769x2_SetRegister(Cell2Interconnect,cell2interconnect,2);
	BQ769x2_SetRegister(Cell3Interconnect,cell3interconnect,2);
	BQ769x2_SetRegister(Cell4Interconnect,cell4interconnect,2);
	BQ769x2_SetRegister(Cell5Interconnect,cell5interconnect,2);
	BQ769x2_SetRegister(Cell6Interconnect,cell6interconnect,2);
	BQ769x2_SetRegister(Cell7Interconnect,cell7interconnect,2);
	BQ769x2_SetRegister(Cell8Interconnect,cell8interconnect,2);
	BQ769x2_SetRegister(Cell9Interconnect,cell9interconnect,2);
	BQ769x2_SetRegister(Cell10Interconnect,cell10interconnect,2);
	BQ769x2_SetRegister(Cell11Interconnect,cell11interconnect,2);
	BQ769x2_SetRegister(Cell12Interconnect,cell12interconnect,2);
	BQ769x2_SetRegister(Cell13Interconnect,cell13interconnect,2);
	BQ769x2_SetRegister(Cell14Interconnect,cell14interconnect,2);
	BQ769x2_SetRegister(Cell15Interconnect,cell15interconnect,2);
	BQ769x2_SetRegister(Cell16Interconnect,cell16interconnect,2);

	BQ769x2_SetRegister(MfgStatusInit,mfgstatusinit,2);
	BQ769x2_SetRegister(BalancingConfiguration,balancingconfiguration,1);

	U32toU8x4(EEPROM_RAM_REG[60],&cellbalanceinterval,&maxinternaltemp,&maxcelltemp,&mincelltemp); ////read mincelltemp & maxcelltemp & maxinternaltemp & cellbalanceinterval from eeprom
	BQ769x2_SetRegister(MinCellTemp,mincelltemp,1);
	BQ769x2_SetRegister(MaxCellTemp,maxcelltemp,1);
	BQ769x2_SetRegister(MaxInternalTemp,maxinternaltemp,1);
	BQ769x2_SetRegister(CellBalanceInterval,cellbalanceinterval,1);

	U32toU16x1U8x2(EEPROM_RAM_REG[61],&cellbalancemincellvcharge,&cellbalancemindeltacharge,&cellbalancemaxcells);////read CellBalanceMinCellVCharge & cellbalancemindeltacharge & cellbalancemaxcells from eeprom
	BQ769x2_SetRegister(CellBalanceMinCellVCharge,cellbalancemincellvcharge,2);
	BQ769x2_SetRegister(CellBalanceMinDeltaCharge,cellbalancemindeltacharge,1);
	BQ769x2_SetRegister(CellBalanceMaxCells,cellbalancemaxcells,1);

	U32toU16x1U8x2(EEPROM_RAM_REG[62],&cellbalancemincellvrelax,&cellbalancemindeltarelax,&cellbalancestopdeltacharge);////read cellbalancemincellvrelax & cellbalancemindeltarelax & cellbalancestopdeltacharge from eeprom
	BQ769x2_SetRegister(CellBalanceStopDeltaCharge,cellbalancestopdeltacharge,1);
	BQ769x2_SetRegister(CellBalanceMinCellVRelax,cellbalancemincellvrelax,2);
	BQ769x2_SetRegister(CellBalanceMinDeltaRelax,cellbalancemindeltarelax,1);

	U32toU16x2(EEPROM_RAM_REG[63],&shutdownstackvoltage,&shutdowncellvoltage);////read shutdownstackvoltage & shutdowncellvoltage from eeprom
	BQ769x2_SetRegister(ShutdownCellVoltage,shutdowncellvoltage,2);
	BQ769x2_SetRegister(ShutdownStackVoltage,shutdownstackvoltage,2);

	U32toU8x4(EEPROM_RAM_REG[64],&shutdowntemperaturedelay,&shutdowntemperature,&lowvshutdowndelay,&cellbalancestopdeltarelax); ////read shutdowntemperature & shutdowntemperaturedelay & lowvshutdowndelay & cellbalancestopdeltarelax from eeprom
	BQ769x2_SetRegister(CellBalanceStopDeltaRelax,cellbalancestopdeltarelax,1);
	BQ769x2_SetRegister(LowVShutdownDelay,lowvshutdowndelay,1);
	BQ769x2_SetRegister(ShutdownTemperature,shutdowntemperature,1);
	BQ769x2_SetRegister(ShutdownTemperatureDelay,shutdowntemperaturedelay,1);


	BQ769x2_SetRegister(FETOffDelay,fetoffdelay,1);
	BQ769x2_SetRegister(ShutdownCommandDelay,shutdowncommanddelay,1);
	BQ769x2_SetRegister(AutoShutdownTime,autoshutdowntime,1);
	BQ769x2_SetRegister(RAMFailShutdownTime,ramfailshutdowntime,1);

	BQ769x2_SetRegister(SleepCurrent,sleepcurrent,2);
	BQ769x2_SetRegister(VoltageTime,voltagetime,1);
	BQ769x2_SetRegister(WakeComparatorCurrent,wakecomparatorcurrent,2);
	BQ769x2_SetRegister(SleepHysteresisTime,sleephysteresistime,1);
	BQ769x2_SetRegister(SleepChargerVoltageThreshold,sleepchargervoltagethreshold,2);
	BQ769x2_SetRegister(SleepChargerPACKTOSDelta,sleepchargerpacktosdelta,2);
	BQ769x2_SetRegister(ConfigRAMSignature,configramsignature,2);


	U32toU16x1U8x2(EEPROM_RAM_REG[69],&cuvdelay,&cuvthreshold,&cuvrecoveryhysteresis);////read CUVDelay & CUVThreshold & CUVRecoveryHysteresis from eeprom
	BQ769x2_SetRegister(CUVThreshold,cuvthreshold,1);
	BQ769x2_SetRegister(CUVDelay,cuvdelay,2);
	BQ769x2_SetRegister(CUVRecoveryHysteresis,cuvrecoveryhysteresis,1);

	U32toU16x1U8x2(EEPROM_RAM_REG[70],&covdelay,&covthreshold,&covrecoveryhysteresis);////read COVDelay & COVThreshold & covrecoveryhysteresis from eeprom
	BQ769x2_SetRegister(COVThreshold,covthreshold,1);
	BQ769x2_SetRegister(COVDelay,covdelay,2);
	BQ769x2_SetRegister(COVRecoveryHysteresis,covrecoveryhysteresis,1);

	U32toU8x4(EEPROM_RAM_REG[71],&occthreshold,&covlrecoverytime,&covlcounterdecdelay,&covllatchlimit); ////read occthreshold & covlrecoverytime & covlcounterdecdelay & covllatchlimit from eeprom
	BQ769x2_SetRegister(COVLLatchLimit,covllatchlimit,1);
	BQ769x2_SetRegister(COVLCounterDecDelay,covlcounterdecdelay,1);
	BQ769x2_SetRegister(COVLRecoveryTime,covlrecoverytime,1);
	BQ769x2_SetRegister(OCCThreshold,occthreshold,1);

	U32toU16x2(EEPROM_RAM_REG[72],&occpacktosdelta,&occrecoverythreshold);////read occpacktosdelta & occrecoverythreshold from eeprom
	BQ769x2_SetRegister(OCCPACKTOSDelta,occpacktosdelta,2);
	BQ769x2_SetRegister(OCCRecoveryThreshold,occrecoverythreshold,2);

	U32toU8x4(EEPROM_RAM_REG[73],&ocd2threshold,&ocd1delay,&ocd1threshold,&occdelay); ////read ocd2threshold & ocd1delay & ocd1threshold & occdelay from eeprom
	BQ769x2_SetRegister(OCCDelay,occdelay,1);
	BQ769x2_SetRegister(OCD1Threshold,ocd1threshold,1);
	BQ769x2_SetRegister(OCD1Delay,ocd1delay,1);
	BQ769x2_SetRegister(OCD2Threshold,ocd2threshold,1);

	U32toU8x4(EEPROM_RAM_REG[74],&scdrecoverytime,&scddelay,&scdthreshold,&ocd2delay); ////read scdrecoverytime & scddelay & scdthreshold & ocd2delay from eeprom
	BQ769x2_SetRegister(OCD2Delay,ocd2delay,1);
	BQ769x2_SetRegister(SCDThreshold,scdthreshold,1);
	BQ769x2_SetRegister(SCDDelay,scddelay,1);
	BQ769x2_SetRegister(SCDRecoveryTime,scdrecoverytime,1);

	U32toU16x2(EEPROM_RAM_REG[75],&ocdrecoverythreshold,&ocd3threshold);////read ocdrecoverythreshold & ocd3threshold from eeprom
	BQ769x2_SetRegister(OCD3Threshold,ocd3threshold,2);
	BQ769x2_SetRegister(OCDRecoveryThreshold,ocdrecoverythreshold,2);

	U32toU8x4(EEPROM_RAM_REG[76],&ocdlrecoverytime,&ocdlcounterdecdelay,&ocdllatchlimit,&ocd3delay); ////read ocdlrecoverytime & ocdlcounterdecdelay & ocd3delay & ocd2delay from eeprom
	BQ769x2_SetRegister(OCD3Delay,ocd3delay,1);
	BQ769x2_SetRegister(OCDLLatchLimit,ocdllatchlimit,1);
	BQ769x2_SetRegister(OCDLCounterDecDelay,ocdlcounterdecdelay,1);
	BQ769x2_SetRegister(OCDLRecoveryTime,ocdlrecoverytime,1);

	U32toU16x1U8x2(EEPROM_RAM_REG[77],&ocdlrecoverythreshold,&scdlcounterdecdelay,&scdllatchlimit);////read ocdlrecoverythreshold & scdlcounterdecdelay & scdllatchlimit from eeprom
	BQ769x2_SetRegister(OCDLRecoveryThreshold,ocdlrecoverythreshold,2);
	BQ769x2_SetRegister(SCDLLatchLimit,scdllatchlimit,1);
	BQ769x2_SetRegister(SCDLCounterDecDelay,scdlcounterdecdelay,1);

	U32toU16x1U8x2(EEPROM_RAM_REG[78],&scdlrecoverythreshold,&otcthreshold,&scdlrecoverytime);////read scdlrecoverythreshold & otcthreshold & scdlrecoverytime from eeprom
	BQ769x2_SetRegister(SCDLRecoveryTime,scdlrecoverytime,1);
	BQ769x2_SetRegister(SCDLRecoveryThreshold,scdlrecoverythreshold,2);
	BQ769x2_SetRegister(OTCThreshold,otcthreshold,1);

	U32toU8x4(EEPROM_RAM_REG[79],&otddelay,&otdthreshold,&otcrecovery,&otcdelay); ////read otddelay & otdthreshold & otcrecovery & otcdelay from eeprom
	BQ769x2_SetRegister(OTCDelay,otcdelay,1);
	BQ769x2_SetRegister(OTCRecovery,otcrecovery,1);
	BQ769x2_SetRegister(OTDThreshold,otdthreshold,1);
	BQ769x2_SetRegister(OTDDelay,otddelay,1);

	U32toU8x4(EEPROM_RAM_REG[80],&otfrecovery,&otfdelay,&otfthreshold,&otdrecovery); ////read otfrecovery & otfdelay & otfthreshold & otdrecovery from eeprom
	BQ769x2_SetRegister(OTDRecovery,otdrecovery,1);
	BQ769x2_SetRegister(OTFThreshold,otfthreshold,1);
	BQ769x2_SetRegister(OTFDelay,otfdelay,1);
	BQ769x2_SetRegister(OTFRecovery,otfrecovery,1);

	U32toU8x4(EEPROM_RAM_REG[81],&otfrecovery,&otfdelay,&otfthreshold,&otdrecovery); ////read otfrecovery & otfdelay & otfthreshold & otdrecovery from eeprom
	BQ769x2_SetRegister(OTINTThreshold,otintthreshold,1);
	BQ769x2_SetRegister(OTINTDelay,otintdelay,1);
	BQ769x2_SetRegister(OTINTRecovery,otintrecovery,1);
	BQ769x2_SetRegister(UTCThreshold,utcthreshold,1);

	U32toU8x4(EEPROM_RAM_REG[82],&utddelay,&utdthreshold,&utcrecovery,&utcdelay); ////read utddelay & utdthreshold & utcrecovery & utcdelay from eeprom
	BQ769x2_SetRegister(UTCDelay,utcdelay,1);
	BQ769x2_SetRegister(UTCRecovery,utcrecovery,1);
	BQ769x2_SetRegister(UTDThreshold,utdthreshold,1);
	BQ769x2_SetRegister(UTDDelay,utddelay,1);

	U32toU8x4(EEPROM_RAM_REG[83],&utintrecovery,&utintdelay,&utintthreshold,&utdrecovery); ////read utintrecovery & utintdelay & utintthreshold & utdrecovery from eeprom
	BQ769x2_SetRegister(UTDRecovery,utdrecovery,1);
	BQ769x2_SetRegister(UTINTThreshold,utintthreshold,1);
	BQ769x2_SetRegister(UTINTDelay,utintdelay,1);
	BQ769x2_SetRegister(UTINTRecovery,utintrecovery,1);

	BQ769x2_SetRegister(ProtectionsRecoveryTime,protectionsrecoverytime,1);
	BQ769x2_SetRegister(HWDDelay,hwddelay,2);
	BQ769x2_SetRegister(LoadDetectActiveTime,loaddetectactivetime,1);
	BQ769x2_SetRegister(LoadDetectRetryDelay,loaddetectretrydelay,1);
	BQ769x2_SetRegister(LoadDetectTimeout,loaddetecttimeout,2);
	BQ769x2_SetRegister(PTOChargeThreshold,ptochargethreshold,2);
	BQ769x2_SetRegister(PTODelay,ptodelay,2);
	BQ769x2_SetRegister(PTOReset,ptoreset,2);
	BQ769x2_SetRegister(CUDEPThreshold,cudepthreshold,2);
	BQ769x2_SetRegister(CUDEPDelay,cudepdelay,1);
	BQ769x2_SetRegister(SUVThreshold,suvthreshold,2);
	BQ769x2_SetRegister(SUVDelay,suvdelay,1);
	BQ769x2_SetRegister(SOVThreshold,sovthreshold,2);
	BQ769x2_SetRegister(SOVDelay,sovdelay,1);
	BQ769x2_SetRegister(TOSSThreshold,tossthreshold,2);
	BQ769x2_SetRegister(TOSSDelay,tossdelay,1);
	BQ769x2_SetRegister(SOCCThreshold,soccthreshold,2);
	BQ769x2_SetRegister(SOCCDelay,soccdelay,1);
	BQ769x2_SetRegister(SOCDThreshold,socdthreshold,2);
	BQ769x2_SetRegister(SOCDDelay,socddelay,1);
	BQ769x2_SetRegister(SOTThreshold,sotthreshold,1);
	BQ769x2_SetRegister(SOTDelay,sotdelay,1);
	BQ769x2_SetRegister(SOTFThreshold,sotfthreshold,1);
	BQ769x2_SetRegister(SOTFDelay,sotfdelay,1);
	BQ769x2_SetRegister(VIMRCheckVoltage,vimrcheckvoltage,2);
	BQ769x2_SetRegister(VIMRMaxRelaxCurrent,vimrmaxrelaxcurrent,2);
	BQ769x2_SetRegister(VIMRThreshold,vimrthreshold,2);
	BQ769x2_SetRegister(VIMRDelay,vimrdelay,1);
	BQ769x2_SetRegister(VIMRRelaxMinDuration,vimrrelaxminduration,2);
	BQ769x2_SetRegister(VIMACheckVoltage,vimacheckvoltage,2);
	BQ769x2_SetRegister(VIMAMinActiveCurrent,vimaminactivecurrent,2);
	BQ769x2_SetRegister(VIMAThreshold,vimathreshold,2);
	BQ769x2_SetRegister(VIMADelay,vimadelay,1);
	BQ769x2_SetRegister(CFETFOFFThreshold,cfetfoffthreshold,2);
	BQ769x2_SetRegister(CFETFOFFDelay,cfetfoffdelay,1);
	BQ769x2_SetRegister(DFETFOFFThreshold,dfetfoffthreshold,2);
	BQ769x2_SetRegister(DFETFOFFDelay,dfetfoffdelay,1);
	BQ769x2_SetRegister(VSSFFailThreshold,vssffailthreshold,2);
	BQ769x2_SetRegister(VSSFDelay,vssfdelay,1);
	BQ769x2_SetRegister(PF2LVLDelay,pf2lvldelay,1);
	BQ769x2_SetRegister(LFOFDelay,lfofdelay,1);
	BQ769x2_SetRegister(HWMXDelay,hwmxdelay,1);
	BQ769x2_SetRegister(SecuritySettings,securitysettings,1);
	BQ769x2_SetRegister(UnsealKeyStep1,unsealkeystep1,2);
	BQ769x2_SetRegister(UnsealKeyStep2,unsealkeystep2,2);
	BQ769x2_SetRegister(FullAccessKeyStep1,fullaccesskeystep1,2);
	BQ769x2_SetRegister(FullAccessKeyStep2,fullaccesskeystep2,2);

	// Exit CONFIGUPDATE mode  - Subcommand 0x0092
	CommandSubcommands(EXIT_CFGUPDATE);
}

/*FETs are made to exit the sleep mode if they detect the charger*/
void PowerConfig_Charger()
{
	// Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
	CommandSubcommands(SET_CFGUPDATE);

	HAL_Delay(10);
	BQ769x2_SetRegister(PowerConfig,powerconfig_nosleep,2);
	// Exit CONFIGUPDATE mode  - Subcommand 0x0092
	CommandSubcommands(EXIT_CFGUPDATE);

}

/*FETs are set again to sleep mode once the pack is fully charged*/
void PowerConfig_FullCharge()
{
	// Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
	CommandSubcommands(SET_CFGUPDATE);

	HAL_Delay(10);
	BQ769x2_SetRegister(PowerConfig,powerconfig,2);
	// Exit CONFIGUPDATE mode  - Subcommand 0x0092
	CommandSubcommands(EXIT_CFGUPDATE);
}

// ********************************* BQ769x2 Status and Fault Commands   *****************************************
uint16_t BQ769x2_ReadSavedPF()
{
	// Read this register to find out why the ALERT pin was asserted
	Subcommands(SAVED_PF_STATUS, 0x00, R);
	return ((RX_data[3]<<24)+(RX_data[2]<<16)+(RX_data[1]<<8) + RX_data[0]);
}

uint16_t BQ769x2_ReadAlarmStatus()
{
	// Read this register to find out why the ALERT pin was asserted
	DirectCommands(AlarmStatus, 0x00, R);
	return (RX_data[1]*256 + RX_data[0]);
}

void BQ769x2_ReadSafetyStatus()
{ //good example functions
	// Read Safety Status A/B/C and find which bits are set
	// This shows which primary protections have been triggered
	DirectCommands(SafetyStatusA, 0x00, R);
	value_SafetyStatusA = (RX_data[1]*256 + RX_data[0]);
	//Example Fault Flags
	UV_Fault = ((0x4 & RX_data[0])>>2);
	OV_Fault = ((0x8 & RX_data[0])>>3);
	SCD_Fault = ((0x8 & RX_data[1])>>3);
	OCD_Fault = ((0x2 & RX_data[1])>>1);
	DirectCommands(SafetyStatusB, 0x00, R);
	value_SafetyStatusB = (RX_data[1]*256 + RX_data[0]);
	DirectCommands(SafetyStatusC, 0x00, R);
	value_SafetyStatusC = (RX_data[1]*256 + RX_data[0]);

	if ((value_SafetyStatusA + value_SafetyStatusB + value_SafetyStatusC) > 1)
	{
		ProtectionsTriggered = 1;
	}
	else
	{
		ProtectionsTriggered = 0;
	}

	/*Modified Portion*/
	//update_fault_condition_voltages();
	//update_fault_counts();
}


void BQ769x2_ReadPFStatus()
{
	// Read Permanent Fail Status A/B/C and find which bits are set
	// This shows which permanent failures have been triggered
	DirectCommands(PFStatusA, 0x00, R);
	value_PFStatusA = (RX_data[1]*256 + RX_data[0]);
	DirectCommands(PFStatusB, 0x00, R);
	value_PFStatusB = (RX_data[1]*256 + RX_data[0]);
	DirectCommands(PFStatusC, 0x00, R);
	value_PFStatusC = (RX_data[1]*256 + RX_data[0]);
	if ((value_PFStatusA + value_PFStatusB + value_PFStatusC) > 1)
	{
		ProtectionsTriggered = 1;
	}
	else
	{
		ProtectionsTriggered = 0;
	}
}

// ********************************* End of BQ769x2 Status and Fault Commands   *****************************************

// ********************************* BQ769x2 Measurement Commands   *****************************************


uint16_t BQ769x2_ReadVoltage(uint8_t command)
// This function can be used to read a specific cell voltage or stack / pack / LD voltage
{
	//RX_data is global var
	DirectCommands(command, 0x00, R);
	if(command >= Cell1Voltage && command <= Cell16Voltage)
	{//Cells 1 through 16 (0x14 to 0x32)

		/*if(command==Cell4Voltage)
		{
			CellVoltage5_temp=RX_data[0];
			CellVoltage6_temp=RX_data[1];
		}
		if(command==Cell1Voltage)
		{
			CellVoltage1_temp=RX_data[0];
			CellVoltage2_temp=RX_data[1];
		}
		if(command==Cell2Voltage)
		{
			CellVoltage3_temp=RX_data[0];
			CellVoltage4_temp=RX_data[1];
		}*/
		return (RX_data[1]*256 + RX_data[0]); //voltage is reported in mV
	}
	else
	{//stack, Pack, LD
		return 10 * (RX_data[1]*256 + RX_data[0]); //voltage is reported in 0.01V units
	}

}
void BQ769x2_ReadAllVoltages()
// Reads all cell voltages, Stack voltage, PACK pin voltage, and LD pin voltage
{
  int cellvoltageholder = Cell1Voltage; //Cell1Voltage is 0x14
  for (int x = 0; x < 16; x++)
  {//Reads all cell voltages

    if(x==6||x==7)
    {
    	CellVoltage[x]=0;
    }
    else
    {
    	CellVoltage[x] = BQ769x2_ReadVoltage(cellvoltageholder);
    }
    cellvoltageholder = cellvoltageholder + 2;
  }

  Stack_Voltage = BQ769x2_ReadVoltage(StackVoltage);
  Pack_Voltage = BQ769x2_ReadVoltage(PACKPinVoltage);
  LD_Voltage = BQ769x2_ReadVoltage(LDPinVoltage);
}

uint16_t BQ769x2_ReadCurrent()
// Reads PACK current
{
	DirectCommands(CC2Current, 0x00, R);
	return (RX_data[1]*256 + RX_data[0]);  // current is reported in mA
}
uint16_t BQ769x2_BatStatus()
// Reads PACK current
{
	DirectCommands(BatteryStatus, 0x00, R);
	return (RX_data[1]*256 + RX_data[0]);  // current is reported in mA
}
void BQ769x2_ReadDASTATUS5()
// Reads PACK current
{
	Subcommands(DASTATUS5, 0x00, R);
	Pack_CC3_Current = (RX_32Byte[21]*256 + RX_32Byte[20]);  // current is reported in userA

	/*Modified Portion*/
	//Pack_CC1_Current = (RX_32Byte[23]*256 + RX_32Byte[22]);  //current is reported in userA
}

/*Modified Portion*/
/*
void BQ769x2_ReadDASTATUS1()
{
	Subcommands(DASTATUS1, 0x00, R);
	Cell1_ADC_Count_V=(RX_32Byte[3]*16777216+RX_32Byte[2]*65536+RX_32Byte[1]*256 + RX_32Byte[0]);
	Cell2_ADC_Count_V=(RX_32Byte[11]*16777216+RX_32Byte[10]*65536+RX_32Byte[9]*256 + RX_32Byte[8]);
	Cell3_ADC_Count_V=(RX_32Byte[19]*16777216+RX_32Byte[18]*65536+RX_32Byte[17]*256 + RX_32Byte[16]);
	Cell4_ADC_Count_V=(RX_32Byte[27]*16777216+RX_32Byte[26]*65536+RX_32Byte[25]*256 + RX_32Byte[24]);

}*/


float BQ769x2_ReadTemperature(uint8_t command)
{
	DirectCommands(command, 0x00, R);
	//RX_data is a global var
	return (0.1 * (float)(RX_data[1]*256 + RX_data[0])) - 273.15;  // converts from 0.1K to Celcius
}

void BQ769x2_ReadPassQ()
{ // Read Accumulated Charge and Time from DASTATUS6
	Subcommands(DASTATUS6, 0x00, R);
	chg_lowestbyte =RX_32Byte[0];
	chg_lowbyte = RX_32Byte[1];
	chg_highbyte = RX_32Byte[2];
	chg_highestbyte = RX_32Byte[3];
	//acc_charge = ((RX_32Byte[1]<<8) + RX_32Byte[0]);
	AccumulatedCharge_Int = ((RX_32Byte[3]<<24) + (RX_32Byte[2]<<16) + (RX_32Byte[1]<<8) + RX_32Byte[0]); //Bytes 0-3
	AccumulatedCharge_Frac = ((RX_32Byte[7]<<24) + (RX_32Byte[6]<<16) + (RX_32Byte[5]<<8) + RX_32Byte[4]); //Bytes 4-7
	AccumulatedCharge_Time = ((RX_32Byte[11]<<24) + (RX_32Byte[10]<<16) + (RX_32Byte[9]<<8) + RX_32Byte[8]); //Bytes 8-11
}
void BQ769x2_CB_Activecells()
{ // Read Accumulated Charge and Time from DASTATUS6
	Subcommands(CB_ACTIVE_CELLS, 0x00, R);
	CB_ActiveCells = RX_32Byte[1]*256 + RX_32Byte[0];
}
// ********************************* End of BQ769x2 Measurement Commands   *****************************************

//  ********************************* FET Control Commands  ***************************************

void BQ769x2_BOTHOFF ()
{
	// Disables all FETs using the DFETOFF (BOTHOFF) pin
	// The DFETOFF pin on the BQ76952EVM should be connected to the MCU board to use this function
	HAL_GPIO_WritePin(FETOFF_GPIO_Port, FETOFF_Pin, GPIO_PIN_SET);  // DFETOFF pin (BOTHOFF) set high
}

void BQ769x2_RESET_BOTHOFF ()
{
	// Resets DFETOFF (BOTHOFF) pin
	// The DFETOFF pin on the BQ76952EVM should be connected to the MCU board to use this function
	HAL_GPIO_WritePin(FETOFF_GPIO_Port, FETOFF_Pin, GPIO_PIN_RESET);  // DFETOFF pin (BOTHOFF) set low
}

void BQ769x2_ReadFETStatus()
{
	// Read FET Status to see which FETs are enabled
	DirectCommands(FETStatus, 0x00, R);
	FET_Status = (RX_data[1]*256 + RX_data[0]);
	DSG = ((0x4 & RX_data[0])>>2);// discharge FET state
  	CHG = (0x1 & RX_data[0]);// charge FET state
  	PCHG = ((0x2 & RX_data[0])>>1);// pre-charge FET state
  	PDSG = ((0x8 & RX_data[0])>>3);// pre-discharge FET state
}

// ********************************* End of FET Control Commands *********************************

// ********************************* BQ769x2 Power Commands   *****************************************

void BQ769x2_ShutdownPin()
{
	// Puts the device into SHUTDOWN mode using the RST_SHUT pin
	// The RST_SHUT pin on the BQ76952EVM should be connected to the MCU board to use this function
	HAL_GPIO_WritePin(SHUT_GPIO_Port, SHUT_Pin, GPIO_PIN_SET);  // Sets RST_SHUT pin
}

void BQ769x2_ReleaseShutdownPin()
{
	// Releases the RST_SHUT pin
	// The RST_SHUT pin on the BQ76952EVM should be connected to the MCU board to use this function
	HAL_GPIO_WritePin(SHUT_GPIO_Port,SHUT_Pin, GPIO_PIN_RESET);  // Resets RST_SHUT pin
}

// ********************************* End of BQ769x2 Power Commands   *****************************************

/*Modified Portion*/
/*
void update_fault_counts()
{
//Updating fault counts from SafetyStatusA Register
	if ((value_SafetyStatusA&0x04)==0x04)
		CUVFault_Count++;
	else if((value_SafetyStatusA&0x08)==0x08)
		COVFault_Count++;
	else if((value_SafetyStatusA&0x10)==0x10)
		OCCFault_Count++;
	else if((value_SafetyStatusA&0x20)==0x20)
		OCD1Fault_Count++;
	else
	{

	}
	update_faults1_in_flash(OCD1Fault_Count,OCCFault_Count,CUVFault_Count,COVFault_Count);

//Updating fault counts from SafetyStatusB Register
	if((value_SafetyStatusB&0x01)==0x01)
		UTCFault_Count++;
	else if ((value_SafetyStatusB&0x02)==0x02)
		UTDFault_Count++;
	else if ((value_SafetyStatusB&0x10)==0x10)
		OTCFault_Count++;
	else if ((value_SafetyStatusB&0x20)==0x20)
		OTDFault_Count++;
	else
	{

	}

	update_faults2_in_flash(UTDFault_Count,UTCFault_Count,OTDFault_Count,OTCFault_Count);

}

void fault_condition_voltages()
{
	  int cellvoltageholder = Cell1Voltage; //Cell1Voltage is 0x14
	  for (int x = 0; x < 16; x++)
	  {//Reads all cell voltages
	    FaultCellVoltage[x] = BQ769x2_ReadVoltage(cellvoltageholder);
	    cellvoltageholder = cellvoltageholder + 2;
	  }

	  for(int i=0; i<8; i++)
			EEPROM_RAM_REG[505-i]=U16x2toU32(FaultCellVoltage[(2*i)+1],FaultCellVoltage[2*i]);




}*/


