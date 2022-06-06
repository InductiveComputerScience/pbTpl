gcc -c TextualTemplates.c -std=c99 -O3 -march=native
gcc -c launcher.c -std=c99 -O3 -march=native
gcc -c supportLib.c -std=c99 -O3 -march=native
gcc launcher.o TextualTemplates.o supportLib.o -lm -o pbtpl
strip pbtpl
