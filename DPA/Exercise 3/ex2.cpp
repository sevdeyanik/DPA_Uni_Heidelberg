#include <iostream>

void compareAndSwap(char& a, char& b) {
    if (a > b) {
        char temp = a;
        a = b;
        b = temp;
    }
}

void fastInefficientRanking(char arr[4][4]) {
    // Perform fast inefficient ranking on the array
    for (int pass = 0; pass < 4; pass++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                compareAndSwap(arr[i][j], arr[i][j+1]);
            }
        }
    }
}

int main() {
    char arr[4][4] = {
        {'I', 'H', 'Y', 'P'},
        {'S', 'B', 'R', 'P'},
        {'X', 'T', 'W', 'J'},
        {'Q', 'D', 'A', 'B'}
    };
    
    // Print the initial array
    std::cout << "Initial Array:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    
    // Perform fast inefficient ranking on the array
    std::cout << "Step-by-Step Process:" << std::endl;
    fastInefficientRanking(arr);
    
    // Print the sorted array
    std::cout << std::endl << "Sorted Array:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
