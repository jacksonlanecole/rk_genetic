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
	cp build/rk_ext.so test/.
	cp build/rk_ext.so notebooks/.
fi
