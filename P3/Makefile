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
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS) $(CFLAGS)

ssoiigle:
	./$(DIREXE)manager 50

clean : 
	rm -rf *~ core $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~