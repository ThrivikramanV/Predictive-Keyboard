#include <math.h>
#include <stdio.h>
#include <float.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include "kann.h"

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

void rest_of_word(char *prefix, char *result, kann_t *rnn, int c2i[256]);
void last_word(char []);

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void disp_predictions(HWND, char [3][40]);