#ifndef __WORD_LIB_H__
#define __WORD_LIB_H__

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

wordlib::wordlib()
{
	dictionary.clear();
	std::ifstream fin("word.bs");
	if(fin.fail())
	{
		fin.close();
		return;
	}
	std::string tmp;
	while(!fin.eof())
	{
		fin>>tmp;
		if(fin.eof())
			break;
		if(tmp.length())
			dictionary.push_back(tmp);
	}
	fin.close();
	return;
}
wordlib::~wordlib()
{
	std::ofstream fout("word.bs");
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
		fout<<*i<<std::endl;
	fout.close();
	return;
}
int wordlib::get_place(std::string str)
{
	int cnt=0;
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i,++cnt)
		if(*i==str)
			return cnt;
	return -1;
}
int wordlib::lib_size()
{
	int cnt=0;
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
		++cnt;
	return cnt;
}
bool wordlib::search(std::string str)
{
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
		if(*i==str)
			return true;
	return false;
}
void wordlib::add_word(std::string str)
{
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
		if(*i==str)
			return;
	dictionary.push_back(str);
	return;
}
void wordlib::add_word_from_file(std::string filename)
{
	std::ifstream fin(filename);
	if(fin.fail())
	{
		std::cout<<">>[Error] cannot find file \'"<<filename<<"\' ."<<std::endl;
		fin.close();
		return;
	}
	std::string tmp;
	std::string str;
	while(!fin.eof())
	{
		fin>>str;
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
				if(tmp.length())
					add_word(tmp);
				tmp="";
			}
		}
		if(tmp.length())
			add_word(tmp);
	}
	fin.close();
	return;
}
void wordlib::print_lib()
{
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
		std::cout<<*i<<"\t";
	return;
}
void wordlib::print_word(int place)
{
	int cnt=0;
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
	{
		if(cnt==place)
		{
			std::cout<<*i<<" ";
			break;
		}
		++cnt;
	}
	return;
}
std::string wordlib::get_word(int place)
{
	int cnt=0;
	for(std::list<std::string>::iterator i=dictionary.begin();i!=dictionary.end();++i)
	{
		if(cnt==place)
			return *i;
		++cnt;
	}
	return " ";
}

wordlib lib;

#endif
