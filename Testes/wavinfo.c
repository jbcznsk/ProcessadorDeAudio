#include "wav.h"

int main(int argc, char **argv){

  FILE *ENTRADA = stdin;
  int opt;

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
      default:
        fprintf(stderr, "Usage: ./wavinfo -i [FILE]\n");
        exit(1);
    }
  }

    Cabecalho_t c = ler_cabecalho_wav(ENTRADA);
    imprimir_cabecalho_wav(c);

    return 0;
}
