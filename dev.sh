#!/bin/zsh

compile () {
	if [[ $1 ]]; then
		compiler=$1
	else
		compiler='g++'
	fi
	$compiler \
	-std=c++14\
	-Wextra -Wall -pedantic\
	-pedantic-errors\
	-Wsign-conversion\
	-D DEBUG\
	-g\
	main.cpp\
	-o a.out
}

