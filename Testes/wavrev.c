#include "wav.h"

int main(int argc, char **argv){

  FILE *ENTRADA = stdin,*SAIDA = stdout;
  int opt;
  Cabecalho_t cab_wav;
  int16_t *musica;

/*================Tratamento de Argumentos ==================================*/

  while((opt = getopt(argc, argv, "i:"))!= -1 )
  {
    switch (opt) {
      case 'i':
        ENTRADA = fopen(optarg, "r");
        if(!ENTRADA)
        {
          fprintf(stderr, "Não foi possível encontrar o arquivo\n");
          exit(1);
        }
      break;
      case 'o':
        SAIDA = fopen(optarg,"w+");
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

/*=========================================================*/

  cab_wav = ler_cabecalho_wav(ENTRADA);
  musica = ler_musica(ENTRADA, cab_wav);

  




    return 0;
}
