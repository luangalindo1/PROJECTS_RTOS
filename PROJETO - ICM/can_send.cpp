//////////////////////////////////////////////////////////////////////////////////
// Universidade de Brasilia (UnB)
// Faculdade do Gama
// Curso: Engenharia Automotiva
// Disciplina : Engenharia de Software Automotivo
/////////////////////////////////////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
#include "board.h"
#include "m_DEFINES.h"
#include "aux_functions.h"

//FRAME_DATA 
unsigned char mEECU1_data[8] = {0xFF}; // Inicializando com valores padrão
unsigned char mETC2_data[8] = {0xFF};
unsigned char mCCVS_data[8] = {0xFF};

// Dados a serem mostrados
volatile uint16_t rpm_usr = 0;   
volatile uint16_t speed_usr = 0; 
volatile uint8_t gear_usr = 0;

// Variáveis para armazenar informações do frame recebido
unsigned char mDLC = 0;
unsigned char mDATA[8];
long unsigned int mID;

/////////////////////////////////////////////////////

void hex_int_convert(unsigned char msB, unsigned char lsB,
					 volatile uint16_t *result){
    // Combinação de operação shift com or
    *result = (msB << 8) | lsB;
}

volatile uint16_t updateRPM(unsigned char *data, int byte_pos, float res) {
    unsigned char rpm_hex[2];
    uint16_t rpm_enc;

	rpm_hex[0] = data[byte_pos - 1];  // Byte do RPM
    rpm_hex[1] = data[byte_pos];

    // Converte os bytes em valor de RPM
    hex_int_convert(rpm_hex[1], rpm_hex[0], &rpm_enc);
    return rpm_enc * res;  // Converte para RPM real
}

volatile uint16_t updateSpeed(unsigned char *data, int byte_pos, float res) {
    unsigned char speed_hex[2];
    uint16_t speed_enc;

	speed_hex[0] = data[byte_pos - 1];  // Byte da velocidade
    speed_hex[1] = data[byte_pos];

    // Converte os bytes em valor de velocidade
    hex_int_convert(speed_hex[1], speed_hex[0], &speed_enc);
    return speed_enc * res;  // Converte para km/h
}

// Função para extrair e atualizar a marcha
volatile uint8_t updateGear(unsigned char *data, int byte_pos, int offset) {
    return data[byte_pos - 1] + offset;  // Converte para valor de marcha real
}

////////////////////////////////////////////////////


// MCP_CAN: Objeto CAN - placa 03, ecu4 com24
MCP_CAN CAN1(ECU4_CAN1_CS); 
							

TASK(ReceiveFrameETC2){
	GetResource(RECEIVE_RESOURCE);
	
	//Se uma interrupção ocorreu interrupção (CAN_INT pino = 0), lê o buffer de recepção
	if(!digitalRead(ECU4_CAN1_INT)){
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		if (mID == (long unsigned int) mETC2_ID) {
            gear_usr = updateGear(mDATA, mETC2_GER_byte, mETC2_low_range);
        }
		Serial.println("\n");
		}
	ReleaseResource(RECEIVE_RESOURCE);
	TerminateTask();
}
	
TASK(ReceiveFrameEECU1){
	GetResource(RECEIVE_RESOURCE);
	
	if(!digitalRead(ECU4_CAN1_INT)){
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		if (mID == (long unsigned int) mEECU1_ID) {
            rpm_usr = updateRPM(mDATA, mEECU1_RPM_byte, mEECU1_bit_res);
        }
		Serial.println("\n");
		}
	
	ReleaseResource(RECEIVE_RESOURCE);
	TerminateTask();
}

TASK(ReceiveFrameCCVS){
	GetResource(RECEIVE_RESOURCE);
	
	//Se uma interrupção ocorreu interrupção (CAN_INT pino = 0), lê o buffer de recepção
	if(!digitalRead(ECU4_CAN1_INT)){
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		if (mID == (long unsigned int) mCCVS_ID) {
            speed_usr = updateSpeed(mDATA, mCCVS_SPD_byte, mCCVS_bit_res);
        }
		Serial.println("\n");
		}
	ReleaseResource(RECEIVE_RESOURCE);
	TerminateTask();
}

TASK(ShowData){
	GetResource(SEND_RESOURCE);
	
	Serial.print(rpm_usr);
	Serial.println(" RPM");
	Serial.print(gear_usr);
	Serial.println("a marcha");
	Serial.print(speed_usr);
	Serial.println(" km/h");
	
	ReleaseResource(SEND_RESOURCE);
	TerminateTask();
}
	
void setup(){
	// Inicializa a interface serial : baudrate = 115200
	Serial.begin(9600);

	// Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK){
		delay(200);
	}
	Serial.println("MCP2515 (ICM) inicializado com sucesso!");
	Serial.println("Autor: Luan Fabio Marinho Galindo");
	
	//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
	
	// Configura o pino de interrupção para recepção
    pinMode(ECU4_CAN1_INT, INPUT);
}