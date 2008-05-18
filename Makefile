tty-snake :snake.c
	
	@cc -lncurses $^ -o tty-snake -Wall -v

install :snake.c
  
	@cc -lncurses $^ -o tty-snake -Wall -v
	@echo "file transfert"
	@cp tty-snake /usr/local/bin/
	@echo "file chmod"
	@chmod +x /usr/local/bin/tty-snake

uninstall :snake.c

	@rm -rf /usr/local/bin/tty-snake

clean :

	@rm tty-snake

