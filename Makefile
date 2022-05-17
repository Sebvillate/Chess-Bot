CXX=g++
CXXFLAGS=-std=c++14 -g -MMD -Werror=vla
OBJECTS= main.o game.o piece.o player.o board.o computerplayer.o computerplayerdiff1.o computerplayerdiff2.o computerplayerdiff3.o computerplayerdiff4.o king.o pawn.o rook.o bishop.o queen.o knight.o move.o window.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=chess

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
