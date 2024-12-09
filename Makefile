day1: src/day1.c
	mkdir -p build
	cc -ggdb -Wall -Wextra -o build/day1 src/day1.c

day2: src/day2.c
	mkdir -p build
	cc -ggdb -Wall -Wextra -o build/day2 src/day2.c

day3: src/day3.c
	mkdir -p build
	cc -ggdb -Wall -Wextra -o build/day3 src/day3.c
