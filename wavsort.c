#include "acesso.h"
#include "tratamento.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>

int funcom(const void *a, const void *b)
{
    return (*(int16_t *)a - *(int16_t *)b);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(int argc, char **argv)
{

    FILE *ENTRADA = stdin, *SAIDA = stdout;
    int opt;
    Audio_t *audio;

    /*===================================================*/

    while ((opt = getopt(argc, argv, "i:o:l:")) != -1)
    {
        switch (opt)
        {
        // Entrada
        case 'i':
            ENTRADA = fopen(optarg, "r");
            if (!ENTRADA)
            {
                fprintf(stderr, "Não foi possível encontrar o arquivo\n");
                exit(1);
            }
            break;

        // Saída
        case 'o':
            SAIDA = fopen(optarg, "w+");
            if (!SAIDA)
            {
                fprintf(stderr, "Erro ao criar saída de dados\n");
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Usage: ./wavvol -i [FILE] -o [FILE] -l V\n");
            exit(1);
        }
    }

    /*===================================================*/

    audio = malloc(sizeof(Audio_t));
    if (!audio)
    {
        fprintf(stderr, "Erro ao alocar espaço para a audio\n");
        exit(1);
    }

    ler_audio_wav(ENTRADA, audio);

    // for (int i = 0; i < audio->tamanho; i++)
    //    audio->dados[i] = abs(audio->dados[i]);

    // for (int i = 0; i < audio->tamanho; i++)
    //     audio->dados[i] = soma_com_limite(audio->dados[i]*0.5, 5000*sin(map(i%(44100),0,44100,0,2*3.141592)), VOLMAX);

    // srand(time(NULL));
    // for (int i = 0; i < audio->tamanho; i++)
    // {
    //     int r = rand()%100;
    //     if(r > 90)
    //         audio->dados[i] *= -1;
    // }

    // for (int i = 0; i < audio->tamanho; i++)
    // {
    //     if (audio->dados[i] > -4000 && audio->dados[i] < 4000)
    //    audio->dados[i] *= 0.1;
    // }

    envia_audio(SAIDA, audio);

    fechar_streams(ENTRADA, SAIDA);

    free(audio->dados);
    free(audio);

    return 0;
}
