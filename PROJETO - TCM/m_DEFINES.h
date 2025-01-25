//////////////////////////////////////////////////////////////////////////
// DEFINIÇÕES DAS MENSAGENS
// LUAN FÁBIO MARINHO GALINDO
//////////////////////////////////////////////////////////////////////////

//Macros para definicoes - mensagem EECU1 - Taxa de comunicação 100ms
#define mEECU1_ID 0x0CF00400    // m -> message, EECU1 -> Nome ou id da mensagem geral
#define mEECU1_DLC 8			// mensagem M2
//#define mEECU1_EXT_FRAME 1
#define mEECU1_RPM_byte 4
//#define mEECU1_bit_start 1
//#define mEECU1_bit_len 16
#define mEECU1_bit_res 0.125f      //rpm, float
//#define mEECU1_low_range 0
//#define mEECU1_high_range 8032 // rpm

//Macros para definicoes - mensagem ETC2 - Taxa de comunicação 200ms
#define mETC2_ID 0x18F00503 // id da mensagem M1
#define mETC2_DLC 8
//#define mETC2_EXT_FRAME 1
#define mETC2_GER_byte 4     // GEaR > GER
//#define mETC2_bit_start 1
//#define mETC2_bit_len 8
//#define mETC2_bit_res 1      // bit/marcha
#define mETC2_low_range -125 // low range também é o offset
//#define mETC2_high_range 125

//Macros para definicoes - mensagem CC-VS - Taxa de comunicação 250ms
#define mCCVS_ID 0x18FEF100 // id da mensagem M3
#define mCCVS_DLC 8
//#define mCCVS_EXT_FRAME 1
#define mCCVS_SPD_byte 2      //SPeeD > SPD  
//#define mCCVS_bit_start 1
//#define mCCVS_bit_len 16
#define mCCVS_bit_res 1.0f / 256.0f // (km/h)/bit, float 
//#define mCCVS_low_range 0 
//#define mCCVS_high_range 250
