//////////////////////////////////////////////////////////////////////////////////
// Universidade de Brasilia (UnB)
// Faculdade do Gama
// Curso: Engenharia Automotiva
// Disciplina : Engenharia de Software Automotivo
//////////////////////////////////////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
#include "board.h"

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


// Variáveis para armazenar informações do frame recebido
unsigned char mDLC_ECM3 = 0;
unsigned char mDATA_ECM3[8];
long unsigned int mID_1;

char msgString[128];

void hex_int_convert(unsigned char byte_ms, unsigned char byte_ls,
                     volatile unsigned int *result) {
    // Combinação de operação shift com or
    *result = (byte_ms << 8) | byte_ls;
}

//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(ECU4_CAN1_CS); // placa 03, ecu1 com21 e ecu4 com24
							// receive com24
void setup(){
	// Inicializa a interface serial : baudrate = 115200
	Serial.begin(9600);
	// Inicializa o controlador can : baudrate = 500K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK){
		delay(200);
		}
	Serial.println("Modulo CAN inicializado!");
	//Configura o controlador MCP2515 no modo normal.
	CAN1.setMode(MCP_NORMAL);
	//Configura o pino de interrupção para recepção
	pinMode(ECU4_CAN1_INT, INPUT);
	Serial.println("MCP2515 exemplo can_receive...");
	Serial.print("ID\t\tType\tDLC\tByte0\tByte1\tByte2");
	Serial.println ("\tByte3\tByte4\tByte5\tByte6\tByte7");
	}
//////////////////////////////////////////////////////////////////////////////////
// Task ReceiveCanFrame
// Utilizada receber as mensagens CAN
// Periodo de execucao da task: 500ms
//////////////////////////////////////////////////////////////////////////////////
TASK(ReceiveFrameECM){
	
	// Calculations
	volatile unsigned int RPM_usr, RPM_aux;
	unsigned char RPM_hex[2];
	
	
	//Se uma interrupção ocorreu interrupção (CAN_INT pino = 0), lê o buffer de recepção
	if(!digitalRead(ECU4_CAN1_INT)){
		//Lê os dados: mID = identificador, mDLC = comprimento, mDATA = dados do frame
		CAN1.readMsgBuf(&mID_1, &mDLC_ECM3, mDATA_ECM3);
		
		// Toma os bytes da mensagem de acordo com o mapa
		RPM_hex[1] = mDATA_ECM3[mECM3_pt3_byte]; // msB
		RPM_hex[0] = mDATA_ECM3[mECM3_pt3_byte - 1]; // lsB
		
		// Converte os bytes em int
		hex_int_convert(RPM_hex[1], RPM_hex[0], &RPM_aux);
		
		// Decodifica o RPM
		RPM_usr = (RPM_aux + mECM3_low_range)*mECM3_res;
		
		//Determina se o frame é do tipo standard (11 bits ) ou estendido (29 bits )
		if((mID_1 & CAN_IS_EXTENDED) == CAN_IS_EXTENDED){
			sprintf(msgString, "0x%.8lX\tExt\t[%1d]\t", (mID_1 & CAN_EXTENDED_ID), mDLC_ECM3);
		}
		else{
			sprintf(msgString, "0x%.3lX\tStd\t[%1d]\t", mID_1, mDLC_ECM3);
		}
		Serial.print(msgString);
		// Verifica se a mensagem é do tipo de requisição remota.
		if ((mID_1 & CAN_IS_REMOTE_REQUEST) == CAN_IS_REMOTE_REQUEST){
			sprintf(msgString,"rrf");
			Serial.print(msgString);
		}
		else{
			for(byte b = 0; b < mDLC_ECM3; b++){
				sprintf(msgString, "0x%.2X\t", mDATA_ECM3[b]);
				Serial.print(msgString);
				}
			Serial.print("\nValor do RPM: ");
			Serial.println(RPM_usr);
		}
		Serial.println("\n");
		}
	}

/*// Função para inverter o vetor com o payload
void vector_inv(char *v, int sz){
	unsigned int bgn = 0;
	unsigned int end_v = sz - 1;
	char aux;
	
	while(bgn < end_v){
		aux = v[bgn];
		v[bgn] = v[end_v]
		v[end_v] = aux;
		bgn++;
		end_v--;
	}
}*/