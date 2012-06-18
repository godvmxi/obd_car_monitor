#include "OBD.h"
typedef struct
{
	void (*formula)(int raw_data, char *buf);  //fromula handler
	char label[32];							  //label
	
//	char ackData[32];						  //not nessory
	char ackBuf[32];
	char cmd[32];								  //cmd 
	int enabled;								  //whether to cmd
	int bytes; 								  // number of data bytes expected from vehicle
	int timeOut;							//命令超时
	int data1;
	int data2;
} OBD_SENSOR;


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
	{ fuel_system1_status_formula,   "Number of trouble codes:",       	"",		"0101\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//错误码
	{ fuel_system1_frame_dtc,		 "Freeze frame DTC:",           	"",	 	"0102\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//动力总成的冻结帧数据
	{ fuel_system1_status_formula,   "Fuel System 1 Status:",           "", 	"0103\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油系统1
//	{ fuel_system2_status_formula,   "Fuel System 2 Status:",           "", 	"0103\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//燃油系统2 
	{ engine_load_formula,           "Calculated Load Value:",          "", 	"0104\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机负荷
	{ coolant_temp_formula,          "Coolant Temperature:",            "", 	"0105\r",      1,    1 ,200,0xFFFFFFFF,0xFFFFFFFF},//发动机冷却液温度
	{ short_term_fuel_trim_formula,  "Short Term Fuel Trim (Bank 1):",  "", 	"0106\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//短时燃油修正BANK1
	{ long_term_fuel_trim_formula,   "Long Term Fuel Trim (Bank 1):",   "", 	"0107\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//长时燃油修正BANK1
	{ short_term_fuel_trim_formula,  "Short Term Fuel Trim (Bank 2):",  "", 	"0108\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//短时燃油修正 BANK2
	{ long_term_fuel_trim_formula,   "Long Term Fuel Trim (Bank 2):",   "", 	"0109\r",      1,    2 ,200,0xFFFFFFFF,0xFFFFFFFF},//长时燃油修正 BANK2
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
	{ o2_sensor_wrv_formula1_1,         "O2 Sensor 1, Bank 1 (WR):",       "",		"0124\r",      1,    4 ,200,0xFFFFFFFF,0xFFFFFFFF},// o2 sensors (wide range)
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

	{ NULL,                          "",                                	"",	"\r",        0,    0 ,200,0xFFFFFFFF,0xFFFFFFFF}


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
	uint16_t size =( ( sysCfg.obdConfig.cmdList[index] & 0xF000 ) >> 12 )*4;
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










