#pragma once

#include <cstring>
#include <sstream>

struct neuron {
	double in;
	double out;
	double *w;
	double bia;
	double diff;
};

double tanh(double x)
{
	x=std::exp(x);
	x*=x;
	return (x-1)/(x+1);
}

double difftanh(double x)
{
	x=tanh(x);
	return 1-x*x;
}

double sigmoid(double x)
{
	return 1.0/(1+std::exp(-x));
}

double diffsigmoid(double x)
{
	x=1.0/(1+std::exp(-x));
	return x*(1-x);
}

class word2vec {
private:
	int* array;
	// array[] gets sequence of words' place
	std::list<int> wordlist;
	// wordlist is used to get all words' place in a sequence and transport all members into array[]
	
	/*
		INUM: number of input neurons
		HNUM: number of hidden layer neurons
		ONUM: number of output layer neurons
		*expect: expect outputs
	*/
	int INUM;
	int HNUM;
	int ONUM;
	double* input;
	neuron* hidden;
	neuron* output;
	double* expect;

public:
	word2vec();
	~word2vec();
	void initializing();
	double main_work(std::string);
	double pre_calc();
	double calc(int,int);
	// calc(int,int) is the main calculation process,
	// including forward and backward propagation and weight-bia-updating process
	void print_vector();
	void file_out_vector();
};

