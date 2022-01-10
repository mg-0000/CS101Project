g++ -std=c++0x -Wall -g  -c main.cpp -o main.o
g++ -o main.exe main.o  -lsprite -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
main.exe