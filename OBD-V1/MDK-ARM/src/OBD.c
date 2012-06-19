#include "OBD.h"
OBD_MSG_BUG obdBuf;
void engine_rpm_formula(int data, char *buf)
{
//      sprintf(buf, "%i r/min", (int)((float)data/4));
	char *point = strstr(buf,"410C");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//			>010C
//			NO DATA



void engine_load_formula(int data, char *buf)
{
//   sprintf(buf, "%.1f%%", (float)data*100/255);
	char *point = strstr(buf,"4104");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}


void coolant_temp_formula(int data, char *buf)
{
//      sprintf(buf, "%i%c C", data-40, 0xB0);
	char *point = strstr(buf,"4105");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
   if (data == 0)
   	sprintf(buf, "unused");
   else if (data == 0x01)
      sprintf(buf, "open loop");
   else if (data == 0x02)
      sprintf(buf, "closed loop");
   else if (data == 0x04)
      sprintf(buf, "open loop (driving conditions)");
   else if (data == 0x08)
      sprintf(buf, "open loop (system fault)");
   else if (data == 0x10)
      sprintf(buf, "closed loop, O2 sensor fault");
   else
      sprintf(buf, "unknown: 0x%02X", data);
}


void fuel_system1_status_formula(int data, char *buf)
{
//   	if(checkObdReturn < 0)
//   	fillObdBuf(data,buf,1);
	char *point = strstr(buf,"4103");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
void short_term_fuel_trim_formula1(int data, char *buf)
{
//   if (data > 0xFF)  // we're only showing bank 1 and 2 FT
//      data >>= 8;
//
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", ((float)data - 128)*100/128);

   	char *point = strstr(buf,"4106");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}
//		>0106
//		410E7E



void long_term_fuel_trim_formula1(int data, char *buf)
{
//   if (data > 0xFF)  // we're only showing bank 1 and 2 FT
//      data >>= 8;
//
//   sprintf(buf, (data == 128) ? "0.0%%" : "%+.1f%%", ((float)data - 128)*100/128);
   	char *point = strstr(buf,"4107");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
		fillObdBuf(data,buf,0);
	}
	else
	{
		fillObdBuf(data,point+4,1);
	}
}

void fuel_system1_frame_dtc(int data, char *buf)
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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


void numberOftroubleCodes(int data, char *buf)
{
//   sprintf(buf, "%i hrs %i min", data/60, data%60);

	char *point = strstr(buf,"4101");
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0xF) < 0 && point != NULL){
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
	if(checkObdReturn(buf,0x1) < 0)
	{
		fillObdBuf(index,buf,0);
	}
	else
	{
	 	fillObdBuf(index,buf,1);
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
uint16_t getBufIndex(uint16_t index)
{
	uint16_t i = 0;	
	static uint16_t mapIndex[200];
	static uint16_t flag = 0;
	if(flag == 0)		
	{ 
		mapIndex[0] = 0;
		for(i = 0;i<sysCfg.obdConfig.cmdNum;i++)
		{
			mapIndex[i+1] += sysCfg.obdConfig.cmdList[index].dataLegth ;		
		}
		printf("\r\nINDEX MAP\r\n");
		for(i = 0 ;i < 200;i++)
		{
		 	if(i %20 == 0)
				printf("\r\n");
			printf("%3d->%3d  ",i,mapIndex[i]);
		}
		flag = 1;
	}
	return mapIndex[index];		

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
	{NULL,							 	"car ",							"",		"ATI\r",	 1,	  	4,  200,0,0}, //49
	{ support_pid_00_1f,   			 "Support pids:",           		"", 	"0100\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//
	{ numberOftroubleCodes,   		 "Number of trouble codes:",       	"",		"0101\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//错误码
	{ fuel_system1_frame_dtc,		 "Freeze frame DTC:",           	"",	 	"0102\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力总成的冻结帧数据
	{ fuel_system1_status_formula,   "Fuel System 1 Status:",           "", 	"0103\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油系统1
//	{ fuel_system2_status_formula,   "Fuel System 2 Status:",           "", 	"0103\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油系统2 
	{ engine_load_formula,           "Calculated Load Value:",          "", 	"0104\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机负荷
	{ coolant_temp_formula,          "Coolant Temperature:",            "", 	"0105\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机冷却液温度
	{ short_term_fuel_trim_formula1,  "Short Term Fuel Trim (Bank 1):",  "", 	"0106\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//短时燃油修正BANK1
	{ long_term_fuel_trim_formula1,   "Long Term Fuel Trim (Bank 1):",   "", 	"0107\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//长时燃油修正BANK1
	{ short_term_fuel_trim_formula2,  "Short Term Fuel Trim (Bank 2):",  "", 	"0108\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//短时燃油修正 BANK2
	{ long_term_fuel_trim_formula2,   "Long Term Fuel Trim (Bank 2):",   "", 	"0109\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//长时燃油修正 BANK2
	{ fuel_pressure_formula,         "Fuel Pressure (gauge):",          "", 	"010A\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油导轨压力
	{ intake_pressure_formula,       "Intake Manifold Pressure:",       "", 	"010B\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//进气歧管绝对压力
	{ engine_rpm_formula,            "Engine RPM:",                     "", 	"010C\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机转速
	{ vehicle_speed_formula,         "Vehicle Speed:",                  "", 	"010D\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//车速
	{ timing_advance_formula,        "Timing Advance (Cyl. #1):",       "", 	"010E\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//1号气缸点火提前角
	{ intake_air_temp_formula,       "Intake Air Temperature:",         "", 	"010F\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//进气温度


	{ air_flow_rate_formula,         "Air Flow Rate (MAF sensor):",     "", 	"0110\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//空气流速
	{ throttle_position_formula,     "Absolute Throttle Position:",     "", 	"0111\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置	
	{ secondary_air_status_formula,  "Secondary air status:",           "", 	"0112\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//二次空气状态
	{ oxygen_sensors_present_1,  	 "Oxygen sensors present 1:",       "", 	"0113\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧含量传感器，多数无
	{ o2_sensor_formula1_1,          "O2 Sensor 1, Bank 1:",            "", 	"0114\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -1
	{ o2_sensor_formula1_2,          "O2 Sensor 2, Bank 1:",            "", 	"0115\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -2
	{ o2_sensor_formula1_3,          "O2 Sensor 3, Bank 1:",            "", 	"0116\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -3
	{ o2_sensor_formula1_4,          "O2 Sensor 4, Bank 1:",            "", 	"0117\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK1 -4
	{ o2_sensor_formula2_1,          "O2 Sensor 1, Bank 2:",            "", 	"0118\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -1
	{ o2_sensor_formula2_2,          "O2 Sensor 2, Bank 2:",            "", 	"0119\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -2
	{ o2_sensor_formula2_3,          "O2 Sensor 3, Bank 2:",            "", 	"011A\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -3
	{ o2_sensor_formula2_4,          "O2 Sensor 4, Bank 2:",            "", 	"011B\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧传感器BANK2 -4
	{ obd_requirements_formula,      "OBD conforms to:",                "", 	"011C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//OBD模式
	{ oxygen_sensors_present_2,      "Oxygen sensors present 2:",       "", 	"011D\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//氧含量传感器2，多数无 
	{ pto_status_formula,            "Power Take-Off Status:",          "", 	"011E\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力输出状态
	{ engine_run_time_formula,       "Time Since Engine Start:",        "", 	"011F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力运行时间

	
	
	// Page 3
	{ support_pid_20_3f,          	 "Supported PIDs 0x21 to 0x3F:",    "", 	"0120\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//Supported PIDs 0x21 to 0x3F
	{ mil_distance_formula,          "Distance since MIL activated:",   "", 	"0121\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//MIL凉气候的行驶距离
	{ frp_relative_formula,          "FRP rel. to manifold vacuum:",    "", 	"0122\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},// 燃油导轨压力（相对于歧管真空压力）
	{ frp_widerange_formula,         "Fuel Pressure (gauge):",          "", 	"0123\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},// 燃油导轨压力
	{ o2_sensor_wrv_formula1_1,         "O2 Sensor 1, Bank 1 (WR):",       "",	"0124\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula1_2,         "O2 Sensor 2, Bank 1 (WR):",       "", 	"0125\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula1_3,         "O2 Sensor 3, Bank 1 (WR):",       "", 	"0126\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula1_4,         "O2 Sensor 4, Bank 1 (WR):",       "", 	"0127\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_1,         "O2 Sensor 1, Bank 2 (WR):",       "", 	"0128\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_2,         "O2 Sensor 2, Bank 2 (WR):",       "", 	"0129\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_3,         "O2 Sensor 3, Bank 2 (WR):",       "", 	"012A\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ o2_sensor_wrv_formula2_4,         "O2 Sensor 4, Bank 2 (WR):",       "", 	"012B\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
	{ commanded_egr_formula,         "Commanded EGR:",                  "", 	"012C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令EGR
	{ egr_error_formula,             "EGR Error:",                      "", 	"012D\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//EGR错误
	{ evap_pct_formula,              "Commanded Evaporative Purge:",    "", 	"012E\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令的燃油蒸汽排出
	{ fuel_level_formula,            "Fuel Level Input:",               "", 	"012F\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油量输入


	{ warm_ups_formula,              "Warm-ups since ECU reset:",       "", 	"0130\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//故障代码清空后的热车次数
	{ clr_distance_formula,          "Distance since ECU reset:",       "", 	"0131\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//故障代码清空后的行驶距离
	{ evap_vp_formula,               "Evap System Vapor Pressure:",     "", 	"0132\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//EVAP蒸汽压力
	{ baro_pressure_formula,         "Barometric Pressure (absolute):", "", 	"0133\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//气压
	{ o2_sensor_wrc_formula1_1,         "O2 Sensor 1, Bank 1 (WR):",       "", 	"0134\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula1_2,         "O2 Sensor 2, Bank 1 (WR):",       "", 	"0135\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula1_3,         "O2 Sensor 3, Bank 1 (WR):",       "", 	"0136\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula1_4,         "O2 Sensor 4, Bank 1 (WR):",       "", 	"0137\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_1,         "O2 Sensor 1, Bank 2 (WR):",       "", 	"0138\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_2,         "O2 Sensor 2, Bank 2 (WR):",       "", 	"0139\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_3,         "O2 Sensor 3, Bank 2 (WR):",       "", 	"013A\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ o2_sensor_wrc_formula2_4,         "O2 Sensor 4, Bank 2 (WR):",       "", 	"013B\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},//o2 sensors (wide range)
	{ cat_temp_formula1,              "CAT Temperature, B1S1:",          "", 	"013C\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK1-传感器1
	{ cat_temp_formula2,              "CAT Temperature, B2S1:",          "", 	"013D\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK1-传感器2
	{ cat_temp_formula3,              "CAT Temperature, B1S2:",          "", 	"013E\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK2-传感器1
	{ cat_temp_formula4,              "CAT Temperature, B2S2:",          "", 	"013F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//催化剂温度BANK2-传感器2

	//
	{ support_pid_41_5f,          	 "Supported PIDs 0x41 to 0x5F:",    "", 	"0140\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//Supported PIDs 0x41 to 0x5F:
	{ monitior_state_formula,           "Monitor status this drive cycle:","", 	"0141\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//
	{ ecu_voltage_formula,           "ECU voltage:",                    "", 	"0142\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//控制模块电压
	{ abs_load_formula,              "Absolute Engine Load:",           "",		"0143\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//绝对载荷值
	{ eq_ratio_formula,              "Commanded Equivalence Ratio:",    "", 	"0144\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令的当量比
	{ relative_tp_formula,           "Relative Throttle Position:",     "", 	"0145\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门的相对位置
	{ amb_air_temp_formula,          "Ambient Air Temperature:",        "", 	"0146\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//环境气温same scaling as $0F 	
	{ abs_tp_formula_b,                "Absolute Throttle Position B:",   "", 	"0147\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置B
	{ abs_tp_formula_c,                "Absolute Throttle Position C:",   "", 	"0148\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置C
	{ abs_tp_formula_d,                "Accelerator Pedal Position D:",   "", 	"0149\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置D
	{ abs_tp_formula_e,                "Accelerator Pedal Position E:",   "", 	"014A\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置E
	{ abs_tp_formula_f,                "Accelerator Pedal Position F:",   "", 	"014B\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//节气门绝对位置F
	{ tac_pct_formula,               "Comm. Throttle Actuator Cntrl:",  "", 	"014C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//指令油门控制commanded TAC
	{ mil_time_formula,              "Engine running while MIL on:",    "", 	"014D\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//MIL亮起后发动机运行分钟数minutes run by the engine while MIL activated
	{ clr_time_formula,              "Time since DTCs cleared:",        "", 	"014E\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//故障码清空后时间
	{ external_test_cfg_1,              "External test configuration #1:", "", 	"014F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//External test configuration #1


	{ external_test_cfg_2,          "External test configuration #2:", "", 		"0150\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//External test configuration #1
	{ fuel_type,           			"Fuel type:",                    	"", 	"0151\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃料型号？？
	{ percentage_alcohol_fuel,		"Percentage of alcohol fuel mix:", "", 		"0152\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油中酒精含量
	{ abs_evap_system_vapor_press,	"Absolute Engine Load:",           "",		"0153\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//绝对蒸发系统蒸汽压力
	{ evap_system_vapor_press,      "Commanded Equivalence Ratio:",    "", 		"0154\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//蒸发系统蒸汽压力
	{ short_term_sec_oxygen_1_3,    "Short term sec-oxygen bank 1 3:",    	"", "0155\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//短期次氧传感器燃油修剪-BANK1 BANK3
	{ long_term_sec_oxygen_1_3,     "Long term sec-oxygen bank 1 3:",       "", "0156\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//长期次氧传感器燃油修剪-BANK1 BANK3	
	{ short_term_sec_oxygen_2_4,    "Short term sec-oxygen bank 2 4:",   	"", "0157\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//短期次氧传感器燃油修剪-BANK2 BANK3
	{ long_term_sec_oxygen_2_4,     "Long term sec-oxygen bank 2  4:",   	"", "0158\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//长期次氧传感器燃油修剪-BANK2 BANK4
	{ fule_rail_pressure,      		"Fuel rail pressure:",   				"", "0159\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},	//
	{ relative_acc_pedal_pos,		"Relative accelerator:",   				"",	"015A\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},	//Relative accelerator pedal positio
	{ hybrid_battery_pack_remain,   "Hybrid battery pack:",   				"",	"015B\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//Hybrid battery pack remaining life
	{ engine_oil_temperature,		"Engine oil temperature:",  			"",	"015C\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF}, // commanded TAC
	{ fuel_injection_timing,        "Fuel injection timing:",    			"",	"015D\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF}, // minutes run by the engine while MIL activated
	{ fuel_rate,             		"Fuel rate:",        					"", "015E\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//not used
	{ emission_requirements,        "Emission requirements:",        		"",	"015F\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//Emission requirements for this vehicle

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
	openUsart1();

	printf("\r\nOBD test+++++++++++++++++++++++++++\r\n");

	OBD_EN(Bit_RESET);
//	while(1){
//		printf("\r\nOBD test+++++++++++++++++++++++++++\r\n");
//		OBD_EN(Bit_SET);
//		delay_ms(5000);
//		printf("\r\nOBD test---------------------------\r\n");
//		OBD_EN(Bit_RESET);
//		delay_ms(5000);
//	}
	delay_ms(1000);
	obdPower(1);
	delay_ms(1000);
	
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
//	printf("\r\n%s\t\t---->%s\r\n",sensors[index].label,sensors[index].pid);
//	memset(OBD_BUF,'\0',300);
//	OBD_COUNT = 0;	
//	USART_SendData(OBD,'0');	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,'1');	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,sensors[index].pid[0]);	
//	while(USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET);
//	USART_SendData(OBD,sensors[index].pid[1]);	
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
	blueToothPower(1);
		
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
	for(x=0;*(at+x)!='\0';x++)
	{
		USART_SendData(OBD,*(at+x));	
		while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET ); 
	}
	BLUE_COUNT = 0;
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
void obdIrq(void){
	static uint32_t index  = 0;
	uint32_t i = 0;
	uint16_t dataBytes= (sysCfg.obdConfig.cmdList[index] & 0xF000) >> 12;
	if( dataBytes != 0 && obdCmdList[index].formula != NULL){
//		if(strstr(OBD_BUF,"NO DATA"))
	 	obdCmdList[index].formula(index,OBD_BUF);
	}
	memset(SIM_BUF,'\0',300);
	index++;
	if(index > sysCfg.obdConfig.cmdNum)
		index = 0;
	for(i = 0;;i++){
		
		USART_SendData(OBD,obdCmdList[sysCfg.obdConfig.cmdList[index] & 0x0FFF].pid[i]);	
		while( USART_GetFlagStatus(OBD,USART_FLAG_TC)==RESET );
		if(obdCmdList[sysCfg.obdConfig.cmdList[index] & 0x0FFF].pid[i] == '\r'){
			break;
		}
	}
}

int16_t fillObdBuf(uint16_t index,char *buf,int value)
{
	uint16_t i = 0;
	uint16_t point; 
	uint16_t size = sysCfg.obdConfig.cmdList[index].dataLegth ;
	printf("\r\nCURRENT INDEX:%d\r\n",index);
	printf("\r\nlabel :%s\r\n",obdCmdList[index].label);
	printf("\r\nCMD :%s\r\n",obdCmdList[index].pid);

	if(size == 0)
	{
		return size;
	}
	point = getBufIndex(index);	
	printf("\r\nneed data size: %d  value :%d\r\n",point,value);
	for(i=0;i<size;i++){
		printf("%3X",buf[i]);
	}
	printf("\r\ndata end\r\n");
 	//copy data
	if(value){
		for(i=0;i<size;i++)
			obdBuf.obdBuf[point+i] = buf[i];
	}
	else
	{
		for(i=0;i<size;i++)
			obdBuf.obdBuf[point+i] = 0;
	}

	return size;
}










