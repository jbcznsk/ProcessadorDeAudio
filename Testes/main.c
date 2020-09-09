#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


#define INPUT "music.wav"

struct Cabecalho_s 
{
	char     RIFF_ChunkID    [4];
	uint32_t RIFF_ChunkSize     ;
	char     RIFF_Format     [4];

	char     fmt_SubChunk1ID [4];
	uint32_t fmt_SubChunk1Size  ;
	uint16_t fmt_AudioFormat    ;
	uint16_t fmt_NrChannels     ;
	uint32_t fmt_SampleRate     ;
	uint32_t fmt_ByteRate       ;
	uint16_t fmt_BlockAling     ;
	uint16_t fmt_BitsPerSample  ;

	char     data_SubChunk2ID[4];
	uint32_t data_SubChunk2Size ;

};

typedef struct Cabecalho_s Cabecalho_t;

int main(int argc, char **argv)
{

	FILE *musica;

	Cabecalho_t cabecalho;

	musica = fopen(INPUT, "r+");

	fread(&cabecalho, sizeof(Cabecalho_t), 1, musica);

	//puts(cabecalho.RIFF_ChunkID);
	for (int i = 0;i < 4;i++)
		putc(cabecalho.RIFF_ChunkID[i],stdout);

	printf("%"PRIu32"\n", cabecalho.RIFF_ChunkSize);

	fclose(musica);

	
	return 0;
}
