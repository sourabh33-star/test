/*
 * BQ76952_default_config.h
 *
 *  Created on: Aug 13, 2022
 *      Author: Rohit N Ghosh
 */

#ifndef INC_BQ76952_DEFAULT_CONFIG_H_
#define INC_BQ76952_DEFAULT_CONFIG_H

//DEFAULT CONFIG

/********************* CALIBRATION ***********************/

#define CELL1GAIN 12056     // Voltage Cell 1 Gain 0x9180
#define CELL2GAIN 12057	     // Voltage Cell 2 Gain 0x9182
#define CELL3GAIN 12058	     // Voltage Cell 3 Gain 0x9184
#define CELL4GAIN 12058		// Voltage Cell 4 Gain 0x9186
#define CELL5GAIN 12058	// Voltage Cell 5 Gain 0x9188
#define CELL6GAIN 12059//12029		// Voltage Cell 6 Gain 0x918A
#define CELL7GAIN 12113		// Voltage Cell 7 Gain 0x918C
#define CELL8GAIN 12113		// Voltage Cell 8 Gain 0x918E
#define CELL9GAIN 12068//12030		// Voltage Cell 9 Gain 0x9190
#define CELL10GAIN 12065//12040	// Voltage Cell 10 Gain 0x9192
#define CELL11GAIN 12065//12043	// Voltage Cell 11 Gain 0x9194
#define CELL12GAIN 12065//12043	// Voltage Cell 12 Gain 0x9196
#define CELL13GAIN 12065//12040	// Voltage Cell 13 Gain 0x9198
#define CELL14GAIN 12065//12046	// Voltage Cell 14 Gain 0x919A
#define CELL15GAIN 12065//12043	// Voltage Cell 15 Gain 0x919C
#define CELL16GAIN 12065 // Voltage Cell 16 Gain 0x919E

/*
#define CELL1GAIN 12056//12036     // Voltage Cell 1 Gain 0x9180
#define CELL2GAIN 12077//12057	     // Voltage Cell 2 Gain 0x9182
#define CELL3GAIN 12078	//12058	     // Voltage Cell 3 Gain 0x9184
#define CELL4GAIN 12078	//12058		// Voltage Cell 4 Gain 0x9186
#define CELL5GAIN 12078//12058	// Voltage Cell 5 Gain 0x9188
#define CELL6GAIN 12078//12029		// Voltage Cell 6 Gain 0x918A
#define CELL7GAIN 12078//12113		// Voltage Cell 7 Gain 0x918C
#define CELL8GAIN 12078//12113		// Voltage Cell 8 Gain 0x918E
#define CELL9GAIN 12078//12030		// Voltage Cell 9 Gain 0x9190
#define CELL10GAIN 12078//12040	// Voltage Cell 10 Gain 0x9192
#define CELL11GAIN 12078//12043	// Voltage Cell 11 Gain 0x9194
#define CELL12GAIN 12078//12043	// Voltage Cell 12 Gain 0x9196
#define CELL13GAIN 12078//12040	// Voltage Cell 13 Gain 0x9198
#define CELL14GAIN 12078//12046	// Voltage Cell 14 Gain 0x919A
#define CELL15GAIN 12078//12043	// Voltage Cell 15 Gain 0x919C
#define CELL16GAIN 12078//12055 // Voltage Cell 16 Gain 0x919E
*/
/*
#define CELL1GAIN 14292//12292//12036     // Voltage Cell 1 Gain 0x9180
#define CELL2GAIN 14321//12225	     // Voltage Cell 2 Gain 0x9182
#define CELL3GAIN 14323//12186//12058	     // Voltage Cell 3 Gain 0x9184
#define CELL4GAIN 14300//12208//12058		// Voltage Cell 4 Gain 0x9186
#define CELL5GAIN 14315//14284//12245//12058	// Voltage Cell 5 Gain 0x9188
#define CELL6GAIN 14291//12207//12029		// Voltage Cell 6 Gain 0x918A
#define CELL7GAIN 14291//12113		// Voltage Cell 7 Gain 0x918C
#define CELL8GAIN 14291//12201//12113		// Voltage Cell 8 Gain 0x918E
#define CELL9GAIN 14300//12201//12201		// Voltage Cell 9 Gain 0x9190
#define CELL10GAIN 14300//12216//12040	// Voltage Cell 10 Gain 0x9192
#define CELL11GAIN 14292//12180//12043	// Voltage Cell 11 Gain 0x9194
#define CELL12GAIN 14312//12190//12043	// Voltage Cell 12 Gain 0x9196
#define CELL13GAIN 14314//12184//12040	// Voltage Cell 13 Gain 0x9198
#define CELL14GAIN 14304//12236//12046	// Voltage Cell 14 Gain 0x919A
#define CELL15GAIN 14300//12109//12043	// Voltage Cell 15 Gain 0x919C
#define CELL16GAIN 14307//12234//12055 // Voltage Cell 16 Gain 0x919E
*/
#define PACKGAIN 0		// Voltage Pack Gain 0x91A0
#define TOSGAIN 0		// Voltage TOS Gain 0x91A2
#define LDGAIN 0		// Voltage LD Gain 0x91A4
#define ADCGAIN 0		// Voltage ADC Gain 0x91A6

#define CCGAIN 7.4768F				// Current CC Gain 0x91A8
#define CAPACITYGAIN 2230042.463F	// Current Capacity Gain 0x91AC

