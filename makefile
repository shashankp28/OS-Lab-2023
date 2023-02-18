all: part1 part2_1a part2_1b part2_2 part2_3

part1: 
	@echo "----------------Part 1------------------"
	@g++ $@.cpp -o $@
	@./$@ sample1.ppm output_$@.ppm

part2_1a: 
	@echo "----------------Part 2: 1a------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample1.ppm output_$@.ppm

part2_1b: 
	@echo "----------------Part 2: 1b------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample1.ppm output_$@.ppm

part2_2: 
	@echo "----------------Part 2: 2------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample1.ppm output_$@.ppm

part2_3: 
	@echo "----------------Part 2: 3------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample1.ppm output_$@.ppm

clean:
	@rm -f part1 part2_1a part2_1b part2_2 part2_3 output_*