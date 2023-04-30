flags: -Wall -Wextra -fanalyzer -Os
src = app.c

app: $(src)
	gcc $(src) $(flags) -o app

run: app
	./app