#define VCELLOFFSET -30//0//732//0           // Vcell Offset Vcell Offset 0x91B0
#define VDIVOFFSET -32				// V Divider Offset Vdiv Offset 0x97B2
#define COULOMBCOUNTEROFFSETSAMPLES 64	// Current Offset Coulomb Counter Offset Samples 0x91C6
#define BOARDOFFSET 0					// Current Offset Board Offset 0x91C8


//Without RC filters

#define INTERNALTEMPOFFSET 0		// Temperature Internal Temp Offset 0x91CA
#define CFETOFFTEMPOFFSET 0			// Temperature CFETOFF Temp Offset 0x91CB
#define DFETOFFTEMPOFFSET 0			// Temperature DFETOFF Temp Offset 0x91CC
#define ALERTTEMPOFFSET 0			// Temperature ALERT Temp Offset 0x91CD
#define TS1TEMPOFFSET 0				// Temperature TS1 Temp Offset 0x91CE
#define TS2TEMPOFFSET 0			// Temperature TS2 Temp Offset 0x91CF
#define TS3TEMPOFFSET 0				// Temperature TS3 Temp Offset 0x91D0
#define HDQTEMPOFFSET 0				// Temperature HDQ Temp Offset 0x91D1
#define DCHGTEMPOFFSET 0			// Temperature DCHG Temp Offset 0x91D2
#define DDSGTEMPOFFSET 0			// Temperature DDSG Temp Offset 0x91D3*/

/*
//With RC filters
#define INTERNALTEMPOFFSET 0		// Temperature Internal Temp Offset 0x91CA
#define CFETOFFTEMPOFFSET -30			// Temperature CFETOFF Temp Offset 0x91CB
#define DFETOFFTEMPOFFSET -30			// Temperature DFETOFF Temp Offset 0x91CC
#define ALERTTEMPOFFSET -30		// Temperature ALERT Temp Offset 0x91CD
#define TS1TEMPOFFSET -30				// Temperature TS1 Temp Offset 0x91CE
#define TS2TEMPOFFSET -30				// Temperature TS2 Temp Offset 0x91CF
#define TS3TEMPOFFSET -30				// Temperature TS3 Temp Offset 0x91D0
#define HDQTEMPOFFSET -30				// Temperature HDQ Temp Offset 0x91D1
#define DCHGTEMPOFFSET -30			// Temperature DCHG Temp Offset 0x91D2
#define DDSGTEMPOFFSET -30			// Temperature DDSG Temp Offset 0x91D3*/




#define INTGAIN 25390		// Internal Temp Model Int Gain 0x91E2
#define INTBASEOFFSET 3032	// Internal Temp Model Int base offset 0x91E4
#define INTMAXIMUMAD 16383	// Internal Temp Model Int Maximum AD 0x91E6
#define INTMAXIMUMTEMP 6379	// Internal Temp Model Int Maximum Temp 0x91E8

#define T18KCOEFFA1 -15524	// 18K Temperature Model Coeff a1 0x91EA
#define T18KCOEFFA2 26423	// 18K Temperature Model Coeff a2 0x91EC
#define T18KCOEFFA3 -22664	// 18K Temperature Model Coeff a3 0x91EE
#define T18KCOEFFA4 28834	// 18K Temperature Model Coeff a4 0x91F0
#define T18KCOEFFA5 672		// 18K Temperature Model Coeff a5 0x91F2
#define T18KCOEFFB1 -371	// 18K Temperature Model Coeff b1 0x91F4
#define T18KCOEFFB2 708		// 18K Temperature Model Coeff b2 0x91F6
#define T18KCOEFFB3 -3498	// 18K Temperature Model Coeff b3 0x91F8
#define T18KCOEFFB4 5051	// 18K Temperature Model Coeff b4 0x91FA
#define T18KADC0 11703		// 18K Temperature Model Adc0 0x91FE

#define T180KCOEFFA1 -17513	// 180K Temperature Model Coeff a1 0x9200
#define T180KCOEFFA2 25759	// 180K Temperature Model Coeff a2 0x9202
#define T180KCOEFFA3 -23593	// 180K Temperature Model Coeff a3 0x9204
#define T180KCOEFFA4 32175	// 180K Temperature Model Coeff a4 0x9206
#define T180KCOEFFA5 2090	// 180K Temperature Model Coeff a5 0x9208
#define T180KCOEFFB1 -2055	// 180K Temperature Model Coeff b1 0x920A
#define T180KCOEFFB2 2955	// 180K Temperature Model Coeff b2 0x920C
#define T180KCOEFFB3 -3427	// 180K Temperature Model Coeff b3 0x920E
#define T180KCOEFFB4 4385	// 180K Temperature Model Coeff b4 0x9210
#define T180KADC0 17246		// 180K Temperature Model Adc0 0x9214

#define CUSTOMCOEFFA1 0		// Custom Temperature Model Coeff a1 0x9216
#define CUSTOMCOEFFA2 0		// Custom Temperature Model Coeff a2 0x9218
#define CUSTOMCOEFFA3 0		// Custom Temperature Model Coeff a3 0x921A
#define CUSTOMCOEFFA4 0		// Custom Temperature Model Coeff a4 0x921C
#define CUSTOMCOEFFA5 0		// Custom Temperature Model Coeff a5 0x921E
#define CUSTOMCOEFFB1 0		// Custom Temperature Model Coeff b1 0x9220
#define CUSTOMCOEFFB2 0		// Custom Temperature Model Coeff b2 0x9222
#define CUSTOMCOEFFB3 0		// Custom Temperature Model Coeff b3 0x9224
#define CUSTOMCOEFFB4 0		// Custom Temperature Model Coeff b4 0x9226
#define CUSTOMRC0 0			// Custom Temperature Model Rc0 0x9228
#define CUSTOMADC0 0		// Custom Temperature Model Adc0 0x922A

