files = conway.cpp 
filebasename = "conway"
depends = cgif.o conway_cgif.o

$(depends): %.o: %.c

all: $(depends)
	/opt/homebrew/Cellar/llvm/12.0.1/bin/clang++ \
		-std=gnu++20 \
		-g \
		$(depends) \
		$(files) \
		-o ${filebasename} \
		-L/opt/homebrew/Cellar/llvm/12.0.1/lib/ 

clean:
	rm -f ${FILEBASENAME}
