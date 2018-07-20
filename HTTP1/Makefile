
.PHONY:all
all:tcp_client tcp_server

tcp_client:tcp_client.c
	gcc -o $@ $^
	
tcp_server:tcp_server.c
	gcc -o $@ $^ -pthread

.PHONY:clean
clean:
	rm -f tcp_client tcp_server
