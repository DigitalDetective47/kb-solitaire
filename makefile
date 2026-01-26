debug: card.h game.c game.h main.c main.h shuffle.c shuffle.h symbols.h terminal.c terminal.h
	gcc -Og -g -std=c2x -Wall -Wextra -Werror game.c main.c shuffle.c terminal.c -o kb_solitaire.elf

clean:
	if [ -e kb_solitaire.elf ]; then\
		rm kb_solitaire.elf;\
	fi 
	if [ -e kb_solitaire.exe ]; then\
		rm kb_solitaire.exe;\
	fi\

run:
	make debug
	./kb_solitaire.elf

o3: card.h game.c game.h main.c main.h shuffle.c shuffle.h symbols.h terminal.c terminal.h
	gcc -O3 -std=c2x -Wall -Wextra -Werror game.c main.c shuffle.c terminal.c -o kb_solitaire.elf

windows: card.h game.c game.h main.c main.h shuffle.c shuffle.h symbols.h terminal.c terminal.h
	x86_64-w64-mingw32-gcc -D MSWIN -O3 -std=c2x -Wall -Wextra -Werror game.c main.c shuffle.c terminal.c -o kb_solitaire.exe
