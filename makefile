all : ser cli
	@echo 'done'
	
ser : common.cpp  common.h  protocol.h  server.cpp  transfer.cpp  transfer.h
	c++ -o ser common.cpp server.cpp transfer.cpp -lpthread

cli : common.cpp  common.h  protocol.h  client.cpp  transfer.cpp  transfer.h	
	c++ -o cli common.cpp client.cpp transfer.cpp -lpthread

clean :
	rm ser cli	