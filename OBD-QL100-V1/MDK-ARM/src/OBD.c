#include "OBD.h"
//OBD_MSG_BUG obdNormalBuf;
//OBD_MSG_BUG obdFastBuf;

void engine_rpm_formula(int data, char *buf)
{
//      sprintf(buf, "%i r/min", (int)((float)data/4));
	char *point = strstr(buf,"410C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010C
//			NO DATA



void engine_load_formula(int data, char *buf) //0104
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"4104");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else		 
	{
		fillObdBuf(data,point+4,1);
	}
}


void coolant_temp_formula(int data, char *buf) //0105
{
//      sprintf(buf, "%i%c C", data-40, 0xB0);
	char *point = strstr(buf,"4105");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//		>01051
//		410582


void fuel_system_status_formula(int data, char *buf)
{
//   if (data == 0)
//   	sprintf(buf, "unused");
//   else if (data == 0x01)
//      sprintf(buf, "open loop");
//   else if (data == 0x02)
//      sprintf(buf, "closed loop");
//   else if (data == 0x04)
//      sprintf(buf, "open loop (driving conditions)");
//   else if (data == 0x08)
//      sprintf(buf, "open loop (system fault)");
//   else if (data == 0x10)
//      sprintf(buf, "closed loop, O2 sensor fault");
//   else
//      sprintf(buf, "unknown: 0x%02X", data);

	char *point = strstr(buf,"410C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}


void fuel_system1_status_formula(int data, char *buf) //0103
{
//   	if(checkObdReturn < 0)
//   	fillObdBuf(data,buf,1);
	char *point = strstr(buf,"4103");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
//   fuel_system_status_formula((data >> 8) & 0xFF, buf);  // Fuel System 1 status: Data A
}
//		>0101
//		410100076100

void fuel_system2_status_formula(int data, char *buf)
{
   fuel_system_status_formula(data & 0xFF, buf);  // Fuel System 2 status: Data B
}


void vehicle_speed_formula(int data, char *buf)
{
//      sprintf(buf, "%i km/h", data);
	  char *point = strstr(buf,"410D");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010D
//			410D00

void intake_pressure_formula(int data, char *buf)
{
//sprintf(buf, "%i kPa", data);
	char *point = strstr(buf,"410B");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010B
//			NO DATA

void timing_advance_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%c", ((float)data-128)/2, 0xB0);
	char *point = strstr(buf,"410E");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010E
//			410E90

void intake_air_temp_formula(int data, char *buf)
{
//      sprintf(buf, "%i%c C", data-40, 0xB0);
	char *point = strstr(buf,"410F");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010F
//			410F67

void air_flow_rate_formula(int data, char *buf)
{
//      sprintf(buf, "%.2f g/s", data*0.01);

	char *point = strstr(buf,"4110");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0110
//			41100178
void throttle_position_formula(int data, char *buf)
{
//	sprintf(buf, "%.1f%%", (float)data*100/255);

	char *point = strstr(buf,"4111");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0111
//			411109
// **** New formulae added 3/11/2003: ****

// Fuel Pressure (guage): PID 0A
void fuel_pressure_formula(int data, char *buf)
{
//      sprintf(buf, "%i kPa", data*3);
	char *point = strstr(buf,"410A");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010A
//			NO DATA

// Fuel Trim statuses: PID 06-09
void short_term_fuel_trim_formula1(int data, char *buf)	//0106
{
//   if (data > 0xFF)  // we're only showing bank 1 and 2 FT
//      data >>= 8;
//
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", ((float)data - 128)*100/128);

   	char *point = strstr(buf,"4106");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//		>0106
//		410E7E



void long_term_fuel_trim_formula1(int data, char *buf)//0107
{
//   if (data > 0xFF)  // we're only showing bank 1 and 2 FT
//      data >>= 8;
//
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", ((float)data - 128)*100/128);
   	char *point = strstr(buf,"4107");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}

//		>0107
//		410779
//	Fuel Trim statuses: PID 06-09
void short_term_fuel_trim_formula2(int data, char *buf)
{
//   if (data > 0xFF)  // we're only showing bank 1 and 2 FT
//      data >>= 8;
//
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", ((float)data - 128)*100/128);

   	char *point = strstr(buf,"4108");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//		>0107
//		410779
//		NO DATA

void long_term_fuel_trim_formula2(int data, char *buf)
{
//   if (data > 0xFF)  // we're only showing bank 1 and 2 FT
//      data >>= 8;
//
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", ((float)data - 128)*100/128);
   	char *point = strstr(buf,"4109");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//		>0108
//		410879
//		NO DATA

// Commanded secondary air status: PID 12
void secondary_air_status_formula(int data, char *buf)
{
//   data = data & 0x0700; // mask bits 0, 1, and 2
//
//   if (data == 0x0100)
//      sprintf(buf, "upstream of 1st cat. conv.");
//   else if (data == 0x0200)
//      sprintf(buf, "downstream of 1st cat. conv.");
//   else if (data == 0x0400)
//      sprintf(buf, "atmosphere / off");
//   else
//      sprintf(buf, "Not supported");

	char *point = strstr(buf,"4112");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0112
//			NO DATA
// Oxygen sensor voltages & short term fuel trims: PID 14-1B
// Format is bankX_sensor

void o2_sensor_formula1_1(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"4114");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>0114
//			411416FF
void o2_sensor_formula1_2(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"4115");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>0115
//			4115107E
void o2_sensor_formula1_3(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"4116");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>0116
//			NO DATA
void o2_sensor_formula1_4(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"4117");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>0117
//			NO DATA
void o2_sensor_formula2_1(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"4118");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>0118
//			NO DATA
void o2_sensor_formula2_2(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"4119");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>0119
//			NO DATA
void o2_sensor_formula2_3(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"411A");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>011A
//			NO DATA
void o2_sensor_formula2_4(int data, char *buf)
{
//   if ((data & 0xFF) == 0xFF)  // if the sensor is not used in fuel trim calculation,
//      sprintf(buf, "%.3f V", (data >> 8)*0.005);
//   else
//      sprintf(buf, ((data & 0xFF) == 128) ? "%.3f V @ 0.0%% s.t. fuel trim" : "%.3f V @ %+.1f%% s.t. fuel trim", (data >> 8)*0.005, ((float)(data & 0xFF) - 128)*100/128);
	char *point = strstr(buf,"411B");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>011B
//			NO DATA

//Power Take-Off Status: PID 1E
void pto_status_formula(int data, char *buf)
{
// 	if ((data & 0x01) == 0x01)
//		sprintf(buf, "active");
//	else
// 		sprintf(buf, "not active");
	char *point = strstr(buf,"411E");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}	
}
//			>011E
//			NO DATA 
// OBD requirement to which vehicle is designed: PID 1C
void obd_requirements_formula(int data, char *buf)
{
//	switch (data)
//	{
//		case 0x01:
//			sprintf(buf, "OBD-II (California ARB)");
//			break;
//		case 0x02:
//			sprintf(buf, "OBD (Federal EPA)");
//			break;
//		case 0x03:
//			sprintf(buf, "OBD and OBD-II");
//			break;
//		case 0x04:
//			sprintf(buf, "OBD-I");
//			break;
//		case 0x05:
//			sprintf(buf, "Not OBD compliant");
//			break;
//		case 0x06:
//			sprintf(buf, "EOBD (Europe)");
//			break;
//		case 0x07:
//			sprintf(buf, "EOBD and OBD-II");
//			break;
//		case 0x08:
//			sprintf(buf, "EOBD and OBD");
//			break;
//		case 0x09:
//			sprintf(buf, "EOBD, OBD and OBD-II");
//			break;
//		case 0x0A:
//			sprintf(buf, "JOBD (Japan)");
//			break;
//		case 0x0B:
//			sprintf(buf, "JOBD and OBD-II");
//			break;
//		case 0x0C:
//			sprintf(buf, "JOBD and EOBD");
//			break;
//		case 0x0D:
//			sprintf(buf, "JOBD, EOBD, and OBD-II");
//			break;
//		default:
//			sprintf(buf, "Unknown: 0x%02X", data);
//   }
	char *point = strstr(buf,"411C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>011C
//			411C06
/* Sensors added 1/2/2003: */

void engine_run_time_formula(int data, char *buf)
{
//   int sec, min, hrs;
//   
//   hrs = data / 3600;  // get hours
//   min = (data % 3600) / 60;  // get minutes
//   sec = data % 60;  // get seconds
//
//   sprintf(buf, "%02i:%02i:%02i", hrs, min, sec);

   char *point = strstr(buf,"411F");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>011F
//			NO DATA
void mil_distance_formula(int data, char *buf)
{
//      sprintf(buf, "%i km", data);
	char *point = strstr(buf,"4121");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0121
//			41210000
void frp_relative_formula(int data, char *buf)
{
//   float kpa, psi;
//   
//   kpa = data*0.079;
//   psi = kpa*0.145037738;
//      sprintf(buf, "%.3f kPa", kpa);

	char *point = strstr(buf,"4122");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0122

void frp_widerange_formula(int data, char *buf)
{
//   int kpa;
//   float psi;
//
//   kpa = data*10;
//   psi = kpa*0.145037738;
//
//      sprintf(buf, "%i kPa", kpa);

	char *point = strstr(buf,"4123");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0123
//			NO DATA
void o2_sensor_wrv_formula1_1(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"4124");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0124
//			NO DATA
void o2_sensor_wrv_formula1_2(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"4125");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0125
//			NO DATA
void o2_sensor_wrv_formula1_3(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"4126");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0126
//			NO DATA
void o2_sensor_wrv_formula1_4(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"4127");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0127
//			NO DATA
void o2_sensor_wrv_formula2_1(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"4128");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0128
//			NO DATA
void o2_sensor_wrv_formula2_2(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"4129");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0129
//			NO DATA
void o2_sensor_wrv_formula2_3(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"412A");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>012A
//			NO DATA
void o2_sensor_wrv_formula2_4(int data, char *buf)
{
//   float eq_ratio, o2_voltage; // equivalence ratio and sensor voltage
//   
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_voltage = (float)(data & 0xFFFF)*0.000122; // data bytes C,D
//   
//   sprintf(buf, "%.3f V, Eq. ratio: %.3f", o2_voltage, eq_ratio);
   char *point = strstr(buf,"412B");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>012B
//			NO DATA
//Commanded EGR status: PID 2C
void commanded_egr_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);

   char *point = strstr(buf,"412C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}

}
//			>012C
//			NO DATA
//EGR error: PID 2D
void egr_error_formula(int data, char *buf)
{
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", (float)(data-128)/255*100);
   char *point = strstr(buf,"412D");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>012D
//			NO DATA
void evap_pct_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data/255*100);
   char *point = strstr(buf,"412E");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>012E
//			NO DATA
void fuel_level_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data/255*100);
	char *point = strstr(buf,"412F");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>012F
//			NO DATA
void warm_ups_formula(int data, char *buf)
{
//   sprintf(buf, "%i", data);
   	char *point = strstr(buf,"4130");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0130
//			NO DATA
void clr_distance_formula(int data, char *buf)
{
//      sprintf(buf, "%i km", data);
	char *point = strstr(buf,"4131");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0131
//			NO DATA
void evap_vp_formula(int data, char *buf)
{
//      sprintf(buf, "%.2f Pa", data*0.25);
	char *point = strstr(buf,"4132");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}

}
//			>0132
//			NO DATA
void baro_pressure_formula(int data, char *buf)
{
//      sprintf(buf, "%i kPa", data);
	char *point = strstr(buf,"4133");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0133
//			NO DATA
//void o2_sensor_wrc_formula(int data, char *buf)
//{
//   float eq_ratio, o2_ma; // equivalence ratio and sensor current
//
//   eq_ratio = (float)(data >> 16)*0.0000305;  // data bytes A,B
//   o2_ma = ((float)(data & 0xFFFF) - 0x8000)*0.00390625; // data bytes C,D
//
//   sprintf(buf, "%.3f mA, Eq. ratio: %.3f", o2_ma, eq_ratio);
//
//}
void o2_sensor_wrc_formula1_1(int data, char *buf)
{
   char *point = strstr(buf,"4134");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0134
//			NO DATA
void o2_sensor_wrc_formula1_2(int data, char *buf)
{
   char *point = strstr(buf,"4135");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0135
//			NO DATA
void o2_sensor_wrc_formula1_3(int data, char *buf)
{
   char *point = strstr(buf,"4136");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0136
//			NO DATA
void o2_sensor_wrc_formula1_4(int data, char *buf)
{
   char *point = strstr(buf,"4137");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0137
//			NO DATA
void o2_sensor_wrc_formula2_1(int data, char *buf)
{
   char *point = strstr(buf,"4138");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0138
//			NO DATA
void o2_sensor_wrc_formula2_2(int data, char *buf)
{
   char *point = strstr(buf,"4139");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0139
//			NO DATA
void o2_sensor_wrc_formula2_3(int data, char *buf)
{
   char *point = strstr(buf,"413A");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>013A
//			NO DATA
void o2_sensor_wrc_formula2_4(int data, char *buf)
{
   char *point = strstr(buf,"413B");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>013B
//			NO DATA

void cat_temp_formula1(int data, char *buf)
{
//   float c,f;
//   
//   c = data*0.1 - 40; // degrees Celcius
//   f = c*9/5 + 32;  // degrees Fahrenheit
//   
//      sprintf(buf, "%.1f%c C", c, 0xB0);
	char *point = strstr(buf,"413C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>013C
//			NO DATA
void cat_temp_formula2(int data, char *buf)
{
//   float c,f;
//   
//   c = data*0.1 - 40; // degrees Celcius
//   f = c*9/5 + 32;  // degrees Fahrenheit
//   
//      sprintf(buf, "%.1f%c C", c, 0xB0);
	char *point = strstr(buf,"413D");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>013D
//			NO DATA
void cat_temp_formula3(int data, char *buf)
{
//   float c,f;
//   
//   c = data*0.1 - 40; // degrees Celcius
//   f = c*9/5 + 32;  // degrees Fahrenheit
//   
//      sprintf(buf, "%.1f%c C", c, 0xB0);
	char *point = strstr(buf,"413E");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>013E
//			NO DATA
void cat_temp_formula4(int data, char *buf)
{
//   float c,f;
//   
//   c = data*0.1 - 40; // degrees Celcius
//   f = c*9/5 + 32;  // degrees Fahrenheit
//   
//      sprintf(buf, "%.1f%c C", c, 0xB0);
	char *point = strstr(buf,"413F");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>013F
//			NO DATA
void ecu_voltage_formula(int data, char *buf)
{
//   sprintf(buf, "%.3f V", data*0.001);
	char *point = strstr(buf,"4142");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0142
//			NO DATA
void abs_load_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"4143");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0143
//			NO DATA
void eq_ratio_formula(int data, char *buf)
{
//   sprintf(buf, "%.3f", data*0.0000305);
	char *point = strstr(buf,"4144");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0144
//			NO DATA
void relative_tp_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
   char *point = strstr(buf,"4145");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}


void amb_air_temp_formula(int data, char *buf)
{
//   int c,f;
//   
//   c = data-40; // degrees Celcius
//   f = (float)c*9/5 + 32;  // degrees Fahrenheit
//   
//      sprintf(buf, "%i%c C", c, 0xB0);
	char *point = strstr(buf,"4146");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0146
//			NO DATA
void abs_tp_formula_b(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"4147");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0147
//			NO DATA
void abs_tp_formula_c(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"4148");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void abs_tp_formula_d(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"4149");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void abs_tp_formula_e(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"414A");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void abs_tp_formula_f(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"414B");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}


void tac_pct_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"414C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>014C
//			NO DATA
void mil_time_formula(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
	char *point = strstr(buf,"414D");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>014D
//			NO DATA
void clr_time_formula(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"414E");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>014E
//			NO DATA
void external_test_cfg_1(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"414F");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>014F
//			NO DATA
void external_test_cfg_2(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4150");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0150
//			NO DATA
void fuel_type(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4151");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0151
//			NO DATA
void percentage_alcohol_fuel(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4152");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0152
//			NO DATA
void abs_evap_system_vapor_press(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4153");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0153
//			NO DATA
void evap_system_vapor_press(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4154");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void short_term_sec_oxygen_1_3(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4155");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void long_term_sec_oxygen_1_3(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4156");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void short_term_sec_oxygen_2_4(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4157");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void long_term_sec_oxygen_2_4(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4158");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void fule_rail_pressure(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"4159");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void relative_acc_pedal_pos(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"415A");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void hybrid_battery_pack_remain(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"415B");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void engine_oil_temperature(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"415C");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void fuel_injection_timing(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"415D");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void fuel_rate(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"415E");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void emission_requirements(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
   char *point = strstr(buf,"415F");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}

void fuel_system1_frame_dtc(int data, char *buf) //0102
{
	char *point = strstr(buf,"4102");
	if(point != NULL)
		fillObdBuf(data,point,1);
	else
		fillObdBuf(data,point,0);
//	uint16_t point; 
//	uint16_t size = ( sysCfg.obdConfig.cmdList[data] & 0xF000 ) >> 12;
//	if(size == 0)
//	{
//		return ;
//	}
//	size = size * 4;
//	point = getBufIndex(index);
   //sprintf(buf, "%i hrs %i min", data/60, data%60);
}

void oxygen_sensors_present_1(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
	char *point = strstr(buf,"4113");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0113
//			NO DATA
void oxygen_sensors_present_2(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
	char *point = strstr(buf,"411D");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>011D

void support_pid_20_3f(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
	char *point = strstr(buf,"4120");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>0120
//			412080000000
void support_pid_00_1f(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);

	char *point = strstr(buf,"4100");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//		>ATH1
//		OK
//		>ATDPN
//		A3
//		>0100
//		486B104100BE1FB811AA
//		>ATH0
//		OK
//		>0100
//		4100BE1FB811


void numberOftroubleCodes(int data, char *buf) //0101
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);

	char *point = strstr(buf,"4101");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
void short_term_seoxygen_1_3(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);
	char *point = strstr(buf,"4155");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}

void long_term_seoxygen_1_3(int data, char *buf)
{
//sprintf(buf, "%i hrs %i min", data/60, data%60);
	char *point = strstr(buf,"4156");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}

void monitior_state_formula(int data, char *buf)
{
	char *point = strstr(buf,"4141");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
	//sprintf(buf, "%i hrs %i min", data/60, data%60);
}
//			>0141
//			NO DATA
void support_pid_41_5f(int data, char *buf)
{
	char *point = strstr(buf,"4140");
	if(checkObdReturn(buf,0xF) < 0 || point == NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
	//sprintf(buf, "%i hrs %i min", data/60, data%60);
}
//			>0140
//			NO DATA
void get_device_identifier(int index, char *buf)
{
	int i = 0;
	if(checkObdReturn(buf,0x1) < 0)
	{
//		fillObdBuf(index,buf,0);
		for(;i<20;i++){
//			vid[i] = buf[i]; 
		}
	}
	else
	{
//	 	fillObdBuf(index,buf,1);
//		memset(vid,20,'\0');
	}

}

/*******************************************************************************
 * 函数名称:checkObdReturn                                                                     
 * 描    述:根据命令的index信息，获取在缓冲区的其实位置                                                                   
 *                                                                               
 * 输    入: type                                                                   
 * 输    出:无                                                                    
 * 返    回:命令执行状态1：正常  -1：错误                                                                       
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
//typedef struct {
//  	uint16_t srcIndex;
//	uint16_t desIndex;
// }MAP_INDEX;
uint16_t getBufIndex(uint16_t index,int32_t mode)
{
	uint16_t i = 0;	
	static uint16_t mapNormalIndex[200],mapFastIndex[200];
	static uint16_t flag = 0;
	if(flag == 0)		
	{ 
		
		mapNormalIndex[0] = 0;
		for(i = 0;i<(sysCfg.obdConfig.normalEnd - sysCfg.obdConfig.normalStart);i++)
		{
			mapNormalIndex[i+1] += sysCfg.obdConfig.cmdList[i + sysCfg.obdConfig.normalStart].dataLength + mapNormalIndex[i] ;		
		}
		printf("\r\nNORMAL INDEX MAP\r\n");
		for(i = 0 ;i < 200;i++)
		{
		 	if(i %5 == 0)
				printf("\r\n");
			printf(":::%d->%d-->%d->%d   ",i,sysCfg.obdConfig.normalStart+i,sysCfg.obdConfig.cmdList[i + sysCfg.obdConfig.normalStart].dataLength,mapNormalIndex[i]);
		}

		mapFastIndex[0] = 0;
		for(i = 0;i<(sysCfg.obdConfig.fastEnd - sysCfg.obdConfig.fastStart);i++)
		{
			mapFastIndex[i+1] += sysCfg.obdConfig.cmdList[i + sysCfg.obdConfig.fastStart].dataLength + mapFastIndex[i] ;		
		}
		printf("\r\nFAST INDEX MAP:\r\n");
		for(i = 0 ;i < 200;i++)
		{
		 	if(i %5 == 0)
				printf("\r\n");
			printf(":::%d->%d-->%d->%d\t",i,sysCfg.obdConfig.fastStart+i,sysCfg.obdConfig.cmdList[i + sysCfg.obdConfig.fastStart].dataLength,mapFastIndex[i]);
		}


		flag = 1;

	}
	if(mode == 0){
		printf("map index %d -> %d->%d-->%d",index,sysCfg.obdConfig.normalStart,sysCfg.obdConfig.normalEnd,mapNormalIndex[index - sysCfg.obdConfig.normalStart]);
		if(index > sysCfg.obdConfig.normalEnd ||  index < sysCfg.obdConfig.normalStart)
		{
		
			return 0;
			
		}
		else
		{
			
			return mapNormalIndex[index - sysCfg.obdConfig.normalStart];
		}
	}
	if(mode == 1){
		if(index > sysCfg.obdConfig.fastEnd ||  index < sysCfg.obdConfig.fastStart)
			return 0;
		else
			return mapFastIndex[index - sysCfg.obdConfig.fastStart];

	}
	return 0;		

}
/*******************************************************************************
 * 函数名称:checkObdReturn                                                                     
 * 描    述:设置蓝牙电源                                                                     
 *                                                                               
 * 输    入: type                                                                   
 * 输    出:无                                                                    
 * 返    回:命令执行状态1：正常  -1：错误                                                                       
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int checkObdReturn(char *buf ,uint16_t type){
	if(type & 0x8){
		if(strstr(buf,"DATA")!= NULL)//find NO DATA
			return -1;	
	}
	if(type & 0x2){
		if(strstr(buf,"NECT")!=NULL)	//FIND CONNECT
			return -2; 	
	}
	if(type & 0x4){
		if(strstr(buf,"RCHING")!=NULL) //SEARCHING
			return -3; 	
	}
	if(type & 0x1){

		if(strstr(buf,">") == NULL)
			return -1;
	}
	return 1;		
}


const OBD_SENSOR obdCmdList[] = {
	{NULL,								"REBOOT",						"",		"ATZ\r",			1,4,200,0,0},//0
	{NULL,								"ECHO OFF",						"",		"ATE0\r",			1,4,200,0,0},
	{NULL,								"ECHO ON",						"",		"ATE1\r",			1,4,200,0,0},
	{NULL,								"MEMORY OFF",					"",		"ATM0\r",			1,4,200,0,0},
	{NULL,								"LINEFEED OFF",					"",		"ATL0\r",			1,4,200,0,0},
	{NULL,								"PRINT SPACES OFF",				"",		"ATS0\r",			1,4,200,0,0},//5
	{NULL,								"DEVICE ID1",					"",		"AT@1\r",			1,4,200,0,0},
	{get_device_identifier,				"DEVICE ID2",					"",		"AT@2\r",			1,4,200,0,0},
	{NULL,								"ELM327 INFO",					"",		"ATI\r",			1,4,200,0,0},
	{NULL,								"STN1110 INFO",					"",		"STI\r",			1,4,200,0,0},
	{NULL,								"??????",						"",		"ATPP 0E OFF\r",	1,4,200,0,0},//10
	{NULL,								"??????",						"",		"ST SL off,on\r",	1,4,200,0,0},
	{NULL,								"ST SL off,on",					"",		"ST SL off,on\r",	1,4,200,0,0},
	{NULL,								"ST SLU off,on",				"",		"ST SLU off,on\r",	1,4,200,0,0},
	{NULL,								"ST SLVL off,on",				"",		"ST SLVL off,on\r",	1,4,200,0,0},
	{NULL,								"ST SLVL off,on",				"",		"ST SLVG off\r",	1,4,200,0,0},//15 
	{NULL,								"PRINT AT PP SUMMARY",			"",		"ATPPS\r",			1,4,200,0,0},	
	{NULL,								"PRINT ST PP SUMMARY",			"",		"STPPS\r",			1,4,200,0,0},
	{NULL,								"HEADER OFF",					"",		"ATH0\r",			1,4,200,0,0},
	{NULL,								"ATAT2",						"",		"ATAT2\r",			1,4,200,0,0},
	{NULL,								"ATSP0",						"",		"ATSP0\r",			1,4,200,0,0},//20
	{NULL,								"SHOW CURRENT PROTOCOL",		"",		"ATDPN\r",			1,4,200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},//25
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},//30
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},//35
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},//40
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},//45
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0},
	{NULL,							 	"car ",							"",		"03\r",	 	1,	  	4,  200,0,0}, //49
	{ support_pid_00_1f,   			 "Support pids:",           		"", 	"0100\r",      1,    8 ,200,0xFFFFFFFF,0xFFFFFFFF},//50
	{ numberOftroubleCodes,   		 "Number of trouble codes:",       	"",		"0101\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//错误码
	{ fuel_system1_frame_dtc,		 "Freeze frame DTC:",           	"",	 	"0102\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力总成的冻结帧数据
	{ fuel_system1_status_formula,   "Fuel System 1 Status:",           "", 	"0103\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油系统1
	{ engine_load_formula,           "Calculated Load Value:",          "", 	"0104\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机负荷
	{ coolant_temp_formula,          "Coolant Temperature:",            "", 	"0105\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机冷却液温度	//55
	{ short_term_fuel_trim_formula1,  "Short Term Fuel Trim (Bank 1):",  "", 	"0106\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//短时燃油修正BANK1
	{ long_term_fuel_trim_formula1,   "Long Term Fuel Trim (Bank 1):",   "", 	"0107\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//长时燃油修正BANK1
	{ short_term_fuel_trim_formula2,  "Short Term Fuel Trim (Bank 2):",  "", 	"0108\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//短时燃油修正 BANK2
	{ long_term_fuel_trim_formula2,   "Long Term Fuel Trim (Bank 2):",   "", 	"0109\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//长时燃油修正 BANK2
	{ fuel_pressure_formula,         "Fuel Pressure (gauge):",          "", 	"010A\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油导轨压力		//60
	{ intake_pressure_formula,       "Intake Manifold Pressure:",       "", 	"010B\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//进气歧管绝对压力
	{ engine_rpm_formula,            "Engine RPM:",                     "", 	"010C\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机转速
	{ vehicle_speed_formula,         "Vehicle Speed:",                  "", 	"010D\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//车速
	{ timing_advance_formula,        "Timing Advance (Cyl. #1):",       "", 	"010E\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//1号气缸点火提前角
	{ intake_air_temp_formula,       "Intake Air Temperature:",         "", 	"010F\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//进气温度//			//65


	{ air_flow_rate_formula,         "Air Flow Rate (MAF sensor):",     "", 	"0110\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//空气流速
	{ throttle_position_formula,     "Absolute Throttle Position:",     "", 	"0111\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置	
	{ secondary_air_status_formula,  "Secondary air status:",           "", 	"0112\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//二次空气状态
	{ oxygen_sensors_present_1,  	 "Oxygen sensors present 1:",       "", 	"0113\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧含量传感器，多数无
	{ o2_sensor_formula1_1,          "O2 Sensor 1, Bank 1:",            "", 	"0114\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -1		//70
	{ o2_sensor_formula1_2,          "O2 Sensor 2, Bank 1:",            "", 	"0115\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -2
	{ o2_sensor_formula1_3,          "O2 Sensor 3, Bank 1:",            "", 	"0116\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -3
	{ o2_sensor_formula1_4,          "O2 Sensor 4, Bank 1:",            "", 	"0117\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -4
	{ o2_sensor_formula2_1,          "O2 Sensor 1, Bank 2:",            "", 	"0118\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -1
	{ o2_sensor_formula2_2,          "O2 Sensor 2, Bank 2:",            "", 	"0119\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -2		//75
	{ o2_sensor_formula2_3,          "O2 Sensor 3, Bank 2:",            "", 	"011A\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -3
	{ o2_sensor_formula2_4,          "O2 Sensor 4, Bank 2:",            "", 	"011B\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -4
	{ obd_requirements_formula,      "OBD conforms to:",                "", 	"011C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//OBD模式
	{ oxygen_sensors_present_2,      "Oxygen sensors present 2:",       "", 	"011D\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧含量传感器2，多数无 
	{ pto_status_formula,            "Power Take-Off Status:",          "", 	"011E\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力输出状态			//80
	{ engine_run_time_formula,       "Time Since Engine Start:",        "", 	"011F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力运行时间

	
	
	// Page 3
	{ support_pid_20_3f,          	 "Supported PIDs 0x21 to 0x3F:",    "", 	"0120\r",      1,    8 ,200,0xFFFFFFFF,0xFFFFFFFF},//Supported PIDs 0x21 to 0x3F
	{ mil_distance_formula,          "Distance since MIL activated:",   "", 	"0121\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//MIL凉气候的行驶距离
	{ frp_relative_formula,          "FRP rel. to manifold vacuum:",    "", 	"0122\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},// 燃油导轨压力（相对于歧管真空压力）
	{ frp_widerange_formula,         "Fuel Pressure (gauge):",          "", 	"0123\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},// 燃油导轨压力			//85
	{ o2_sensor_wrv_formula1_1,         "O2 Sensor 1, Bank 1 (WR):",       "",	"0124\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula1_2,         "O2 Sensor 2, Bank 1 (WR):",       "", 	"0125\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula1_3,         "O2 Sensor 3, Bank 1 (WR):",       "", 	"0126\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula1_4,         "O2 Sensor 4, Bank 1 (WR):",       "", 	"0127\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_1,         "O2 Sensor 1, Bank 2 (WR):",       "", 	"0128\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)	//90
	{ o2_sensor_wrv_formula2_2,         "O2 Sensor 2, Bank 2 (WR):",       "", 	"0129\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_3,         "O2 Sensor 3, Bank 2 (WR):",       "", 	"012A\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_4,         "O2 Sensor 4, Bank 2 (WR):",       "", 	"012B\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ commanded_egr_formula,         "Commanded EGR:",                  "", 	"012C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令EGR
	{ egr_error_formula,             "EGR Error:",                      "", 	"012D\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//EGR错误					//95
	{ evap_pct_formula,              "Commanded Evaporative Purge:",    "", 	"012E\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令的燃油蒸汽排出
	{ fuel_level_formula,            "Fuel Level Input:",               "", 	"012F\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油量输入


	{ warm_ups_formula,              "Warm-ups since ECU reset:",       "", 	"0130\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//故障代码清空后的热车次数
	{ clr_distance_formula,          "Distance since ECU reset:",       "", 	"0131\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//故障代码清空后的行驶距离
	{ evap_vp_formula,               "Evap System Vapor Pressure:",     "", 	"0132\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//EVAP蒸汽压力				//100
	{ baro_pressure_formula,         "Barometric Pressure (absolute):", "", 	"0133\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//气压
	{ o2_sensor_wrc_formula1_1,         "O2 Sensor 1, Bank 1 (WR):",       "", 	"0134\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula1_2,         "O2 Sensor 2, Bank 1 (WR):",       "", 	"0135\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula1_3,         "O2 Sensor 3, Bank 1 (WR):",       "", 	"0136\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula1_4,         "O2 Sensor 4, Bank 1 (WR):",       "", 	"0137\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)	//105
	{ o2_sensor_wrc_formula2_1,         "O2 Sensor 1, Bank 2 (WR):",       "", 	"0138\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_2,         "O2 Sensor 2, Bank 2 (WR):",       "", 	"0139\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_3,         "O2 Sensor 3, Bank 2 (WR):",       "", 	"013A\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_4,         "O2 Sensor 4, Bank 2 (WR):",       "", 	"013B\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ cat_temp_formula1,              "CAT Temperature, B1S1:",          "", 	"013C\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK1-传感器1	//110
	{ cat_temp_formula2,              "CAT Temperature, B2S1:",          "", 	"013D\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK1-传感器2
	{ cat_temp_formula3,              "CAT Temperature, B1S2:",          "", 	"013E\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK2-传感器1
	{ cat_temp_formula4,              "CAT Temperature, B2S2:",          "", 	"013F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK2-传感器2

	//
	{ support_pid_41_5f,          	 "Supported PIDs 0x41 to 0x5F:",    "", 	"0140\r",      1,    8 ,200,0xFFFFFFFF,0xFFFFFFFF},//Supported PIDs 0x41 to 0x5F:
	{ monitior_state_formula,           "Monitor status this drive cycle:","", 	"0141\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//							  //115
	{ ecu_voltage_formula,           "ECU voltage:",                    "", 	"0142\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//控制模块电压
	{ abs_load_formula,              "Absolute Engine Load:",           "",		"0143\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//绝对载荷值
	{ eq_ratio_formula,              "Commanded Equivalence Ratio:",    "", 	"0144\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令的当量比
	{ relative_tp_formula,           "Relative Throttle Position:",     "", 	"0145\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门的相对位置
	{ amb_air_temp_formula,          "Ambient Air Temperature:",        "", 	"0146\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//环境气温same scaling as $0F //120	
	{ abs_tp_formula_b,                "Absolute Throttle Position B:",   "", 	"0147\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置B
	{ abs_tp_formula_c,                "Absolute Throttle Position C:",   "", 	"0148\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置C
	{ abs_tp_formula_d,                "Accelerator Pedal Position D:",   "", 	"0149\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置D
	{ abs_tp_formula_e,                "Accelerator Pedal Position E:",   "", 	"014A\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置E
	{ abs_tp_formula_f,                "Accelerator Pedal Position F:",   "", 	"014B\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置F			   //125
	{ tac_pct_formula,               "Comm. Throttle Actuator Cntrl:",  "", 	"014C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令油门控制commanded TAC
	{ mil_time_formula,              "Engine running while MIL on:",    "", 	"014D\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//MIL亮起后发动机运行分钟数minutes run by the engine while MIL activated
	{ clr_time_formula,              "Time since DTCs cleared:",        "", 	"014E\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//故障码清空后时间
	{ external_test_cfg_1,              "External test configuration #1:", "", 	"014F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//External test configuration #1


	{ external_test_cfg_2,          "External test configuration #2:", "", 		"0150\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//External test configuration #1	//130
	{ fuel_type,           			"Fuel type:",                    	"", 	"0151\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃料型号？？
	{ percentage_alcohol_fuel,		"Percentage of alcohol fuel mix:", "", 		"0152\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油中酒精含量
	{ abs_evap_system_vapor_press,	"Absolute Engine Load:",           "",		"0153\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//绝对蒸发系统蒸汽压力
	{ evap_system_vapor_press,      "Commanded Equivalence Ratio:",    "", 		"0154\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//蒸发系统蒸汽压力
	{ short_term_sec_oxygen_1_3,    "Short term sec-oxygen bank 1 3:",    	"", "0155\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//短期次氧传感器燃油修剪-BANK1 BANK3	//135
	{ long_term_sec_oxygen_1_3,     "Long term sec-oxygen bank 1 3:",       "", "0156\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//长期次氧传感器燃油修剪-BANK1 BANK3	
	{ short_term_sec_oxygen_2_4,    "Short term sec-oxygen bank 2 4:",   	"", "0157\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//短期次氧传感器燃油修剪-BANK2 BANK3
	{ long_term_sec_oxygen_2_4,     "Long term sec-oxygen bank 2  4:",   	"", "0158\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//长期次氧传感器燃油修剪-BANK2 BANK4
	{ fule_rail_pressure,      		"Fuel rail pressure:",   				"", "0159\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},	//
	{ relative_acc_pedal_pos,		"Relative accelerator:",   				"",	"015A\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},	//Relative accelerator pedal positio //140
	{ hybrid_battery_pack_remain,   "Hybrid battery pack:",   				"",	"015B\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//Hybrid battery pack remaining life
	{ engine_oil_temperature,		"Engine oil temperature:",  			"",	"015C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF}, // commanded TAC
	{ fuel_injection_timing,        "Fuel injection timing:",    			"",	"015D\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF}, // minutes run by the engine while MIL activated
	{ fuel_rate,             		"Fuel rate:",        					"", "015E\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//not used
	{ emission_requirements,        "Emission requirements:",        		"",	"015F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//Emission requirements for this vehicle	 //145

	{ NULL,                          "",                                	"",	"\r",        0,    0 ,200,0xFFFFFFFF,0xFFFFFFFF},


};

/*******************************************************************************
 * 函数名称:initObdMode                                                                     
 * 描    述:设置OBD工作模式                                                                    
 *                                                                               
 * 输    入:                                                                    
 * 输    出:无                                                                    
 * 返    回:命令执行状态                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int	initObdMode(int type){
	printf("\r\nset obd work mode\r\n");
//	obdAtAndWait("","",200);
	return 1; 


}
/*******************************************************************************
 * 函数名称:obdTest                                                                     
 * 描    述:OBD 测试代码                                                                    
 *                                                                               
 * 输    入:                                                                    
 * 输    出:无                                                                    
 * 返    回:无                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
void obdTest(void) {
	
	char cmd[4];
	int index = 0;
	cmd[0] = 'A';
	cmd[1] = 'T';
	cmd[2] = 0x04;
	
	printf("\r\ncmd :%s\r\n",cmd);
	ISP_DIRECTION=USART_OBD;
//	openUsart1();

	printf("\r\nOBD test+++++++++++++++++++++++++++\r\n");

	OBD_EN(Bit_RESET);
//	delay_ms(1000);
	obdPower(1);
//	delay_ms(1000);
//	delay_ms(1000);
	while(1){
		obdIrq();
		delay_ms(400);
	}
	
	
	while(1)
	{
		printf("\r\n");
		obdAtAndWait(cmd,"OK",500);
		printf("\r\n");
		obdAtAndWait(cmd,"OK",500);
		printf("\r\n");
		obdAtAndWait(cmd,"OK",500);
		printf("\r\n");
		obdAtAndWait(cmd,"OK",500);
		for(index = 0;index< 54;index++){
			execObd(index);
			delay_ms(500);
		}  
	} 
}



int16_t obdExec(int16_t index){ //放在timer中定时执行，选择一个合适的时钟	

//	if(sensors[index].enabled == 0){
//		index ++;
//		return 0;
//	}
//	USART_SendData(OBD,'0');	
//	while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
//	USART_SendData(OBD,'1');	
//	while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
//
//	USART_SendData(OBD,sensors[index].pid[0]);	
//	while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
//	USART_SendData(OBD,sensors[index].pid[1]);	
//	while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
//	USART_SendData(OBD,'\r');	
//	while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
//	index ++;
//	if(index == OBD_MAX_CMD_NUM)
//		index = 0;

	return 1;

}

void execObd(int index)
{
//	int i = 0;
////	printf("\r\n%s\t\t---->%s\r\n",sensors[index].label,sensors[index].pid);
//	memset(OBD_BUF,'\0',300);
//	OBD_COUNT = 0;	
////	for(i = 0;i<)
//	USART_SendData(OBD,'0');	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,'1');	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,sensors[index].pid[2]);	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,sensors[index].pid[3]);	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,'\r');	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	delay_ms(2000);
//	printf("\r\nDATA :\r\n");
//	for(i = 0;i<40;i++){
//		printf("%3X",OBD_BUF[i]);
//	}
}
void obdBlue(void)
{
	BLUE_OBD = 1;
	obdPower(0);
	blueToothPower(1);
	delay_ms(1000);
	obdPower(1);
	delay_ms(1000);
	ISP_DIRECTION=USART_SIM;
	
	while(1);
		
}
/*******************************************************************************
 * 函数名称:obdAtAndWait                                                                     
 * 描    述:OBD AT命令执行                                                                     
 *                                                                               
 * 输    入:at命令串，返回值查询关键字，命令执行等待时间                                                                    
 * 输    出:无                                                                     
 * 返    回:0：命令未出现期望结果，1：命令出现期望结果                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
int obdAtAndWait(char *at,char *key,int ms){
	int x = 0;
	memset(OBD_BUF,0,300);
	OBD_COUNT = 0;
	printf("\r\nAT--->%s\r\n",at);
	for(x=0;*(at+x)!='\0';x++)
	{
//		printf("%3X",*(at+x));
		USART_SendData(OBD,*(at+x));
		USART_SendData(ISP,*(at+x));
			
		while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET ); 
	}
//	USART_SendData(OBD,0X0A);
//	while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
	printf("\r\n");
	
	delay_ms(ms);
	if(strstr(OBD_BUF,key)){
		return 1;
	}
	else{
		return 0;
	}
}  

/*******************************************************************************
 * 函数名称:obdPower                                                                     
 * 描    述:设置OBD电源                                                                     
 *                                                                               
 * 输    入:0：关闭  1：启动                                                                     
 * 输    出:无                                                                    
 * 返    回:命令执行状态                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int obdPower(int state){

	delay_ms(1000);
	ISP_DIRECTION = USART_OBD ;
	if(state == 0){
		printf("\r\nOBD power off\r\n");
		OBD_EN(Bit_RESET);
		return 1;
	}
	else{
		printf("\r\nOBD power on\r\n");	   		
		OBD_EN(Bit_SET);
		return 1;		
	}
}
/*******************************************************************************
 * 函数名称:obdReset                                                                     
 * 描    述:复位OBD设备                                                                     
 *                                                                               
 * 输    入:0：芯片复位引脚复位  1：电源复位                                                                    
 * 输    出:无                                                                    
 * 返    回:命令执行状态                                                                     
 * 作    者:蛋蛋                                                                     
 * 修改日期:2012年3月1日                                                                    
 *******************************************************************************/
int	obdReset(int type){
	if(type){
		printf("\r\nOBD power off\r\n");
		OBD_EN(Bit_RESET);
		delay_ms(1000);
		OBD_EN(Bit_SET); 		
	}
	else{
		
//		OBD_RST(Bit_RESET);
		
		printf("\r\nOBD power on++++++++++++++++++++++++++\r\n");
		delay_ms(1000);
//		OBD_RST(Bit_SET);
	}
	return 1; 

}
int32_t obdIrq(void){
	uint32_t i = 0;
	uint16_t dataBytes = 0;
	static uint8_t counter = 0;
	static uint32_t initFlag  = 0;
	static uint16_t normalCounter = 0;
	static uint16_t fastCounter = 0;
	static uint16_t switchFlag = 0;
	static uint16_t normalIndex  = 0;
	static uint16_t fastIndex = 0;
	static uint16_t obdErrorFlag = 1;
	static uint16_t initIndex =  0 ;//sysCfg.obdConfig.initStart;
	char tmp = 0;
	
	if(obdErrorFlag == 1){
		if(initIndex == 0){
			initIndex = sysCfg.obdConfig.initStart;
//			memset("");
		}
		for(i = 0;;i++){
			tmp =  obdCmdList[sysCfg.obdConfig.cmdList[initIndex].cmdIndex].pid[i];
			USART_SendData(OBD,obdCmdList[sysCfg.obdConfig.cmdList[initIndex].cmdIndex].pid[i]);
			USART_SendData(ISP,obdCmdList[sysCfg.obdConfig.cmdList[initIndex].cmdIndex].pid[i]);	
			while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
			if(obdCmdList[sysCfg.obdConfig.cmdList[initIndex].cmdIndex].pid[i] == '\r'){
				break;
			}
		}
		initIndex++;
		if(initIndex > sysCfg.obdConfig.initEnd){
			obdErrorFlag = 0;
		}
		return ;		
	}
	
	if(initFlag == 0){
		initFlag = 1;
		normalIndex  = sysCfg.obdConfig.normalStart-1;
		fastIndex = sysCfg.obdConfig.fastStart-1;
	}					
	
	if(OBD_MODE == 0)
	{
//		printf("\r\nOBD in mormal mode++\r\n");
		//clear another process state
		fastIndex  = sysCfg.obdConfig.fastStart-1;
		fastCounter  = 0;
		
		if(normalCounter <= sysCfg.obdConfig.cmdList[normalIndex].timeOut){
		//	printf("\r\ntime left --> %d\r\n",sysCfg.obdConfig.cmdList[normalIndex].timeOut-normalCounter);
			normalCounter++;  			
			return 0;
		}
		else{
			normalCounter = 0;
		}
		dataBytes = sysCfg.obdConfig.cmdList[normalIndex].dataLength;

		if( dataBytes != 0 && obdCmdList[normalIndex].formula != NULL){
			if(normalIndex >= sysCfg.obdConfig.normalStart)
		 		obdCmdList[normalIndex].formula(normalIndex,OBD_BUF);
		}
		memset(OBD_BUF,'\0',300);
		OBD_COUNT = 0;
		normalIndex++;
		if(normalIndex > sysCfg.obdConfig.normalEnd){
//			printf("\r\n\nnormal obd  CIRCLE OVER,start next timer\n");
			normalIndex = sysCfg.obdConfig.normalStart;//	
		}
//		printf("\r\nCMD-->%d --> %d  -->%d --> %d\r\n",normalIndex,dataBytes,counter,sysCfg.obdConfig.cmdList[normalIndex].timeOut);
//		printf("\r\n normal index :%d ,cmd :%s\r\n\r\n",normalIndex,obdCmdList[sysCfg.obdConfig.cmdList[normalIndex].cmdIndex].pid);
//		printf("\r\ncmd :");
		for(i = 0;;i++){
			tmp =  obdCmdList[sysCfg.obdConfig.cmdList[normalIndex].cmdIndex].pid[i];
			USART_SendData(OBD,obdCmdList[sysCfg.obdConfig.cmdList[normalIndex].cmdIndex].pid[i]);
			USART_SendData(ISP,obdCmdList[sysCfg.obdConfig.cmdList[normalIndex].cmdIndex].pid[i]);	
			while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
			if(obdCmdList[sysCfg.obdConfig.cmdList[normalIndex].cmdIndex].pid[i] == '\r'){
				break;
			}
		}	
	}
	if(OBD_MODE == 1){
		printf("\r\nOBD in fast mode++\r\n");
		normalIndex = sysCfg.obdConfig.normalStart-1;
		normalCounter = 0;

		if(fastCounter <= sysCfg.obdConfig.cmdList[normalIndex].timeOut){
			fastCounter++;
			return 0;
		}
		else{
			fastCounter = 0;
		}

		dataBytes = sysCfg.obdConfig.cmdList[fastIndex].dataLength;

		if( dataBytes != 0 && obdCmdList[fastIndex].formula != NULL){
			if(fastIndex >= sysCfg.obdConfig.fastStart)
		 		obdCmdList[fastIndex].formula(fastIndex,OBD_BUF);
		}
		memset(OBD_BUF,'\0',300);
		OBD_COUNT = 0;
		fastIndex++;
		if(fastIndex > sysCfg.obdConfig.fastEnd){
//			printf("\r\n\nnormal obd  CIRCLE OVER,start next timer\n");
			fastIndex = sysCfg.obdConfig.fastStart;//	
		}
//		printf("\r\nCMD-->%d --> %d  -->%d --> %d\r\n",normalIndex,dataBytes,counter,sysCfg.obdConfig.cmdList[normalIndex].timeOut);
//		printf("\r\n normal index :%d ,cmd :%s\r\n\r\n",normalIndex,obdCmdList[sysCfg.obdConfig.cmdList[normalIndex].cmdIndex].pid);
//		printf("\r\ncmd :");
		for(i = 0;;i++){
			tmp =  obdCmdList[sysCfg.obdConfig.cmdList[fastIndex].cmdIndex].pid[i];
			USART_SendData(OBD,obdCmdList[sysCfg.obdConfig.cmdList[fastIndex].cmdIndex].pid[i]);
			USART_SendData(ISP,obdCmdList[sysCfg.obdConfig.cmdList[fastIndex].cmdIndex].pid[i]);	
			while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
			if(obdCmdList[sysCfg.obdConfig.cmdList[fastIndex].cmdIndex].pid[i] == '\r'){
				break;
			}
		}	
	}
//	printf("\r\nend\r\n");
	return 1;
}
/*******************************************************************************
 * 函数名称:fillObdBuf                                                                     
 * 描    述:对指定OBD命令的返回值填充到数据库                                                                   
 *                                                                               
 * 输    入:index ：需要执行的命令索引 buf：命令返回数据的缓冲区 value：数据有效性                                                                    
 * 输    出:无                                                                     
 * 返    回:0：命令未出现期望结果，1：命令出现期望结果                                                                    
 * 作    者:蛋蛋                                                                     
 * 修改日期:2011年7月13日                                                                    
 *******************************************************************************/
int16_t fillObdBuf(uint16_t index,char *buf,int value)
{															
	uint16_t i = 0;
	uint16_t point = 0; 
	uint16_t size = sysCfg.obdConfig.cmdList[index].dataLength ;
	printf("\r\nCURRENT INDEX:%d\r\n",index);
	printf("\r\nlabel :%s\r\n",obdCmdList[index].label);
	printf("\r\nCMD :%s\r\n",obdCmdList[index].pid);

	if(size == 0)
	{
		return size;
	}
	if(OBD_MODE == 0){
		//normal mode
		point = getBufIndex(index,OBD_MODE);	
		printf("\r\nneed data size-->%d  index-->%d value-->%d\r\n",size,point,value);
		for(i=0;i<size;i++){
			printf("%3X",buf[i]);
		}

	 	//copy data
		if(value){
			for(i=0;i<size;i++)
				obdNormalBuf.buf[point+i] = buf[i];
		}
		else
		{
			for(i=0;i<size;i++)
				obdNormalBuf.buf[point+i] = '?';
		}
	
		return size;		


	}
	if(OBD_MODE == 1){
		point = getBufIndex(index,OBD_MODE);	
		printf("\r\nneed data size-->%d  index-->%d value-->%d\r\n",size,point,value);
		for(i=0;i<size;i++){
			printf("%3X",buf[i]);
		}

	 	//copy data
		if(value){
			for(i=0;i<size;i++)
				obdFastBuf.buf[point+i] = buf[i];
		}
		else
		{
			for(i=0;i<size;i++)
				obdFastBuf.buf[point+i] = '?';
		}
	
		return size;	
	}						
}
void obdGetAllData(void){
	char cmd[10];
	int i1,i2,i3,i4,timeout=10000;
	char table[] = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
	BLUE_OBD = 0;
	again:
	ISP_DIRECTION=USART_OBD;
	printf("\r\nbegin obd test\r\n");
	for(i1 = 0;i1<10;i1++) {
		printf("0x%3X  ",table[i1]);
	}
	obdPower(0);
	delay_ms(3000);
	obdPower(1);

	delay_ms(5000);


	while(1)
	{
		
		obdAtAndWait("BT+RDTC\r\n",NULL,timeout);
		obdAtAndWait("BT+EDTC\r\n",NULL,timeout);
		obdAtAndWait("BT+MIL\r\n",NULL,timeout);
		obdAtAndWait("BT+PIDS\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.AD_FEH\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.VSS\r\n",NULL,timeout);
		obdAtAndWait("BT+MUL=&01&03&04&05&06&07&0B&0C&0D&0E\r\n",NULL,timeout);
		obdAtAndWait("BT+MUL=&0F&11&13&14&15&1C&20&21&40&47\r\n",NULL,timeout);
		obdAtAndWait("BT+MUL=&90&93&94&95&96\r\n",NULL,timeout);
		obdAtAndWait("BT+INFO.VIN\r\n",NULL,timeout);
		obdAtAndWait("BT+INFO.EID\r\n",NULL,timeout);
		obdAtAndWait("BT+SPWR\r\n",NULL,timeout);
		obdAtAndWait("BT+PMIL=12345\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.ECT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.SHRTFT1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LOAD\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.RPM\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.OBDSUP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.RUNTIME\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Fuel Sys1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Fuel Sys2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.load\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.ECT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.SHRTFT1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.SHRTFT2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.SHRTFT3\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.SHRTFT4\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LONGFT1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LONGFT2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LONGFT3\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LONGFT4\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Fuel P\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.MAP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.RPM\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.VSS\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Spark Adv\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.IAT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.MAF\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.TPS\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Air Stat\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B1S1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B1S2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B1S3\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B1S4\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B2S1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B2S2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B2S3\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.O2 B2S4\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.OBDSUP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.PTO_STAT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.RunTime\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Mil_dist\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.FRP_mv\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.FRP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT11\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT12\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT13\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT14\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT21\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT22\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT23\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT24\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EGR\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EGR_ERR\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EVAP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Fuel Lvl\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Warm ups\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.Clear Dist\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EVAP P\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.BARO\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT11\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT12\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT13\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT14\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT21\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT22\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT23\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT24\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.CAT TEMP11\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.CAT TEMP21\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.CAT TEMP12\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.CAT TEMP22\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.VPWR\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LOAD ABS\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EQ_RAT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.TP_R\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.AAT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.TPS_B\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.TPS_C\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.APP_D\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.APP_E\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.APP_F\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.TAC_PCT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.MIL_TIME\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.CLR_TIME\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.ALCH_PCT\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EVAP_VPA\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.EVAP_VP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.STSO2FT1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.STSO2FT2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.STSO2FT3\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.STSO2FT4\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LGSO2FT1\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LGSO2FT2\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LGSO2FT3\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.LGSO2FT4\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.FRPA\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.APP_R\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.IFE\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.WHP\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.AD_Mil\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.AD_Accel\r\n",NULL,timeout);
		obdAtAndWait("BT+DATA.AD_FEH\r\n",NULL,timeout);
	}
	obdAtAndWait("ATS0\r",NULL,500);
	obdAtAndWait("AT@1\r",NULL,500);
	obdAtAndWait("ATI\r",NULL,500);
	obdAtAndWait("STI\r",NULL,500);
	obdAtAndWait("ATPP 0E OFF\r",NULL,500);
	obdAtAndWait("ST SL off,on\r",NULL,500);
	obdAtAndWait("ST SLU off,on\r",NULL,500);
	obdAtAndWait("ST SLVL off,on\r",NULL,500);
	obdAtAndWait("ST SLVG off\r",NULL,500);
	obdAtAndWait("ATAT1\r",NULL,500);
	obdAtAndWait("ATSP0\r",NULL,500);
	obdAtAndWait("0100\r",NULL,5000);
	obdAtAndWait("ATDPN\r",NULL,500);
	obdAtAndWait("0902\r",NULL,500);
	obdAtAndWait("09025\r",NULL,500);

//	obdAtAndWait("atign\r",NULL,5000);
	obdAtAndWait("at@1\r",NULL,500);
	obdAtAndWait("at@2\r",NULL,500);
	for(i4 = 0;i4 < 30;i4++){
		printf("%3X",OBD_BUF[i4]);
	}
	obdAtAndWait("ati\r",NULL,500);
	
	obdAtAndWait("ATE0\r",NULL,500);
	
	obdAtAndWait("at@1\r",NULL,500);
	obdAtAndWait("at@2\r",NULL,200);
	


//	obdAtAndWait("0900\r",NULL,5000);
//	obdAtAndWait("0910\r",NULL,5000);
//	obdAtAndWait("0920\r",NULL,5000);
//	obdAtAndWait("0930\r",NULL,5000);
//	obdAtAndWait("0940\r",NULL,5000);
//	obdAtAndWait("0950\r",NULL,5000);
//	obdAtAndWait("0960\r",NULL,5000);
//	obdAtAndWait("0970\r",NULL,5000);
//	obdAtAndWait("0980\r",NULL,5000);
//	obdAtAndWait("0990\r",NULL,5000);
//	obdAtAndWait("09A0\r",NULL,5000);
//	obdAtAndWait("09B0\r",NULL,5000);
//	obdAtAndWait("09C0\r",NULL,5000);
//	obdAtAndWait("09D0\r",NULL,5000);
//	obdAtAndWait("09E0\r",NULL,5000);
//	obdAtAndWait("09F0\r",NULL,5000);
//	obdAtAndWait("09\r",NULL,5000);
//	obdAtAndWait("03\r",NULL,5000);
//
//	for(i4 = 0;i4 < 30;i4++){
//		printf("%3X",OBD_BUF[i4]);
//	}
//	obdAtAndWait("ati\r",NULL,500);
//
//
//	for(i1 = 9;i1 < 10;i1++){
//		for(i2=0;i2<16;i2++)
//		{
//			for(i3 = 0;i3<16;i3++)
//			{
//				cmd[0] = '0';
//				cmd[1] = table[i1];
//				cmd[2] = table[i2];
//				cmd[3] = table[i3];
//				cmd[4] = '\r' ;
//
//				printf("\r\n\r\nCMD :%s\r\n",cmd);
//				obdAtAndWait(cmd,NULL,2000);
//				printf("BUF DATA: ");
//				for(i4 = 0;i4 < 30;i4++){
//					printf("%3X",OBD_BUF[i4]);
//				}
//
//			}	
//		}
//	}
	for(i1 = 1;i1 < 10;i1++){
		for(i2=0;i2<16;i2++)
		{
			for(i3 = 0;i3<16;i3++)
			{
				cmd[0] = '0';
				cmd[1] = table[i1];
				cmd[2] = table[i2];
				cmd[3] = table[i3];
				cmd[4] = '\r' ;
				cmd[5] = '\n' ;

				printf("\r\n\r\nCMD :%s\r\n",cmd);
				obdAtAndWait(cmd,NULL,1500);
				printf("BUF DATA: ");
				for(i4 = 0;i4 < 30;i4++){
					printf("%3X",OBD_BUF[i4]);
				}

			}	
		}
	}
	goto again;
	
	obdAtAndWait("",NULL,500);
}
void obdInitData(void){
	obdNormalBuf.itemNum = 0;
	obdFastBuf.itemNum = 0;
	memset(obdNormalBuf.buf,'#',400);
	memset(obdFastBuf.buf,'#',400);
}
void obdInitChip(void){
	char cmd[10];
	obdInitData();


	BLUE_OBD = 0;
	obdPower(0);
	delay_ms(1000);
	obdPower(1);
	delay_ms(1000);
	ISP_DIRECTION=USART_OBD;
	return ;
//	for();

	obdAtAndWait("at\r",NULL,500);
	obdAtAndWait("ATE0\r",NULL,500);
	obdAtAndWait("ATE0\r",NULL,500);
	obdAtAndWait("ATM0\r",NULL,500);
	obdAtAndWait("ATL0\r",NULL,500);
	obdAtAndWait("ATS0\r",NULL,500);
	obdAtAndWait("AT@1\r",NULL,500);
	obdAtAndWait("ATI\r",NULL,500);
	obdAtAndWait("STI\r",NULL,500);
	obdAtAndWait("ATPP 0E OFF\r",NULL,500);
	obdAtAndWait("ST SL off,on\r",NULL,500);
	obdAtAndWait("ST SLU off,on\r",NULL,500);
	obdAtAndWait("ST SLVL off,on\r",NULL,500);
	obdAtAndWait("ST SLVG off\r",NULL,500);
	obdAtAndWait("ATAT1\r",NULL,500);
	obdAtAndWait("ATSP0\r",NULL,500);
	obdAtAndWait("0100\r",NULL,5000);
	obdAtAndWait("ATDPN\r",NULL,500);
	obdAtAndWait("0902\r",NULL,500);
	obdAtAndWait("09025\r",NULL,500);  
	
}










