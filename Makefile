CFLAGS=-Wall -Wextra -Wpedantic

.PHONY: all test_avl run_test

run_test: test_avl

test_avl: ./avl
	./avl

./%: test/%.c
	gcc -ansi $(CFLAGS) $^ -o $@

clean:
	rm avl
