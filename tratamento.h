#ifndef __TRATAMENTO__
#define __TRATAMENTO__

#include "estruturas.h"
#include <string.h>

// Realiza uma operação com os valores limites de +- limite
int16_t soma_com_limite(int16_t a, int16_t b, int16_t lim);
int16_t mult_com_limite(int16_t a, float b, int16_t lim);

// Função auxiliar para imprimir_cabecalho_wav
void printTag(char *tagName, char *tag, int tam);
void imprimir_cabecalho_wav(Audio_t *audio);


// Mudar Volume
void ajustar_volume(Audio_t *audio, float level);

// Normalizar
void normalizar_volume(Audio_t *audio);

// Reverter
void troca(int16_t *a, int16_t *b);
void reverter_audio(Audio_t *audio);

// Eco
void ecoar(Audio_t *audio, float level, int delay);

// Estereo Ampliado
int confere_dois_canais(Audio_t *audio);
void estereo_amplificado(Audio_t *audio, int k);

// Função auxiliar utilizada em "concatenar" e "mixar"
int audios_compativeis(Audio_t *audioA, Audio_t *audioB);

// Retorna a concatenação das duas músicas na música A
void concatatenar_audios(Audio_t *audioA, Audio_t *audioB);

// Retorna a mixagem nas duas músicas na audioA
void mixar_audios(Audio_t *audioA, Audio_t *audioB);

// Retorna o audio apenas com valores positivos
void abs_audio(Audio_t *audio);

// Fecha os arquivos de streams
void fechar_streams(FILE *ENTRADA, FILE *SAIDA);


void tratar_audios(int argc, char **argv, FILE *ENTRADA, Audio_t *audio, Audio_t *apendice);

#endif