/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * Author: Rohit N Ghosh
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  ******************************************************************************

  TODO:LL UART DRIVER
  TODO: LL I2C DRIVER
  TODO: DOD Algorithm

  NOTE: DISABLE  __HAL_LOCK(huart); for UART Tx and Rx HAL functions.

  NOTE: Timer1,Timer2 and Timer3 are variables. Timer1 and Timer2 are related to TIM3 of the MCU and Timer3 is related to TIM15 of the MCU

	This code is for STM32F072C8Tx microcontroller
  */

/*
 * In this version of code, charger connection and disconnection detection is implemented. Independent WatchDog Timer implemented.
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "bq76952.h"
#include "BQ76952_default_config.h"
#include "flash_072c8.h"
#include "SOCtasks.h"
#include "math.h"
#include "watchdog.h"
#include <stdlib.h>


//#include "fonts.h"
//#include "can.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Temperature sensor calibration value address */
#define FWVER 81
#define HWVER 134
#define BMS_YEAR 25
#define BMS_MONTH 10
#define BMS_DAY 19
#define BMS_ID_1 00
#define BMS_ID_2 00
#define BMS_ID_3 02


#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))
float temperature; /* will contain the temperature in degrees Celsius */
#define PID_OFFSET 0

#define DCDP1 3300
#define DCDP2 3200
#define DCDP3 3100
#define DCDP4 3050
#define DCDP5 3000
#define DCDP6 2950
#define DCDP7 2900
#define DCDP8 2850
#define DCDP9 2800


#define CHDP1 3900
#define CHDP2 4000
#define CHDP3 4150
//#define Total_ID 20250419000012ULL

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

IWDG_HandleTypeDef hiwdg;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

extern uint16_t vcellmode;
uint8_t dataframe[128] = {0};
extern float Temperature[9];
extern uint16_t AlarmBits;
extern int16_t Pack_CC2_Current;
extern int16_t Pack_CC3_Current;

extern uint8_t ProtectionsTriggered;


extern uint16_t value_fetstatus;
extern int32_t AccumulatedCharge_Int; // in BQ769x2_READPASSQ func
extern int32_t AccumulatedCharge_Frac;// in BQ769x2_READPASSQ func
extern int32_t AccumulatedCharge_Time;// in BQ769x2_READPASSQ func
volatile uint16_t adc1Buffer[5];
extern uint16_t CB_ActiveCells;  // Cell Balancing Active Cells
extern uint16_t bat_status;
extern uint16_t CellVoltage [16];
extern int16_t shutdownstackvoltage;
extern uint8_t value_SafetyStatusA;  // Safety Status Register A
extern uint8_t value_SafetyStatusB;  // Safety Status Register B
extern uint8_t value_SafetyStatusC;  // Safety Status Register C
extern uint8_t value_PFStatusA;   // Permanent Fail Status Register A
extern uint8_t value_PFStatusB;   // Permanent Fail Status Register B
extern uint8_t value_PFStatusC;   // Permanent Fail Status Register C
extern uint16_t Stack_Voltage;
extern uint16_t Pack_Voltage;
extern uint8_t FET_Status;

volatile uint16_t Timer1 = 0, Timer2 = 0,Timer3 = 0;
volatile uint8_t wake_timer = 0;
int32_t BATT_SOC = 50;
uint8_t BATT_DOD = 100;
uint8_t BATT_SOH = 100;
uint32_t ShutStVol = 0;
uint16_t pfram;
uint32_t temp = 0;
uint8_t volt_above_SD_vol = 0;
uint8_t state = 3,current_state = 3,previous_state = 3;
uint8_t counter = 0;
uint16_t CellVoltage_Min =0,CellVoltage_Max =0,CellVoltage_Delta = 0;
uint8_t Max_Cell_ID = 0, Min_cell_ID =0;
//uint16_t Temp_Int = 0;
int32_t Temp_Int = 0;
RTC_TimeTypeDef currTime = { 0 };
RTC_DateTypeDef currDate = { 0 };
volatile uint8_t command_buffer[8] = { 0 };
volatile uint8_t message_buffer[8] = { 0 };
uint8_t uart_buffer[32] = { 0 };
uint8_t uart_buffer_ptr = 0;
uint8_t command_source = 0;
uint8_t BMS_ID1[16]={0};
/*Modified Portion*/
uint32_t command_count=0;
uint8_t command_value=0;
char time_str[11];
char date_str[10];
char soc_str[10];
char pack_str[10];
//uint8_t updateflag_1=0;
//uint8_t pow_dis=0;
//uint8_t PA3_val=0;
//uint8_t flag=0;
uint8_t required_current = 20;
//uint8_t voltage_high=58;
//uint8_t voltage_low=10;

//Upper limit is 58.8
uint8_t voltage_high=0xF8;
uint8_t voltage_low=0x16;

//Upper limit is 58.10
//uint8_t voltage_high=0xB2;
//uint8_t voltage_low=0x16;

uint8_t charger_byte=1;
uint8_t MinCellVoltageFlag=0;
float dodcalc = 0.0,driftclac = 0.0;

uint8_t can_transmit_error_flag=0;
uint8_t can_receive_error_flag=0;

float voltage = 0.0;
//float current_temp = 0.0;


/*Modified Portion*/

//Uncomment this if required during debugging
/*
extern uint8_t chg_lowestbyte;
extern uint8_t chg_lowbyte;
extern uint8_t chg_highbyte;
extern uint8_t chg_highestbyte;*/

float previous_temp=0;
float soc_value;
float initial_soc;
float soc_value_1; //For continuous CC
float soc_value_2; //For continuous Kalman
uint8_t nonzero_current_flag=0;

int32_t init_charge=0;
float pressure_sensor_voltage;
float pressure_in_kpa;

//Calibrated values provided in the datasheet for pressure sensor
float a_value=0.00229;
float b_value=-0.01429;
float sense_voltage = 0;

uint16_t sensor_val=0;
uint8_t chargerdetect_flag = 0;
uint8_t configcharger_flag = 0;
uint8_t fullcharge_flag = 0;

uint32_t last_time_plugged = 0;
uint32_t current_plug_time = 0;
uint32_t can_rx_counter = 0;
uint8_t unplug_count=0;
uint8_t voltage_threshold_reached=0;

volatile uint8_t sec=0;
volatile uint8_t min =0;
volatile uint8_t hrs = 0;

volatile uint32_t switch_reset_count = 0;

uint8_t conv_flag =0;
uint32_t battery_capacity = 0;

float acc_charge_val=0;
uint32_t acc_charge_int=0;

uint8_t cuv_protection_released = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM15_Init(void);
static void MX_ADC_Init(void);
static void MX_RTC_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */
void configs();
void state_management();
void SafetyStatusActions();
void Calc_Delta();

