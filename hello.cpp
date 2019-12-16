#include <iostream>
#include <thread>
#include <vector>
void run()
{
	std::cout<<"hello world"<<std::endl;
}
void fun()
{
    
}
int main()
{
	std::thread t1(run);
	t1.join();
	return 0;
}
