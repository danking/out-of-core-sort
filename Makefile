CXX = /usr/local/Cellar/llvm/7.0.0_1/bin/clang++

build/bsort/src/bsort:
	mkdir -p build/bsort
	cd build/bsort && curl https://media.githubusercontent.com/media/pelotoncycle/bsort/master/downloads/bsort-0.0.1.tar.gz | tar -xvz
	cd build/bsort && autoreconf --install
	cd build/bsort && ./configure && $(MAKE)

build/input:
	mkdir -p build
	cat /dev/urandom | head -c 10000000 > build/input

build/sorted: build/input build/bsort/src/bsort
	mkdir -p build
	cp $< $@
	build/bsort/src/bsort -k 4 -r 4 $@

build/output: sort
	./sort build/input build/output $$(stat -f %z build/input)

build/%.xxd: build/%
	xxd $< > $@

build/diff: build/sorted.xxd build/output.xxd
	-diff -y $^ > $@

.PHONY: diff
diff: build/diff
	less build/diff

.PHONY: cdiff
cdiff: build/diff
	cat build/diff | colordiff

.PHONY: clean
clean:
	rm -rf build/output

.PHONY: fullclean
fullclean:
	rm -rf build
