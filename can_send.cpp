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
//Macro para definicoes
#define mEEC1_ID 0x0CF00400
#define mEEC1_DLC 8
#define mEEC1_EXT_FRAME 1

//vetor de caracteres utilizado para armazenar o campo de dados do frame
//FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(ECU1_CAN1_CS); // placa 03, ecu1 com21 e ecu4 com24
							// send com21
void setup(){
// Inicializa a interface serial : baudrate = 115200
	Serial.begin(9600);

// Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK){
		delay(200);
	}
	Serial.println ("MCP2515 can_send inicializado com sucesso!");

//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
	}

//////////////////////////////////////////////////////////////////////////////////
// Task SendCanFrame
// Utilizada enviar as mensagens CAN
// Taxa de transmissao e repeticao : 1024ms
//////////////////////////////////////////////////////////////////////////////////
// importante observar que o nome da task (neste caso SendCanFrame) deve ser o mesmo do que fora
// especificado no arquivo can_send.oil.
TASK(SendCanFrame){
	static byte ret = 0;

//Envia mensagem para o barramento
	ret=CAN1.sendMsgBuf(mEEC1_ID,CAN_EXTID,mEEC1_DLC,mEEC1_data);
	if (ret==CAN_OK){
		Serial.println ("can_send: mensagem transmitida com sucesso");
		}
	else if (ret == CAN_SENDMSGTIMEOUT){
		Serial.println ("can_send: Message timeout!");
		}
	else{
		Serial.println ("can_send: Error to send!");
		}
	}
