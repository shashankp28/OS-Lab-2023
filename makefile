all: part1 part2_1a part2_1b part2_2 part2_3

part1: 
	@echo "----------------Part 1------------------"
	@g++ $@.cpp -o $@
	@./$@ sample3.ppm transformed_$@.ppm

part2_1a: 
	@echo "----------------Part 2: 1a------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample3.ppm transformed_$@.ppm

part2_1b: 
	@echo "----------------Part 2: 1b------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample3.ppm transformed_$@.ppm

part2_2: 
	@echo "----------------Part 2: 2------------------"
	@g++ $@.cpp -o $@ -lpthread
	@./$@ sample3.ppm transformed_$@.ppm

part2_3: 
	@echo "----------------Part 2: 3------------------"
	@echo "In Progress"

clean:
	@rm -f part1 part2_1a part2_1b part2_2 part2_3 transformed_*