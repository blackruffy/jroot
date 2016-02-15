include ../../../Makefile.in

CPP_FILES := $(wildcard *.cpp)
CPP_OBJS := $(addsuffix .o,$(basename ${CPP_FILES}))

ifeq ($(OS),Darwin)
CPP_OPT := -w -fPIC -Xlinker -rpath -Xlinker ${CPP_ROOT_DIR}/lib
CPP_LIB_DIR := ../../lib/darwin
CPP_OUTPUT_LIB := libJRoot.dylib
CPP_FLAGS := -I./ -I${JAVA_HOME}/include ${CPP_ROOT_FLAGS}
else
CPP_OPT := -w -fPIC
CPP_LIB_DIR := ../../lib
CPP_OUTPUT_LIB := libJRoot.so
CPP_FLAGS := -I./ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ${CPP_ROOT_FLAGS}
endif

all: ${CPP_OBJS}
	g++ ${CPP_OPT} -shared -o ${CPP_LIB_DIR}/${CPP_OUTPUT_LIB} $^ ${CPP_FLAGS} ${CPP_ROOT_LIBS}

%.o: %.cpp %.h
	g++ ${CPP_OPT} -c -o $@ $< ${CPP_FLAGS}
