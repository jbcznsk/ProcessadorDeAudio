#include "acesso.h"
#include "tratamento.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *level);

int main(int argc, char **argv)
{
	FILE *ENTRADA = stdin, *SAIDA = stdout;
	Audio_t *audio;
	float level = 1.0; // Valor Padrão

	tratar_argumentos(argc, argv, ENTRADA, SAIDA, &level);

	audio = malloc(sizeof(Audio_t));
	if (!audio)
	{
		fprintf(stderr, "Erro ao alocar espaço para a audio\n");
		exit(1);
	}

	ler_audio_wav(ENTRADA, audio);

	// Ajuste do volume, respeitando os valores máximos
	ajustar_volume(audio, level);

	envia_audio(SAIDA, audio);

	fechar_streams(ENTRADA, SAIDA);

	free(audio->dados);
	free(audio);

	return 0;
}

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *level)
{
	int opt;

	while ((opt = getopt(argc, argv, "i:o:l:")) != -1)
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
			SAIDA = freopen(optarg, "w", SAIDA);
			if (!SAIDA)
			{
				fprintf(stderr, "Erro ao criar saída de dados\n");
				exit(1);
			}
			break;

		// Volume
		case 'l':
			*level = atof(optarg);
			if (*level < 0.0 || *level > 10.0)
			{
				fprintf(stderr, "Level deve ser um valor entre 0.0 e 10.0\n");
				exit(1);
			}
			break;

		default:
			fprintf(stderr, "Usage: ./wavvol -i [FILE] -o [FILE] -l V\n");
			exit(1);
		}
	}
}