/*Function to initialize time*/
void set_time();


uint32_t EEPROM_RAM_REG[512] = {0};
volatile uint8_t buff1AA[32] = { '0' };
void CAN_Filter_config();
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);
void write_dataframe();
void CAN_TX();
void update_message_buffer();
void LED_ON();
void LED_OFF();
void BUZZER_ON();

void GPIO_init();
void cuv_reset();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t same_cond_cntr = 0;
uint32_t cell_uv_timer = 0;

/*Device UID*/
/*
uint32_t UID1=0;
uint32_t UID2=0;
uint32_t UID3=0;*/



void tostring(char str[], uint32_t num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void ADC_Channel1_Init()
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

}

void ADC_TempChannel_Init()
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
/*
static void ull_to_str(unsigned long long value, char *buf)
{
    char temp[32];
    int i = 0, j = 0;

    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value);

    while (i > 0) {
        buf[j++] = temp[--i];
    }
    buf[j] = '\0';
}
void BMS_ID_ASSIGN()
{
	  unsigned long long num_value = Total_ID;
	  char str[32];
	  //uint8_t BMS_ID[16];
	  int count = 0;

	  ull_to_str(num_value, str);
	  int len = strlen(str);

	  for (int i = 0; i < len; i += 2) {
	      if (i + 1 < len) {
	          BMS_ID1[count++] = (uint8_t)((str[i]-'0') * 10 + (str[i+1]-'0'));
	      } else {
	          BMS_ID1[count++] = (uint8_t)(str[i] - '0');
	      }
	  }
}*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
 // BMS_ID_ASSIGN();
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_I2C2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM15_Init();
  MX_ADC_Init();
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */


  HAL_GPIO_WritePin(POW_EN_GPIO_Port, POW_EN_Pin, GPIO_PIN_SET);
  GPIO_init();
  LED_ON();
  HAL_Delay(50);
  LED_OFF();
  HAL_Delay(50);
  LED_ON();
  HAL_Delay(50);
  LED_OFF();
  HAL_Delay(50);
  LED_ON();
  //LED_EXT_ON();

  Flash_Data_init();
  CommandSubcommands(BQ769x2_RESET);  // Resets the BQ769x2 registers
  HAL_Delay(100);

  //Uncomment this for dummy code
  //while(1);



  BQ769x2_Init();  // Configure all of the BQ769x2 register settings


  //Uncomment this for dummy code
  //while(1);
  //Finding the device id
  //UID1 = WAFER_XY;
  //UID2 = WAFERID_LOTLSB;
  //UID3 = LOT_MSB;

 // CommandSubcommands(SLEEP_DISABLE);
  HAL_ADCEx_Calibration_Start(&hadc);
  //HAL_ADC_Start_DMA(&hadc,(uint32_t*) &Temp_Int, 1);

  //HAL_ADC_Start_DMA(&hadc,(uint32_t*) raw_adc_values, 2);

  HAL_TIM_Base_Start_IT(&htim3);

  HAL_TIM_Base_Start_IT(&htim15);

  HAL_GPIO_WritePin(FETOFF_GPIO_Port, FETOFF_Pin, GPIO_PIN_RESET);  // deassert DFETOFF pin (BOTHOFF)
  //while(1); //Uncomment this for testing FETs on BQStudio
  //HAL_Delay(1000);

  CAN_Filter_config();
  HAL_GPIO_WritePin(CAN_STDBY_GPIO_Port,CAN_STDBY_Pin, GPIO_PIN_RESET);  // RST_SHUT pin set low
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF);
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
	 // Error_Handler();
  }
  CommandSubcommands(RESET_PASSQ);
  BQ769x2_ReadPassQ();

  HAL_GPIO_WritePin(MCU_WAKE_GPIO_Port, MCU_WAKE_Pin, GPIO_PIN_RESET);

  /*Modified Portion*/
  //Initializing the LCD screen

  //ssd1306_Init();
	/*Modified Portion*/
	//Displaying on the screen
  //ssd1306_SetCursor(10,10);
  //ssd1306_WriteChar("HELLO",Font_7x10,White);
  //ssd1306_UpdateScreen();
  //SSD1306_UpdateScreen();
  //HAL_PWR_EnterSTANDBYMode();
  LED_OFF();

  //Here it is just for testing. Depending on the value of the pressure sensor, the buzzer should be turned on or off.
  //BUZZER_ON();

  //Estimating the initial SoC for filter parameter initialization.
  BQ769x2_ReadAllVoltages(); //Read all voltages


  //Initializing the Kalman Filter for SoC estimation
  init_filter_parameters(0.5f, 0.01f, 0.001f, 0.01f, BATTERY_CAPACITY);
 // HAL_PWR_EnterSTANDBYMode();
  //HAL_Delay(1000);
  //Initializing the time with backup
/*  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
      {
        set_time();
      }*/

//Checking for Timer 15 operation