#define COULOMBCOUNTERDEADBAND 9	// Current Deadband Coulomb Counter Deadband 0x922D

#define CUVTHRESHOLDOVERRIDE 0xFFFF	// CUV CUV Threshold Override 0x91D4
#define COVTHRESHOLDOVERRIDE 0xFFFF	// COV COV Threshold Override 0x91D6

/********************* SETTINGS ***********************/

#define MINBLOWFUSEVOLTAGE 6000		// Fuse Min Blow Fuse Voltage 0x9231
#define FUSEBLOWTIMEOUT 30			// Fuse Fuse Blow Timeout 0x9233

/*To enable sleep mode*/
//#define POWERCONFIG 0x29BC // 0x29BC//0x2D80 // 0x2982			// Configuration Power Config 0x9234

/*To enable FAST ADC mode*/
//#define POWERCONFIG 0x298C
//Disable sleep mode for testing
#define POWERCONFIG 0x28BC

/*To disable sleep mode*/
//#define POWERCONFIG_NOSLEEP 0x28BC // 0x29BC//0x2D80 // 0x2982			// Configuration Power Config 0x9234

/*To enable FAST ADC mode*/
#define POWERCONFIG_NOSLEEP 0x288C

#define REG12CONFIG 0xFD // 0x00			// Configuration REG12 Config 0x9236
#define REG0CONFIG 0x01 //0x00				// Configuration REG0 Config 0x9237
#define HWDREGULATOROPTIONS 0x00	// Configuration HWD Regulator Options 0x9238

#define COMMTYPE 0				// Configuration Comm Type 0x9239
#define I2CADDRESS 0			// Configuration I2C Address 0x923A
#define SPICONFIGURATION 0x20	// Configuration SPI Configuration 0x923C
#define COMMIDLETIME 0			// Configuration Comm Idle Time 0x923D

#define CFETOFFPINCONFIG 0x07	// Configuration CFETOFF Pin Config 0x92FA
#define DFETOFFPINCONFIG 0x42 //0x00	// Configuration DFETOFF Pin Config 0x92FB
#define ALERTPINCONFIG 0x07 //0x00		// Configuration ALERT Pin Config 0x92FC
#define TS1CONFIG 0x07			// Configuration TS1 Config 0x92FD
#define TS2CONFIG 0x00			// Configuration TS2 Config 0x92FE
#define TS3CONFIG 0x07 //0x00			// Configuration TS3 Config 0x92FF
#define HDQPINCONFIG 0x07 //0x00		// Configuration HDQ Pin Config 0x9300
/*This config gives FET over temperature protection*/
//#define DCHGPINCONFIG 0x0F		// Configuration DCHG Pin Config 0x9301

/*Modified Portion*/
#define DCHGPINCONFIG 0x07		// Configuration DCHG Pin Config 0x9301
#define DDSGPINCONFIG 0x07		// Configuration DDSG Pin Config 0x9302

#define DACONFIGURATION 0x06 //0x05	// Configuration DA Configuration 0x9303

/*Modified Portion*/
//This configuration is to calculate sleep mode current
//#define DACONFIGURATION 0x02 //0x05	// Configuration DA Configuration 0x9303


#define VCELLMODE 0xFF3F //0x0000		// Configuration Vcell Mode 0x9304

#define CC3SAMPLES 80			// Configuration CC3 Samples 0x9307

#define PROTECTIONCONFIGURATION 0x0602 // 0x0002	// Protection Protection Configuration 0x925F

#define ENABLEDPROTECTIONSA 0xFC //0x88		// Protection Enabled Protections A 0x9261




#define ENABLEDPROTECTIONSB 0xF0 //0xF7		// Protection Enabled Protections B 0x9262

/*Modified Portion to disable temperature protections*/
//#define ENABLEDPROTECTIONSB 0x00

#define ENABLEDPROTECTIONSC 0xF0 //0x00		// Protection Enabled Protections C 0x9263

#define CHGFETPROTECTIONSA 0x98			// Protection CHG FET Protections A 0x9265
#define CHGFETPROTECTIONSB 0xD5 //0x44 //0xD5			// Protection CHG FET Protections B 0x9266
#define CHGFETPROTECTIONSC 0x50 //0x56			// Protection CHG FET Protections C 0x9267

#define DSGFETPROTECTIONSA 0xE4			// Protection DSG FET Protections A 0x9269
#define DSGFETPROTECTIONSB 0xE6 //0x60 //0xE6			// Protection DSG FET Protections B 0x926A
#define DSGFETPROTECTIONSC 0xe2 // 0x60 //0xE2			// Protection DSG FET Protections C 0x926B

#define BODYDIODETHRESHOLD 50			// Protection Body Diode Threshold 0x9273

#define DEFAULTALARMMASK 0xF882 //0xF800			// Alarm Default Alarm Mask 0x926D

