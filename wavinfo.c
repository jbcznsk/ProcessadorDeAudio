#include "acesso.h"
#include "tratamento.h"

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA);

int main(int argc, char **argv)
{
	FILE *ENTRADA = stdin, *SAIDA = stdout;
	Audio_t audio;

	tratar_argumentos(argc, argv, ENTRADA);

	ler_cabecalho_wav(ENTRADA, &audio);

	imprimir_cabecalho_wav(&audio);

	fechar_streams(ENTRADA, SAIDA);

	return 0;
}

void tratar_argumentos(int argc, char **argv, FILE *ENTRADA)
{
	int opt;
	while ((opt = getopt(argc, argv, "i:")) != -1)
	{
		switch (opt)
		{
		case 'i':
			ENTRADA = freopen(optarg, "r", ENTRADA);
			if (!ENTRADA)
			{
				fprintf(stderr, "Não foi possível encontrar o arquivo\n");
				exit(1);
			}
			break;

		default:
			fprintf(stderr, "Usage: ./wavinfo -i [FILE]\n");
			exit(1);
		}
	}
}