I did a research and found out there are only 3 random access containers(STL ARRAY,VECTOR,DEQUE).
BUT ARRAY DOESN'T HAS HAVE push_back OR resize METHODS.
SO IT IS IMPOSSIBLE ARRAY TO HAVE SAME CONSTRUCTOR WITH VECTOR AND DEQUE.
SO MY ADAPTER CLASS ONLY WORKS FOR VECTOR AND DEQUE.

MY MENU IN DRIVER CODE:


1st option: INSERT NEW GAME
	INPUT BOARD SIZE:
		INPUT GAME MODE(1,2):
			CHOSE CONTAINER(1,2,3):
				IF YOU CHOOSE 3:
					CHOSE CONTAINER(1,2):

2nd option:
	CHOSE GAME(1,2,3,4...) OR COMPARE GAMES (12,23...)(12 compares games 1 and 2) OR QUIT(0)
	IF YOU CHOSE GAME: you can input: a1,b5 etc.. , SAVE location.txt , LOAD location.txt ,SETSIZE newsize(newsize is int), reset , quit

3rd option: quit

PLEASE PLAY FULLSCREEN
