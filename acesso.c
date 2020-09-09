#include "acesso.h"

void ler_cabecalho_wav(FILE *ENTRADA, Audio_t *audio)
{
    fread(&audio->cab, sizeof(Cabecalho_t), 1, ENTRADA);
}

void ler_dados_wav(FILE *ENTRADA, Audio_t *audio)
{

    audio->dados = malloc(audio->cab.data.SubChunk2Size);
    if (!audio->dados)
    {
        fprintf(stderr, "Erro ao alocar espaço para a música\n");
        exit(1);
    }

    audio->tamanho = (audio->cab.data.SubChunk2Size) / (audio->cab.fmt.BitsPerSample / 8);

    fread(audio->dados, sizeof(int16_t), audio->tamanho, ENTRADA);
}

void ler_audio_wav(FILE *ENTRADA, Audio_t *audio)
{
    ler_cabecalho_wav(ENTRADA, audio);
    ler_dados_wav(ENTRADA, audio);
}

void envia_audio(FILE *SAIDA, Audio_t *audio)
{
	fwrite(&audio->cab, sizeof(Cabecalho_t), 1, SAIDA);
	fwrite(audio->dados, sizeof(int16_t), audio->tamanho, SAIDA);
}