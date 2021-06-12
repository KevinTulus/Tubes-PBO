#include <iostream>
using namespace std;


int main(){
	int a, b;
	try{
		cin >> a >> b;
		if(b == 0) throw 1;
		cout << a << " / " << b << " = " << a / b << endl;
	}	catch (int e){
		cout << "Hasil tidak dapat didefinisi" << endl;
	}

    int c, d;
    cin >> c >> d;
    if ( d == 0 )
    {
        cout << "Hasil tidak dapat didefinisi" << endl;
    } else
    {
        cout << c << " / " << d << " = " << c / d << endl;
    }
    return 0;
}