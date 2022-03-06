#include <iostream>

#include <fstream>
#include <istream>

#include <algorithm>

#include <string.h>
#include<sstream>
#include <vector>

#include <math.h>
#include <time.h>

using namespace std;

string TMP_FILE_A	= "file_a.txt";
string TMP_FILE_B	= "file_b.txt";
string SOURCE_FILE	= "data.txt";
string OUT_FILE	= "out.txt";

class Fibonacci
{
	private:
		const int x0 = 0;
		const int x1 = 1;

		long long cached_fibbonacci = x0;
	public:
		bool is_fibonacci(int number){
			if (number == cached_fibbonacci)
				return true;
			long double root5 = sqrt(5);
			long double phi = (1 + root5) / 2;

			long idx = floor(log(number * root5) / log(phi) + 0.5);
			long u = floor(pow(phi, idx) / root5 + 0.5);

			return (u == number);
		}
		long long get_by_num(int number_fibonacci){
			long int f0 = x0;
			long int f1 = x1;

			for(long int i = 0 ; i < number_fibonacci; i++){
				long int _tmp = f0;
				f0 = f1; f1 += _tmp;
			}
			cached_fibbonacci = f1;
			return f1;
		}
		vector <long int> previous_number(long int number){
			vector <long int> fib_num = {x0, x1};

			while(fib_num[0]+fib_num[1] < number){
					long int _tmp = fib_num[0];
					fib_num[0] = fib_num[1]; fib_num[1] += _tmp;
				}

			return fib_num;
		}
};

class CustomError{
	public:
		void FileCannotBeOpen(){
			cout << "The file cannot be opened or is missing" << endl;
			_Exit(1);
		}
};

class PolyPhaseMerge{
	private:

		CustomError error;
		Fibonacci fib;

	public:
		void sort_file(string file_name = SOURCE_FILE){
			//Selection method
			ifstream file(file_name);
			if(!file.is_open()){
				file.close();
				error.FileCannotBeOpen();
			}
			vector <int> content;
			int tmp;
			while(file >> tmp){
				content.push_back(tmp);
			}

			for(int startIndex = 0; startIndex < content.size() - 1; startIndex++){

				int smallesrIndex = startIndex;

				for(int currentIndex = startIndex + 1; currentIndex < content.size(); currentIndex++){
					if(content[currentIndex] < content[smallesrIndex])
						smallesrIndex = currentIndex;
				}

				swap(content[startIndex], content[smallesrIndex]);
			}
			file.clear();
			file.close();

			ofstream _file(file_name);
			if(!_file.is_open()){
				_file.close();
				error.FileCannotBeOpen();
			}
			for(int i = 0; i < content.size(); i++)
				_file << content[i] << " ";

			_file.close();
		}

