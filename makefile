build:
	g++ FIFO.cpp -o FIFO
	g++ LRU.cpp -o LRU
	g++ Random.cpp -o Random

clean:
	rm -rf *.exe