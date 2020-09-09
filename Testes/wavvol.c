#include "wav.h"

int
main (int argc, char **argv)
{

  FILE *ENTRADA = stdin, *SAIDA = stdout;
  int opt, tamMusica;
  float level = 1.0;
  Cabecalho_t cab_wav;
  int16_t *musica;

  while ((opt = getopt (argc, argv, "i:o:l:")) != -1)
	{
	  switch (opt)
		{
		case 'i':
		  ENTRADA = fopen (optarg, "r");
		  if (!ENTRADA)
			{
			  fprintf (stderr, "Não foi possível encontrar o arquivo\n");
			  exit (1);
          }\
		  break;
		case 'o':
		  SAIDA = fopen (optarg, "w+");
		  if (!SAIDA)
			{
			  fprintf (stderr, "Erro ao criar saída de dados\n");
			  exit (1);
			}
		  break;
		case 'l':
		  level = atof (optarg);
		  if (level < 0.0 || level > 10.0)
			{
			  fprintf (stderr, "Level deve ser um valor entre 0.0 e 10.0\n");
			  exit (1);
			}
		  break;
		default:
		  fprintf (stderr, "Usage: ./wavvol -i [FILE] -o [FILE] -l V\n");
		  exit (1);
		}
	}

  cab_wav = ler_cabecalho_wav (ENTRADA);
  musica = ler_musica (ENTRADA, cab_wav);

  tamMusica = (cab_wav.data.SubChunk2Size) / (cab_wav.fmt.BitsPerSample / 8);

  for (int i = 0; i < tamMusica; i++)
	{

	  if ((musica[i] * level) >= 32000)
		{
		  musica[i] = 32000;
		}
	  else if ((musica[i] * level) <= -32000)
		{
		  musica[i] = -32000;
		}
	  else
		{
		  musica[i] *= (level);
		}

	}

  fwrite (&cab_wav, sizeof (Cabecalho_t), 1, SAIDA);
  fwrite (musica, sizeof (int16_t),
		  (cab_wav.data.SubChunk2Size) / (cab_wav.fmt.BitsPerSample / 8),
		  SAIDA);

  fclose (ENTRADA);
  fclose (SAIDA);
  free (musica);

  return 0;
}
