
#include "bq76952.h"
#include "main.h"
#include "SOCtasks.h"
#include <stdlib.h>
#include "math.h"

float SoC_value;
float prev_SoC;
float P_val;
float Q_val;
float R_val;
uint32_t C_val;
float K;                 // Kalman Gain

uint8_t init_flag=0; // Flag to check for initialization
uint8_t no_switch_event = 1; //Flag to check if an on switch from 0 event happened
uint8_t instant_currentdrop_event = 0; //Flag to check if an instant current drop has occurred


/*Since time step is 1 second, this portion is commented. This needs to be initialized if sampling interval is different*/
//float dt;                // Time step

// Polynomial coefficients for the SoC to voltage relationship
/*These need to be modified based on the battery characteristics*/

const float a = 18.777f;
const float b = -59.596f;
const float c = 71.712f;
const float d = -40.466f;
const float e = 11.538f;
const float g = 2.143f;

//Values of slope and intercept
const float m=50400.0f;
const float c_val=137520.0f;

//Offset value of 0.5Ah
const float offset = 1800;


//Battery parameters
//float R0= 0.004;
float R0=0.0004;
//float R1 = 0.011;
float R1 = 0.1;
float C1 = 1000;

//Variable to store previous voltage values
float previous_voltage;
float previous_current;

//latest battery capacity and current battery capacity
float new_battery_capacity;
float current_battery_capacity = BATTERY_CAPACITY;

//float voltage_range =(COVTHRESHOLD-CUVTHRESHOLD)*50.6;
//float current_battery_capacity = voltage_range*m + c_val + offset; //Should fit a line to calculate the capacity for different ranges


//Detect the battery full charge event and battery full discharge event
//uint8_t battery_fullcharge_event=0;
//uint8_t battery_discharge_event=0;

uint8_t reset_acccharge_flag = 0;

//Variable to calculate charge accumulated in SoC function
float charge_acc = 0;


//Estimating initial SoC
float init_soc(uint16_t CellVoltages[16])
{
	int sum = 0;
	for (int i=0;i<16;i++)
	{

		sum = sum + CellVoltages[i];
	}
	sum=sum/14.0f;

	return ((MAX_CELL_VOLTAGE-sum)/(MAX_CELL_VOLTAGE-MIN_CELL_VOLTAGE));

}

//Calculating the average of all cell voltages.
//If required, this needs to be changed to weighted average.
float Compute_Cell_Average(uint16_t CellVoltages[16])
{
	int sum = 0;
	for (int i=0;i<16;i++)
	{

		sum = sum + CellVoltages[i];
	}
	return (sum/14.0f);
}

//The model needs to be changed depending on the accuracy
float estimate_ocv(float soc_temp)
{
	return (a * soc_temp * soc_temp * soc_temp * soc_temp * soc_temp + b * soc_temp *soc_temp *soc_temp *soc_temp + c * soc_temp * soc_temp *soc_temp + d *soc_temp *soc_temp + e *soc_temp +g);

}

//Estimating e^(-t/RC) using taylor's series approximation.
float estimate_exp_term(float x)
{
	return (1-x+(x*x)/2);
}

//Initialization of filter parameters
void init_filter_parameters(float initial_SoC, float initial_P, float process_noise, float measurement_noise, uint32_t battery_capacity)
{
	//Set the SoC value and covariance
	SoC_value = initial_SoC;
    P_val = initial_P;

    // Set the process and measurement noise covariances
    Q_val = process_noise;
    R_val = measurement_noise;

    // Set the battery capacity
    C_val = battery_capacity;

    init_flag=1;
}

//Predicting and updating the soc value depending on present voltage and current values
float compute_soc(float Voltage,int16_t Current,uint8_t nonzero_current_flag,uint8_t safetystat)
{

		float current_temp= Current/100.0f; //Actual current value
		current_temp = current_temp/13.0f; //Estimating the current in each cell
	    float exp_term = estimate_exp_term(1/(R1*C1)); //Finding the taylor's series approximation term
	    float AFE_voltage_reading = 0;


		/*SoC algo modification*/
	    //If there is no current, then previous voltage value should be saved
	    /*
	    if((nonzero_current_flag==1)&&(abs(Current)<10))
	    {
	    	AFE_voltage_reading = previous_voltage;
	    }
	    else
	    {
		    AFE_voltage_reading = Voltage;
	    }*/

	    AFE_voltage_reading = Voltage;

		//Kalman filter implementation
		// Prediction step

		float SoC_pred = SoC_value- (current_temp / C_val);
		float P_pred = P_val + Q_val;


		// Measurement update step
		float V_pred = estimate_ocv(SoC_pred)-(current_temp*R0)-(current_temp*R1*(1-exp_term));
		float y = AFE_voltage_reading - V_pred;
		float S = P_pred + R_val;
		K = P_pred / S;

		SoC_value = SoC_pred + K * y;
		prev_SoC = SoC_value;
					//if (SoC_value>100)
						//SoC_value=100;
		P_val = (1 - K) * P_pred;



		previous_voltage = Voltage;
		/*SoC algo modification*/
		//previous_voltage = AFE_voltage_reading;
		previous_current = Current;



		//Ensuring that SoC_value is between 0 and 1
		if (SoC_value>1)
			SoC_value=1;
		if (SoC_value<0)
			SoC_value=0;

	    return SoC_value;
}

void chg_dsg_done(void)
{
	reset_acccharge_flag = 0;
}

float compute_soc_cc(int16_t Current,float init_soc,uint8_t safetystat)
{
	float temp_current = Current/100.0f;
	//temp_current = temp_current/13.0f;
	//charge_acc = charge_acc + (temp_current)/BATTERY_CAPACITY;

	//Resetting the accumulated charge value to 0 whenever charging or discharging is done

	if (reset_acccharge_flag ==0)
	{
		charge_acc=0;
		reset_acccharge_flag=1;
	}

	charge_acc = charge_acc + temp_current;
	float soc2 = charge_acc/current_battery_capacity;



	soc2 = init_soc + soc2;


	//Capacity adjustment after full charge. Until COV is recovered this loop continues to execute
	if(safetystat == 0x08)
	{

		//Needs to be modified for updating battery capacity
		/*
		if(battery_fullcharge_event==0)
		{
			new_battery_capacity = soc2*current_battery_capacity;
			if(new_battery_capacity < current_battery_capacity )
			{
				//Capacity to be stored
				current_battery_capacity=new_battery_capacity;
			}
			battery_fullcharge_event = 1;
		}
		if(battery_discharge_event==1)
		{
			battery_discharge_event = 0;
		}*/
	}


	if (soc2>1)
		soc2 =1;
	if (soc2<0)
		soc2 = 0;

	//Capacity adjustment after full discharge. Until CUV is recovered, this continues to execute.
	if(safetystat == 0x04)
	{

/*		if(battery_discharge_event ==0)
		{
			new_battery_capacity = current_battery_capacity-abs(soc2*current_battery_capacity);
			if(new_battery_capacity < current_battery_capacity )
			{
				//Capacity to be stored
				current_battery_capacity=new_battery_capacity;
			}
			battery_discharge_event = 1;
		}
		if(battery_fullcharge_event==1)
		{
			battery_fullcharge_event = 0;
		}*/
		soc2 = 0;
	}
	return soc2;

}