#define SFALERTMASKA 0xFC	// Alarm SF Alert Mask A 0x926F
#define SFALERTMASKB 0xF7	// Alarm SF Alert Mask B 0x9270
#define SFALERTMASKC 0xF4	// Alarm SF Alert Mask C 0x9271
#define PFALERTMASKA 0x5F	// Alarm PF Alert Mask A 0x92C4
#define PFALERTMASKB 0x9F	// Alarm PF Alert Mask B 0x92C5
#define PFALERTMASKC 0x00	// Alarm PF Alert Mask C 0x92C6
#define PFALERTMASKD 0x00	// Alarm PF Alert Mask D 0x92C7

#define ENABLEDPFA 0x8f		// Permanent Failure Enabled PF A 0x92C0
#define ENABLEDPFB 0x00		// Permanent Failure Enabled PF B 0x92C1
#define ENABLEDPFC 0x77		// Permanent Failure Enabled PF C 0x92C2
#define ENABLEDPFD 0x01	// Permanent Failure Enabled PF D 0x92C3

#define FETOPTIONS 0x1D				// FET FET Options 0x9308

/*Modified for SparkyV1.3*/
//#define FETOPTIONS 0x0D

#define CHGPUMPCONTROL 0x05			// FET Chg Pump Control 0x9309
#define PRECHARGESTARTVOLTAGE 0		// FET Precharge Start Voltage 0x930A

#define PRECHARGESTOPVOLTAGE 31000 // FET Precharge Stop Voltage 0x930C

/*Modified Portion*/
//#define PRECHARGESTOPVOLTAGE 22000 // FET Precharge Stop Voltage 0x930C


//#define PREDISCHARGETIMEOUT 100		// FET Predischarge Timeout 0x930E

/*Modified Portion*/
//Predischarge time out made 0 to exit based on voltage conditions
#define PREDISCHARGETIMEOUT 255

#define PREDISCHARGESTOPDELTA 100	// FET Predischarge Stop Delta 0x930F

/*Modified Portion*/
//Predischarge stop delta to exit based on voltage condition
//#define PREDISCHARGESTOPDELTA 250	// FET Predischarge Stop Delta 0x930F


#define DSGCURRENTTHRESHOLD 10	// Current Thresholds Dsg Current Threshold 0x9310
#define CHGCURRENTTHRESHOLD 5		// Current Thresholds Chg Current Threshold 0x9312

#define CHECKTIME 5					// Cell Open-Wire Check Time 0x9314

#define CELL1INTERCONNECT 1		// Interconnect Resistances Cell 1 Interconnect 0x9315
#define CELL2INTERCONNECT 1		// Interconnect Resistances Cell 2 Interconnect 0x9317
#define CELL3INTERCONNECT 1		// Interconnect Resistances Cell 3 Interconnect 0x9319
#define CELL4INTERCONNECT 1		// Interconnect Resistances Cell 4 Interconnect 0x931B
#define CELL5INTERCONNECT 1		// Interconnect Resistances Cell 5 Interconnect 0x931D
#define CELL6INTERCONNECT 1		// Interconnect Resistances Cell 6 Interconnect 0x931F
#define CELL7INTERCONNECT 1		// Interconnect Resistances Cell 7 Interconnect 0x9321
#define CELL8INTERCONNECT 1		// Interconnect Resistances Cell 8 Interconnect 0x9323
#define CELL9INTERCONNECT 1		// Interconnect Resistances Cell 9 Interconnect 0x9325
#define CELL10INTERCONNECT 1	// Interconnect Resistances Cell 10 Interconnect 0x9327
#define CELL11INTERCONNECT 1	// Interconnect Resistances Cell 11 Interconnect 0x9329
#define CELL12INTERCONNECT 1	// Interconnect Resistances Cell 12 Interconnect 0x932B
#define CELL13INTERCONNECT 1	// Interconnect Resistances Cell 13 Interconnect 0x932D
#define CELL14INTERCONNECT 1	// Interconnect Resistances Cell 14 Interconnect 0x932F
#define CELL15INTERCONNECT 1	// Interconnect Resistances Cell 15 Interconnect 0x9331
#define CELL16INTERCONNECT 1	// Interconnect Resistances Cell 16 Interconnect 0x9333

#define MFGSTATUSINIT 0x50	// Manufacturing Mfg Status Init 0x9343

#define BALANCINGCONFIGURATION 0x17 //0x00	// Cell Balancing Config Balancing Configuration 0x9335

#define MINCELLTEMP -20				// Cell Balancing Config Min Cell Temp 0x9336
#define MAXCELLTEMP 60				// Cell Balancing Config Max Cell Temp 0x9337
#define MAXINTERNALTEMP 70			// Cell Balancing Config Max Internal Temp 0x9338

#define CELLBALANCEINTERVAL 20		// Cell Balancing Config Cell Balance Interval 0x9339
#define CELLBALANCEMAXCELLS 4 //1		// Cell Balancing Config Cell Balance Max Cells 0x933A

#define CELLBALANCEMINCELLVCHARGE 3900 //3900	// Cell Balancing Config Cell Balance Min Cell V (Charge) 0x933B

/*Modified Portion*/
//#define CELLBALANCEMINCELLVCHARGE 2200 //3900	// Cell Balancing Config Cell Balance Min Cell V (Charge) 0x933B

