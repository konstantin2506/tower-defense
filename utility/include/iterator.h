#pragma once

#include <iterator>

namespace TowerDefence
{
    template<std::default_initializable T>
    class Matrix;

    /**
     * @brief Итератор для матрицы.
     *
     * @tparam T Тип элементов матрицы.
     * @tparam isConst Флаг константности итератора.
     */
    template<typename T, bool isConst>
    class MatrixIterator {
    public:
        using pointer = std::conditional_t<isConst, const T*, T*>; ///< Тип указателя.
        using reference = std::conditional_t<isConst, const T&, T&>; ///< Тип ссылки.
        using difference_type = std::ptrdiff_t; ///< Тип разницы между итераторами.
        using value_type = T; ///< Тип значения.
        using iterator_category = std::contiguous_iterator_tag; ///< Категория итератора.

    private:
        pointer ptr_; ///< Указатель на текущий элемент.

        friend Matrix<T>;
        friend MatrixIterator<T, !isConst>;

        /**
         * @brief Приватный конструктор.
         * @param ptr Указатель на элемент.
         */
        explicit MatrixIterator(pointer ptr) : ptr_(ptr){}

    public:
        MatrixIterator() = default; ///< Конструктор по умолчанию.

        /**
         * @brief Конструктор преобразования между константными и неконстантными итераторами.
         */
        template<bool otherConst>
        MatrixIterator(const MatrixIterator<T, otherConst>& other) noexcept
        requires(isConst >= otherConst) : ptr_(other.ptr_){}

        /**
         * @brief Оператор присваивания между константными и неконстантными итераторами.
         */
        template<bool otherConst>
        MatrixIterator& operator=(const MatrixIterator<T, otherConst>& other) noexcept
        requires(isConst >= otherConst)
        {
            ptr_ = other.ptr_;
            return *this;
        }

        /**
         * @brief Префиксный инкремент.
         */
        MatrixIterator& operator++() noexcept
        {
            ++ptr_;
            return *this;
        }

        /**
         * @brief Постфиксный инкремент.
         */
        MatrixIterator operator++(int) noexcept
        {
            MatrixIterator res = MatrixIterator(ptr_);
            ++(*this);
            return res;
        }

        /**
         * @brief Префиксный декремент.
         */
        MatrixIterator& operator--() noexcept
        {
            --ptr_;
            return *this;
        }

        /**
         * @brief Постфиксный декремент.
         */
        MatrixIterator operator--(int) noexcept
        {
            MatrixIterator res = MatrixIterator(ptr_);
            --(*this);
            return res;
        }

        /**
         * @brief Оператор разыменования.
         */
        reference operator*() const noexcept
        {
            return *ptr_;
        }

        /**
         * @brief Оператор доступа к члену.
         */
        pointer operator->() const noexcept
        {
            return ptr_;
        }

        /**
         * @brief Возвращает базовый указатель.
         */
        [[nodiscard]] pointer base() const noexcept
        {
            return ptr_;
        }

        /**
         * @brief Оператор сравнения на равенство.
         */
        template<bool otherConst>
        bool operator==(const MatrixIterator<T, otherConst>& other) const noexcept
        {
            return ptr_ == other.ptr_;
        }

        /**
         * @brief Трехстороннее сравнение.
         */
        template<bool otherConst>
        std::strong_ordering operator<=>(const MatrixIterator<T, otherConst>& other) const noexcept
        {
            return ptr_ <=> other.ptr_;
        }

        /**
         * @brief Сдвиг итератора вперед.
         */
        MatrixIterator& operator+=(difference_type diff) noexcept
        {
            ptr_ += diff;
            return *this;
        }

        /**
         * @brief Сдвиг итератора назад.
         */
        MatrixIterator& operator-=(difference_type diff) noexcept
        {
            ptr_ -= diff;
            return *this;
        }

        /**
         * @brief Сложение итератора с числом.
         */
        MatrixIterator operator+(difference_type diff) const noexcept
        {
            return MatrixIterator(ptr_ + diff);
        }

        /**
         * @brief Вычитание числа из итератора.
         */
        MatrixIterator operator-(difference_type diff) const noexcept
        {
            return MatrixIterator(ptr_ - diff);
        }

        /**
         * @brief Разность между итераторами.
         */
        template<bool otherConst>
        difference_type operator-(const MatrixIterator<T, otherConst>& other) const noexcept
        {
            return ptr_ - other.ptr_;
        }

        /**
         * @brief Оператор индексации.
         */
        reference operator[](size_t diff) const noexcept
        {
            return ptr_[diff];
        }

    };

    /**
     * @brief Сложение числа с итератором (симметричная версия).
     */
    template<typename T, bool isConst>
    MatrixIterator<T, isConst> operator+(typename MatrixIterator<T, isConst>::difference_type diff, const MatrixIterator<T, isConst>& it)
    noexcept
    {
        return it + diff;
    }

    // Статические проверки концептов итераторов
    static_assert(std::contiguous_iterator<MatrixIterator<int, false>>);
    static_assert(std::contiguous_iterator<MatrixIterator<int, true>>);
}