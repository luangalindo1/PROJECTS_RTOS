/////////////////////////////////////////////////////
// Header para as funções desenvolvidas no projeto
// LUAN FÁBIO MARINHO GALINDO
////////////////////////////////////////////////////

#ifndef __AUX_FUNCTIONS_H__ 
#define __AUX_FUNCTIONS_H__

#include <stdint.h> // Para uint8_t, uint16_t, etc.
#include <string.h> // para a função strlen

#ifdef __cplusplus
extern "C" {
#endif
	
// MÓDULO TCM

/**
 * @brief Converte uint8_t para hexadecimal.
 * 
 * @param value Valor de entrada.
 * @param result Ponteiro para armazenar o resultado.
 */
void int8_to_hex(volatile uint8_t value, unsigned char *result);

// MÓDULO ECM

/**
 * @brief Inverte a posição dos elementos de uma string hexadecimal.
 * 
 * @param str String de entrada a ser invertida.
 */
void inverter_hex(char *str);

/**
 * @brief Converte um valor uint16_t para duas partes hexadecimais (msB, lsB).
 * 
 * @param value Valor de entrada.
 * @param msB Ponteiro para o byte mais significativo.
 * @param lsB Ponteiro para o byte menos significativo.
 */
void int_hex_convert(volatile uint16_t value, unsigned char *msB, unsigned char *lsB);

/**
 * @brief Converte um valor hexadecimal (1 byte) em uint8_t.
 */
void hex_int8_convert(unsigned char hex, volatile uint8_t *result);

/**
 * @brief Converte dois valores hexadecimais em uint16_t.
 */
void hex_int_convert(unsigned char msB, unsigned char lsB, volatile uint16_t *result);

/**
 * @brief Calcula a velocidade com base no RPM e na marcha.
 */
void speed_calc(volatile uint16_t *rpm, volatile uint8_t *gear, volatile uint16_t *speed);

// MÓDULO ICM

/**
 * @brief Atualiza o valor de RPM extraído de um array de dados.
 */
volatile uint16_t updateRPM(unsigned char *data, int byte_pos, float res);

/**
 * @brief Atualiza o valor de velocidade extraído de um array de dados.
 */
volatile uint16_t updateSpeed(unsigned char *data, int byte_pos, float res);

/**
 * @brief Atualiza a marcha extraída de um array de dados.
 */
volatile uint8_t updateGear(unsigned char *data, int byte_pos, int offset);

#ifdef __cplusplus
}
#endif

#endif
