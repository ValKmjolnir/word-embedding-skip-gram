#include "nlp.h"

void word_lib_initializing()
{
	/*
		at the constructor of class wordlib
		word.bs file will input all words that have been found into wordlib
		this function is used to check if there is an update of words
	*/
	std::string filename="trainingset.txt";
	lib.add_word_from_file(filename);
	return;
}
void word2vec_main()
{
	// input training set
	std::string filename="trainingset.txt";
	/*
		initializing weights and bias
		in this process,neural network will get the number of words
		and the input[] and expect[],output[] share the same size
		the size is the number of words
		but the hidden layer uses a special size, and this size is usually smaller than the number of words
	*/
	w2v_neural_network.initializing();
	
	int cnt=0;
	double error=1e7;
	double last_error=1e8;
	// abs(last_error-error) is used to check if the training process is about to end
	while(std::abs(last_error-error)>0.001)
	{
		++cnt;
		last_error=error;
		error=w2v_neural_network.main_work(filename);
		if(!(cnt%10))
		{
			std::cout<<"epoch "<<cnt<<"| loss "<<std::abs(last_error-error)<<std::endl;
			w2v_neural_network.print_vector();
		}
	}
	
	// this function is used to output all vectors of each word into a file
	w2v_neural_network.file_out_vector();
	return;
}
int main()
{
	word_lib_initializing();
	word2vec_main();
	return 0;
}
