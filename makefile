debug: card.h game.c game.h main.c main.h shuffle.c shuffle.h symbols.h terminal.c terminal.h
	gcc -Og -g -std=c23 -Wall game.c main.c shuffle.c terminal.c -o kb_solitaire.elf

clean: kb_solitaire.elf
	rm kb_solitaire.elf

run:
	make debug
	./kb_solitaire.elf

o3: card.h game.c game.h main.c main.h shuffle.c shuffle.h symbols.h terminal.c terminal.h
	gcc -O3 -std=c23 -Wall game.c main.c shuffle.c terminal.c -o kb_solitaire.elf
