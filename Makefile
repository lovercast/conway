CWD = $(shell pwd)
source_folder = src
external_folder = external
source = ${CWD}/${source_folder}/conway.cpp 
filebasename = conway
obj = ${CWD}/${external_folder}/cgif/cgif.o ${CWD}/${source_folder}/conway_cgif.o
compiler = /opt/homebrew/Cellar/llvm/12.0.1/bin/clang++
libs = /opt/homebrew/Cellar/llvm/12.0.1/lib/ 


$(depends): %.o: %.c

all: $(depends)
	${compiler} \
		-std=gnu++2a \
		-g \
		$(obj) \
		$(source) \
		-o ${CWD}/${filebasename} \
		-L${libs}

clean:
	rm -f $(obj) 
	rm -f ${filebasename}