/*
  while(1)
  {


	  if (!Timer3) //(!wake_timer)Once a second
	  {
		HAL_GPIO_TogglePin (LED_GPIO_Port, LED_Pin);
			Timer3 = 1;


	  }


  }*/





	  //CommandSubcommands(FET_ENABLE);
	 // CommandSubcommands(PDSGTEST);
  //ssd1306_Init();

  //iwdg_init();
  //MX_IWDG_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //CommandSubcommands(FET_CONTROL);
  MX_IWDG_Init();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  HAL_IWDG_Refresh(&hiwdg);

	 // HAL_PWR_EnterSTANDBYMode();

	  if(!Timer2) //every 100ms during idle 20ms during active
	  {
		    	AlarmBits = BQ769x2_ReadAlarmStatus();
  				//BQ769x2_ReadSafetyStatus();
  				//BQ769x2_ReadPFStatus();
	  			//BQ769x2_ReadPFStatus();

	  			if (AlarmBits & 0x80)
	  			{
	  				/*Use this line only for debugging purposes. It causes RAM issues and hence no proper restart*/
	  				//BQ769x2_ReadDASTATUS1();
	  				// Check if FULLSCAN is complete. If set, new measurements are available
	  				BQ769x2_ReadAllVoltages();
	  				Pack_CC2_Current = BQ769x2_ReadCurrent();
	  				BQ769x2_ReadFETStatus();
	  				DirectCommands(AlarmStatus, 0x0080, W);  // Clear the FULLSCAN bit
		  			Calc_Delta();
		  			BQ769x2_ReadDASTATUS5();
	  			}

	  			if (AlarmBits & 0x2000)
	  			{  // If Safety Status bits are showing in AlarmStatus register
	  				BQ769x2_ReadPFStatus(); // Read the Safety Status registers to find which protections have triggered
	  				if (ProtectionsTriggered & 1)
	  				{
	  					LED_ON();
	  				}// Turn on the LED to indicate Protection has triggered
	  				//DirectCommands(AlarmStatus, 0xF800, W); // Clear the Safety Status Alarm bits.
	  			}



	  			if (AlarmBits & 0xC000)
	  			{  // If Safety Status bits are showing in AlarmStatus register
	  				BQ769x2_ReadSafetyStatus(); // Read the Safety Status registers to find which protections have triggered
	  				//update_time_in_flash();
		  			//read_snapshot_from_flash();

	  				if (ProtectionsTriggered & 1)
	  				{
	  					LED_ON();
	  				}// Turn on the LED to indicate Protection has triggered
	  				DirectCommands(AlarmStatus, 0xF800, W); // Clear the Safety Status Alarm bits.
	  				SafetyStatusActions();
	  			}


   			    else
	  			{
	  				if (ProtectionsTriggered & 1)
	  				{
	  					BQ769x2_ReadSafetyStatus();
	  					if (!(ProtectionsTriggered & 1))
	  					{
	  						LED_OFF();
	  					}
	  				} // Turn off the LED if Safety Status has cleared which means the protection condition is no longer present
	  			}


	  			state_management();


	  			if(state == IDLE)
	  			{
	  				Timer2=100;
	  			}
	  			else
	  			{
	  				Timer2=10;
	  			}
	  }
	  if (!Timer3) //(!wake_timer)Once a second
	  {

		//Can be commented/uncommented whenever required

		  /*
		  if((Pack_Voltage>53000))
			{
				if(voltage_threshold_reached==0)
				{
					required_current = 16;
					//required_current=15;
					voltage_threshold_reached=1;
				}	//protection_released=0;
			}*/


	  			//sprintf((char*)time_str,"%02d:%02d:%02d:%02d",currDate.WeekDay,currTime.Hours,currTime.Minutes,currTime.Seconds);
	  			//sprintf((char*)date_str,"%02d:%02d:%02d",currDate.Date,currDate.Month,2000+currDate.Year);
	  			//update_time_in_flash();
	  			//CommandSubcommands(CHGTEST);
  				//update_time_in_flash();
	  			//fault_condition_voltages();
	  			//time_in_flash();
	  			//update_in_flash();
	  			//read_snapshot_from_flash();

	  			BQ769x2_ReadPassQ();
	  			bat_status = BQ769x2_BatStatus();
	  			BQ769x2_CB_Activecells();

	  			ShutStVol=BQ769x2_ReadRegister(ShutdownStackVoltage);
	  			//temp=BQ769x2_ReadRegister(SCDDelay);

	  			//pfram=BQ769x2_ReadSavedPF();
	  		   // CommandSubcommands(DSG_PDSG_OFF);

	  			if(ShutStVol!=shutdownstackvoltage)
	  			{
	  				configs();
	  			}

	  			if(Stack_Voltage>(shutdownstackvoltage*10) + 50)
	  			{
	  				volt_above_SD_vol = 1;
	  			}
/*
	  			if((Stack_Voltage<(shutdownstackvoltage*10) + 50) && volt_above_SD_vol)
	  			{

	  				same_cond_cntr++;
	  				if(same_cond_cntr > 10)
	  				{
	  					same_cond_cntr = 0;
	  					configs();
	  					CommandSubcommands(SHUTDOWN);
	  					CommandSubcommands(SHUTDOWN);
	  					//pow_dis=1;
	  					HAL_GPIO_WritePin(POW_EN_GPIO_Port, POW_EN_Pin, GPIO_PIN_RESET);  // POW_EN pin set High
	  					//HAL_GPIO_WritePin(POW_EN_GPIO_Port, POW_EN_Pin, GPIO_PIN_SET);  // POW_EN pin set High

	  				}
	  			}
	  			if(AlarmBits==bat_status && bat_status == CellVoltage[0] && CellVoltage[0] == CellVoltage[15])
	  			{
	  				same_cond_cntr++;
	  				if(same_cond_cntr > 10)
	  				{
	  					same_cond_cntr = 0;
	  					configs();
	  					HAL_GPIO_WritePin(POW_EN_GPIO_Port, POW_EN_Pin, GPIO_PIN_RESET);  // POW_EN pin set High
	  					//HAL_GPIO_WritePin(POW_EN_GPIO_Port, POW_EN_Pin, GPIO_PIN_SET);  // POW_EN pin set High

	  				}

	  			}*/

	  			//If the cell under voltage fault is more than 3 minutes, the AFE registers are reset.
	  			//This feature is used if the charger is not able to detect the voltage in CUV fault condition.

	  			//cuv_reset();

	  			//Manually reset the mcu
	  			switch_reset();

	  			//Reading the sensor value and internal temperature
	  		    //sensor_val=raw_adc_values[0];
	  		    //Temp_Int=raw_adc_values[1];


	  			//Disabling Channel 16 and enabling Channel1
	  			ADC1->CHSELR&=~(1<<16);
	  			ADC1->CHSELR|=(1<<1);

	  			//Starting the channel1 sampling and conversion
	  			ADC_Channel1_Init();
	  			HAL_ADC_Start(&hadc);
	  			HAL_ADC_PollForConversion(&hadc, 5);
	  			sensor_val = HAL_ADC_GetValue(&hadc);
	  			HAL_ADC_Stop(&hadc);


	  			//Disabling channel 1 and enabling channel 16
	  			ADC1->CHSELR&=~(1<<1);
	  			ADC1->CHSELR|=(1<<16);

	  			//Starting Channel 16 sampling and conversion
	  			ADC_TempChannel_Init();
	  			HAL_ADC_Start(&hadc);
	  			HAL_ADC_PollForConversion(&hadc, 5);
	  			Temp_Int = HAL_ADC_GetValue(&hadc);
	  			HAL_ADC_Stop(&hadc);





	  			//temperature = (((int32_t) Temp_Int * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
	  			temperature = (Temp_Int * (int32_t)VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR ;
	  			temperature = temperature * (int32_t)(110 - 30);
	  			temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
	  			//temperature = temperature-30; //Check this correction. It is working on some boards and not on others.
	  			//temperature = (Temp_Int-1.43)/((4.3+25)*10);
	  			//sense_voltage = (3.3f/4096) *Temp_Int;
	  			//temperature = (1.43-sense_voltage)/0.0043 + 25;

	  			//Existing thermistor readings
	  			/*
	  			Temperature[0] = BQ769x2_ReadTemperature(TS1Temperature);
  				Temperature[1] = BQ769x2_ReadTemperature(TS3Temperature);
  				Temperature[2] = BQ769x2_ReadTemperature(ALERTTemperature);
  				Temperature[3] = BQ769x2_ReadTemperature(HDQTemperature);
  				Temperature[4] = BQ769x2_ReadTemperature(CFETOFFTemperature);
  				Temperature[5] = BQ769x2_ReadTemperature(DDSGTemperature);
  				Temperature[6] = BQ769x2_ReadTemperature(DCHGTemperature);*/


  				//Modified thermistor readings
	  			Temperature[0] = BQ769x2_ReadTemperature(DDSGTemperature);
  				Temperature[1] = BQ769x2_ReadTemperature(DCHGTemperature);;
  				Temperature[2] = BQ769x2_ReadTemperature(CFETOFFTemperature);
  				Temperature[3] = BQ769x2_ReadTemperature(HDQTemperature);
  				Temperature[4] = BQ769x2_ReadTemperature(ALERTTemperature);
  				Temperature[5] = BQ769x2_ReadTemperature(TS3Temperature);
  				Temperature[6] = BQ769x2_ReadTemperature(TS1Temperature);

  				Temperature[7] = BQ769x2_ReadTemperature(IntTemperature);
  				Temperature[8] = temperature;

  				/*
  				//Thermal Runaway condition
  				//(i) Monitoring the selected cell voltage. It should drop during thermal runaway.
  				 // (ii) Monitoring dT/dt. dT/dt>=(1 degree/1 second)



  				 present_temp = Temperature[1];
  				 present_cell_voltage =
  				 if(((present_cell_voltage-previous_cell_voltage)>1000)&&((present_temp-previous_temp)>=100))
  				 {
  					 LED_ON();
  					 BUZZER_ON();
  					 thermal_runaway_detection=1;
  				 }

  				 if(thermal_runaway_detection==0)
  				 {
  				 	 previous_cell_voltage = present_cell_voltage;
  				 	 previous_temp=present_temp;//Needs to be changed based on the selected thermistor
	  	  	  	  }*/






	  		    //Computing the pressure value
	  		    if(sensor_val>0)
	  		    {
	  		    	pressure_value();
	  		    }
//////////////////////////////////////////////////////////////////////////////SOC STUFF//////////////////////////////////


	  		    //Calculating the charge accumulated value seperately

	  		   acc_charge_val = acc_charge_val+(Pack_CC3_Current/100.0f); //Battery capacity in ampere seconds

	  		   acc_charge_int =  (int)acc_charge_val;

	  		    //Reading all cell voltages before calculating the SoC

	  		    voltage = Compute_Cell_Average(CellVoltage);
	  		    voltage = voltage/1000.0f;


	  			if(voltage>0)
	  			{


	  				//To find out when the current becomes non zero for the first time.
	  				if(nonzero_current_flag==0)
	  				{

	  					/*Modified Portion*/



	  					if(cuv_protection_released == 1)
	  					{
	  						nonzero_current_flag = 1;
	  						cuv_protection_released = 0;
	  						initial_soc = 0.01;
	  						soc_value = initial_soc;
	  					}
	  					else
	  					{
	  						if(abs(Pack_CC3_Current)>10)
	  						{
	  							nonzero_current_flag = 1;
	  						}

		  					initial_soc = compute_soc(voltage,Pack_CC3_Current,nonzero_current_flag,value_SafetyStatusA);
		  					//soc_value = compute_soc(voltage,Pack_CC3_Current,nonzero_current_flag,value_SafetyStatusA);
		  				//soc_value = soc_value - 0.13;
		  					//initial_soc = soc_value;

		  				//Corrections for reset. Needs to be modified

		  					if(initial_soc  >= 0.95)
		  					{
		  						initial_soc = initial_soc+(1-initial_soc);
		  					}
		  					soc_value = initial_soc;

		  				}





	  					/*Previous unmodified*/

	  					/*
	  					if(abs(Pack_CC3_Current)>10)
	  					{
	  						nonzero_current_flag = 1;
	  					}


		  				soc_value = compute_soc(voltage,Pack_CC3_Current,nonzero_current_flag,value_SafetyStatusA);
		  				//soc_value = soc_value - 0.13;
		  				initial_soc = soc_value;

		  				//Corrections for reset. Needs to be modified

		  				if(initial_soc  >= 0.95)
		  				{
		  					initial_soc = initial_soc+(1-initial_soc);
		  				}
		  				if(initial_soc<0.1)
		  				{
		  					initial_soc = 0;
		  				}*/
	  				}
	  				//soc_value = compute_soc(voltage,Pack_CC3_Current,nonzero_current_flag,value_SafetyStatusA);

	  				else
	  				{
	  					//soc_value = compute_soc_cc(Pack_CC3_Current,initial_soc);
	  					soc_value_1 = compute_soc_cc(Pack_CC3_Current,initial_soc,value_SafetyStatusA);
	  					//soc_value_2 = compute_soc(voltage,Pack_CC3_Current,nonzero_current_flag,value_SafetyStatusA);

	  					//Double averaging the obtained SoC values
	  					//soc_value = (soc_value_1 + soc_value_2)/2;
	  					//soc_value = (soc_value +soc_value_1)/2;
	  					soc_value = soc_value_1;
	  					//soc_value = soc_value_1;
	  					//Ensuring that soc is within the range
	  					if (soc_value>1)
	  					{
	  						soc_value = 1;
	  					}
	  					else if(soc_value < 0)
	  					{
	  						soc_value = 0;
	  					}
	  					else
	  					{
	  						soc_value = soc_value;
	  					}
	  				}
	  			}
	  			else
	  			{
	  				soc_value = 0;
	  			}
  				//compute_soc(state,acc_charge,safety_stat,Stack_Voltage);
  				//compute_soc(init_charge,AccumulatedCharge_Int,safety_stat,Stack_Voltage);

  				BATT_SOC = (int)(soc_value*100);

	  			//SoC along with applied correction factor
	  			/*
	  			if(nonzero_current_flag==0)
	  			{
	  				BATT_SOC = (int)(initial_soc*100);
	  			}
	  			else
	  			{
	  				BATT_SOC = (int)(soc_value*100); //Rounding off the float value
	  			}*/
	  			//BATT_SOC = soccalc;
	  			//BATT_SOH = sohcalc;
	  			//ssd1306_SetCursor(10,10);
	  		    //ssd1306_WriteString(sprintf(soc_str,"%d",BATT_SOC), Font_7x10, Blue);
	  		    //ssd1306_UpdateScreen();
	  			//sprintf(soc_str,"%d",BATT_SOC);
	  			//ssd1306_SetCursor(10,10);
	  			//ssd1306_WriteString("Hello", Font_7x10, Blue);
	  			//ssd1306_UpdateScreen();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	  			//Calculating the battery capacity in mAh
	  			//battery_capacity = battery_capacity + (abs(Pack_CC3_Current)/360);

	  			//Calculating the battery capacity in Ah
	  			battery_capacity = battery_capacity + (abs(Pack_CC3_Current)/360000);

	  			wake_timer = 1;
	  			Timer3 = 1;
	  		  command_count++;

		  		//Incrementing the current plugging time

	  		  /*

		  		if(configcharger_flag==1)
		  		{
		  			current_plug_time++;
		  		}*/
	  		  if(configcharger_flag==1)
	  		  {
	  			  if((can_rx_counter==1)&&(last_time_plugged==0))
	  			  {
	  				  unplug_count++;
	  			  }
	  		  }
	  }
	  if(flash_update_config_status())
	  {
		  configs();

	  }
	  if(flash_save_config_status())
	  {
		  save_flash_reg();


	  }
	  if(command_source)
	  {
		  update_message_buffer();
		  if(command_source&0x01)
		  {
			  uint8_t crlf[] ="\r\n";
			  HAL_GPIO_WritePin(REDE_GPIO_Port, REDE_Pin, GPIO_PIN_SET);
			  HAL_UART_Transmit(&huart1,message_buffer,8, HAL_MAX_DELAY);
			  HAL_UART_Transmit(&huart1,crlf,2, HAL_MAX_DELAY);
			  HAL_GPIO_WritePin(REDE_GPIO_Port, REDE_Pin, GPIO_PIN_RESET);
			  command_source = command_source & ~0x01;
		  }
		  if(command_source&0x02)
		  {
			  CAN_TX();
			  command_source = command_source & ~0x02;
		  }
		  uint8_t i=0;
		  for(i = 0 ;i<8;i++)
		  {
			  message_buffer[i] = 0;
		  }
	  }


	  /*Displaying on the LCD Screen*/

	 /* tostring(soc_str,BATT_SOC);
	  ssd1306_SetCursor(10,30);
	  ssd1306_WriteString("SOC=", Font_7x10, Blue);
	  ssd1306_SetCursor(40,30);
	  ssd1306_WriteString(soc_str, Font_7x10, Blue);
	  ssd1306_SetCursor(60,30);
	  ssd1306_WriteString("%", Font_7x10, Blue);
	  ssd1306_SetCursor(10,20);
	  ssd1306_WriteString("PV=", Font_7x10, Blue);
	  tostring(pack_str,Pack_Voltage/1000);
	  ssd1306_SetCursor(30,20);
	  ssd1306_WriteString(pack_str, Font_7x10, Blue);
	  ssd1306_UpdateScreen();*/

	  //Configuring the FETs to detect the charger

	  if((chargerdetect_flag==1) && (configcharger_flag==0))
	  {
		  //CommandSubcommands(BQ769x2_RESET);  // Resets the BQ769x2 registers
		  //HAL_Delay(20);
		  //BQ769x2_Init();  // Configure all of the BQ769x2 register settings
		   if(value_SafetyStatusA==0x04)
		   {
				DirectCommands(SafetyStatusA, 0x00, W); //Clearing the fault in case of CUV
				value_SafetyStatusA = 0x00; //Updating this value for SoC calculation

				nonzero_current_flag=0;

				//Function to reset the charge accumulated value. This will act like reset.
				chg_dsg_done();
  				voltage_threshold_reached=0;


				cuv_protection_released = 1;
		   }

		  PowerConfig_Charger();//Configuring the FETs to be out of sleep mode
		  configcharger_flag=1;// Since charger is configured, this flag is set

	  }



	  //If the charging is complete, configure the FETs back to sleep mode
	  /*if((charger_byte==3)&&(fullcharge_flag==0))
	  {
		  PowerConfig_FullCharge();
		  fullcharge_flag=1;
	  }*/
	  //Checking for charger unplugged event and reconfigure the FETs to sleep mode if unplugged

	  if(configcharger_flag==1)
	  {


		  /*
		  if(value_SafetyStatusA==0x08)
		  {
			  if((current_plug_time-last_time_plugged)>10)
			  {
				   chargerdetect_flag=0;
				   configcharger_flag=0;
				   current_plug_time=0;
				   last_time_plugged=0;


				   PowerConfig_FullCharge();
			  }
		  }*/


		  if(unplug_count>10)
		  {
			   unplug_count=0;
			   chargerdetect_flag=0;
			   configcharger_flag=0;
			   current_plug_time=0;
			   last_time_plugged=0;

			   if(value_SafetyStatusA==0x08)
			   {
					//DirectCommands(SafetyStatusA, 0x00, W); //Clearing the fault in case of CUV
					//value_SafetyStatusA = 0x00; //Updating this value for SoC calculation
					//Estimating the SoC after detecting charger removal at COV fault
				    nonzero_current_flag=0;

					//Function to reset the charge accumulated value. This will act like reset.
					chg_dsg_done();

				   /*
					voltage = Compute_Cell_Average(CellVoltage);
		  		    voltage = voltage/1000.0f;
	  				soc_value = compute_soc(voltage,Pack_CC3_Current,nonzero_current_flag,value_SafetyStatusA);
	  				initial_soc = soc_value;
	  				if(initial_soc  >= 0.95)
	  				{
	  					initial_soc = initial_soc+(1-initial_soc);
	  				}*/
				    //This can be one option for resetting
					//NVIC_SystemReset();

			   }

			   PowerConfig_FullCharge();
		  }

		  /*
		  if((can_rx_counter==1)&&(last_time_plugged==0))
		  {
				 unplug_count++;
				 if(unplug_count >10)
				 {
					  chargerdetect_flag=0;
					  configcharger_flag=0;
					  last_time_plugged=0;
					  unplug_count=0;
					  PowerConfig_FullCharge();

				 }

		  }*/


/*
		  else
		  {
			  if((can_rx_counter-last_time_plugged)>10)
			  {
				   chargerdetect_flag=0;
				   configcharger_flag=0;
				   current_plug_time=0;
				   last_time_plugged=0;

				   PowerConfig_FullCharge();
			  }
		  }*/
	  }

	  //Enable refresh
	  //IWDG->KR =IWDG_REFRESH_ENABLE;
	  //while(1);
  }


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = ENABLE;
  hadc.Init.LowPowerAutoPowerOff = ENABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */
  /*
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }


  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }*/
  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN;
  hcan.Init.Prescaler = 1;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  // Enable CAN Error Interrupts
  __HAL_CAN_ENABLE_IT(&hcan, CAN_IT_ERROR);
  __HAL_CAN_ENABLE_IT(&hcan, CAN_IT_BUSOFF);

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x2000090E;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Window = IWDG_WINDOW_DISABLE;
  hiwdg.Init.Reload = 2000;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 11;
  sTime.Minutes = 53;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JULY;
  sDate.Date = 1;
  sDate.Year = 24;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 8000-1;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 1000-1;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BUZZER_IP_Pin|POW_EN_Pin|REDE_Pin|CAN_STDBY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_EXT_Pin|LED_Pin|SHUT_Pin|FETOFF_Pin
                          |MCU_WAKE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUZZER_IP_Pin POW_EN_Pin REDE_Pin CAN_STDBY_Pin */
  GPIO_InitStruct.Pin = BUZZER_IP_Pin|POW_EN_Pin|REDE_Pin|CAN_STDBY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : GP1_Pin */
  GPIO_InitStruct.Pin = GP1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GP1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_EXT_Pin LED_Pin SHUT_Pin FETOFF_Pin
                           MCU_WAKE_Pin */
  GPIO_InitStruct.Pin = LED_EXT_Pin|LED_Pin|SHUT_Pin|FETOFF_Pin
                          |MCU_WAKE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//XXX User Code Begin 4