#define CELLBALANCEMINDELTACHARGE 30	// Cell Balancing Config Cell Balance Min Delta (Charge) 0x933D
#define CELLBALANCESTOPDELTACHARGE 10 //20	// Cell Balancing Config Cell Balance Stop Delta (Charge) 0x933E
#define CELLBALANCEMINCELLVRELAX 4000	// Cell Balancing Config Cell Balance Min Cell V (Relax) 0x933F
#define CELLBALANCEMINDELTARELAX 20 //40		// Cell Balancing Config Cell Balance Min Delta (Relax) 0x9341
#define CELLBALANCESTOPDELTARELAX 10// 20	// Cell Balancing Config Cell Balance Stop Delta (Relax) 0x9342

/********************* POWER ***********************/

#define SHUTDOWNCELLVOLTAGE 0000			// Shutdown Shutdown Cell Voltage 0x923F
#define SHUTDOWNSTACKVOLTAGE 2500		// Shutdown Shutdown Stack Voltage 0x9241
#define LOWVSHUTDOWNDELAY 0				// Shutdown Low V Shutdown Delay 0x9243
#define SHUTDOWNTEMPERATURE 85			// Shutdown Shutdown Temperature 0x9244
#define SHUTDOWNTEMPERATUREDELAY 5		// Shutdown Shutdown Temperature Delay 0x9245

/*Modified here for v1.3*/
//#define FETOFFDELAY 1			// Shutdown FET Off Delay 0x9252
#define FETOFFDELAY 0

#define SHUTDOWNCOMMANDDELAY 4	// Shutdown Shutdown Command Delay 0x9253
#define AUTOSHUTDOWNTIME 1		// Shutdown Auto Shutdown Time 0x9254

/*Modified here for v1.3*/
//#define AUTOSHUTDOWNTIME 0
#define RAMFAILSHUTDOWNTIME 5	// Shutdown RAM Fail Shutdown Time 0x9255

#define SLEEPCURRENT 20			// Sleep Sleep Current 0x9248
#define VOLTAGETIME 5			// Sleep Voltage Time 0x924A
#define WAKECOMPARATORCURRENT 500			// Sleep Wake Comparator Current 0x924B
#define SLEEPHYSTERESISTIME 10// Sleep Sleep Hysteresis Time 0x924D
#define SLEEPCHARGERVOLTAGETHRESHOLD 5800	// Sleep Sleep Charger Voltage Threshold 0x924E
#define SLEEPCHARGERPACKTOSDELTA 200		// Sleep Sleep Charger PACK-TOS Delta 0x9250

/********************* SYSTEM DATA ***********************/

#define CONFIGRAMSIGNATURE 0		// Integrity Config RAM Signature 0x91E0

/********************* PROTECTIONS ***********************/

//Reducing the limit for testing in cycler
#define CUVTHRESHOLD 52 //2.53V				// CUV Threshold 0x9275

//Actual setting in the BMS
//#define CUVTHRESHOLD 60 //50		//3.036V		// CUV Threshold 0x9275
#define CUVDELAY 1515 //303 //74					// CUV Delay 0x9276
#define CUVRECOVERYHYSTERESIS 9 //2		// CUV Recovery Hysteresis 0x927B

//Increasing the limit for testing in cycler
#define COVTHRESHOLD 82//82 //4.199V 				// COV Threshold 0x9278

//Actual setting in the BMS
//#define COVTHRESHOLD 82 //86			//4.149V	// COV Threshold 0x9278

#define COVDELAY 1515//303 //74					// COV Delay 0x9279
//#define COVRECOVERYHYSTERESIS 5 //2		// COV Recovery Hysteresis 0x927C

/*Modified Portion*/
#define COVRECOVERYHYSTERESIS 4 //2		// COV Recovery Hysteresis 0x927C

#define COVLLATCHLIMIT 3 //0			// COVL Latch Limit 0x927D
#define COVLCOUNTERDECDELAY 60 //10		// COVL Counter Dec Delay 0x927E
#define COVLRECOVERYTIME 200			// COVL Recovery Time 0x927F

#define OCCTHRESHOLD 13 //2				// OCC Threshold 0x9280
//#define OCCTHRESHOLD 5 //2				// OCC Threshold 0x9280
//#define OCCDELAY 1					//20ms delay // OCC Delay 0x9281

//Change the delay for testing purpose
#define OCCDELAY 13 // 40ms delay
#define OCCRECOVERYTHRESHOLD -200	// OCC Recovery Threshold 0x9288
#define OCCPACKTOSDELTA 200			// OCC PACK-TOS Delta 0x92B0

//#define OCD1THRESHOLD 10 //40 //4			// OCD1 Threshold 0x9282 units of 2mV

/*Modified Portion*/
#define OCD1THRESHOLD 33 //40 //4			// OCD1 Threshold 0x9282 units of 2mV

#define OCD1DELAY 4 //20ms delay			// OCD1 Delay 0x9283
//#define OCD1DELAY 1

#define OCD2THRESHOLD 35 //3			// OCD2 Threshold 0x9284

/*Modified Portion*/
//#define OCD2THRESHOLD 30 //3			// OCD2 Threshold 0x9284
#define OCD2DELAY 15 //7				// OCD2 Delay 0x9285

#define SCDTHRESHOLD 4//20		//4	// SCD Threshold 0x9286
#define SCDDELAY 2 //31 //2				// SCD Delay 0x9287

/*Modified Portion*/
//#define SCDTHRESHOLD 4              //20A
//#define SCDDELAY 0
#define SCDRECOVERYTIME  10//3 //5		// SCD Recovery Time 0x9294
//#define SCDRECOVERYTIME 5 //5		// SCD Recovery Time 0x9294

