#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <getopt.h>

struct Cabecalho_s
{
	struct RIFF_s {
		char     ChunkID    [4];
		uint32_t ChunkSize     ;
		char     Format     [4];
	} RIFF;

	struct fmt_s{
		char     SubChunk1ID [4];
		uint32_t SubChunk1Size  ;
		uint16_t AudioFormat    ;
		uint16_t NrChannels     ;
		uint32_t SampleRate     ;
		uint32_t ByteRate       ;
		uint16_t BlockAling     ;
		uint16_t BitsPerSample  ;
	} fmt;

	struct data_s{
		char     SubChunk2ID[4];
		uint32_t SubChunk2Size ;
	} data;

};
typedef struct Cabecalho_s Cabecalho_t;

struct Musica_s
{
	Cabecalho_t cab;
	int16_t *dados;
	int tamanho;
};
typedef struct Musica_s Musica_t;

Cabecalho_t ler_cabecalho_wav(FILE *ENTRADA);

void printTag(char *tagName, char *tag, int tam);

void imprimir_cabecalho_wav(Cabecalho_t cab_wav);

//int16_t *ler_musica(FILE *ENTRADA, Cabecalho_t cab_wav);
void ler_musica(FILE *ENTRADA, Musica_t msc);
