//////////////////////////////////////////////////////////////////////////////////
// Universidade de Brasilia (UnB)
// Faculdade do Gama
// Curso: Engenharia Automotiva
// Disciplina : Engenharia de Software Automotivo
/////////////////////////////////////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
#include "board.h"


/*as linhas 16 a 18 especificam diretivas para estabelecer o ID, o
DLC, e o formato do frame de dados*/
//Macros para definicoes - RPM - SCANIA CAN MESSAGES - pg77 point 3
#define mECM3_ID 0x18EBFF00 // m -> message, ECM1 -> Nome ou id da mensagem geral
#define mECM3_DLC 8			// (o nome pode mudar)
#define mECM3_EXT_FRAME 1
#define mECM3_pt3_byte 7
//#define mECM3_bit_start 1
#define mECM3_bit_len 16
#define mECM3_res 0.125 //rpm
#define mECM3_low_range 0
//#define mECM3_high_range 8031.875 // rpm

/*//Macros para definicoes - speed - SCANIA CAN MESSAGES - pg39
#define mCCVS_ID 0x18FEF100 
#define mCCVS_DLC 8			
#define mCCVS_EXT_FRAME 1
#define mCCVS_speed_byte 2
//#define mCCVS_bit_start 1 // bit start in the byte
#define mCCVS_bit_len 16
#define mCCVS_bit_res (1.0f / 256.0f) // (km/h)/bit
#define mCCVS_low_range 0
#define mCCVS_high_range 251
*/

//vetor de caracteres utilizado para armazenar o campo de dados do frame
//FRAME_DATA ENGINE CONFIG MESSAGES
unsigned char mECM3_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//FRAME_DATA CRUISE CTRL/VEHICLE SPEED
//unsigned char mCCVS_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Serão usados dois payloads diferentes por termos
// identificadores diferentes

//void inverter_hex(char *str);
void convert_to_hex_bytes(volatile unsigned int value, 
						  unsigned char *high_byte, 
						  unsigned char *low_byte);
	
//Constroi um objeto MCP_CAN e configura o chip selector (CS) para o pino 10.
MCP_CAN CAN1(ECU1_CAN1_CS); // placa 03, ecu1 com21 e ecu4 com24
							// send com21
void setup(){
// Inicializa a interface serial : baudrate = 115200
	Serial.begin(9600);

// Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK){
		delay(200);
	}
	Serial.println("MCP2515 CanSend inicializado com sucesso!");

//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
	}

//////////////////////////////////////////////////////////////////////////////////
// Tasks SendCanFrame
// Utilizadas para enviar as mensagens CAN
// Taxa de transmissao e repeticao : 1024ms ou 512ms
//////////////////////////////////////////////////////////////////////////////////

TASK(SendFrameECM){
	static byte sd_msg = 0; // flag
	
	// Calculations
	volatile unsigned int RPM_usr, RPM_aux;
	unsigned char RPM_hex[2] = {0xFF};
		
	RPM_usr = 2500; //Valor teste
	RPM_aux = (RPM_usr / mECM3_res) - mECM3_low_range; // O low_range é o offset
	convert_to_hex_bytes(RPM_aux, &RPM_hex[1], &RPM_hex[0]);
	mECM3_data[mECM3_pt3_byte - 1] = RPM_hex[0];  // Alocando o dado na posição
	mECM3_data[mECM3_pt3_byte] = RPM_hex[1];      // adequada dentro do payload
	
	//Envia mensagem para o barramento e usa uma flag para confirmar
	sd_msg = CAN1.sendMsgBuf(mECM3_ID, CAN_EXTID, mECM3_DLC, mECM3_data);
	if (sd_msg == CAN_OK){ 
		Serial.println("Message transmited!");
		}
	else if (sd_msg == CAN_SENDMSGTIMEOUT){
		Serial.println("Send message timeout!");
		}
	else{
		Serial.println("Error to send!");
		}
	}

	/*void inverter_hex(char *str){ // Função para inverter os valores
		int start = 0;			  // Hexadecimais
		int end = strlen(str) - 1;

		while (start < end){
			char temp = str[start];
			str[start] = str[end];
			str[end] = temp;
			start++;
			end--;
		}
	}*/
	void convert_to_hex_bytes(volatile unsigned int value, 
							  unsigned char *high_byte, 
							  unsigned char *low_byte){
    // Extrair os 8 bits superiores (byte alto)
    *high_byte = (value >> 8) & 0xFF;

    // Extrair os 8 bits inferiores (byte baixo)
    *low_byte = value & 0xFF;
}
	