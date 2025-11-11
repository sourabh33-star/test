

#ifndef INC_SOCTASKS_H_
#define INC_SOCTASKS_H_

#include "BQ76952_default_config.h"

#define BATTERY_CAPACITY 223200 //In Ampere Seconds (62Ahx3600)
//#define BATTERY_CAPACITY 216000 //60Ahx3600
//#define BATTERY_CAPACITY 198000//55Ahx3600
//#define BATTERY_CAPACITY 190800//53Ahx3600

//#define BATTERY_CAPACITY 12500
#define MAX_CELL_VOLTAGE 4150
#define MIN_CELL_VOLTAGE 3000
float VoltageModel(float soc_temp);
float Compute_Cell_Average(uint16_t CellVoltages[16]);
void init_filter_parameters(float initial_SoC, float initial_P, float process_noise, float measurement_noise, uint32_t battery_capacity);
float compute_soc(float Voltage,int16_t Current,uint8_t nonzero_current_flag,uint8_t safetystat);
float compute_soc_cc(int16_t Current,float init_soc,uint8_t safetystat);

#endif /* INC_SOCTASKS_H_ */
