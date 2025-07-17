#include <assert.h>
#include <vector>

#include "linq.hpp"

int main() {
    int xs[] = {1, 2, 3, 4, 5};

    std::vector<int> res =
        linq::from(xs, xs + 5)                    // Взять элементы xs
            .select([](int x) { return x * x; })  // Возвести в квадрат
            .where_neq(25)  // Оставить только значения != 25
            .where([](int x) { return x > 3; })  // Оставить только значения > 3
            .drop(2)       // Убрать два элемента из начала
            .to_vector();  // Преобразовать результат в вектор

    std::vector<int> expected = {16};
    assert(res == expected);

    return 0;
}
