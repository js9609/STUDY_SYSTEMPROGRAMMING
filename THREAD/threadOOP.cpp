#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class tClass{
	
	private:
		pthread_t pt;
		int* a;
		void prn(void){
			cout << "This is private Method " << *a << endl;
		}
		void tFunc(void){
			*a = 1;
			prn();
			while(1){
				cout << "This is thread method " << (*a)++ << endl;
				sleep(1);
			}
		}
		static void* callThread(void *arg);

	public:
		tClass(void){
			cout << "This is constructor" << endl;
			a = new int;
			pthread_create(&pt, NULL, callThread, this); // 클래스 객체 this를 넘겨 받음으로써, 객체에 있는 멤버 변수 사용가능하도록 함.. 와;;이런 생각을..
		}
		~tClass(void){
			int i;
			for(i = 0; i < 3; i++){
				cout << "This is destructor" << endl;
				sleep(1);
			}
			delete a;
		}


};

//Static이므로 밖에서 정의해야함
void* tClass::callThread(void *arg){
	tClass* tp;
	tp = (tClass*) arg;
	tp->tFunc();
}
int main(){
	int i;
	tClass tc;
	for(i = 0; i< 3; i++){
		cout << "This is main " << endl;
		sleep(1);
	}
	cout << "main exit" << endl;
	return (1);
}