		void sort(string file_name		= SOURCE_FILE,
				string tmp_file_a_name	= TMP_FILE_A,
				string tmp_file_b_name	= TMP_FILE_B,
				string out_file_name	= OUT_FILE){


			vector <int> content;

			ifstream _file(file_name);

			if(!_file.is_open()){
				_file.close();
				error.FileCannotBeOpen();
			}

			int _tmp;
			int file_lenght = 0;

			//Counting the number of numbers in a file
			while(_file >> _tmp)
				++file_lenght;
			_file.close();

			ifstream file(file_name);

			if (!file.is_open()){
				file.close();
				error.FileCannotBeOpen();
			}

			vector <long int> fibonacci_num = fib.previous_number(file_lenght);

			ofstream file_a(tmp_file_a_name);
			if(!file_a.is_open()){
				file_a.close();
				error.FileCannotBeOpen();
			}
			file_a.clear();

			ofstream file_b(tmp_file_b_name);
			if(!file_b.is_open()){
				file_b.close();
				error.FileCannotBeOpen();
			}
			file_b.clear();

			//Split the file into 2 temporary
			for(int i = 0; i < file_lenght; i++){
				file >> _tmp;
				if(i < fibonacci_num[0])
					file_a << _tmp << " ";
				else if(i < fibonacci_num[0]+fibonacci_num[1])
					file_b << _tmp << " ";
			}

			file_a.close();
			file_b.close();

			file.close();

			sort_file(tmp_file_a_name);
			sort_file(tmp_file_b_name);

			//Merge temporary files into the destination
			ofstream out_file(out_file_name);
			ifstream _file_a(tmp_file_a_name);
			ifstream _file_b(tmp_file_b_name);

			if(!out_file.is_open() || !_file_a.is_open() || !_file_b.is_open()){
				out_file.close();
				error.FileCannotBeOpen();
			}
			out_file.clear();

			int tmp_a;
			int tmp_b;

			bool tmp_a_is_write = true;
			bool tmp_b_is_write = true;

			for(int i = 0; i < file_lenght; i++){
				if(!_file_a.eof() && tmp_a_is_write){
					_file_a >> tmp_a;
					tmp_a_is_write = false;
				}
				if(!_file_b.eof() && tmp_b_is_write){
					_file_b >> tmp_b;
					tmp_b_is_write = false;
				}
				if(!_file_a.eof() && tmp_a < tmp_b && !tmp_a_is_write){
					out_file << tmp_a << " ";
					tmp_a_is_write = true;
				}else if(!_file_b.eof() && tmp_b < tmp_a){
					out_file << tmp_b << " ";
					tmp_b_is_write = true;
				}else{
					out_file << tmp_b << " ";
					tmp_b_is_write = true;
				}
			}

			_file_a.close();
			_file_b.close();
			out_file.close();
		}

		void gen_file(string file_name = SOURCE_FILE, long int lenght = 21, int magnitude = 50){

			if(!fib.is_fibonacci(lenght)){
				cout << "The collection of values in a file is not a multiple of the Fibonacci number" << endl;
				_Exit(1);
			}

			ofstream file(file_name, ios :: out);

			if(!file.is_open()){
				file.close();
				error.FileCannotBeOpen();
			}

			file.clear();

			for(long int i = 0; i < lenght; i++)
				file << to_string(int(rand()%magnitude)) << " ";

			file.close();
		}

		void test(){
			gen_file();
			sort();
		}
};

bool question(string answer){
	if(answer == "Y" || answer == "Yes" || answer == "yes" || answer == "y")
		return true;
	else
		return false;
}

int main()
{
	srand(time(NULL));

	cout << "Hello! It's Poly Phase Merge Sort Programm! Let's go to the configuration!" << endl;

	PolyPhaseMerge ppm;
	string do_sort;
	cout << "Do you want to create a randomly generated file for sorting?[Yes(Y)]: ";
	cin >> do_sort;
	string out_file = OUT_FILE;
	string source_file = SOURCE_FILE;

	string do_create_source_file;
	string tmp;

	long int lenght;
	long int magnitude;

	//Если пользователь хочет сгенерировать файл для сортировки
	if(question(do_sort)){
		cout << "Want to change the name of the original file?[Yes(Y)]: ";
		cin >> do_create_source_file; cout << endl;
		//Если пользователь хочет поменять имя файла
		if(question(do_create_source_file)){
			cout << "Enter source file name: ";
			cin >> source_file; cout << endl;
		}
		cout << "Enter file lenght (fibonacci number): ";
		cin >> lenght;
		cout << "\nEnter magnitude: ";
		cin >> magnitude; cout << endl;
		ppm.gen_file(source_file, lenght, magnitude);
	}else{
		cout << "Compress and fill in file\nWhen you're done, enter any character" << endl;
		cin >> tmp;
		cout << "Enter source file name: ";
		cin >> source_file; cout << endl;
	}
	//Если пользователь хочет изменить имя выходного файла
	cout << "Want to change the name of the output file?[Yes(Y)]: ";
	cin >> tmp; cout << endl;
	if(question(tmp)){
		cout << "Enter out file name: ";
		cin >> out_file; cout << endl;
	}

	ppm.sort(source_file, TMP_FILE_A, TMP_FILE_B, out_file);

	cout << "Sorting is complete!" << endl;
	cout << "Your out file: " << out_file << endl;

	return 0;
}
