g++ -c client/Client.cpp -o build/Client.o
g++ -c component/Component.cpp -o build/Component.o
g++ -c component/unknown.cpp -o build/unknown.o
g++ build/Client.o build/Component.o build/unknown.o -o build/Main.exe