void Calc_Delta()
{
	uint8_t i =0;//,temp = vcellmode;
	CellVoltage_Min = CellVoltage[0];
	CellVoltage_Max = CellVoltage[0];
	for (i=0;i<16;i++)
	{
		if((CellVoltage_Min > CellVoltage[i]) && (CellVoltage[i]>1500) && (CellVoltage[i] < 4500))
		{
			CellVoltage_Min = CellVoltage[i];
			Min_cell_ID =i+1;
		}
	}
	for (i=0;i<16;i++)
	{
		if((CellVoltage_Max < CellVoltage[i]) && (CellVoltage[i]>1500) && (CellVoltage[i] < 4500))
		{
			CellVoltage_Max = CellVoltage[i];
			Max_Cell_ID =i+1;
		}
	}
	CellVoltage_Delta = CellVoltage_Max-CellVoltage_Min;

	/*Modified Portion*/
	//If any cell voltage is less than 2.7 voltage flag is raised to 1
	if (CellVoltage_Min<2700)
		MinCellVoltageFlag=1;
	else
		MinCellVoltageFlag=0;
}
void SafetyStatusActions()
{
	if((state == IDLE || state == DISCHARGE)&&(value_SafetyStatusA & 4))
	{
		BQ769x2_ReadPassQ();
		driftclac = 0.0;
		if(AccumulatedCharge_Int !=0)
		{
			CommandSubcommands(RESET_PASSQ);
		}
	}
	if((state == IDLE || state == CHARGE)&&(value_SafetyStatusA & 8))
	{
		BQ769x2_ReadPassQ();
		driftclac = 0.0;
		EEPROM_RAM_REG[507]= AccumulatedCharge_Int;
		if(EEPROM_RAM_REG[507]>EEPROM_RAM_REG[508])
		{
			EEPROM_RAM_REG[508] = EEPROM_RAM_REG[507];
			flash_save_config_set();
		}
	}
}
void configs()
{
	LED_ON();
	HAL_Delay(60);
	BQ769x2_Init();
	HAL_Delay(10);
	LED_OFF();
}
void state_management()
{
	if(Pack_CC3_Current < STATE_DSG_THRESHOLD)
	{
		if(current_state!=DISCHARGE)
		{
			counter = 0;

		}
		current_state = DISCHARGE;
		counter++;
		if(counter>STATE_CHANGE_TIME)
		{
			state = DISCHARGE;
		}
	}
	else if(Pack_CC3_Current > STATE_CHG_THRESHOLD)
	{
		if(current_state!=CHARGE)
		{
			counter = 0;
		}
		current_state = CHARGE;
		counter++;
		if(counter>STATE_CHANGE_TIME)
		{
			state = CHARGE;
		}
	}
	else
	{
		if(current_state!=IDLE)
		{
				counter = 0;
		}
		current_state = IDLE;
		counter++;
		if(counter>STATE_CHANGE_TIME*STATE_IDLE_TIME_MULTIPLIER)
		{
			state = IDLE;
		}
	}

	if(state == CHARGE || state == DISCHARGE)
	{

	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM3 )
	{
		if(Timer1>0)
		{
			Timer1--;
		}
		if(Timer2>0)
		{
			Timer2--;
		}

	}
	if( htim->Instance == TIM15 )
	{
		if(Timer3>0)
		{
			Timer3--;
		}
	}
}
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(wake_timer > 0)
	{
		wake_timer --;
	}
}

