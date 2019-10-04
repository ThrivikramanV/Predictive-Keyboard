#include "requirements.h"


// kann_t *load_network(char *fn, int c2i[256])
// {
// 	FILE *fp;
// 	kann_t *rnn;
// 	fp = fn && strcmp(fn, "-") ? fopen(fn, "rb") : stdin;
// 	rnn = kann_load_fp(fp);
// 	fread(c2i, sizeof(int), 256, fp);
// 	fclose(fp);
// 	return rnn;
// }

// To get the last word from the sentence
void last_word(char sentence[40])
{
    // printf("%s\n", sentence);
    char word[40], c=' ';
    int i=0, j=0;
    while(c!='\0')
    {
        word[i] = sentence[j];
        c = word[i];
        if(c==' ')
            i = 0;
        else
            i++;
        j++;
    }
    // printf(" word%s\n", word);
    i = 0;
    c = ' ';
    while(c != '\0')
    {
        sentence[i] = word[i];
        c = word[i];
        i++;
    }
    // printf("sentence %s\n", sentence);
}

// This will give the rest of the text to fp

void gen_text(char *fp, kann_t *ann, float temp, int len, const int c2i[256], const char *seed)
{
	strcpy(fp, seed);
	int i, c, n_char, i2c[256], i_temp;
	float x[256];
	memset(i2c, 0, 256 * sizeof(int));
	// printf("What about this?\n");
	for (i = 0; i < 256; ++i)
		if (c2i[i] >= 0) i2c[c2i[i]] = i;
	// printf("What about this?\n");
	n_char = kann_dim_in(ann);
	i_temp = kann_find(ann, 0, -1);
	if (i_temp >= 0) ann->v[i_temp]->x[0] = 1.0f / temp;
	kann_rnn_start(ann);
	for (c = 0; c < ann->n; ++c) {
		kad_node_t *p = ann->v[c];
		if (p->pre) {
			int l = kad_len(p);
			for (i = 0; i < l; ++i)
				p->x[i] = 2.0 * kann_drand() - 1.0;
		}
	}
	// printf("Not bad really\n");
	if (seed) {
		const char *p;
		for (p = seed; *p; ++p) {
			const float *y;
			float max = -1.0f;
			int max_c = -1;
			c = c2i[(int)*p];
			assert(c >= 0);
			memset(x, 0, n_char * sizeof(float));
			x[c] = 1.0f;
			y = kann_apply1(ann, x);
			for (c = 0; c < n_char; ++c)
				if (max < y[c]) max = y[c], max_c = c;
			c = max_c;
		}
		// fprintf(fp, "%s%c", seed, i2c[c]);
		*(fp++) = i2c[c];
		// printf("%c--\n", i2c[c]);
		// strcpy(fp, seed);
	} else c = c2i[(int)' '];
	for (i = 0; i < len; ++i) {
		float s, r;
		const float *y;
		memset(x, 0, n_char * sizeof(float));
		x[c] = 1.0f;
		y = kann_apply1(ann, x);
		r = kann_drand();
		for (c = 0, s = 0.0f; c < n_char; ++c)
			if (s + y[c] >= r) break;
			else s += y[c];
		*(fp++) = i2c[c];
		// printf("%c--\n", i2c[c]);
		// fputc(i2c[c], fp);
		break;
	}
	// fputc('\n', fp);
	// *(fp++) = '\n';
	*fp = '\0';
	kann_rnn_end(ann);
	if (i_temp >= 0) ann->v[i_temp]->x[0] = 1.0f;
}

void rest_of_word(char *prefix, char *result, kann_t *rnn, int c2i[256])
{
	int c, seed = 11, output_len = 100;
	float temp = 1.0f;
	char long_word[50] = {};
	char *output = long_word;

	// kann_srand(seed);
	// kad_trap_fe();
	// rnn = load_network(model_name, c2i);
	// printf("Works here!\n");

	gen_text(output, rnn, temp, output_len, c2i, prefix);
	kann_delete(rnn);
	// printf("Works upto here\n");
	// strcat(prefix, output);
	int i;
	for (i = 0; prefix[i]!='\0'; ++i)
	{
		result[i] = prefix[i];
	}
	for (; *output != '\0'; ++i)
	{
		result[i] = *(output++);
	}
	result[i] = '\0';
}

// int main()
// {
// 	char *madness="", start[20], result[40];
// 	int c2i[256];
// 	kann_t *model = load_network("mod1.kan", c2i);
// 	printf("Enter the word\n");
// 	scanf("%s", start);
// 	rest_of_word(start, result, model);
// 	printf("%s\n", result);
// }