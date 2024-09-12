#include <iostream>

using namespace std;

//定义类
class Test
{
	public:
		void  function1(/* args */) {
			/* code */
			printf("%s\n", "hello word");
		}
		inline Test* get_this(){return this;} 
		inline void print_data(){
			printf("%d\n", data_);
		}
	private:
		int data_=0;

};


int main(int argc, char const *argv[]) {
	/* code */

	// Test my_test;
	// my_test.function1();
	// cout<<&my_test<<endl;
	// cout<<my_test.get_this()<<endl;

	Test* my_test2=nullptr;
	cout<<(*my_test2).get_this()<<endl;
	//(*my_test2).print_data();//报错不存在
	return 0;
} 