#define OCD3THRESHOLD -6000		// OCD3 Threshold 0x928A
#define OCD3DELAY 2//2				// OCD3 Delay 0x928C

#define OCDRECOVERYTHRESHOLD 200 //200	// OCD Recovery Threshold 0x928D

#define OCDLLATCHLIMIT 2//0			// OCDL Latch Limit 0x928F
#define OCDLCOUNTERDECDELAY 100 //10		// OCDL Counter Dec Delay 0x9290
#define OCDLRECOVERYTIME 110 //15			// OCDL Recovery Time 0x9291
#define OCDLRECOVERYTHRESHOLD 200 //200	// OCDL Recovery Threshold 0x9292

//#define SCDLLATCHLIMIT 2 //0			// SCDL Latch Limit 0x9295
#define SCDLLATCHLIMIT 2
#define SCDLCOUNTERDECDELAY 10 //2//10		// SCDL Counter Dec Delay 0x9296
#define SCDLRECOVERYTIME 15 //3 //15			// SCDL Recovery Time 0x9297
#define SCDLRECOVERYTHRESHOLD 200 // 200	// SCDL Recovery Threshold 0x9298

//#define OTCTHRESHOLD 55 //55			// OTC Threshold 0x929A
#define OTCTHRESHOLD 60 //55			// OTC Threshold 0x929A
#define OTCDELAY 6 //2				// OTC Delay 0x929B
#define OTCRECOVERY 55 //50			// OTC Recovery 0x929C
//#define OTCRECOVERY 60 //50			// OTC Recovery 0x929C
//#define OTDTHRESHOLD 85 //60			// OTD Threshold 0x929D
#define OTDTHRESHOLD 60 //60			// OTD Threshold 0x929D
#define OTDDELAY 5 //2				// OTD Delay 0x929E
//#define OTDRECOVERY 80 //55			// OTD Recovery 0x929F
#define OTDRECOVERY 55 //55			// OTD Recovery 0x929F

#define OTFTHRESHOLD 80			// OTF Threshold 0x92A0
#define OTFDELAY 2				// OTF Delay 0x92A1
#define OTFRECOVERY 65			// OTF Recovery 0x92A2
//#define OTINTTHRESHOLD 85		// OTINT Threshold 0x92A3
#define OTINTTHRESHOLD 80
#define OTINTDELAY 2			// OTINT Delay 0x92A4
//#define OTINTRECOVERY 80		// OTINT Recovery 0x92A5
#define OTINTRECOVERY 70		// OTINT Recovery 0x92A5


#define UTCTHRESHOLD 0			// UTC Threshold 0x92A6
#define UTCDELAY 5				// UTC Delay 0x92A7
#define UTCRECOVERY 5			// UTC Recovery 0x92A8
#define UTDTHRESHOLD 0			// UTD Threshold 0x92A9
#define UTDDELAY 5				// UTD Delay 0x92AA
#define UTDRECOVERY 5			// UTD Recovery 0x92AB
#define UTINTTHRESHOLD -20		// UTINT Threshold 0x92AC
#define UTINTDELAY 2			// UTINT Delay 0x92AD
#define UTINTRECOVERY -15		// UTINT Recovery 0x92AE

#define PROTECTIONSRECOVERYTIME 1	// Recovery Time 0x92AF

#define HWDDELAY 60					// HWD Delay 0x92B2

#define LOADDETECTACTIVETIME 5		// Load Detect Active Time 0x92B4.
										//If it is 0, Load detection is disabled.
//#define LOADDETECTACTIVETIME 5 //0		// Load Detect Active Time 0x92B4.

#define LOADDETECTRETRYDELAY 50		// Load Detect Retry Delay 0x92B5
#define LOADDETECTTIMEOUT 1			// Load Detect Timeout 0x92B6

#define PTOCHARGETHRESHOLD 250		// PTO Charge Threshold 0x92BA
#define PTODELAY 1800				// PTO Delay 0x92BC
#define PTORESET 2					// PTO Reset 0x92BE

/********************* PERMANENT FAIL ***********************/

#define CUDEPTHRESHOLD 1500			// CUDEP Threshold 0x92C8
#define CUDEPDELAY 2				// CUDEP Delay 0x92CA

#define SUVTHRESHOLD 2200		// SUV Threshold 0x92CB
#define SUVDELAY 5				// SUV Delay 0x92CD

#define SOVTHRESHOLD 4500		// SOV Threshold 0x92CE
#define SOVDELAY 5				// SOV Delay 0x92D0

#define TOSSTHRESHOLD 500		// TOS Threshold 0x92D1
#define TOSSDELAY 5				// TOS Delay 0x92D3

#define SOCCTHRESHOLD 10000		// SOCC Threshold 0x92D4
#define SOCCDELAY 5				// SOCC Delay 0x92D6
#define SOCDTHRESHOLD -32000	// SOCD Threshold 0x92D7
#define SOCDDELAY 5				// SOCD Delay 0x92D9

#define SOTTHRESHOLD 65			// SOT Threshold 0x92DA
#define SOTDELAY 5				// SOT Delay 0x92DB
#define SOTFTHRESHOLD 85		// SOTF Threshold 0x92DC
#define SOTFDELAY 5				// SOTF Delay 0x92DD

