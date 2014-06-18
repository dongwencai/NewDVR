
PLATFORM :=${ARCH}
PLATFORM ?=hi3520d
install:
	cp arch/$(PLATFORM) arch/include build/ -rf
	cp lib/$(PLATFORM)  build/lib -rf
	cp winForm/	    build/ -rf
	cp graphics/	    build/ -rf
	cp common/ include/ build/ -rf
clean:
	cd build/
	rm -rf `ls |grep -v ^Makefile`