void CAN_Filter_config()
{
	CAN_FilterTypeDef can1_filter;
	can1_filter.FilterActivation = ENABLE;
	can1_filter.FilterBank = 0;
	can1_filter.FilterFIFOAssignment = 0;
	can1_filter.FilterIdHigh = 0;
	can1_filter.FilterIdLow = 0;
	can1_filter.FilterMaskIdHigh = 0x00;
	can1_filter.FilterMaskIdLow = 0x00;
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;

	if (HAL_CAN_ConfigFilter(&hcan, &can1_filter) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHead;
	uint8_t rcvd[8] =	{ 0 };

	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHead, rcvd) != HAL_OK)
	{
		can_receive_error_flag=1;
		Error_Handler();
	}

/*	if (RxHead.StdId == EEPROM_RAM_REG[510] || RxHead.ExtId == EEPROM_RAM_REG[510])
	{
			uint8_t i = 0;
			for(i=0;i<8;i++)
			{
				command_buffer[i]=rcvd[i];
			}
			command_source = command_source | 0x02;
	}*/
	/*Modified Portion*/
	//Receiving the data for both 0x1AA and 0x1AD

	//After charger is detected, increment the counter whenever this function is called

	can_rx_counter=0;
	last_time_plugged=0;


	if(configcharger_flag==1)
	{
		can_rx_counter=1;
	}


	if (RxHead.StdId == EEPROM_RAM_REG[510] || RxHead.ExtId == EEPROM_RAM_REG[510])
	{


			uint8_t i = 0;
			for(i=0;i<8;i++)
			{
				command_buffer[i]=rcvd[i];
			}
			command_source = command_source | 0x02;
			//When charger is detected, the sleep mode will be disabled.

			/*
			if (current_plug_time>0)
			{
				last_time_plugged++; //Getting the time when charger is plugged recently
			}*/


			//Detect the last plugged time after configuring the charger


			if(configcharger_flag==1)
			{
				last_time_plugged=1;
				unplug_count=0;
			}


			if((chargerdetect_flag==0)&&(configcharger_flag==0))
			{
				//PowerConfig_Charger();
				chargerdetect_flag=1;
				//fullcharge_flag =0;
			}
	}
	else if (RxHead.StdId == 0x1AD || RxHead.ExtId == 0x1AD)
	{
			uint8_t i = 0;
			for(i=0;i<8;i++)
			{
				command_buffer[i]=rcvd[i];
			}
			command_source = command_source | 0x02;
	}

	//CAN Con address
	else if(RxHead.StdId == 0x1AE || RxHead.ExtId == 0x1AE)
	{
		uint8_t i = 0;
		for(i=0;i<8;i++)
		{
			command_buffer[i]=rcvd[i];
		}
		command_source = command_source | 0x02;

			//LED_ON();
			//HAL_PWR_EnterSTANDBYMode();




	}



}

