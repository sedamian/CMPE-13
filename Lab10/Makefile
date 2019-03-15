all: labten
	
labten: rpg.o	Player.o	Game.o
	gcc -o labten rpg.o	Game.o Player.o
	
rpg.o: rpg.c
	gcc -Wall -c rpg.c
			
Game.o: Game.c
	gcc -Wall -c Game.c
			
Player.o: Player.c
	gcc -Wall -c Player.c
			
clean:
	rm *.o
	rm labten