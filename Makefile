
PLATFORM :=${HIARCH}
PLATFORM ?=hi3520d

install:
	cp arch/$(PLATFORM) arch/include build/ -rf
	cp lib/$(PLATFORM)  build/lib -rf
	cp winForm/	    build/ -rf
	cp graphics/	    build/ -rf
	cp common/ 	    build/ -rf
	cd build/ && make -f Makefile.$(PLATFORM) HIARCH=$(PLATFORM)  install
testing:
	cp test/	    build/ -rf	
	cp arch/$(PLATFORM) arch/include build/ -rf
	cp lib/$(PLATFORM)  build/lib -rf
	cp winForm/	    build/ -rf
	cp graphics/	    build/ -rf
	cp common/ 	    build/ -rf
	cd build/ && make -f Makefile.$(PLATFORM) HIARCH=$(PLATFORM)  testing
clean:
	cd build/ && rm -rf `ls |grep -v ^Makefile`
	
	