// Function to reset the CAN peripheral in case of bus off error
void HAL_CAN_Reset(CAN_HandleTypeDef *hcan) {
    // Disable CAN Busoff interrupt and the error interrupt
	__HAL_CAN_DISABLE_IT(hcan,CAN_IT_BUSOFF);
	__HAL_CAN_DISABLE_IT(hcan,CAN_IT_ERROR);

    // Provide a small delay
    HAL_Delay(10);

    // Reinitialize CAN peripheral
    MX_CAN_Init();

    //Turn off the LED indicator
    LED_OFF();
}


//Function to handle error flags that are generated randomly
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{

    /*CAN_FLAG_BOF is a BUS OFF error flag.
     * It indicates that there are severe errors in the network and
     * the controller disconnects itself from the CAN BUS*/

	if (__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_BOF)) {

        //LED_ON();

        // Reset the CAN controller
        HAL_CAN_Reset(hcan);
    }

/*
	if (__HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EPV))
	{
        LED_ON();

        // Reset the CAN controller
        HAL_CAN_Reset(hcan);
	}*/

}

void CAN_TX()
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;
	TxHeader.DLC = 8;
	TxHeader.RTR = CAN_RTR_DATA;
	if(EEPROM_RAM_REG[509] < 0x800)
	{
		TxHeader.StdId = EEPROM_RAM_REG[509];
		TxHeader.IDE = CAN_ID_STD;
	}
	else
	{
		TxHeader.ExtId = EEPROM_RAM_REG[509];
		TxHeader.IDE = CAN_ID_EXT;
	}

	/*Unique extended id*/
	//TxHeader.ExtId=EEPROM_RAM_REG[509]<<16+message[0]<<15+message[2]<<7;
	if (HAL_CAN_AddTxMessage(&hcan,&TxHeader, &message_buffer, &TxMailbox) != HAL_OK)
	{
		can_transmit_error_flag=1;
		Error_Handler();
	}

}
void uart_handler(void)
{
  uint8_t buff;
 if(HAL_UART_Receive(&huart1, (uint8_t *)&buff, 1, 995)!= HAL_OK)
 {
	USART1->ICR |= (1 << 3); // make sure overrun error is cleared, otherwise can stall here
 }
  uart_buffer[uart_buffer_ptr] = buff;
  uart_buffer_ptr++;
  if(uart_buffer_ptr>15)
  {
	  uart_buffer_ptr = 0;
  }
  if(uart_buffer[8] == '\r' && uart_buffer[9] == '\n')
  {
	  uint8_t i;
	  for(i=0;i<8;i++)
	  {
	  		command_buffer[i] = uart_buffer[i];
	  }
	  uart_buffer[8] = 0;
	  uart_buffer[9] = 0;
	  uart_buffer_ptr = 0;
	  command_source =command_source|0x01;
  }
}