#define VIMRCHECKVOLTAGE 3500	// VIMR Check Voltage 0x92DE
#define VIMRMAXRELAXCURRENT 10	// VIMR Max Relax Current 0x92E0
#define VIMRTHRESHOLD 500		// VIMR Threshold 0x92E2
#define VIMRDELAY 5					// VIMR Delay 0x92E4
#define VIMRRELAXMINDURATION 100	// VIMR Relax Min Duration 0x92E5
#define VIMACHECKVOLTAGE 3700		// VIMA Check Voltage 0x92E7
#define VIMAMINACTIVECURRENT 50		// VIMA Min Active Current 0x92E9
#define VIMATHRESHOLD 200			// VIMA Threshold 0x92EB
#define VIMADELAY 5					// VIMA Delay 0x92ED

#define CFETFOFFTHRESHOLD 20		// CFETF OFF Threshold 0x92EE
#define CFETFOFFDELAY 5				// CFETF OFF Delay 0x92F0
#define DFETFOFFTHRESHOLD -20		// DFETF OFF Threshold 0x92F1
#define DFETFOFFDELAY 5				// DFETF OFF Delay 0x92F3

#define VSSFFAILTHRESHOLD 100		// VSSF Fail Threshold 0x92F4
#define VSSFDELAY 5					// VSSF Delay 0x92F6
#define PF2LVLDELAY 5				// 2LVL Delay 0x92F7
#define LFOFDELAY 5					// LFOF Delay 0x92F8
#define HWMXDELAY 5					// HWMX Delay 0x92F9

/********************* SECURITY ***********************/

#define SECURITYSETTINGS 0x00		// Settings Security Settings 0x9256

#define UNSEALKEYSTEP1 0x0414		// Keys Unseal Key Step 1 0x9257
#define UNSEALKEYSTEP2 0x3672		// Keys Unseal Key Step 2 0x9259
#define FULLACCESSKEYSTEP1 0xFFFF	// Keys Full Access Key Step 1 0x925B
#define FULLACCESSKEYSTEP2 0xFFFF	// Keys Full Access Key Step 2 0x925D

/*Modified Portion*/
/********************DATE AND TIME SETTINGS*************/


