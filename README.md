# C-Mini-Project
Mini project for sem 2

This is a text editor which can predict the text that you want to input, even before you completely type a word.
Predkey makes use of the great kann library, which helps to make neural networks in c, even RNNs with LSTMs which we have used to train our models.
The main.c file handles the GUI and the integration of the rest of the files, predict.c Handles the actual prediction of the words.
It can be modified to predict more than one word too.

To install and get the model working you need the files in this directory expect the testing folder.
Once you have cloned this repo, you can compile the main file by using the command:

```
gcc main.c predict.c kann.c kautodiff.c -lm -o editor
```

And then to execute the editor 
```
./editor
```

Bugs can be reported in the issues coloumn and they will be fixed soon.
Known bugs can be found in the issues page too.

