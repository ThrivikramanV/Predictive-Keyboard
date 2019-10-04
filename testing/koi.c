#include"requirements.h"

kann_t *load_network(char *fn, int c2i[256])
{
	FILE *fp;
	kann_t *rnn;
	fp = fn && strcmp(fn, "-") ? fopen(fn, "rb") : stdin;
	rnn = kann_load_fp(fp);
	fread(c2i, sizeof(int), 256, fp);
	fclose(fp);
	return rnn;
}

void mkrndstr_ipa(int length, char *randomString) 
{ // const size_t length, supra
	static char charset[] = "abcdefghijklmnopqrstuvwxyz";

	if (length) {
	    if (randomString) {
	        int l = (int) (sizeof(charset) -1);
	        for (int n = 0;n < length;n++) {
	            int key = rand() % l;          // per-iteration instantiation
	            randomString[n] = charset[key];
	        }

	        randomString[length] = '\0';
	    }
	}
}

void T_predicts(char *initial, char outputs[3][40])
{
	char *saved_models[10] = {"mod1.kan", "mod2.kan", "mod4.kan"}, prediction[3][40];
	int c2i[3][256];
	kann_t **loaded_models;
	// printf("%s\n", saved_models[0]);
	loaded_models[0] = load_network(saved_models[0],c2i[0]);
	loaded_models[1] = load_network(saved_models[1],c2i[1]);
	loaded_models[2] = load_network(saved_models[2],c2i[2]);

	for (int i = 0; i < 3; ++i)
	{
		rest_of_word(initial, prediction[i], loaded_models[i], c2i[i]);
		// printf("Done %d \n",i);
	}
	// printf("First prediction: %s\n", prediction[0]);
	// printf("Second prediction: %s\n", prediction[1]);
	// printf("Third prediction: %s\n", prediction[2]);
	for(int i=0; i<3; i++)
		strcpy(outputs[i], prediction[i]);
}

int main()
{
	char init[20]={}, Tpre[3][40], words[20][5];
	int contd = 1, length = 3;
	printf("Starting the generation\n");

	for(int i=0; i<20; i++)
	{
	// scanf("%s", init);
	// strlwr(words[i]);			// This is important because the program does not accept Capital letters

	T_predicts(words, Tpre);
	// scanf("%d", &contd);
	}
	// printf("%s\n", Tpre[0]);

}
