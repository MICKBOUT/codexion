CCC		= cc -Wall -Werror -Wextra
SRC		= main.c
MAIN	= codexion



all:
	$(CCC) $(SRC) -o $(MAIN)

run: all
	./$(MAIN) 12 1 2 3 4 10 6 "edf"

val: all
	valgrind ./$(MAIN) 8 7 6 5 4 3 2 "edf"

clean:
	rm -rf $(MAIN)