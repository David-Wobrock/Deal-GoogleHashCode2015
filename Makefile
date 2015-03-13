INCPATH=
LIBPATH=

EXE=hashcode
CLEAN=clean

COMP=@g++
CPPFLAGS= -Wall -pedantic -std=c++11
LINK=@g++
LINKFLAGS=
INT=Serveur.h Emplacement.h Groupe.h
SRC=$(INT:.h=.cpp)
OBJ=$(INT:.h=.o)
LIBS=

MAINCPP=main.cpp
MAINOBJ=$(MAINCPP:.cpp=.o)

ECHO=@echo
RM=rm
RMFLAGS=-f

$(EXE): $(MAINOBJ) $(OBJ)
	$(ECHO) "Edition des liens de $(EXE)"
	$(LINK) $(LINKFLAGS) -o $(EXE) $(MAINOBJ) $(OBJ) $(LIBS)
		
%.o: %.cpp
	$(ECHO) "Compilation de $<"
	$(COMP) $(CPPFLAGS) -c $<

$(MAINOBJ): $(MAINCPP)
	$(ECHO) "Compilation de $(MAINCPP)"
	$(COMP) $(CPPFLAGS) -c $(MAINCPP)
	
.PHONY: $(CLEAN)
$(CLEAN):
	$(RM) $(RMFLAGS) $(OBJ) $(MAINOBJ) $(EXE) core
