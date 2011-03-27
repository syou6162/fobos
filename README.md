# FOBOS
Implementation of Support Vector Machine with L1 regularization and Logistic Regression with L1. Optimization algorithm is FOBOS(Forward-Backward Splitting).

# Usage
	./wscript configure
	./wscript build

# Example
	wget http://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary/news20.binary.bz2
	bzip2 -d news20.binary.bz2 
	ruby shuffle news20.binary > aaa.txt
	head -n 15000 aaa.txt > train.txt; tail -n 4996 aaa.txt > test.txt
	./build/src/main --train train.txt --test test.txt # default is svm
	./build/src/main --train train.txt --test test.txt -c logistic
