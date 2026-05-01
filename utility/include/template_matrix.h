#pragma once


#include "iterator.h"
#include <iostream>
#include <algorithm>
#include <cstddef>
#include <concepts>
#include <limits>
#include <memory>
#include <ranges>
#include <vector>


namespace TowerDefence
{
    using std::size_t;

    /**
     * @brief Шаблонный класс матрицы.
     *
     * @tparam T Тип элементов матрицы, должен быть default_initializable.
     */
    template<std::default_initializable T>
    class Matrix {
        T* data_; ///< Указатель на данные матрицы.
        size_t rows_{}; ///< Количество строк.
        size_t cols_{}; ///< Количество столбцов.

    public:
        using iterator = MatrixIterator<T, false>; ///< Тип неконстантного итератора.
        using const_iterator = MatrixIterator<T, true>; ///< Тип константного итератора.
        using value_type = T; ///< Тип значения.
        using pointer = T*; ///< Тип указателя.
        using const_pointer = const T*; ///< Тип константного указателя.
        using reference = T&; ///< Тип ссылки.
        using const_reference = const T&; ///< Тип константной ссылки.
        using size_type = size_t; ///< Тип размера.
        using difference_type = std::ptrdiff_t; ///< Тип разницы.
        using reverse_iterator = std::reverse_iterator<iterator>; ///< Тип обратного итератора.
        using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< Тип константного обратного итератора.

        Matrix(): data_(nullptr), rows_(0), cols_(0) {}; ///< Конструктор по умолчанию.

        /**
         * @brief Конструктор с заданными размерами и значением.
         */
        Matrix(size_t rows, size_t cols, const T& value);

        /**
         * @brief Конструктор с заданными размерами (значения инициализируются по умолчанию).
         */
        Matrix(size_t rows, size_t cols);

        Matrix(const Matrix& other); ///< Конструктор копирования.
        Matrix(Matrix&& other) noexcept; ///< Конструктор перемещения.
        Matrix& operator=(const Matrix& other); ///< Оператор присваивания копированием.
        Matrix& operator=(Matrix&& other) noexcept; ///< Оператор присваивания перемещением.
        ~Matrix() {delete[] data_;} ///< Деструктор.

        [[nodiscard]] size_t rows() const noexcept { return rows_; } ///< Возвращает количество строк.
        [[nodiscard]] size_t cols() const noexcept { return cols_; } ///< Возвращает количество столбцов.
        [[nodiscard]] size_type size() const noexcept { return rows_ * cols_; } ///< Возвращает общее количество элементов.
        [[nodiscard]] bool empty() const noexcept { return rows_ == 0 || cols_ == 0;} ///< Проверяет, пуста ли матрица.
        static size_type max_size() noexcept { return std::numeric_limits<size_type>::max();} ///< Максимально возможный размер.

        // Методы для итераторов
        iterator begin() noexcept { return iterator(data_); }
        const_iterator begin() const noexcept { return const_iterator(data_); }
        const_iterator cbegin() const noexcept { return const_iterator(data_); }
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

        iterator end() noexcept { return iterator(data_ + rows_ * cols_); }
        const_iterator end() const noexcept { return const_iterator(data_ + rows_ * cols_); }
        const_iterator cend() const noexcept { return const_iterator(data_ + rows_ * cols_); }
        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

        void clear(); ///< Очищает матрицу.

        /**
         * @brief Создает элемент на месте.
         */
        template<typename ... Args>
        iterator emplace(const_iterator it, Args&& ... args)
        requires std::constructible_from<T, Args...>;

        /**
         * @brief Вставляет элемент по значению.
         */
        iterator insert(const_iterator it, const T& value)
        requires std::copy_constructible<T>;

        /**
         * @brief Вставляет диапазон элементов.
         */
        template< std::forward_iterator InputIt >
        iterator insert( const_iterator pos, InputIt first, InputIt last );

        /**
         * @brief Вставляет элемент с перемещением.
         */
        iterator insert(const_iterator it, T&& value)
        requires std::move_constructible<T>;

        /**
         * @brief Вставляет список инициализации.
         */
        iterator insert(const_iterator pos, std::initializer_list<T> list);

        void swap(Matrix& other) noexcept; ///< Обменивает содержимое двух матриц.

        /**
         * @brief Заменяет содержимое матрицы.
         */
        void assign(size_type rows, size_type cols, const T& value = T())
        requires std::copy_constructible<T>;

