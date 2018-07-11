cc=gcc
.PHONY:all
all:udp_client udp_server

udp_client:udp_client.c
	$(cc) -o $@ $^

udp_server:udp_server.c
	$(cc) -o $@ $^

.PHONY:clean
clean:
	rm -f udp_client udp_server


