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

// vetor de caracteres utilizado para armazenar o campo de dados do frame
// como as mensagens possuem ids diferentes, cada uma terá o seu

//FRAME_DATA 
unsigned char mEECU1_data[8] = {0xFF}; // Inicializando com valores padrão
unsigned char mETC2_data[8] = {0xFF};
unsigned char mCCVS_data[8] = {0xFF};

// VARIÁVEIS GLOBAIS - valores padrão - teste
volatile uint16_t RPM_usr = 2500; // rpm
volatile uint16_t speed_usr = 0; // espera-se ~36km/h com base nos dados padrão
volatile uint8_t gear_usr = 2;

// Variáveis para armazenar informações do frame recebido
unsigned char mDLC = 0;
unsigned char mDATA[8];
long unsigned int mID;

///////////////////////////////////////////////////////////

void inverter_hex(char *str){
	int start = 0;			    //
	int end = strlen(str) - 1;

	while (start < end){
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

void int_hex_convert(volatile uint16_t value,
						 unsigned char *msB,
						 unsigned char *lsB){
	
    *msB = (value >> 8) & 0xFF; // Extrair os 8 bits superiores (msB)
    
    *lsB = value & 0xFF; // Extrair os 8 bits inferiores (lsB)
}

void hex_int8_convert(unsigned char hex, volatile uint8_t *result){
	*result = (volatile uint8_t)(hex & 0xFF);
}

void hex_int_convert(unsigned char msB, unsigned char lsB,
					 volatile uint16_t *result){
    
    *result = (msB << 8) | lsB; // Combinação de operação shift com or
}

 
#define M_PI 3.14159265358979323846 // melhor que incluir a biblioteca math inteira

void speed_calc(volatile uint16_t *rpm, volatile uint8_t *gear,
				volatile uint16_t *speed){
	const float Rw = 0.326; // raio do pneu (m)
	const float nd = 3.55; // taxa de trans. do diff
	const float ng[5] = {3.83, 2.36, 1.69, 1.31, 1}; // tx. trans. gbx.
	const float fc = (2 * M_PI * 3.6)/ 60; // fator de conversão para km/h

	if (((*gear) >= 1) && ((*gear) <= 5)){
		*speed = (Rw * (*rpm) * fc) / (nd * ng[(*gear) - 1]);
	}
	else{
		*speed = 0;
	}
}

//////////////////////////////////////////////////////////

//Constroi um objeto MCP_CAN e configura o chip selector (CS) para o pino 10.
MCP_CAN CAN1(ECU2_CAN1_CS); // placa 03, ecu1 com21 e ecu4 com24
							// ecu2 com22
	// neste código, ECM = ecu2

// Envio do frame do RPM a cada 100ms
TASK(SendFrameEECU1){
	GetResource(SEND_RESOURCE); // reservando o barramento
	
	static byte sd_msg = 0; // flag de transmissão
	
	// Calculations
	volatile uint16_t RPM_enc; // ENCoded > enc
	unsigned char RPM_hex[2] = {0xFF};
	
	RPM_enc = (RPM_usr / mEECU1_bit_res); // - offset; Com o offset nulo, economizei cpu
	int_hex_convert(RPM_enc, &RPM_hex[1], &RPM_hex[0]);
	mEECU1_data[mEECU1_RPM_byte - 1] = RPM_hex[0];  // Alocando o dado na posição
	mEECU1_data[mEECU1_RPM_byte] = RPM_hex[1];      // adequada dentro do payload
	
	//Envia a mensagem para o barramento e usa uma flag para confirmar
	sd_msg = CAN1.sendMsgBuf(mEECU1_ID, CAN_EXTID, mEECU1_DLC, mEECU1_data);
	if (sd_msg == CAN_OK){ 
		Serial.print(RPM_usr);
		Serial.println(" RPM enviado!");
		}
	else if (sd_msg == CAN_SENDMSGTIMEOUT){
		Serial.println("Send message timeout!");
		}
	else{
		Serial.println("Erro ao enviar o RPM");
		}
	
	Serial.println("\n");
	
	ReleaseResource(SEND_RESOURCE); // liberando o barramento
	TerminateTask(); // Finaliza a tarefa
	}

// Recepção do frame da marcha a cada 200ms
TASK(ReceiveFrameETC2){
	GetResource(RECEIVE_RESOURCE);
	
	// Calculations
	volatile uint8_t GR_aux; // GeaR > GR
	unsigned char GR_hex;
	
	
	//Se uma interrupção ocorreu interrupção (CAN_INT pino = 0), lê o buffer de recepção
	if(!digitalRead(ECU2_CAN1_INT)){
		//Lê os dados: mID = identificador, mDLC = comprimento, mDATA = dados do frame
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		
		if (mID == (long unsigned int) mETC2_ID) {
            GR_hex = mDATA[mETC2_GER_byte - 1];
			hex_int8_convert(GR_hex, &GR_aux);
			gear_usr = GR_aux + mETC2_low_range; // res = 1
            Serial.print("Marcha recebida: ");
            Serial.println(gear_usr);
        }
		Serial.println("\n");
		}
	ReleaseResource(RECEIVE_RESOURCE);
	TerminateTask();
	}

// Enviar velocidade do veículo a cada 250ms
TASK(SendFrameCCVS) {
    GetResource(SEND_RESOURCE);
	
	static byte sd_msg = 0; // flag
	volatile uint16_t speed_enc;
	unsigned char speed_hex[2];
	
	speed_calc(&RPM_usr, &gear_usr, &speed_usr);
    speed_enc = (speed_usr / mCCVS_bit_res);
    int_hex_convert(speed_enc, &speed_hex[1], &speed_hex[0]);
    mCCVS_data[mCCVS_SPD_byte - 1] = speed_hex[0];
    mCCVS_data[mCCVS_SPD_byte] = speed_hex[1];

    sd_msg = CAN1.sendMsgBuf(mCCVS_ID, CAN_EXTID, mCCVS_DLC, mCCVS_data);
	if (sd_msg == CAN_OK){ 
		Serial.print("Velocidade de ");
		Serial.print(speed_usr);
		Serial.println("km/h enviada!");
		}
	else if (sd_msg == CAN_SENDMSGTIMEOUT){
		Serial.println("Send message timeout!");
	}
	else{
		Serial.println("Erro ao enviar a velocidade!");
	}
	
	
	Serial.println("\n");
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
	Serial.println("MCP2515 (ECM) inicializado com sucesso!");
	Serial.println("Autor: Luan Fábio Marinho Galindo");

	// Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
	
	// Configura o pino de interrupção para recepção
	pinMode(ECU2_CAN1_INT, INPUT);
	}