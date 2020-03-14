flex -t myLex.l > sample.c
gcc -c .\sample.c
gcc sample.o -o lexicalAnalyzer.exe

.\lexicalAnalyzer.exe