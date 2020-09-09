#include "wav.h"
#include <getopt.h>


int main(int argc, char **argv)
{

	FILE *saida1, *saida2;

	Cabecalho_t cabecalho;

	fread(&cabecalho, sizeof(Cabecalho_t), 1, stdin);

	saida1 = fopen("saida1", "w+");
	saida2 = fopen("saida2", "w+");

	fwrite(&cabecalho, sizeof(Cabecalho_t), 1, saida1);

	imprimir_cabecalho_wav(cabecalho);

	int16_t *musica, *rev;

	musica = malloc(cabecalho.data.SubChunk2Size);
	rev = malloc(cabecalho.data.SubChunk2Size);

	if (!musica){
		printf("deu ruim\n");
		exit(1);
	}

	//int16_t musica [cabecalho.data.SubChunk2Size/2];

	fread(musica,sizeof(int16_t),cabecalho.data.SubChunk2Size/2,stdin);


	/* VOLUME  */
	for (int i = 0; i < cabecalho.data.SubChunk2Size/2; i++){
		musica[i] *= 2;
	}

	/* REVERSÃO */
	// printf("Reversão\n");
	// for (int i = 0; i < cabecalho.data.SubChunk2Size/2; i++){
	// 	rev[i] = musica[(cabecalho.data.SubChunk2Size/2)-i-1];
	// }


	fwrite(&cabecalho, sizeof(Cabecalho_t), 1, saida2);
	fwrite(musica, sizeof(int16_t), cabecalho.data.SubChunk2Size/2, saida2);
/*
	for (uint32_t i = 0;i < cabecalho.data.SubChunk2Size/16; i++){
		printf("%"PRIu16, musica[i]);
	}
*/
	//free(musica);
	return 0;
}
