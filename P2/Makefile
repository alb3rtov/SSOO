DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA)
LDLIBS := -pthread -std=c++11
CC := g++

all : dirs manager

dirs:
	mkdir -p $(DIREXE)

manager: $(DIRSRC)manager.cpp 
	$(CC) $(DIRSRC)Result.cpp $(DIRSRC)ThreadInfo.cpp -o $(DIREXE)$@ $^ $(LDLIBS) $(CFLAGS)

ssoiigle:
	./$(DIREXE)manager "17 LEYES DEL TRABAJO EN EQUIPO.txt" cuadro 8

clean : 
	rm -rf *~ core $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~