//#define EEPROM_TEST_BYTE CELL1GAIN+CELL2GAIN+CELL3GAIN+CELL4GAIN+CELL5GAIN+CELL6GAIN+CELL7GAIN+CELL8GAIN+CELL9GAIN+CELL10GAIN+CELL11GAIN+CELL12GAIN+CELL13GAIN+CELL14GAIN+CELL15GAIN+CELL16GAIN+PACKGAIN+TOSGAIN+LDGAIN+ADCGAIN+CCGAIN+CAPACITYGAIN+VCELLOFFSET+VDIVOFFSET+COULOMBCOUNTEROFFSETSAMPLES+BOARDOFFSET+INTERNALTEMPOFFSET+CFETOFFTEMPOFFSET+DFETOFFTEMPOFFSET+ALERTTEMPOFFSET+TS1TEMPOFFSET+TS2TEMPOFFSET+TS3TEMPOFFSET+HDQTEMPOFFSET+DCHGTEMPOFFSET+DDSGTEMPOFFSET+INTGAIN+INTBASEOFFSET+INTMAXIMUMAD+INTMAXIMUMTEMP+T18KCOEFFA1+T18KCOEFFA2+T18KCOEFFA3+T18KCOEFFA4+T18KCOEFFA5+T18KCOEFFB1+T18KCOEFFB2+T18KCOEFFB3+T18KCOEFFB4+T18KADC0+T180KCOEFFA1+T180KCOEFFA2+T180KCOEFFA3+T180KCOEFFA4+T180KCOEFFA5+T180KCOEFFB1+T180KCOEFFB2+T180KCOEFFB3+T180KCOEFFB4+T180KADC0+CUSTOMCOEFFA1+CUSTOMCOEFFA2+CUSTOMCOEFFA3+CUSTOMCOEFFA4+CUSTOMCOEFFA5+CUSTOMCOEFFB1+CUSTOMCOEFFB2+CUSTOMCOEFFB3+CUSTOMCOEFFB4+CUSTOMRC0+CUSTOMADC0+COULOMBCOUNTERDEADBAND+CUVTHRESHOLDOVERRIDE+COVTHRESHOLDOVERRIDE+MINBLOWFUSEVOLTAGE+FUSEBLOWTIMEOUT+POWERCONFIG+REG12CONFIG+REG0CONFIG+HWDREGULATOROPTIONS+COMMTYPE+I2CADDRESS+SPICONFIGURATION+COMMIDLETIME+CFETOFFPINCONFIG+DFETOFFPINCONFIG+ALERTPINCONFIG+TS1CONFIG+TS2CONFIG+TS3CONFIG+HDQPINCONFIG+DCHGPINCONFIG+DDSGPINCONFIG+DACONFIGURATION+VCELLMODE+CC3SAMPLES+PROTECTIONCONFIGURATION+ENABLEDPROTECTIONSA+ENABLEDPROTECTIONSB+ENABLEDPROTECTIONSC+CHGFETPROTECTIONSA+CHGFETPROTECTIONSB+CHGFETPROTECTIONSC+DSGFETPROTECTIONSA+DSGFETPROTECTIONSB+DSGFETPROTECTIONSC+BODYDIODETHRESHOLD+DEFAULTALARMMASK+SFALERTMASKA+SFALERTMASKB+SFALERTMASKC+PFALERTMASKA+PFALERTMASKB+PFALERTMASKC+PFALERTMASKD+ENABLEDPFA+ENABLEDPFB+ENABLEDPFC+ENABLEDPFD+FETOPTIONS+CHGPUMPCONTROL+PRECHARGESTARTVOLTAGE+PRECHARGESTOPVOLTAGE+PREDISCHARGETIMEOUT+PREDISCHARGESTOPDELTA+DSGCURRENTTHRESHOLD+CHGCURRENTTHRESHOLD+CHECKTIME+CELL1INTERCONNECT+CELL2INTERCONNECT+CELL3INTERCONNECT+CELL4INTERCONNECT+CELL5INTERCONNECT+CELL6INTERCONNECT+CELL7INTERCONNECT+CELL8INTERCONNECT+CELL9INTERCONNECT+CELL10INTERCONNECT+CELL11INTERCONNECT+CELL12INTERCONNECT+CELL13INTERCONNECT+CELL14INTERCONNECT+CELL15INTERCONNECT+CELL16INTERCONNECT+MFGSTATUSINIT+BALANCINGCONFIGURATION+MINCELLTEMP+MAXCELLTEMP+MAXINTERNALTEMP+CELLBALANCEINTERVAL+CELLBALANCEMAXCELLS+CELLBALANCEMINCELLVCHARGE+CELLBALANCEMINDELTACHARGE+CELLBALANCESTOPDELTACHARGE+CELLBALANCEMINCELLVRELAX+CELLBALANCEMINDELTARELAX+CELLBALANCESTOPDELTARELAX+SHUTDOWNCELLVOLTAGE+SHUTDOWNSTACKVOLTAGE+LOWVSHUTDOWNDELAY+SHUTDOWNTEMPERATURE+SHUTDOWNTEMPERATUREDELAY+FETOFFDELAY+SHUTDOWNCOMMANDDELAY+AUTOSHUTDOWNTIME+RAMFAILSHUTDOWNTIME+SLEEPCURRENT+VOLTAGETIME+WAKECOMPARATORCURRENT+SLEEPHYSTERESISTIME+SLEEPCHARGERVOLTAGETHRESHOLD+SLEEPCHARGERPACKTOSDELTA+CONFIGRAMSIGNATURE+CUVTHRESHOLD+CUVDELAY+CUVRECOVERYHYSTERESIS+COVTHRESHOLD+COVDELAY+COVRECOVERYHYSTERESIS+COVLLATCHLIMIT+COVLCOUNTERDECDELAY+COVLRECOVERYTIME+OCCTHRESHOLD+OCCDELAY+OCCRECOVERYTHRESHOLD+OCCPACKTOSDELTA+OCD1THRESHOLD+OCD1DELAY+OCD2THRESHOLD+OCD2DELAY+SCDTHRESHOLD+SCDDELAY+SCDRECOVERYTIME+OCD3THRESHOLD+OCD3DELAY+OCDRECOVERYTHRESHOLD+OCDLLATCHLIMIT+OCDLCOUNTERDECDELAY+OCDLRECOVERYTIME+OCDLRECOVERYTHRESHOLD+SCDLLATCHLIMIT+SCDLCOUNTERDECDELAY+SCDLRECOVERYTIME+SCDLRECOVERYTHRESHOLD+OTCTHRESHOLD+OTCDELAY+OTCRECOVERY+OTDTHRESHOLD+OTDDELAY+OTDRECOVERY+OTFTHRESHOLD+OTFDELAY+OTFRECOVERY+OTINTTHRESHOLD+OTINTDELAY+OTINTRECOVERY+UTCTHRESHOLD+UTCDELAY+UTCRECOVERY+UTDTHRESHOLD+UTDDELAY+UTDRECOVERY+UTINTTHRESHOLD+UTINTDELAY+UTINTRECOVERY+PROTECTIONSRECOVERYTIME+HWDDELAY+LOADDETECTACTIVETIME+LOADDETECTRETRYDELAY+LOADDETECTTIMEOUT+PTOCHARGETHRESHOLD+PTODELAY+PTORESET+CUDEPTHRESHOLD+CUDEPDELAY+SUVTHRESHOLD+SUVDELAY+SOVTHRESHOLD+SOVDELAY+TOSSTHRESHOLD+TOSSDELAY+SOCCTHRESHOLD+SOCCDELAY+SOCDTHRESHOLD+SOCDDELAY+SOTTHRESHOLD+SOTDELAY+SOTFTHRESHOLD+SOTFDELAY+VIMRCHECKVOLTAGE+VIMRMAXRELAXCURRENT+VIMRTHRESHOLD+VIMRDELAY+VIMRRELAXMINDURATION+VIMACHECKVOLTAGE+VIMAMINACTIVECURRENT+VIMATHRESHOLD+VIMADELAY+CFETFOFFTHRESHOLD+CFETFOFFDELAY+DFETFOFFTHRESHOLD+DFETFOFFDELAY+VSSFFAILTHRESHOLD+VSSFDELAY+PF2LVLDELAY+LFOFDELAY+HWMXDELAY+SECURITYSETTINGS+UNSEALKEYSTEP1+UNSEALKEYSTEP2+FULLACCESSKEYSTEP1+FULLACCESSKEYSTEP2
#define EEPROM_TEST_BYTE 0x42
#endif /* INC_BQ76952_DEFAULT_CONFIG_H_ */
