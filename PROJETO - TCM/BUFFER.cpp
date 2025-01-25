#include <string.h>

// FUNÇÕES DO BUFFER

// Definição de constantes
#define MAX_BUFFERS 10
#define BUFFER_DS 8 // DS - Data Size

// Variáveis globais
char m_buffers[MAX_BUFFERS][BUFFER_DS]; 
long unsigned int id_buffer[MAX_BUFFERS]; 
unsigned char dlc_buffer[MAX_BUFFERS]; 
int current_size = 0; 

// Função para remover o dado mais antigo do buffer
void remove_oldest() {
    if (current_size == 0) {
        //Serial.println("Erro: Buffer vazio.");
        return;
	}
    // Deslocar os dados para a esquerda no buffer
    for (int i = 1; i < current_size; i++) {
        strncpy(m_buffers[i - 1], m_buffers[i], BUFFER_DS);
        id_buffer[i - 1] = id_buffer[i];
        dlc_buffer[i - 1] = dlc_buffer[i];
    }
    current_size--; // Reduzir o tamanho atual do buffer
}

// Função para adicionar dados ao buffer
void add_to_buffer(const char *buffer, long unsigned int mID, unsigned char mDLC) {
    if (current_size >= MAX_BUFFERS) {
        //Serial.println("Erro: Buffer cheio.");
        remove_oldest();
		return;
    }
    strncpy(m_buffers[current_size], buffer, BUFFER_DS);
    m_buffers[current_size][BUFFER_DS - 1] = '\0'; // Garantir terminação nula
    id_buffer[current_size] = mID;
    dlc_buffer[current_size] = mDLC;
    current_size++;
}

// Função para buscar o índice do dado mais antigo com base no identificador
int find_oldest_by_id(long unsigned int id) {
    for (int i = 0; i < current_size; i++) {
        if (id_buffer[i] == id) {
            return i; // Retorna o índice do dado mais antigo com o ID correspondente
        }
    }
    return -1; // Retorna -1 se o ID não for encontrado
}

// Função para acessar e remover o dado mais antigo com base no identificador
void access_and_remove_by_id(long unsigned int id, unsigned char *m_data) {
    // Encontra o índice do dado mais antigo com o ID
    int index = find_oldest_by_id(id);
    if (index == -1) {
        return;
    }

    // Acessa o dado encontrado
    memcpy(m_data, m_buffers[index], BUFFER_DS); 
	//id_buffer[index]; 
	//dlc_buffer[index];

    // Remove o dado encontrado e reorganiza o buffer
    for (int i = index; i < current_size - 1; i++) {
        strncpy(m_buffers[i], m_buffers[i + 1], BUFFER_DS);
        id_buffer[i] = id_buffer[i + 1];
        dlc_buffer[i] = dlc_buffer[i + 1];
    }
    current_size--; // Reduz o tamanho atual do buffer
}