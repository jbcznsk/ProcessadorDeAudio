#include <stdio.h>
#include "tratamento.h"

int16_t soma_com_limite(int16_t a, int16_t b, int16_t lim)
{
	if ((a + b) > lim)
		return (lim);
	else if ((a + b) < -lim)
		return (-lim);
	else
		return a + b;
}

int16_t mult_com_limite(int16_t a, float b, int16_t lim)
{
	if ((a * b) > lim)
		return (lim);
	else if ((a * b) < -lim)
		return (-lim);
	else
		return a * b;
}

void printTag(char *tagName, char *tag, int tam)
{
	printf("%s       (4 bytes): \"", tagName);
	for (int i = 0; i < tam; i++)
		putc(tag[i], stdout);
	fprintf(stdout, "\"\n");
}

void imprimir_cabecalho_wav(Audio_t *audio)
{

	printTag("riff tag", audio->cab.RIFF.ChunkID, 4);
	printf("riff size      (4 bytes): %" PRIu32 "\n", audio->cab.RIFF.ChunkSize);
	printTag("wave tag", audio->cab.RIFF.Format, 4);

	printTag("form tag", audio->cab.fmt.SubChunk1ID, 4);
	printf("fmt_size       (4 bytes): %" PRIu32 "\n", audio->cab.fmt.SubChunk1Size);
	printf("audio_format   (2 bytes): %" PRIu16 "\n", audio->cab.fmt.AudioFormat);
	printf("num_channels   (2 bytes): %" PRIu16 "\n", audio->cab.fmt.NrChannels);
	printf("sample_rate    (4 bytes): %" PRIu32 "\n", audio->cab.fmt.SampleRate);
	printf("byte_rate      (4 bytes): %" PRIu32 "\n", audio->cab.fmt.ByteRate);
	printf("block_align    (2 bytes): %" PRIu16 "\n", audio->cab.fmt.BlockAling);
	printf("bits_per_sample(2 bytes): %" PRIu16 "\n", audio->cab.fmt.BitsPerSample);

	printTag("data tag", audio->cab.data.SubChunk2ID, 4);
	printf("data size      (4 bytes): %" PRIu32 "\n", audio->cab.data.SubChunk2Size);

	printf("bytes per sample        : %" PRIu32 "\n", audio->cab.fmt.BitsPerSample/8);
	printf("samples per channel     : %" PRIu32 "\n", (audio->cab.data.SubChunk2Size/audio->cab.fmt.NrChannels)/(audio->cab.fmt.BitsPerSample/8));
}



// Ajuste do volume, respeitando os valores máximos
void ajustar_volume(Audio_t *audio, float level)
{	if (level != 1.0)
		for (int i = 0; i < audio->tamanho; i++)
			audio->dados[i] = mult_com_limite(audio->dados[i], level, VOLMAX);
}

void normalizar_volume(Audio_t *audio)
{
	int16_t valorAtual, maiorValor = 0;

	// Acha o maior valor absoluto
	for (int i = 0; i < audio->tamanho; i++)
	{
		valorAtual = abs(audio->dados[i]);
		if (valorAtual > maiorValor)
			maiorValor = valorAtual;
	}

	float level = (float)VOLMAX / (float)maiorValor;
	ajustar_volume(audio, level);
}

void troca(int16_t *a, int16_t *b)
{
	int16_t aux = *a;
	*a = *b;
	*b = aux;
}

void reverter_audio(Audio_t *audio)
{
	for (int i = 0; i < audio->tamanho / 2; i++)
		troca(&audio->dados[i], &audio->dados[audio->tamanho - 1 - i]);
}

void ecoar(Audio_t *audio, float level, int delay)
{
	int pontoInicial;
	uint32_t amostragem;

	amostragem = audio->cab.fmt.SampleRate;
	pontoInicial = amostragem * delay / 1000;

	for (int i = pontoInicial; i < audio->tamanho; i++)
		audio->dados[i] = soma_com_limite(audio->dados[i], level * audio->dados[i - pontoInicial], VOLMAX);
}