void uart_idleHandler()
{
	uart_buffer_ptr = 0;
}




/*
 * Reset using switch
 */
void switch_reset()
{



	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)==GPIO_PIN_SET)
	{

		switch_reset_count++;
		//LED_ON();
		//Turn off the LED when there is COV (Fully charged) or CUV (Fully discharged) and when it is idle
		if(switch_reset_count>2)
		{
			if((value_SafetyStatusA==0x04)||(value_SafetyStatusA==0x08))
			{
				LED_OFF();
			}

			//Turn on the LED in other cases
			else
			{
				LED_ON();
			}
			NVIC_SystemReset();
		}
	}
	else
	{
		switch_reset_count=0;
		//LED_OFF();
	}

}

void update_message_buffer()
{

	if(command_buffer[1]==0x01 ||command_buffer[1]==0x02)
	{
		uint8_t message[8] = {0};
		uint8_t message_type = command_buffer[2];
		if(PID_OFFSET>0)
		{
			message_type = message_type - PID_OFFSET;
		}
		uint8_t cell1 = message_type*2;
		uint8_t cell2 = cell1+1;

		int16_t temperature_temp = 0;
		uint8_t temp1 = 0;

		if(message_type>0x0A)
		{
			temp1 = 2*(message_type-0x0B);
		}
		uint8_t temp2=temp1+1;

		message[0] = 0x06;
		//message[1] = command_buffer[1];
		message[1]=0x02;
		//message[1]=0x01;
		message[2] = message_type;
		if((message_type > 0x00 && message_type < 0x08) || message_type ==0x00)
		{

			/*
			message[6]=CellVoltage[cell1] & 0xFF;
			message[5]=CellVoltage[cell1] >> 0x08;

			message[4]=CellVoltage[cell2] & 0xFF;
			message[3]=CellVoltage[cell2] >> 0x08;*/
			if((cell1<0x06)||(cell1>0x06))
			{
				message[6]=CellVoltage[cell1] & 0xFF;
				message[5]=CellVoltage[cell1] >> 0x08;

				message[4]=CellVoltage[cell2] & 0xFF;
				message[3]=CellVoltage[cell2] >> 0x08;

			}
			else
			{
				message[6]=0;
				message[5]=0;
				message[4]=0;
				message[3]=0;

			}

		}
		else if(message_type == 0x08)
		{
			message[4]=Pack_CC3_Current & 0xFF;
			message[3]=Pack_CC3_Current >> 0x08;

			message[6]=Stack_Voltage & 0xFF;
			message[5]=Stack_Voltage >> 0x08;
		}
		else if(message_type == 0x09)
		{
			message[4]=Pack_Voltage & 0xFF;
			message[3]=Pack_Voltage >> 0x08;

			message[6]=CB_ActiveCells & 0xFF;
			message[5]=CB_ActiveCells >> 0x08;
		}
		else if(message_type == 0x0A)
		{
			message[4]=value_SafetyStatusA;
			message[3]=ProtectionsTriggered;

			message[6]=value_SafetyStatusC;
			message[5]=value_SafetyStatusB;

			if(value_SafetyStatusA==0x08)
				charger_byte=3;
			else if((value_SafetyStatusB!=0)||(value_SafetyStatusC!=0))
				charger_byte=2;
			else
				charger_byte=1;

		}
		/*

		else if((message_type > 0x0A && message_type <0x010 ))
		{
			temperature_temp = Temperature[temp1]*100;
			message[4]=temperature_temp & 0xFF;
			message[3]=temperature_temp >> 0x08;
			if(message_type!=0x0F)
				temperature_temp = Temperature[temp2]*100;
			if(message_type == 0x0F)
				temperature_temp = 0;

			message[6]=temperature_temp & 0xFF;
			message[5]=temperature_temp >> 0x08;
		}*/

		/*Modified Portion*/
		else if((message_type > 0x0A && message_type <0x0F ))
		{
			temperature_temp = Temperature[temp1]*100;
			message[4]=temperature_temp & 0xFF;
			message[3]=temperature_temp >> 0x08;


			temperature_temp = Temperature[temp2]*100;
			message[6]=temperature_temp & 0xFF;
			message[5]=temperature_temp >> 0x08;

		}

		else if(message_type==0x0F)
		{
			temperature_temp = Temperature[temp1]*100;
			//message[3]=temperature_temp >> 0x08;
			message[3]=MinCellVoltageFlag;
            message[4]=required_current;
            message[5]=voltage_high;
            message[6]=voltage_low;
		}
		else if(message_type == 0x10)
		{
			//message[4]=BATT_SOH;
			message[4]=charger_byte;
			message[3]=BATT_SOC;
			message[5]=FET_Status;
			message[6]=state;
		}

		/*Modified Portion*/
		else if (message_type == 0x11)
		{
			message[4]=CellVoltage_Min&0xFF;
			message[3]=CellVoltage_Min>>0x08;
			message[6]=CellVoltage_Max&0xFF;
			message[5]=CellVoltage_Max>>0x08;

		}
		else if (message_type == 0x12)
		{
			message[4]=0x00;
			//message[3]=0x00;
			message[3]=nonzero_current_flag;
			message[6]=Max_Cell_ID;
			message[5]=Min_cell_ID;

		}

		else if (message_type == 0x13)
		{
			temperature_temp = Temperature[8]*100;
			message[4]=temperature_temp&0xFF;
			message[3]=temperature_temp>>0x08;
			message[6]=CellVoltage_Delta&0xFF;
			message[5]=CellVoltage_Delta>>0x08;
		}

		else if (message_type == 0x14)
		{
			message[6]=acc_charge_int&0xFF;
		    message[5]=(acc_charge_int>>8)&0xFF;
		    message[4]=(acc_charge_int>>0x10)&0xFF;
		    message[3]=	(acc_charge_int>>0x18);
		}
		else if(message_type == 0x15)
		{
			message[6]=BMS_MONTH;
		    message[5]=BMS_YEAR;
		    message[4]=FWVER;
		    message[3]=HWVER;


		}

		else if(message_type == 0x16)
		{
			message[6]=BMS_ID_3;
		    message[5]=BMS_ID_2;
		    message[4]=BMS_ID_1;
		    message[3]=BMS_DAY;



		}
		message[7] = message[3]+message[4]+message[5]+message[6];
		uint8_t i = 0;
		for (i=0;i<8;i++)
		{
			message_buffer[i]=message[i];
			command_buffer[i] = 0;
		}
	}
	if(command_buffer[1]==0x10)
	{
		FLASH_COMMAND(command_buffer[2],command_buffer[3],command_buffer[4],command_buffer[5],command_buffer[6],command_buffer[7]);
		uint8_t i = 0;
		for (i=0;i<8;i++)
		{
			message_buffer[i]=command_buffer[i];
			command_buffer[i] = 0;
		}
	}

}
void LED_ON()
{
	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(LED_EXT_GPIO_Port, LED_EXT_Pin, GPIO_PIN_SET); //Turning on the external LED
}
void LED_OFF()
{
	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LED_EXT_GPIO_Port, LED_EXT_Pin, GPIO_PIN_RESET); //Turning off the external LED
}


