g++ -c main.cpp -o ./obj/main.o `pkg-config --cflags --libs gtk+-2.0`
g++ -c gui.cpp -o ./obj/gui.o `pkg-config --cflags --libs gtk+-2.0`
g++ -c flash89s.cpp -o ./obj/flash89s.o `pkg-config --cflags --libs gtk+-2.0`

g++ -c libAT89S.cpp -o ./obj/libAT89S.o `pkg-config --cflags --libs gtk+-2.0`
g++ -c libHexFile.cpp -o ./obj/libHexFile.o
g++ -c libPgmISPv2.cpp -o ./obj/libPgmISPv2.o
g++ -c libProgrammer.cpp -o ./obj/libProgrammer.o
g++ -c libUSBBULK.cpp -o ./obj/libUSBBULK.o

g++ ./obj/main.o ./obj/gui.o ./obj/libUSBBULK.o ./obj/libProgrammer.o ./obj/libAT89S.o ./obj/flash89s.o ./obj/libHexFile.o ./obj/libPgmISPv2.o -lusb-1.0 -o ispv2 `pkg-config --cflags --libs gtk+-2.0`