        /**
         * @brief Заменяет содержимое матрицы из диапазона.
         */
        template<std::forward_iterator It>
        void assign(size_type rows, size_type cols, It first, It last);

        /**
         * @brief Заменяет содержимое матрицы из списка инициализации.
         */
        void assign(std::initializer_list<std::initializer_list<T>> list);

        /**
         * @brief Оператор доступа к строке.
         */
        iterator operator[](size_type row) {return iterator(data_ + row * cols_);}
        const_iterator operator[](size_type row) const {return const_iterator(data_ + row * cols_);}

        reference at(size_type row, size_type col); ///< Доступ с проверкой границ.
        const_reference at(size_type row, size_type col) const; ///< Константный доступ с проверкой границ.

        pointer data() { return data_; } ///< Возвращает указатель на данные.
        const_pointer data() const { return data_; } ///< Возвращает константный указатель на данные.

        reference front() { return data_[0]; } ///< Первый элемент.
        const_reference front() const { return data_[0]; } ///< Константный первый элемент.

        reference back() { return data_[size() - 1]; } ///< Последний элемент.
        const_reference back() const { return data_[size() - 1]; } ///< Константный последний элемент.

        void push_back_row(const T& value = T{}); ///< Добавляет строку в конец.
        void push_back_col(const T& value = T{}); ///< Добавляет столбец в конец.

        void insert_row(size_type pos, const T& value = T{}); ///< Вставляет строку в указанную позицию.
        void insert_col(size_type pos, const T& value = T{}); ///< Вставляет столбец в указанную позицию.

        void erase_row(size_type pos); ///< Удаляет строку.
        void erase_col(size_type pos); ///< Удаляет столбец.

        auto row_range(size_type row); ///< Возвращает диапазон для строки.
        auto row_range(size_type row) const; ///< Константная версия.

        auto column_range(size_type col); ///< Возвращает диапазон для столбца.
        auto column_range(size_type col) const; ///< Константная версия.
    };

    template<std::default_initializable T>
    Matrix<T>::Matrix(size_t rows, size_t cols, const T& value )
    {
        data_ = new T[rows * cols];
        rows_ = rows;
        cols_ = cols;
        try {
            std::ranges::fill_n(data_, cols * rows, value);
        }catch(...) {
            delete[] data_;
            throw;
        }
    }

    template<std::default_initializable T>
    Matrix<T>::Matrix(size_t rows, size_t cols): data_(new T[rows * cols]{}), rows_(rows), cols_(cols)
    {
    }

    template<std::default_initializable T>
    Matrix<T>::Matrix(const Matrix& other) : data_(nullptr), rows_(0), cols_(0)
    {
        assign(other.rows_, other.cols_, other.begin(), other.end());
    }

    template<std::default_initializable T>
    Matrix<T>::Matrix(Matrix &&other) noexcept : data_(nullptr)
    {
        swap(other);
    }

    template<std::default_initializable T>
    Matrix<T> & Matrix<T>::operator=(const Matrix &other)
    {
        if (this != &other) {
            Matrix temp(other);
            swap(temp);
        }
        return *this;
    }

    template<std::default_initializable T>
    Matrix<T> & Matrix<T>::operator=(Matrix &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        swap(other);
        return *this;
    }

    template<std::default_initializable T>
    void Matrix<T>::clear()
    {
        Matrix tmp{0,0};
        swap(tmp);
    }

    template<std::default_initializable T>
    template<typename ... Args>
    Matrix<T>::iterator Matrix<T>::emplace(const_iterator it, Args &&...args) requires std::constructible_from<
        T, Args...>
    {
        auto ptr = const_cast<pointer>(it.base());
        T tmp{std::forward<Args>(args)...};

        std::destroy_at(ptr);
        std::construct_at(ptr, std::move(tmp));

        return iterator(ptr);
    }

    template<std::default_initializable T>
    Matrix<T>::iterator Matrix<T>::insert(const_iterator it, const T &value) requires std::copy_constructible<T>
    {
        return emplace(it, value);
    }