void BUZZER_ON()
{
	  HAL_GPIO_WritePin(BUZZER_IP_GPIO_Port, BUZZER_IP_Pin, GPIO_PIN_RESET);  // Buzzer ip pin set LOW

}

void BUZZER_OFF()
{
	  HAL_GPIO_WritePin(BUZZER_IP_GPIO_Port, BUZZER_IP_Pin, GPIO_PIN_SET);  // Buzzer ip pin set HIGH

}

void pressure_value()
{
	pressure_sensor_voltage=(sensor_val*(1.0)/4096)*3.3;//VREFINT is 1.23V. Change this value
	pressure_sensor_voltage = pressure_sensor_voltage+0.8;
	pressure_in_kpa = (pressure_sensor_voltage/5)-b_value;
	pressure_in_kpa = pressure_in_kpa/a_value;

	/*
	if ((pressure_in_kpa)>=100)
	{
		BUZZER_ON();
	}
	else
	{
		BUZZER_OFF();
	}*/
	//BUZZER_ON();


}

void GPIO_init()
{
	  HAL_GPIO_WritePin(POW_EN_GPIO_Port, POW_EN_Pin, GPIO_PIN_SET);  // POW_EN pin set High
	  HAL_GPIO_WritePin(SHUT_GPIO_Port,SHUT_Pin, GPIO_PIN_RESET);  // RST_SHUT pin set low
	  HAL_GPIO_WritePin(REDE_GPIO_Port, REDE_Pin, GPIO_PIN_RESET);  // POW_EN pin set High
	  HAL_GPIO_WritePin(CAN_STDBY_GPIO_Port,CAN_STDBY_Pin, GPIO_PIN_SET);  // CAN Low Power Mode
	  //HAL_GPIO_WritePin(CAN_STDBY_GPIO_Port,CAN_STDBY_Pin, GPIO_PIN_RESET);  // CAN High Speed Mode
	  HAL_GPIO_WritePin(FETOFF_GPIO_Port, FETOFF_Pin, GPIO_PIN_SET);  // assert DFETOFF pin (BOTHOFF)
	  HAL_GPIO_WritePin(MCU_WAKE_GPIO_Port, MCU_WAKE_Pin, GPIO_PIN_SET); //wake AFE if in shutdown
	  HAL_GPIO_WritePin(BUZZER_IP_GPIO_Port, BUZZER_IP_Pin, GPIO_PIN_SET);  // Buzzer ip pin set High

}

/*Modified Portion*/
//Function to initialize the time

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();

  while (1)
  {
	  //Handling the CAN transmission error
	  if(can_transmit_error_flag==1)
	  {

		  MX_CAN_Init();
		  HAL_Delay(100);
		  can_transmit_error_flag=0;
		  break;
	  }

	  //Handling the CAN reception error
	  if(can_receive_error_flag==1)
	  {
		  //LED_ON(); //To confirm if the error is in the CAN reception portion
		  MX_CAN_Init();
		  HAL_Delay(100);
		  can_receive_error_flag=0;
		  break;

	  }


  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
