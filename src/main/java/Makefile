include ../../../Makefile.in

JAVA_LIB_DIR := ../../lib
JAVA_OUTPUT_CLASS_DIR := ../../classes
JAVA_OUTPUT_JAR = jroot

JAVADOC_DIR = ../../doc
ifeq ($(OS),Darwin)
JAVA_OPT = -XstartOnFirstThread
else
endif

all: build-jar

build-classes: *.java
	echo ${JAVA_HOME}
	echo ${JAVAC}
	${JAVAC} -d ${JAVA_OUTPUT_CLASS_DIR} $^

build-jar: build-classes
	${JAR} cvf ${JAVA_LIB_DIR}/${JAVA_OUTPUT_JAR}_${ROOT_VERSION}_v${VERSION}.jar -C ${JAVA_OUTPUT_CLASS_DIR} .

javadoc: *.java
	${JAVADOC} -locale en_US -cp ${JAVA_OUTPUT_CLASS_DIR} -d ${JAVADOC_DIR} $^

