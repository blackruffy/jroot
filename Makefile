include Makefile.in

CPP_SRC_DIR = src/main/cpp
CPP_OBJ_DIR = build/objects
ifeq ($(OS),Darwin)
CPP_LIB_DIR = build/lib/darwin
else
CPP_LIB_DIR = build/lib
endif
CPP_BIN_DIR = build/bin
CPP_BUILD_SRC_DIR = build/src/cpp
CPP_FILES = $(wildcard ${CPP_SRC_DIR}/*.cpp)
CPP_OBJS = $(addsuffix .o,$(basename $(notdir ${CPP_FILES})))
CPP_OBJS_PATH = $(addprefix ${CPP_OBJ_DIR}/,${CPP_OBJS})
CPP_JNI_FILES = $(wildcard ${CPP_BUILD_SRC_DIR}/net_blackruffy_root_*.cpp)
CPP_JNI_OBJS = $(addsuffix .jnio,$(basename $(notdir ${CPP_JNI_FILES})))

ifeq ($(OS),Darwin)
CPP_OPT = -w -fPIC -Xlinker -rpath -Xlinker ${CPP_ROOT_DIR}/lib
CPP_OUTPUT_LIB = libJRoot.dylib
else
CPP_OPT = -w -fPIC
CPP_OUTPUT_LIB = libJRoot.so
endif

JAVA_SRC_DIR = src/main/java
#JAVA_CLASSPATH = lib/jna-4.2.0.jar
JAVA_OUTPUT_CLASS_DIR = build/classes
JAVA_BUILD_SRC_DIR = build/src/java
JAVA_LIB_DIR = build/lib
JAVA_OUTPUT_JAR = jroot

ifeq ($(OS),Darwin)
JAVA_OPT = -XstartOnFirstThread
endif

all:
	${MAKE} srcgen
	${MAKE} build-java
	${MAKE} gen-jnih
	${MAKE} build-cpp

srcgen: mkdir update-makefile
	g++ ${CPP_OPT} -o ${CPP_BIN_DIR}/$@ ${CPP_SRC_DIR}/$@.cpp ${CPP_ROOT_FLAGS} ${CPP_ROOT_LIBS}
	./build/bin/srcgen
	cp -v ${CPP_SRC_DIR}/addr.h ${CPP_BUILD_SRC_DIR}/.
	cp -v ${CPP_SRC_DIR}/addr.cpp ${CPP_BUILD_SRC_DIR}/.
	cp -v ${CPP_SRC_DIR}/net_blackruffy_root_PointerNative.cpp ${CPP_BUILD_SRC_DIR}/.
	cp -v ${JAVA_SRC_DIR}/Pointer.java ${JAVA_BUILD_SRC_DIR}/.

update-makefile:
	cp -v ${CPP_SRC_DIR}/Makefile ${CPP_BUILD_SRC_DIR}/.
	cp -v ${JAVA_SRC_DIR}/Makefile ${JAVA_BUILD_SRC_DIR}/.

build-cpp:
	${MAKE} -C ${CPP_BUILD_SRC_DIR}

compile-java: ${JAVA_SRC_DIR}/*.java
	javac -cp ${JAVA_CLASSPATH}:${JAVA_OUTPUT_CLASS_DIR} -d ${JAVA_OUTPUT_CLASS_DIR} $^

build-java: 
	${MAKE} -C ${JAVA_BUILD_SRC_DIR}

javadoc:
	${MAKE} -C ${JAVA_BUILD_SRC_DIR} javadoc

gen-jnih:
	./bin/genjnih.sh

run-java:
	java  -cp ${JAVA_CLASSPATH}:${JAVA_OUTPUT_CLASS_DIR}:${CPP_ROOT_DIR}/lib:${CPP_LIB_DIR} ${ARG1}

run-example:
	javac -cp ${JAVA_OUTPUT_CLASS_DIR} -d examples/java examples/java/${ARG1}.java
	java ${JAVA_OPT} -Djava.library.path=${CPP_LIB_DIR} -cp ${JAVA_OUTPUT_CLASS_DIR}:${CPP_ROOT_DIR}/lib:${CPP_LIB_DIR}:examples/java ${ARG1}

run-example-cpp:
	g++ ${CPP_OPT} -o examples/cpp/${ARG1} examples/cpp/${ARG1}.cpp -I${CPP_BUILD_SRC_DIR} -L${CPP_LIB_DIR} -lJRoot ${CPP_ROOT_FLAGS} ${CPP_ROOT_LIBS}
	LD_LIBRARY_PATH=./build/lib:${LD_LIBRARY_PATH}; ./examples/cpp/${ARG1}

mkdir:
	mkdir -p ${CPP_LIB_DIR} ${CPP_OBJ_DIR} ${CPP_BIN_DIR} ${CPP_BUILD_SRC_DIR} ${JAVA_OUTPUT_CLASS_DIR} ${JAVA_BUILD_SRC_DIR} ${JAVADOC_DIR}

package:
	${MAKE} clean
	${MAKE} mkdir
	tar zcvf build/jroot_${ROOT_VERSION}_v${VERSION}.tgz ../jroot

clean:
	rm -vf *.root
	cd examples/cpp && rm -vf `ls | grep -v '.*\.cpp'`
	rm -vf examples/java/*.class
	rm -rfv ./build
