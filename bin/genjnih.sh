#!/bin/bash

for i in ./build/classes/net/blackruffy/root/*Native.class; do name=${i/*\//}; name=${name/.class/}; echo ${name}; javah -classpath ./build/classes -d ./build/src/cpp net.blackruffy.root.${name}; done
