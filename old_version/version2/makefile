Src=$(wildcard *.c)
Obj=$(patsubst %.c,%.o,$(Src))
webserver:$(Obj)
	gcc -o webserver $(Obj) -lpthread

%.o:%.c
	gcc -c $< -o $@

clean:
	rm *.o webserver
