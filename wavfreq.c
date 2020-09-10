#include "acesso.h"
#include "tratamento.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA);
void aumenta_frequencia(Audio_t *audio);

int main(int argc, char **argv)
{
	FILE *ENTRADA = stdin, *SAIDA = stdout;
	Audio_t *audio;

	tratar_argumentos(argc, argv, ENTRADA, SAIDA);

	audio = malloc(sizeof(Audio_t));
	if (!audio)
	{
		fprintf(stderr, "Erro ao alocar espaço para a audio\n");
		exit(1);
	}

	ler_audio_wav(ENTRADA, audio);

	aumenta_frequencia(audio);

	envia_audio(SAIDA, audio);

	fechar_streams(ENTRADA, SAIDA);

	free(audio->dados);
	free(audio);

	return 0;
}

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA)
{
	int opt;
	while ((opt = getopt(argc, argv, "i:o:")) != -1)
	{
		switch (opt)
		{

		// Entrada
		case 'i':
			ENTRADA = freopen(optarg, "r", ENTRADA);
			if (!ENTRADA)
			{
				fprintf(stderr, "Não foi possível encontrar o arquivo\n");
				exit(1);
			}
			break;

		// Saída
		case 'o':
			SAIDA = freopen(optarg, "w+", SAIDA);
			if (!SAIDA)
			{
				fprintf(stderr, "Erro ao criar saída de dados\n");
				exit(1);
			}
			break;

		default:
			fprintf(stderr, "Usage: ./wavrev -i [FILE] -o [FILE]\n");
			exit(1);
		}
	}
}

void aumenta_frequencia(Audio_t *audio)
{   
    int j = 0;
    for (int i = 0; i < audio->tamanho; i+=2)
    {  
        audio->dados[i]   = (audio->dados[j]   + audio->dados[j+2])/2;
        audio->dados[i+1] = (audio->dados[j+1] + audio->dados[j+3])/2;
        j = (j+4)% audio->tamanho;
    }

    audio->cab.RIFF.ChunkSize -= audio->cab.data.SubChunk2Size/2;
    audio->cab.data.SubChunk2Size = audio->cab.data.SubChunk2Size / 2;
    audio->tamanho /= 2;
}