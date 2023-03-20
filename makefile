all: random fifo lru test
random: 
	@g++ random.cpp -o random
fifo:
	@g++ fifo.cpp -o fifo
lru:
	@g++ lru.cpp -o lru
test:
	@./random 40 40 40 ./Test_files/req1.dat
	@./fifo 40 40 40 ./Test_files/req1.dat
	@./lru 40 40 40 ./Test_files/req1.dat
clean:
	@rm -f random fifo lru