///////////////////////////////////////////////////////////
// Implementações das funções
// LUAN FÁBIO MARINHO GALINDO
//////////////////////////////////////////////////////////

#include "aux_functions.h"

// MÓDULO TCM

void int8_to_hex(volatile uint8_t value, unsigned char *result) {
    *result = (unsigned char)(value & 0xFF); // Garante apenas os 8 bits
}

// MÓDULO ECM

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
	// Extrair os 8 bits superiores (msB)
    *msB = (value >> 8) & 0xFF;
    // Extrair os 8 bits inferiores (lsB)
    *lsB = value & 0xFF;
}

void hex_int8_convert(unsigned char hex, volatile uint8_t *result){
	*result = (volatile uint8_t)(hex & 0xFF);
}

void hex_int_convert(unsigned char msB, unsigned char lsB,
					 volatile uint16_t *result){
    // Combinação de operação shift com or
    *result = (msB << 8) | lsB;
}

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

// MÓDULO ICM

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