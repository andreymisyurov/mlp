all: clean dist build dvi install tests

install:
	cd build && make

build:
	mkdir "build" && cd build && cmake ./../Neuro

tests:
	cd build && make test
	cd build && ./test
	cd build && lcov -t "test" -c -d . -o test.info
	cd build && genhtml -o report test.info
	cd build && open ./report/index.html

uninstall: clean

dist:
	mkdir dist
	tar --totals -cvf dist/MLP.tar .
	gzip dist/MLP.tar

dvi:
	doxygen
	-open ./doc/html/index.html

clean:
	rm -rf build doc dist

clang-format:
	sh clang.sh

clang:
	sh clang_check.sh
