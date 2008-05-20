
tty-snake : snake.c

	@echo "*** Building ***"
	@cc -lncurses $^ -o tty-snake -Wall -v
	@echo "*** TTY-Snake build ***"
	@echo ""

install :tty-snake
  
	@echo "*** Moving the snake ***"
	@cp tty-snake /usr/local/bin/
	@echo "*** Snake moved ***"
	@echo ""
	@echo "*** Chmod the snake ***"
	@chmod +x /usr/local/bin/tty-snake
	@echo "*** TTY-Snake is now installed ***"
	@echo ""

uninstall :

	@echo "*** Uninstall TTY-Snake ***"
	@rm -f /usr/local/bin/tty-snake
	@echo "*** Snake decapited :'( ***"

clean :

	@echo "*** Cleaning TTY-snake ***"
	@rm tty-snake
	@echo "*** Your snake is clean ;) ***"