    template<std::default_initializable T>
    template<std::forward_iterator It>
    Matrix<T>::iterator Matrix<T>::insert(const_iterator pos, It first, It last)
    {
        auto d = std::distance(first, last);
        if (d > std::distance(pos, cend())) {
            throw std::out_of_range("out of range");
        }
        Matrix tmp{};
        tmp.assign(1, d, pos, pos+d);
        iterator it = iterator(const_cast<T*>(pos.base()));
        try {
            while (first != last) {
                *it = *first;
                ++first;
                ++it;
            }
        }catch(...){
            std::ranges::move(tmp.begin(), tmp.begin() + std::distance(pos, const_iterator(it)), iterator(const_cast<T*>(pos.base())));
            throw;
        }
        return iterator(const_cast<T*>(pos.base()));
    }


    template<std::default_initializable T>
    Matrix<T>::iterator Matrix<T>::insert(const_iterator it, T &&value) requires std::move_constructible<T>
    {
        return emplace(it, std::move(value));
    }

    template<std::default_initializable T>
    Matrix<T>::iterator Matrix<T>::insert(const_iterator pos, std::initializer_list<T> list)
    {
        return insert(pos, list.begin(), list.end());

    }

    template<std::default_initializable T>
    void Matrix<T>::swap(Matrix &other) noexcept
    {
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
        std::swap(data_, other.data_);
    }

    template<std::default_initializable T>
    void Matrix<T>::assign(size_type rows, size_type cols, const T &value ) requires std::copy_constructible<T>
    {
        auto tmp = Matrix{rows, cols, value};
        swap(tmp);
    }



    template<std::default_initializable T>
    template<std::forward_iterator It>
    void Matrix<T>::assign(size_type rows, size_type cols, It first, It last)
    {
        const size_type required = rows * cols;
        const size_type actual = std::distance(first, last);

        if (actual > required) {
            throw std::invalid_argument(
                "Too many elements: expected " + std::to_string(required) +
                ", got " + std::to_string(actual));
        }

        Matrix tmp(rows, cols);
        std::ranges::copy_n(first, actual, tmp.data_);
        swap(tmp);
    }

    template<std::default_initializable T>
    void Matrix<T>::assign(std::initializer_list<std::initializer_list<T>> list)
    {
        if (list.size() == 0) {
            clear();
            return;
        }

        const size_type rows = list.size();
        const size_type cols = list.begin()->size();

        if (!std::ranges::all_of(list, [cols](const auto& row) {
            return row.size() == cols;
        })) {
            throw std::invalid_argument("All rows must have the same length");
        }

        Matrix tmp{rows, cols};

        auto dest = tmp.data_;
        for (const auto& row : list) {
            dest = std::ranges::copy(row, dest).out;
        }

        swap(tmp);
    }

