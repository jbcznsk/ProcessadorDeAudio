#include "acesso.h"
#include "tratamento.h"

int confere_nr_argumentos(int argc, int sel_saida);
void tratar_argumentos(int argc, char **argv, FILE *SAIDA, int *sel_saida);

int main(int argc, char **argv)
{

    FILE *ENTRADA = stdin, *SAIDA = stdout;
    Audio_t *audio, *apendice;
    int sel_saida = 0;

    tratar_argumentos(argc, argv, SAIDA, &sel_saida);

	// Checa o numero de argumentos, devem existir no mínimo 2 argumentos validos
    if (!confere_nr_argumentos(argc, sel_saida))
    {
        fprintf(stderr, "Faltam argumentos\n");
        exit(1);
    }

    /*===================================================*/

	// Aloca os espaços para os audios
    audio = malloc(sizeof(Audio_t));
    if (!audio)
    {
        fprintf(stderr, "Erro ao alocar espaço para a audio\n");
        exit(1);
    }

    apendice = malloc(sizeof(Audio_t));
    if (!apendice)
    {
        free(audio);
        fprintf(stderr, "Erro ao alocar espaço para a audio\n");
        exit(1);
    }

    /*===================================================*/

    int i = 1;
    // Procura o primeiro argumento que não seja -o [file]
    while (i < argc)
    {
        if (!strcmp("-o", argv[i]))
        {
            i++;
        }
        else
        {
            ENTRADA = freopen(argv[i], "r", ENTRADA);
            if (!ENTRADA)
            {
                fprintf(stderr, "Nao foi possivel abrir o arquivo");
                free(audio);
                free(apendice);
                exit(1);
            }
            break;
        }
        i++;
    }

    ler_audio_wav(ENTRADA, audio);

    i++;
    while (i < argc)
    {
        if (!strcmp("-o", argv[i]))
        {
            i++;
        }
        else
        {
            ENTRADA = freopen(argv[i], "r", ENTRADA);
            if (!ENTRADA)
            {
                fprintf(stderr, "Nao foi possivel abrir o arquivo");
                free(audio->dados);
                free(audio);
                free(apendice);
                exit(1);
            }

            ler_audio_wav(ENTRADA, apendice);
            mixar_audios(audio, apendice);
            free(apendice->dados);
        }

        i++;
    }

    envia_audio(SAIDA, audio);

    fechar_streams(ENTRADA, SAIDA);

    free(audio->dados);
    free(audio);
    free(apendice);

    return 0;
}

int confere_nr_argumentos(int argc, int sel_saida)
{
    if ((sel_saida && (argc < 5)) || (!sel_saida && (argc < 3)))
        return 0;
    return 1;
}

void tratar_argumentos(int argc, char **argv, FILE *SAIDA, int *sel_saida)
{
    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1)
    {
        switch (opt)
        {
        // Saída
        case 'o':
            *sel_saida = 1;
            SAIDA = freopen(optarg, "w+", SAIDA);
            if (!SAIDA)
            {
                fprintf(stderr, "Erro ao criar saída de dados\n");
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Usage: ./wavmix file1 file2 ... filen -o [FILE] \n");
            exit(1);
        }
    }
}
