flags = -Wall -Wextra -fanalyzer -Os -I include
src = $(wildcard src/*.c)

app: $(src)
	gcc $(src) $(flags) -o app

check_leaks:
	valgrind ./app test.txt 1000000
