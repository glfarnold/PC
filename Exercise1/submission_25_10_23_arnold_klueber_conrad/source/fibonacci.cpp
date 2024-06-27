#include <iostream>

int fibonacci(int n){
    if(n<=1)
        return n;
    else
        return fibonacci(n-1) + fibonacci(n-2);
}

int main(){
    int n;
    std::cin >> n;

    for(int i=0; i<n-1; i++)
        std::cout << fibonacci(i) << ", ";
    std::cout << fibonacci(n-1) << "\n";

    return 0;
}

