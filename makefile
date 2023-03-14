all: random fifo lru test
random: 
	@g++ random.cpp -o random
fifo:
	@g++ fifo.cpp -o fifo
lru:
	@g++ lru.cpp -o lru
test:
	@./random 60 20 60 req1.dat
	@./fifo 60 20 60 req1.dat
	@./lru 60 20 60 req1.dat
clean:
	@rm -f random fifo lru