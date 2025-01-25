/////////////////////////////////////////////////////////////////////////////////
// Automotive Network Evaluation Board v1.1
// Arquivo: board.h
//
// Arquivo de definicao da pinagem utilizada pela placa dedesenvolvimento.
// Utilize este arquivo de definicoes para desenvolver aplicacoes
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//1 - ENTRADAS DIGITAIS (PUSHBUTTONS)
/////////////////////////////////////////////////////////////////////////////////

//ECU1
#define ECU1_DIN1 MCU_DIN0_PIN
#define ECU1_DIN2 MCU_DIN1_PIN
#define ECU1_DIN3 MCU_DIN2_PIN
#define ECU1_DIN4 MCU_DIN3_PIN

//ECU 2
#define ECU2_DIN1 MCU_DIN0_PIN
#define ECU2_DIN2 MCU_DIN1_PIN
#define ECU2_DIN3 MCU_DIN2_PIN
#define ECU2_DIN4 MCU_DIN3_PIN

//ECU3
#define ECU3_DIN1 MCU_DIN0_PIN
#define ECU3_DIN2 MCU_DIN1_PIN
#define ECU3_DIN3 MCU_DIN2_PIN
#define ECU3_DIN4 MCU_DIN3_PIN

//ECU4
#define ECU4_DIN1 MCU_DIN0_PIN
#define ECU4_DIN2 MCU_DIN1_PIN
#define ECU4_DIN3 MCU_DIN2_PIN
#define ECU4_DIN4 MCU_DIN3_PIN

/////////////////////////////////////////////////////////////////////////////////
//2 - SAIDAS DIGITAIS
/////////////////////////////////////////////////////////////////////////////////

//ECU1
#define ECU1_DOUT1 MCU_DOUT0_PIN
#define ECU1_DOUT2 MCU_DOUT1_PIN
#define ECU1_BUZZER MCU1_BUZZ_PIN

//ECU2
#define ECU2_DOUT1 MCU_DOUT0_PIN
#define ECU2_DOUT2 MCU_DOUT1_PIN

//ECU3
#define ECU3_DOUT1 MCU_DOUT0_PIN
#define ECU3_DOUT2 MCU_DOUT1_PIN

//ECU4
#define ECU4_DOUT1 MCU_DOUT0_PIN
#define ECU4_DOUT2 MCU_DOUT1_PIN

/////////////////////////////////////////////////////////////////////////////////
//3 - ENTRADAS ANALOGICAS (POTENCIOMETROS)
/////////////////////////////////////////////////////////////////////////////////

//ECU1
#define ECU1_AIN1 MCU_AIN0_PIN
#define ECU1_AIN2 MCU_AIN1_PIN
#define ECU1_AIN3 MCU_AIN2_PIN
#define ECU1_AIN4 MCU_AIN3_PIN
#define ECU1_LDR_AIN MCU1_AIN0_PIN

//ECU2
#define ECU2_AIN1 MCU_AIN0_PIN
#define ECU2_AIN2 MCU_AIN1_PIN
#define ECU2_AIN3 MCU_AIN2_PIN
#define ECU2_AIN4 MCU_AIN3_PIN
#define ECU2_LOOP_AIN MCU2_AIN0_PIN

//ECU3
#define ECU3_AIN1 MCU_AIN0_PIN
#define ECU3_AIN2 MCU_AIN1_PIN
#define ECU3_AIN3 MCU_AIN2_PIN
#define ECU3_AIN4 MCU_AIN3_PIN

//ECU4
#define ECU4_AIN1 MCU_AIN0_PIN
#define ECU4_AIN2 MCU_AIN1_PIN
#define ECU4_AIN3 MCU_AIN2_PIN
#define ECU4_AIN4 MCU_AIN3_PIN

/////////////////////////////////////////////////////////////////////////////////
//5 - SAIDAS ANALOGICAS (LDR LED E LOOP)
/////////////////////////////////////////////////////////////////////////////////

#define ECU1_LOOP MCU1_AOUT0_PIN //Varia de 0 a 1023
#define ECU1_LDR_LED MCU1_AOUT1_PIN //Varia 0 a 255

/////////////////////////////////////////////////////////////////////////////////
//6 - REDE CAN (CAN1_BUS E CAN2_BUS)
/////////////////////////////////////////////////////////////////////////////////

//ECU1- Somente CAN1_BUS
#define ECU1_CAN1_CS CAN1_CS_PIN //Chip selector (CS)
#define ECU1_CAN1_INT CAN_INT_PIN //Interrupt pin

//ECU2 - CAN1_BUS e CAN2_BUS
#define ECU2_CAN1_CS CAN1_CS_PIN //Chip selector (CS)
#define ECU2_CAN2_CS CAN2_CS_PIN //Chip selector (CS)
#define ECU2_CAN1_INT CAN_INT_PIN //Interrupt pin
#define ECU2_CAN2_INT CAN_INT_PIN //Interrupt pin

//ECU3 - CAN1_BUS ou CAN2_BUS
#define ECU3_CAN1_CS CAN1_CS_PIN //Chip selector (CS)
#define ECU3_CAN2_CS CAN1_CS_PIN //Chip selector (CS)
#define ECU3_CAN1_INT CAN_INT_PIN //Interrupt pin
#define ECU3_CAN2_INT CAN_INT_PIN //Interrupt pin

//ECU4 - CAN1_BUS ou CAN2_BUS
#define ECU4_CAN1_CS CAN1_CS_PIN //Chip selector (CS)
#define ECU4_CAN2_CS CAN1_CS_PIN //Chip selector (CS)
#define ECU4_CAN1_INT CAN_INT_PIN //Interrupt pin
#define ECU4_CAN2_INT CAN_INT_PIN //Interrupt pin

/////////////////////////////////////////////////////////////////////////////////
//7 - REDE LIN (ECU3 E ECU4)
//Obs: Utilize a biblioteca SoftwareSerial
/////////////////////////////////////////////////////////////////////////////////

//ECU3
#define ECU3_LIN_TX LIN_TX_PIN
#define ECU3_LIN_RX LIN_RX_PIN

//ECU4
#define ECU4_LIN_TX LIN_TX_PIN
#define ECU4_LIN_RX LIN_RX_PIN

/////////////////////////////////////////////////////////////////////////////////
//PIN MAPPING
/////////////////////////////////////////////////////////////////////////////////

#define MCU_DIN0_PIN A4
#define MCU_DIN1_PIN A5
#define MCU_DIN2_PIN 9
#define MCU_DIN3_PIN 8
#define MCU1_BUZZ_PIN 7
#define MCU_DOUT0_PIN 3
#define MCU_DOUT1_PIN 4
#define MCU_AIN0_PIN A0
#define MCU_AIN1_PIN A1
#define MCU_AIN2_PIN A2
#define MCU_AIN3_PIN A3
#define MCU1_AIN0_PIN A6
#define MCU2_AIN0_PIN A6
#define MCU2_AIN0_PIN A7
#define MCU1_AOUT0_PIN 5
#define MCU1_AOUT1_PIN 6
#define CAN1_CS_PIN 10
#define CAN2_CS_PIN 6
#define CAN_INT_PIN 2

//Lin tx and rx pin
#define LIN_TX_PIN 5
#define LIN_RX_PIN 6