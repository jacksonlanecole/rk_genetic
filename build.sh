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
	cp lib/rk_ext.so python/nbody/.

	if [[ ! -L notebooks/nbody ]]
	then
		ln -s python/nbody notebooks/nbody
	fi
fi
