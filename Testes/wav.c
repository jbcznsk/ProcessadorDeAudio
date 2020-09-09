#include "wav.h"

Cabecalho_t ler_cabecalho_wav(FILE *ENTRADA){
    Cabecalho_t cab_waw;
    fread(&cab_waw, sizeof(Cabecalho_t),1,ENTRADA);
    return cab_waw;
}

void printTag(char *tagName, char *tag, int tam){
	printf("%s       : \"", tagName);
	for (int i = 0; i < tam; i++)
		putc(tag[i], stdout);
	fprintf(stdout, "\"\n");
}

void imprimir_cabecalho_wav(Cabecalho_t cab_wav){

	printTag("riff tag", cab_wav.RIFF.ChunkID,4);
	printf("riff size      : %"PRIu32"\n", cab_wav.RIFF.ChunkSize);
	printTag("wave tag", cab_wav.RIFF.Format,4);

    printTag("form tag", cab_wav.fmt.SubChunk1ID,4);
    printf("fmt_size       : %"PRIu32"\n", cab_wav.fmt.SubChunk1Size);
    printf("audio_format   : %"PRIu16"\n", cab_wav.fmt.AudioFormat);
    printf("num_channels   : %"PRIu16"\n", cab_wav.fmt.NrChannels);
    printf("sample_rate    : %"PRIu32"\n", cab_wav.fmt.SampleRate);
    printf("byte_rate      : %"PRIu32"\n", cab_wav.fmt.ByteRate);
    printf("block_align    : %"PRIu16"\n", cab_wav.fmt.BlockAling);
    printf("bits_per_sample: %"PRIu16"\n", cab_wav.fmt.BitsPerSample);

    printTag("data tag", cab_wav.data.SubChunk2ID,4);
    printf("data size      : %"PRIu32"\n", cab_wav.data.SubChunk2Size);

}

int16_t *ler_musica(FILE *ENTRADA, Cabecalho_t cab_wav){

  int16_t *msc;

  // Aloca espaço o suficiente para a musica inteira de acordo com o cabeçalho
  msc = malloc(cab_wav.data.SubChunk2Size);
  if (!msc)
  {
    fprintf(stderr, "Erro ao alocar espaço para a música\n");
    exit(1);
  }

  // Lê a música da entrada atual
  // cab_wav.data.SubChunk2Size/(cab_wav.fmt.BitsPerSample/8) para armazenar o número correto de amostras
  fread(msc,sizeof(int16_t),cab_wav.data.SubChunk2Size/(cab_wav.fmt.BitsPerSample/8),ENTRADA);

  return msc;
}
