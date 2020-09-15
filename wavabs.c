#include "acesso.h"
#include "tratamento.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA);
void abs_audio(Audio_t *audio);

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

	// Reverte a audio
	abs_audio(audio);

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

void abs_audio(Audio_t *audio)
{
	for (int i = 0; i < audio->tamanho; i++)
       audio->dados[i] = abs(audio->dados[i]);
}
