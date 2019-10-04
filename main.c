#include "requirements.h"

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

void T_predicts(char *initial, char outputs[3][40])
{
    char prediction[3][40];
    kann_t **loaded_models;
    char *saved_models[10] = {"mod1.kan", "mod2.kan", "mod4.kan"};
    int c2i[3][256];
    loaded_models[0] = load_network(saved_models[0],c2i[0]);
    loaded_models[1] = load_network(saved_models[1],c2i[1]);
    loaded_models[2] = load_network(saved_models[2],c2i[2]);
    for (int i = 0; i < 3; ++i)
    {
        rest_of_word(initial, prediction[i], loaded_models[i], c2i[i]);
        last_word(prediction[i]);
        // printf("Done %d \n",i);
    }
    // printf("First prediction: %s\n", prediction[0]);
    // printf("Second prediction: %s\n", prediction[1]);
    // printf("Third prediction: %s\n", prediction[2]);
    for(int i=0; i<3; i++)
        strcpy(outputs[i], prediction[i]);
}

HWND TextBox,predictions[3];
char textSaved[100];
int len,temp=0,xPos,yPos,i,flag=0;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Predictive Keyboard"),       /* Title Text */
           WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char predicts[3][40] = {};
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:

            TextBox = CreateWindow("EDIT",
                                   "",
                                   WS_BORDER | WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
                                   5,5,485,420,
                                   hwnd,NULL,NULL,NULL);

            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                // printf("textSaved\n");
                case 1:
                    len = strlen(textSaved);
                    i = len-1;
                    while(textSaved[i]!=' ' && len!=0)
                    {
                        textSaved[i] = '\0';
                        i--;
                        len--;
                    }
                    strcat(textSaved, predicts[0]);
                    SetWindowText(TextBox, textSaved);
                    break;

                case 2:
                    len = strlen(textSaved);
                    i = len-1;
                    while(textSaved[i]!=' ' && len!=0)
                    {
                        textSaved[i] = '\0';
                        i--;
                        len--;
                    }
                    strcat(textSaved, predicts[1]);
                    SetWindowText(TextBox, textSaved);
                    break;

                case 3:
                    len = strlen(textSaved);
                    i = len-1;
                    while(textSaved[i]!=' ' && len!=0)
                    {
                        textSaved[i] = '\0';
                        i--;
                        len--;
                    }
                    strcat(textSaved, predicts[2]);
                    SetWindowText(TextBox, textSaved);
                    break;
            }
            break;
        default:
            GetWindowText(TextBox, textSaved, 100);
            len = strlen(textSaved);
            // printf("%s\n", textSaved);
            if(len>1)
                flag=1;
            if(len != temp)
            {
                strlwr(textSaved);
                T_predicts(textSaved, predicts);
                disp_predictions(hwnd, predicts);
                temp=len;
            }

            /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
void disp_predictions(HWND hwnd, char predictions_main[3][40])
{
                if(flag)
                {
                    DestroyWindow(predictions[0]);
                    DestroyWindow(predictions[1]);
                    DestroyWindow(predictions[2]);
                }
                predictions[0] = CreateWindow("BUTTON",
                                predictions_main[0],
                                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                                5,425,161,40,
                                hwnd,(HMENU)1,NULL,NULL);

                predictions[1] = CreateWindow("BUTTON",
                                predictions_main[1],
                                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                                166,425,161,40,
                                hwnd,(HMENU)2,NULL,NULL);

                predictions[2] = CreateWindow("BUTTON",
                                predictions_main[2],
                                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                                327,425,161,40,
                                hwnd,(HMENU)3,NULL,NULL);
}
