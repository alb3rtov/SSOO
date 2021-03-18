DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

LDLIBS := -pthread -std=c++11
CC := g++

all : dirs manager

dirs:
	mkdir -p $(DIREXE)

manager: $(DIRSRC)manager.cpp 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

ssoiigle:
	./$(DIREXE)manager $(file) $(word) $(tn)

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~
