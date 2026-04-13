CCC		= cc -Wall -Werror -Wextra
SRC		= main.c
MAIN	= codexion

all: $(MAIN)
	$(CCC) $(SRC) -o $(MAIN)

run: all
	./$(MAIN) 12 1 2 3 4 10 6 "edf"

clean:
	rm -rf $(MAIN)