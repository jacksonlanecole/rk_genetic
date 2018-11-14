#!/bin/sh
display_usage() {
cat <<EOF
Usage: sh $0 [argument]

	0 | Run ONLY make in the build directory
	1 | Run both CMake and make in build directory

EOF
}

if [[ $# -ne 1 ]]
then
	display_usage
else
	sh scripts/run_cmake.sh $1

	if [[ ! -d lib ]]
	then
		mkdir -p lib
	fi

	cp build/rk_ext.so lib/.
	cp lib/rk_ext.so test/.
	cp lib/rk_ext.so src/python/nbody/.

	if [[ ! -d notebooks/nbody ]]
	then
		ln -s src/python/nbody notebooks/.
	fi
fi