int confere_dois_canais(Audio_t *audio)
{
	if (audio->cab.fmt.NrChannels != 2)
		return 0;
	return 1;
}
void estereo_amplificado(Audio_t *audio, int k)
{
	int16_t diff;

	for (int i = 0; i < audio->tamanho; i += 2)
	{
		diff = audio->dados[i + 1] - audio->dados[i];
		audio->dados[i + 1] = soma_com_limite(audio->dados[i + 1], k * diff, VOLMAX); // Canal direito
		audio->dados[i] = soma_com_limite(audio->dados[i + 1], -(k * diff), VOLMAX);  // Canal esquerdo
	}
}

int audios_compativeis(Audio_t *audioA, Audio_t *audioB)
{
	// Nr de canais
	if (audioA->cab.fmt.NrChannels != audioB->cab.fmt.NrChannels)
		return 0;

	// Taxa de amostragem
	if (audioA->cab.fmt.SampleRate != audioB->cab.fmt.SampleRate)
		return 0;

	// Codificação utilizada
	if (audioA->cab.fmt.AudioFormat != audioB->cab.fmt.AudioFormat)
		return 0;

	// Bits por sample
	if (audioA->cab.fmt.BitsPerSample != audioB->cab.fmt.BitsPerSample)
		return 0;

	return 1;
}

// Retorna a concatenação das duas músicas na música A
void concatatenar_audios(Audio_t *audioA, Audio_t *audioB)
{

	// Conferir compatibilidade
	if (!audios_compativeis(audioA, audioB))
	{
		// Desaloca tudo

		// Imprime mensagem de erro
		fprintf(stderr, "Arquivos nao compativeis\n");

		// Encerrar o programa
		exit(1);
	}

	// Modificar o cabeçalho da audio destino
	//// RIFF ChunkSize
	audioA->cab.RIFF.ChunkSize += audioB->cab.data.SubChunk2Size;

	//// Data SubChunk2Size
	audioA->cab.data.SubChunk2Size += audioB->cab.data.SubChunk2Size;

	// Realocar audio->dados da audio destino
	audioA->dados = realloc(audioA->dados, audioA->cab.data.SubChunk2Size);

	// Concatenar os audio->dados
	for (int i = 0; i < audioB->tamanho; i++)
	{
		audioA->dados[i + audioA->tamanho - 1] = audioB->dados[i];
	}

	audioA->tamanho += audioB->tamanho;
}

// Retorna a mixagem nas duas músicas na audioA
void mixar_audios(Audio_t *audioA, Audio_t *audioB)
{
	// Conferir compatibilidade
	if (!audios_compativeis(audioA, audioB))
	{
		// Desaloca tudo

		// Imprime mensagem de erro
		fprintf(stderr, "Arquivos nao compativeis\n");

		// Encerrar o programa
		exit(1);
	}

	// Começa assumindo que o audio B é menor
	int menor = audioB->tamanho;

	// Caso não seja, arruma o audio A para que ele comporte o tamanho de B
	if (audioA->tamanho < audioB->tamanho)
	{
		menor = audioA->tamanho;
		audioA->tamanho = audioB->tamanho;
		audioA->cab = audioB->cab;
		audioA->dados = realloc(audioA->dados, audioB->cab.data.SubChunk2Size);

		for (int i = menor; i < audioB->tamanho; i++)
			audioA->dados[i] = audioB->dados[i];
	}

	for (int i = 0; i < menor; i++)
		audioA->dados[i] = soma_com_limite(audioA->dados[i], audioB->dados[i], VOLMAX);
}


void fechar_streams(FILE *ENTRADA, FILE *SAIDA)
{
	if (ENTRADA != stdin)
		fclose(ENTRADA);
	if (SAIDA != stdout)
		fclose(SAIDA);
}

void tratar_audios(int argc, char **argv, FILE *ENTRADA, Audio_t *audio, Audio_t *apendice)
{
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
	// Continuo a busca por todos os argumentos que não são '-o'
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
			concatatenar_audios(audio, apendice);
			free(apendice->dados);
		}

		i++;
	}
}