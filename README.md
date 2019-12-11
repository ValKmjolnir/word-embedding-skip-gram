# word-embedding-skip-gram
skip-gram word embedding model by C++

in header __nlp.h__
```C++
#include "word_lib.h"
#include "bp_network.h"
```

in header __word_lib.h__
```C++
class wordlib
{
	private:
		std::list<std::string> dictionary;
	public:
		wordlib();
		~wordlib();
		int get_place(std::string);
		int lib_size();
		bool search(std::string);
		void add_word(std::string);
		void add_word_from_file(std::string);
		void print_lib();
		void print_word(int);
		std::string get_word(int);
};
```
this class is used to record all words that exist in training set.

in header __bp_network.h__
```C++
struct neuron;
double tanh(double x);
double difftanh(double x);
double sigmoid(double x);
double diffsigmoid(double x);
class word2vec;
```
word2vec will help you __calculate each word's embedding vector__ and __output information on the screen and into the file__ .

default input file's name is __"trainingset.txt"__ ,you can change it if you like in __word_embedding.cpp__ .

# the most important thing
If you want to change the length of vectors,please go to __bp_network.h__ and find this function: __void word2vec::initializing()__

then change the __HNUM__ , this parameter decides the length of vectors.
