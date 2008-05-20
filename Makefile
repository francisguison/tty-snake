
tty-snake : snake.c

	@echo "*** Building the snake ***"
	@cc -lncurses $^ -o tty-snake -Wall -v
	@echo "*** The snake have been built ***"
	@echo "*** You can execute it without install with ./tty-snake --help ***"
	@echo ""

install :tty-snake
  
	@echo "*** Moving the snake ***"
	@cp tty-snake /usr/local/bin/
	@echo "*** Snake moved ***"
	@echo ""
	@echo "*** Chmoding the snake ***"
	@chmod +x /usr/local/bin/tty-snake
	@echo "*** Your snake is installed. play with it: tty-snake --help ***"
	@echo ""

uninstall :

	@echo "*** Decapiting the snake ***"
	@rm -f /usr/local/bin/tty-snake
	@echo "*** Snake decapited :'( ***"

clean :

	@echo "*** Cleaning the snake ***"
	@rm tty-snake
	@echo "*** Your snake is clean ;) ***"
