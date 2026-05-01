#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "template_matrix.h"

TEST_CASE("Matrix - Конструкторы и основные операции")
{
    using MatrixInt = TowerDefence::Matrix<int>;

    SECTION("Конструкторы") {
        SECTION("Конструктор по умолчанию") {
            MatrixInt m;
            REQUIRE(m.empty());
            REQUIRE(m.rows() == 0);
            REQUIRE(m.cols() == 0);
            REQUIRE(m.size() == 0);
        }

        SECTION("Конструктор с размерами") {
            MatrixInt m(3, 4);
            REQUIRE(m.rows() == 3);
            REQUIRE(m.cols() == 4);
            REQUIRE(m.size() == 12);
            REQUIRE_FALSE(m.empty());

            for (size_t i = 0; i < m.rows(); ++i) {
                for (size_t j = 0; j < m.cols(); ++j) {
                    REQUIRE(m[i][j] == 0);
                }
            }
        }

        SECTION("Конструктор с размерами и значением") {
            MatrixInt m(2, 3, 42);
            REQUIRE(m.rows() == 2);
            REQUIRE(m.cols() == 3);

            for (size_t i = 0; i < m.rows(); ++i) {
                for (size_t j = 0; j < m.cols(); ++j) {
                    REQUIRE(m[i][j] == 42);
                }
            }
        }

        SECTION("Конструктор копирования") {
            MatrixInt m1(2, 3, 5);
            MatrixInt m2{m1};

            REQUIRE(m1.rows() == m2.rows());
            REQUIRE(m1.cols() == m2.cols());

            m1[0][0] = 10;
            REQUIRE(m2[0][0] == 5);

            REQUIRE(m1.data() != m2.data());
        }

        SECTION("Конструктор перемещения") {
            MatrixInt m1(2, 3, 7);
            int* old_data = m1.data();
            size_t old_rows = m1.rows();
            size_t old_cols = m1.cols();

            MatrixInt m2(std::move(m1));

            REQUIRE(m2.data() == old_data);
            REQUIRE(m2.rows() == old_rows);
            REQUIRE(m2.cols() == old_cols);
            REQUIRE(m1.data() == nullptr);
            REQUIRE(m1.empty());
        }
    }

    SECTION("Операторы присваивания") {
        SECTION("Копирующее присваивание") {
            MatrixInt m1(2, 3, 8);
            MatrixInt m2;

            m2 = m1;

            REQUIRE(m1.rows() == m2.rows());
            REQUIRE(m1.cols() == m2.cols());

            m1[0][0] = 20;
            REQUIRE(m2[0][0] == 8);

            m1 = m1;
            REQUIRE(m1.rows() == 2);
            REQUIRE(m1.cols() == 3);
        }

        SECTION("Перемещающее присваивание") {
            MatrixInt m1(3, 4, 9);
            MatrixInt m2;

            int* old_data = m1.data();
            size_t old_rows = m1.rows();
            size_t old_cols = m1.cols();

            m2 = std::move(m1);

            REQUIRE(m2.data() == old_data);
            REQUIRE(m2.rows() == old_rows);
            REQUIRE(m2.cols() == old_cols);
            REQUIRE(m1.data() == nullptr);

            m2 = std::move(m2);
            REQUIRE(m2.rows() == old_rows);
        }
    }

    SECTION("Доступ к элементам") {
        MatrixInt m(3, 4);

        for (size_t i = 0; i < m.rows(); ++i) {
            for (size_t j = 0; j < m.cols(); ++j) {
                m[i][j] = static_cast<int>(i * m.cols() + j);
            }
        }

        SECTION("operator[]") {
            REQUIRE(m[0][0] == 0);
            REQUIRE(m[0][1] == 1);
            REQUIRE(m[1][0] == 4);
            REQUIRE(m[2][3] == 11);

            m[1][2] = 99;
            REQUIRE(m[1][2] == 99);
        }

        SECTION("at()") {
            REQUIRE(m.at(0, 0) == 0);
            REQUIRE(m.at(2, 3) == 11);

            m.at(1, 1) = 50;
            REQUIRE(m.at(1, 1) == 50);

            REQUIRE_THROWS(m.at(3, 0));
            REQUIRE_THROWS(m.at(0, 4));
            REQUIRE_THROWS(m.at(3, 4));

            const MatrixInt& cm = m;
            REQUIRE(cm.at(0, 0) == 0);
            REQUIRE_THROWS(cm.at(3, 0));
        }

        SECTION("front(), back(), data()") {
            REQUIRE(m.front() == 0);
            REQUIRE(m.back() == 11);

            m.front() = 100;
            m.back() = 200;

            REQUIRE(m[0][0] == 100);
            REQUIRE(m[2][3] == 200);

            REQUIRE(m.data() == &m[0][0]);

            const MatrixInt& cm = m;
            REQUIRE(cm.data() == &cm[0][0]);
        }
    }

    SECTION("Итераторы") {
        MatrixInt m(2, 3);
        for (int i = 0; i < 6; ++i) {
            m.data()[i] = i;
        }

        SECTION("begin/end") {
            auto it = m.begin();
            REQUIRE(*it == 0);
            ++it;
            REQUIRE(*it == 1);

            int sum = 0;
            for (auto val : m) {
                sum += val;
            }
            REQUIRE(sum == 15);

            const MatrixInt& cm = m;
            int csum = 0;
            for (auto it = cm.cbegin(); it != cm.cend(); ++it) {
                csum += *it;
            }
            REQUIRE(csum == 15);
        }

        SECTION("reverse iterators") {
            std::vector<int> reversed;
            for (auto it = m.rbegin(); it != m.rend(); ++it) {
                reversed.push_back(*it);
            }

            std::vector<int> expected = {5, 4, 3, 2, 1, 0};
            REQUIRE(reversed == expected);

            const MatrixInt& cm = m;
            std::vector<int> creversed;
            for (auto it = cm.crbegin(); it != cm.crend(); ++it) {
                creversed.push_back(*it);
            }
            REQUIRE(creversed == expected);
        }

        SECTION("Свойства итераторов") {
            REQUIRE(std::contiguous_iterator<MatrixInt::iterator>);
            REQUIRE(std::contiguous_iterator<MatrixInt::const_iterator>);
            REQUIRE(std::ranges::contiguous_range<MatrixInt>);
            REQUIRE(std::ranges::random_access_range<MatrixInt>);
            REQUIRE(std::ranges::sized_range<MatrixInt>);
            REQUIRE(std::ranges::common_range<MatrixInt>);
        }
    }

    SECTION("Модификация матрицы") {
        SECTION("clear()") {
            MatrixInt m(3, 4, 10);
            REQUIRE_FALSE(m.empty());

            m.clear();
            REQUIRE(m.empty());
            REQUIRE(m.rows() == 0);
            REQUIRE(m.cols() == 0);
        }

        SECTION("swap()") {
            MatrixInt m1(2, 3, 1);
            MatrixInt m2(4, 5, 2);

            int* m1_data = m1.data();
            int* m2_data = m2.data();
            size_t m1_rows = m1.rows();
            size_t m2_rows = m2.rows();

            m1.swap(m2);

            REQUIRE(m1.data() == m2_data);
            REQUIRE(m2.data() == m1_data);
            REQUIRE(m1.rows() == m2_rows);
            REQUIRE(m2.rows() == m1_rows);

            REQUIRE(m1[0][0] == 2);
            REQUIRE(m2[0][0] == 1);
        }

        SECTION("assign() с размерами и значением") {
            MatrixInt m;
            m.assign(3, 4, 42);

            REQUIRE(m.rows() == 3);
            REQUIRE(m.cols() == 4);

            for (size_t i = 0; i < m.rows(); ++i) {
                for (size_t j = 0; j < m.cols(); ++j) {
                    REQUIRE(m[i][j] == 42);
                }
            }

            m.assign(2, 2);
            REQUIRE(m.rows() == 2);
            REQUIRE(m.cols() == 2);
            REQUIRE(m[0][0] == 0);
        }

        SECTION("assign() с итераторами") {
            std::vector<int> vec = {1, 2, 3, 4, 5, 6};
            MatrixInt m;

            m.assign(2, 3, vec.begin(), vec.end());

            REQUIRE(m.rows() == 2);
            REQUIRE(m.cols() == 3);
            REQUIRE(m[0][0] == 1);
            REQUIRE(m[0][1] == 2);
            REQUIRE(m[0][2] == 3);
            REQUIRE(m[1][0] == 4);
            REQUIRE(m[1][1] == 5);
            REQUIRE(m[1][2] == 6);

            std::vector<int> big_vec(10, 1);
            REQUIRE_THROWS(m.assign(2, 3, big_vec.begin(), big_vec.end()));
        }

        SECTION("assign() со списком инициализации") {
            MatrixInt m;

            m.assign({{1, 2, 3},
                      {4, 5, 6},
                      {7, 8, 9}});

            REQUIRE(m.rows() == 3);
            REQUIRE(m.cols() == 3);

            REQUIRE(m[0][0] == 1);
            REQUIRE(m[0][1] == 2);
            REQUIRE(m[0][2] == 3);
            REQUIRE(m[1][0] == 4);
            REQUIRE(m[1][1] == 5);
            REQUIRE(m[1][2] == 6);
            REQUIRE(m[2][0] == 7);
            REQUIRE(m[2][1] == 8);
            REQUIRE(m[2][2] == 9);

            REQUIRE_THROWS(m.assign({{1, 2},
                                     {3, 4, 5}}));

            m.assign({});
            REQUIRE(m.empty());
        }
    }

    SECTION("Вставка элементов") {
        MatrixInt m(2, 2);
        m[0][0] = 1; m[0][1] = 2;
        m[1][0] = 3; m[1][1] = 4;

        SECTION("insert() с одним значением") {
            auto it = m.insert(m.cbegin() + 1, 99);
            REQUIRE(*it == 99);
            REQUIRE(m[0][1] == 99);
        }

        SECTION("insert() со списком инициализации") {
            auto it = m.insert(m.cbegin() + 1, {10, 20});

            REQUIRE(m.rows() == 2);
            REQUIRE(m.cols() == 2);
            REQUIRE(m[0][0] == 1);
            REQUIRE(m[0][1] == 10);
            REQUIRE(m[1][0] == 20);
            REQUIRE(m[1][1] == 4);
        }

        SECTION("emplace()") {
            auto it = m.emplace(m.cbegin() + 3, 50);
            REQUIRE(*it == 50);
            REQUIRE(m[1][1] == 50);
        }
    }

    SECTION("Добавление строк и столбцов") {
        SECTION("push_back_row() и insert_row()") {
            MatrixInt m(2, 3);
            m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
            m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;

            m.push_back_row(99);
            REQUIRE(m.rows() == 3);
            REQUIRE(m.cols() == 3);

            REQUIRE(m[2][0] == 99);
            REQUIRE(m[2][1] == 99);
            REQUIRE(m[2][2] == 99);

            m.insert_row(1, 77);
            REQUIRE(m.rows() == 4);

            REQUIRE(m[0][0] == 1);
            REQUIRE(m[1][0] == 77);
            REQUIRE(m[2][0] == 4);
            REQUIRE(m[3][0] == 99);

            REQUIRE_THROWS(m.insert_row(10, 0));

            MatrixInt empty;
            empty.insert_row(0, 5);
            REQUIRE(empty.rows() == 1);
            REQUIRE(empty.cols() == 1);
            REQUIRE(empty[0][0] == 5);
        }

        SECTION("push_back_col() и insert_col()") {
            MatrixInt m(3, 2);
            m[0][0] = 1; m[0][1] = 2;
            m[1][0] = 3; m[1][1] = 4;
            m[2][0] = 5; m[2][1] = 6;


            m.push_back_col(88);
            REQUIRE(m.rows() == 3);
            REQUIRE(m.cols() == 3);

            REQUIRE(m[0][2] == 88);
            REQUIRE(m[1][2] == 88);
            REQUIRE(m[2][2] == 88);


            m.insert_col(1, 55);
            REQUIRE(m.cols() == 4);

            REQUIRE(m[0][0] == 1);
            REQUIRE(m[0][1] == 55);
            REQUIRE(m[0][2] == 2);
            REQUIRE(m[0][3] == 88);

            REQUIRE(m[2][0] == 5);
            REQUIRE(m[2][1] == 55);
            REQUIRE(m[2][2] == 6);
            REQUIRE(m[2][3] == 88);


            REQUIRE_THROWS(m.insert_col(10, 0));

            MatrixInt empty;
            REQUIRE_NOTHROW(empty.insert_col(0, 5));
            REQUIRE(empty.cols() == 1);
            REQUIRE(empty.rows() == 1);
            REQUIRE(empty[0][0] == 5);

        }

        SECTION("erase_row()") {
            MatrixInt m(3, 3);
            for (int i = 0; i < 9; ++i)
                m.data()[i] = i + 1;

            m.erase_row(1);
            REQUIRE(m.rows() == 2);
            REQUIRE(m.cols() == 3);

            REQUIRE(m[0][0] == 1);
            REQUIRE(m[0][1] == 2);
            REQUIRE(m[0][2] == 3);
            REQUIRE(m[1][0] == 7);
            REQUIRE(m[1][1] == 8);
            REQUIRE(m[1][2] == 9);

            m.erase_row(0);
            REQUIRE(m.rows() == 1);
            REQUIRE(m[0][0] == 7);

            m.erase_row(0);
            REQUIRE(m.empty());

            m.assign(2, 2, 1);
            REQUIRE_THROWS(m.erase_row(2));
        }

        SECTION("erase_col()") {
            MatrixInt m(3, 3);
            for (int i = 0; i < 9; ++i) m.data()[i] = i + 1;

            m.erase_col(1);
            REQUIRE(m.rows() == 3);
            REQUIRE(m.cols() == 2);

            REQUIRE(m[0][0] == 1);
            REQUIRE(m[0][1] == 3);
            REQUIRE(m[1][0] == 4);
            REQUIRE(m[1][1] == 6);
            REQUIRE(m[2][0] == 7);
            REQUIRE(m[2][1] == 9);

            m.erase_col(0);
            REQUIRE(m.cols() == 1);
            REQUIRE(m[0][0] == 3);
            REQUIRE(m[1][0] == 6);
            REQUIRE(m[2][0] == 9);

            m.erase_col(0);
            REQUIRE(m.empty());

            m.assign(2, 2, 1);
            REQUIRE_THROWS(m.erase_col(2));
        }
    }

    SECTION("Диапазоны строк и столбцов") {
        MatrixInt m(3, 4);
        for (int i = 0; i < 12; ++i) m.data()[i] = i;

        SECTION("row_range()") {
            auto row1 = m.row_range(1);
            std::vector<int> row1_vals;
            for (int val : row1) row1_vals.push_back(val);

            std::vector<int> expected1 = {4, 5, 6, 7};
            REQUIRE(row1_vals == expected1);

            const MatrixInt& cm = m;
            auto crow1 = cm.row_range(1);
            std::vector<int> crow1_vals;
            for (int val : crow1) crow1_vals.push_back(val);
            REQUIRE(crow1_vals == expected1);


            for (int& val : m.row_range(0)) {
                val += 100;
            }
            REQUIRE(m[0][0] == 100);
            REQUIRE(m[0][3] == 103);
        }

        SECTION("column_range()") {
            auto col2 = m.column_range(2);
            std::vector<int> col2_vals;
            for (int val : col2) col2_vals.push_back(val);

            std::vector<int> expected2 = {2, 6, 10};
            REQUIRE(col2_vals == expected2);

            const MatrixInt& cm = m;
            auto ccol2 = cm.column_range(2);
            std::vector<int> ccol2_vals;
            for (int val : ccol2) ccol2_vals.push_back(val);
            REQUIRE(ccol2_vals == expected2);

            for (int& val : m.column_range(1)) {
                val += 200;
            }
            REQUIRE(m[0][1] == 201);
            REQUIRE(m[1][1] == 205);
            REQUIRE(m[2][1] == 209);
        }
    }

    SECTION("Граничные случаи и исключения") {
        SECTION("Исключения при создании") {
            REQUIRE(MatrixInt::max_size() == std::numeric_limits<size_t>::max());

            MatrixInt m1(0, 0);
            REQUIRE(m1.empty());

            MatrixInt m2(0, 5);
            REQUIRE(m2.empty());

            MatrixInt m3(5, 0);
            REQUIRE(m3.empty());
        }

        SECTION("Исключения при доступе") {
            MatrixInt m(2, 3);

            REQUIRE_NOTHROW(m[0][0]);
            REQUIRE_NOTHROW(m[1][2]);

            REQUIRE_THROWS_AS(m.at(2, 0), std::out_of_range);
            REQUIRE_THROWS_AS(m.at(0, 3), std::out_of_range);
            REQUIRE_THROWS_AS(m.at(2, 3), std::out_of_range);

            const MatrixInt& cm = m;
            REQUIRE_THROWS_AS(cm.at(2, 0), std::out_of_range);
        }

        SECTION("Исключения при вставке диапазона") {
            MatrixInt m(2, 2);
            std::vector<int> vec = {1, 2, 3, 4, 5};

            REQUIRE_THROWS(m.insert(m.cbegin(), vec.begin(), vec.end()));

            REQUIRE_NOTHROW(m.insert(m.cbegin(), vec.begin(), vec.begin() + 2));
        }
    }

    SECTION("Проверка концептов и свойств типов") {
        MatrixInt m(2, 3);


        REQUIRE(std::contiguous_iterator<decltype(m.begin())>);
        REQUIRE(std::contiguous_iterator<decltype(m.cbegin())>);

        REQUIRE(std::ranges::contiguous_range<MatrixInt>);
        REQUIRE(std::ranges::random_access_range<MatrixInt>);
        REQUIRE(std::ranges::sized_range<MatrixInt>);
        REQUIRE(std::ranges::common_range<MatrixInt>);

        REQUIRE(std::is_same_v<MatrixInt::value_type, int>);
        REQUIRE(std::is_same_v<MatrixInt::pointer, int*>);
        REQUIRE(std::is_same_v<MatrixInt::const_pointer, const int*>);
        REQUIRE(std::is_same_v<MatrixInt::reference, int&>);
        REQUIRE(std::is_same_v<MatrixInt::const_reference, const int&>);
        REQUIRE(std::is_same_v<MatrixInt::size_type, size_t>);
    }
}

TEST_CASE("Matrix - Специальные типы") {
    SECTION("Матрица с пользовательским типом") {
        struct UserClass {
            std::string data;
            UserClass() : data("default") {}
            UserClass(const char* s) : data(s) {}
            UserClass(const UserClass&) = default;
            UserClass& operator=(const UserClass&) = default;
        };

        TowerDefence::Matrix<UserClass> m(2, 2, "hello");

        REQUIRE(m.rows() == 2);
        REQUIRE(m.cols() == 2);
        REQUIRE(m[0][0].data == "hello");
        REQUIRE(m[1][1].data == "hello");

        m[0][0] = UserClass("world");
        REQUIRE(m[0][0].data == "world");

        auto m2 = m;
        REQUIRE(m2[0][0].data == "world");
        m[0][0] = UserClass("changed");
        REQUIRE(m2[0][0].data == "world");
    }

    SECTION("Матрица с типом, не имеющим конструктора по умолчанию") {

         //struct NoDefault {
         //    NoDefault() = delete;
         //    NoDefault(int) {}
         //};
         //TowerDefence::Matrix<NoDefault> m;
    }
}