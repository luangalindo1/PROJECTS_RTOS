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
//#include "aux_functions.h"

// Vetor de dados para o payload da mensagem
unsigned char mETC2_data[8] = {0xFF}; // Inicia com valores padrão

// Variáveis globais
volatile uint8_t gear_usr = 3; // Marcha atual padrão (exemplo)

///////////////////////////////////////////////////////////

void int8_to_hex(volatile uint8_t value, unsigned char *result) {
    *result = (unsigned char)(value & 0xFF); // Garante apenas os 8 bits
}

//////////////////////////////////////////////////////////	

// MCP_CAN: Objeto CAN - placa 03, ecu1 com21
MCP_CAN CAN1(ECU1_CAN1_CS);

// Função para encapsular e transmitir a marcha atual
TASK(SendFrameETC2) {
    GetResource(SEND_RESOURCE);
	
	static byte sd_msg = 0; // flag de transmissão
    unsigned char gear_hex;
	volatile uint8_t gear_enc;

    gear_enc = gear_usr - mETC2_low_range; // res = 1
    int8_to_hex(gear_enc, &gear_hex);
    mETC2_data[mETC2_GER_byte - 1] = gear_hex;

    // Transmissão da mensagem
    sd_msg = CAN1.sendMsgBuf(mETC2_ID, CAN_EXTID, mETC2_DLC, mETC2_data);
    if (sd_msg == CAN_OK) {
        Serial.print(gear_usr);
		Serial.println(" marcha enviada com sucesso!");
    } else if (sd_msg == CAN_SENDMSGTIMEOUT) {
        Serial.println("Erro: tempo de envio expirado!");
    } else {
        Serial.println("Erro ao enviar a mensagem!");
    }
    Serial.println("\n");
	ReleaseResource(SEND_RESOURCE);
	TerminateTask();
}

void setup() {
    // Inicialização da interface serial
    Serial.begin(9600);

    // Inicializa o controlador CAN
    while (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
        delay(200);
    }
    Serial.println("MCP2515 (TCM) inicializado com sucesso!");
	Serial.println("Autor: Luan Fábio Marinho Galindo");

    // Configura o modo normal de operação
    CAN1.setMode(MCP_NORMAL);

    // Configura o pino de interrupção para recepção
    pinMode(ECU1_CAN1_INT, INPUT);
}
