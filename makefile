debug: card.h main.c main.h shuffle.c shuffle.h
	gcc -Og -Wall main.c shuffle.c -o kb_solitaire.elf

clean: kb_solitaire.elf
	rm kb_solitaire.elf

run:
	make debug
	./kb_solitaire.elf
