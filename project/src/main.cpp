#include <iostream>
#include "matrix.h"

int main() {
    prep::Matrix mtr(2, 2);
    mtr(0, 0) = 1;
    mtr(0, 1) = 2;
    mtr(1, 0) = 3;
    mtr(1, 1) = 4;
    std::cout << mtr << std::endl;
    std::cout << mtr.transp() << std::endl;
    std::cout << "1";
    std::cout << mtr.det() << std::endl;
    std::cout << "2";
    std::cout << mtr.adj() << std::endl;
    std::cout << mtr.inv() << std::endl;
    return 0;
}
