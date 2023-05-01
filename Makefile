flags = -Wall -Wextra -fanalyzer -Os -I include
src = $(wildcard src/*.c)

app: $(src)
	gcc $(src) $(flags) -o app
