
.PHONY:all
all:tcp_client tcp_server

tcp_client:tcp_client.c
	gcc -o $@ $^
	
tcp_server:tcp_server.c condition.c threadpool.c
	gcc -o $@ $^ -lpthread -lrt

.PHONY:clean
clean:
	rm -f tcp_client tcp_server