    template<std::default_initializable T>
    Matrix<T>::reference Matrix<T>::at(size_type row, size_type col)
    {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("Matrix::at : ("+ std::to_string(row) + "," + std::to_string(col) + ")");
        }
        return data_[row * cols_ + col];
    }

    template<std::default_initializable T>
    Matrix<T>::const_reference Matrix<T>::at(size_type row, size_type col) const
    {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("Matrix::const at :("+ std::to_string(row) + "," + std::to_string(col) + ")");
        }
        return data_[row * cols_ + col];
    }

    template<std::default_initializable T>
    void Matrix<T>::push_back_row(const T &value)
    {
        insert_row(rows_, value);
    }

    template<std::default_initializable T>
    void Matrix<T>::insert_row(size_type pos, const T &value)
    {
        if (empty()) {
            assign(1, 1, value);
            return;
        }
        if (pos > rows_) {
            throw std::out_of_range("Row position out of range");
        }

        const size_type newRows = rows_ + 1;
        const size_type newCols = cols_;

        Matrix tmp(newRows, newCols);

        if (pos > 0) {
            auto src = begin();
            auto dst = tmp.begin();
            for (size_type i = 0; i < pos * cols_; ++i) {
                *dst = std::move_if_noexcept(*src);
                ++src;
                ++dst;
            }
        }

        std::ranges::fill_n(tmp.data_ + pos * newCols, newCols, value);

        if (pos < rows_) {
            auto src = begin() + pos * cols_;
            auto dst = tmp.begin() + (pos + 1) * newCols;
            for (size_type i = 0; i < (rows_ - pos) * cols_; ++i) {
                *dst = std::move_if_noexcept(*src);
                ++src;
                ++dst;
            }
        }

        swap(tmp);
    }

    template<std::default_initializable T>
    void Matrix<T>::push_back_col(const T &value)
    {
        insert_col(cols_, value);
    }

    template<std::default_initializable T>
    void Matrix<T>::insert_col(size_type pos, const T &value)
    {
        if (pos > cols_) {
            throw std::out_of_range("Column position out of range");
        }

        if (empty()) {
            assign(1, 1, value);
            return;
        }

        const size_type newRows = rows_;
        const size_type newCols = cols_ + 1;

        Matrix tmp(newRows, newCols);

        for (size_type i = 0; i < rows_; ++i) {
            auto srcRow = begin() + i * cols_;
            auto dstRow = tmp.begin() + i * newCols;

            if (pos > 0) {
                for (size_type j = 0; j < pos; ++j) {
                    dstRow[j] = std::move_if_noexcept(srcRow[j]);
                }
            }
            dstRow[pos] = value;
            if (pos < cols_) {
                for (size_type j = pos; j < cols_; ++j) {
                    dstRow[j + 1] = std::move_if_noexcept(srcRow[j]);
                }
            }
        }

        swap(tmp);
    }

    template<std::default_initializable T>
    void Matrix<T>::erase_row(size_type pos)
    {
        if (pos >= rows_) {
            throw std::out_of_range("Row position out of range");
        }

        if (rows_ == 1) {
            clear();
            return;
        }

        const size_type newRows = rows_ - 1;
        const size_type newCols = cols_;

        Matrix tmp(newRows, newCols);

        auto dst = tmp.begin();

        if (pos > 0) {
            auto src = begin();
            for (size_type i = 0; i < pos * cols_; ++i) {
                *dst = std::move_if_noexcept(*src);
                ++src;
                ++dst;
            }
        }

        if (pos < newRows) {
            auto src = begin() + (pos + 1) * cols_;
            for (size_type i = 0; i < (newRows - pos) * cols_; ++i) {
                *dst = std::move_if_noexcept(*src);
                ++src;
                ++dst;
            }
        }

        swap(tmp);
    }

    template<std::default_initializable T>
    void Matrix<T>::erase_col(size_type pos)
    {
        if (pos >= cols_) {
            throw std::out_of_range("Column position out of range");
        }

        if (cols_ == 1) {
            clear();
            return;
        }

        const size_type newRows = rows_;
        const size_type newCols = cols_ - 1;

        Matrix tmp(newRows, newCols);

        for (size_type i = 0; i < rows_; ++i) {
            auto src_row = begin() + i * cols_;
            auto dst_row = tmp.begin() + i * newCols;

            if (pos > 0) {
                for (size_type j = 0; j < pos; ++j) {
                    dst_row[j] = std::move_if_noexcept(src_row[j]);
                }
            }
            if (pos < newCols) {
                for (size_type j = pos; j < cols_ - 1; ++j) {
                    dst_row[j] = std::move_if_noexcept(src_row[j + 1]);
                }
            }
        }

        swap(tmp);
    }

    template<std::default_initializable T>
    auto Matrix<T>::row_range(size_type row)
    {
        return std::views::iota(size_type{0}, cols_)
               | std::views::transform([this, row](size_type col) -> reference {
                   return (*this)[row][col];
               });
    }

    template<std::default_initializable T>
    auto Matrix<T>::row_range(size_type row) const
    {
        return std::views::iota(size_type{0}, cols_)
               | std::views::transform([this, row](size_type col) -> const_reference {
                   return (*this)[row][col];
               });
    }

    template<std::default_initializable T>
    auto Matrix<T>::column_range(size_type col)
    {
        return std::views::iota(size_type{0}, rows_)
               | std::views::transform([this, col](size_type row) -> reference {
                   return (*this)[row][col];
               });
    }

    template<std::default_initializable T>
    auto Matrix<T>::column_range(size_type col) const
    {
        return std::views::iota(size_type{0}, rows_)
               | std::views::transform([this, col](size_type row) -> const_reference {
                   return (*this)[row][col];
               });
    }

    static_assert(std::contiguous_iterator<Matrix<int>::iterator>);
    static_assert(std::contiguous_iterator<Matrix<int>::const_iterator>);
    static_assert(std::ranges::contiguous_range<Matrix<int>>);


    static_assert(std::ranges::random_access_range<Matrix<int>>);
    static_assert(std::ranges::sized_range<Matrix<int>>);
    static_assert(std::ranges::common_range<Matrix<int>>);

}