word2vec::word2vec()
{
	array=NULL;
	input=NULL;
	hidden=NULL;
	output=NULL;
	expect=NULL;
	INUM=HNUM=ONUM=0;
	return;
}
word2vec::~word2vec()
{
	if(array)
		delete []array;
	if(input)
		delete []input;
	if(hidden)
	{
		for(int i=0;i<HNUM;++i)
			delete []hidden[i].w;
		delete []hidden;
	}
	if(output)
	{
		for(int i=0;i<ONUM;++i)
			delete []output[i].w;
		delete []output;
	}
	if(expect)
		delete []expect;
	return;
}
void word2vec::initializing()
{
	INUM=ONUM=lib.lib_size();
	// get the total number of words
	// and the input is one-hot
	
	HNUM=64;// test size of hidden layer
	
	if(!INUM)
	{
		std::cout<<">>[Error] null word lib."<<std::endl;
		return;// if INUM is 0 then the process will not begin
	}
	
	std::srand(unsigned(time(NULL)));
	
	input=new double[INUM];
	expect=new double[ONUM];
	hidden=new neuron[HNUM];
	output=new neuron[ONUM];
	
	for(int i=0;i<HNUM;++i)
	{
		hidden[i].bia=(std::rand()%2? 1:-1)*(std::rand()%10+1.0)/(INUM*5.0);
		hidden[i].w=new double[INUM];
		for(int j=0;j<INUM;++j)
			hidden[i].w[j]=(std::rand()%2? 1:-1)*(std::rand()%10+1.0)/(INUM*10.0);
	}
	for(int i=0;i<ONUM;++i)
	{
		output[i].bia=(std::rand()%2? 1:-1)*(std::rand()%10+1.0)/(HNUM*5.0);
		output[i].w=new double[HNUM];
		for(int j=0;j<HNUM;++j)
			output[i].w[j]=(std::rand()%2? 1:-1)*(std::rand()%10+1.0)/(HNUM*10.0);
	}
	return;
}
double word2vec::main_work(std::string filename)
{
	double error=0;
	
	std::ifstream fin(filename);
	if(fin.fail())
	{
		std::cout<<">>[Error] cannot open file \'"<<filename<<"\' ."<<std::endl;
		fin.close();
		return 0;
	}
	std::string tmp,str;
	while(!fin.eof())
	{
		// input sequence of char and generate wordlist(std::list<int>)
		wordlist.clear();
		std::getline(fin,str);
		if(fin.eof())
			break;
		tmp="";
		for(int i=0;i<str.length();++i)
		{
			if(('a'<=str[i] && str[i]<='z') || ('A'<=str[i] && str[i]<='Z') || str[i]=='\'')
			{
				if('a'<=str[i] && str[i]<='z' || str[i]=='\'')
					tmp+=str[i];
				else if('A'<=str[i] && str[i]<='Z')
					tmp+=str[i]-'A'+'a';
			}
			else
			{
				if(lib.search(tmp))
					wordlist.push_back(lib.get_place(tmp));
				tmp="";
			}
		}
		if(tmp.length() && lib.search(tmp))
			wordlist.push_back(lib.get_place(tmp));
		// calculate this sequence
		error+=pre_calc();
	}
	fin.close();
	return error;
}
double word2vec::pre_calc()
{
	// generate word sequence: array[]
	// transport all member of wordlist into array[]
	int cnt=0;
	for(std::list<int>::iterator i=wordlist.begin();i!=wordlist.end();++i)
		++cnt;
	if(cnt>0)
	{
		if(array)
		{
			delete []array;
			array=new int[cnt];
		}
		else
			array=new int[cnt];
		int j=0;
		for(std::list<int>::iterator i=wordlist.begin();i!=wordlist.end();++i,++j)
			array[j]=*i;
	}
	else
		return 0;
	
	// main calculation,window size is 2
	double error=0;
	for(int i=0;i<cnt;++i)
	{
		if(i-2>=0)
			error+=calc(array[i],array[i-2]);
		if(i-1>=0)
			error+=calc(array[i],array[i-1]);
		error+=calc(array[i],array[i]);
		if(i+1<cnt)
			error+=calc(array[i],array[i+1]);
		if(i+2<cnt)
			error+=calc(array[i],array[i+2]);
	}
	return error;
}
double word2vec::calc(int in_place,int out_place)
{
	double error=0;
	
	for(int i=0;i<INUM;++i)
		input[i]=0;
	input[in_place]=1;
	for(int i=0;i<ONUM;++i)
		expect[i]=0;
	expect[out_place]=1;
	
	// forward propagation
	for(int i=0;i<HNUM;++i)
	{
		hidden[i].in=hidden[i].bia;
		for(int j=0;j<INUM;++j)
			hidden[i].in+=hidden[i].w[j]*input[j];
		hidden[i].out=tanh(hidden[i].in);
	}
	double softmax=0;
	for(int i=0;i<ONUM;++i)
	{
		output[i].in=output[i].bia;
		for(int j=0;j<HNUM;++j)
			output[i].in+=output[i].w[j]*hidden[j].out;
		softmax+=std::exp(output[i].in);
	}
	for(int i=0;i<ONUM;++i)
	{
		output[i].out=std::exp(output[i].in)/softmax;
		error+=0.5*(expect[i]-output[i].out)*(expect[i]-output[i].out);
	}
	
	// backward propagation
	for(int i=0;i<ONUM;++i)
		output[i].diff=(expect[i]-output[i].out)*difftanh(output[i].in);
	for(int i=0;i<HNUM;++i)
	{
		hidden[i].diff=0;
		for(int j=0;j<ONUM;++j)
			hidden[i].diff+=output[j].w[i]*output[j].diff;
		hidden[i].diff*=difftanh(hidden[i].in);
	}
	
	// update weights and bias
	double lr=0.03;
	for(int i=0;i<HNUM;++i)
	{
		hidden[i].bia+=lr*hidden[i].diff;
		for(int j=0;j<INUM;++j)
			hidden[i].w[j]+=lr*input[j]*hidden[i].diff;
	}
	for(int i=0;i<ONUM;++i)
	{
		output[i].bia+=lr*output[i].diff;
		for(int j=0;j<HNUM;++j)
			output[i].w[j]+=lr*hidden[j].out*output[i].diff;
	}
	
	return error;
}
void word2vec::print_vector()
{
	int cnt=lib.lib_size();
	for(int i=0;i<cnt;++i)
	{
		lib.print_word(i);
		std::cout<<": ";
		for(int j=0;j<HNUM/8;++j)
			std::cout<<hidden[j].w[i]<<" ";
		std::cout<<std::endl;
	}
	return;
}
void word2vec::file_out_vector()
{
	// be aware of the filename "data.txt"
	// if you have any file named the same
	// the file will be re-written and you will lost the data in it
	std::ofstream fout("data.txt");
	int cnt=lib.lib_size();
	for(int i=0;i<cnt;++i)
	{
		fout<<lib.get_word(i);
		fout<<": ";
		for(int j=0;j<HNUM;++j)
			fout<<hidden[j].w[i]<<" ";
		fout<<std::endl;
	}
	fout.close();
	return;
}

word2vec w2v_neural_network;
