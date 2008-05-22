
tty-snake : snake.c

	@echo "*** Building ***"
	@cc -lncurses $^ -o tty-snake -Wall -v
	@echo "*** TTY-Snake build ***"
	@echo ""

install :tty-snake
  
	@echo "*** Moving TTY-Snake ***"
	@cp tty-snake /usr/local/bin/
	@echo "*** Snake moved ***"
	@echo ""
	@echo "*** Chmod TTY-Snake ***"
	@chmod +x /usr/local/bin/tty-snake
	@echo "*** TTY-Snake is now installed ***"
	@echo ""

uninstall :

	@echo "*** Uninstall TTY-Snake ***"
	@rm -f /usr/local/bin/tty-snake
	@echo "*** TTY-Snake uninstalled :'( ***"

clean :

	@echo "*** Cleaning TTY-snake ***"
	@rm tty-snake
	@echo "*** Your TTY-Snake is clean ;) ***"
