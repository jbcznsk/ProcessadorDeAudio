#include "acesso.h"
#include "tratamento.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *k);

int main(int argc, char **argv)
{

	FILE *ENTRADA = stdin, *SAIDA = stdout;

	Audio_t *audio;
	float k = 1.0;

	tratar_argumentos(argc, argv, ENTRADA, SAIDA, &k);

	audio = malloc(sizeof(Audio_t));
	if (!audio)
	{
		fprintf(stderr, "Erro ao alocar espaço para a audio\n");
		exit(1);
	}

	ler_audio_wav(ENTRADA, audio);

	// Confere se o efeito é aplicavel
	if (!confere_dois_canais(audio))
	{
		fprintf(stderr, "O audio fornecido precisa ter 2 canais\n");
		fechar_streams(ENTRADA, SAIDA);
		free(audio->dados);
		free(audio);
		exit(1);
	}

	// Aplica o eco
	estereo_amplificado(audio, k);

	envia_audio(SAIDA, audio);

	fechar_streams(ENTRADA, SAIDA);

	free(audio->dados);
	free(audio);

	return 0;
}

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA, FILE *SAIDA, float *k)
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
			SAIDA = freopen(optarg, "w+", SAIDA);
			if (!SAIDA)
			{
				fprintf(stderr, "Erro ao criar saída de dados\n");
				exit(1);
			}
			break;

		case 'l':
			*k = atof(optarg);
			if (*k < 0.0 || *k > 10.0)
			{
				fprintf(stderr, "Level deve ser um valor entre 0.0 e 1\n");
				exit(1);
			}
			break;

		default:
			fprintf(stderr, "Usage: ./wavwide -i [FILE] -o [FILE] -l k\n");
			exit(1);
		}
	}
}