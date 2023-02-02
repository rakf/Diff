#pragma once

#include <vector>
#include <algorithm>
#include <optional>
#include <cassert>

/* @brief Тип операции */
enum class OPERATION_TYPE
{
    ADDED, DELETED, CHANGED, MOVED,
};

/* @brief Структура хранит информацию об операции изменения */
template < typename ValueType >
struct OperationData
{
    /* Тип операции */
    OPERATION_TYPE mType;

    /* Копия элемента, с которым произошла операция. В случае операции изменения - здесь хранится копия старого значения элемента */
    ValueType mValue;

    /* Новое значение элемента с которым произошла операция - Необходимо только для операции изменения (CHANGED) */
    std::optional< ValueType > mNewValue;

    /* Позиция элемента, с которым произошла операция. В случае перемещения хранится старая позиция элемента */
   std:: size_t mPositionStart;

    /* Новое значение позиции элемента с которым произошла операция - Необходимо только для операции изменения (MOVED) */
    std::optional< std::size_t > mPositionEnd;

    bool operator==( const OperationData& rhs ) const
    {
        return this->mType == rhs.mType &&
               this->mValue == rhs.mValue &&
               this->mNewValue == rhs.mNewValue &&
               this->mPositionStart == rhs.mPositionStart &&
               this->mPositionEnd == rhs.mPositionEnd;
    }
};

/* @brief Структура хранит результат сравнения 2 списков */
template < typename ValueType >
struct CompareResult
{
    /* Операции добавления */
    std::vector<OperationData<ValueType>> mAddedOperations;

    /* Операции удаления */
    std::vector<OperationData<ValueType>> mDeletedOperations;

    /* Операции изменения */
    std::vector<OperationData<ValueType>> mChandedOperations;

    /* Операции перемещения */
    std::vector<OperationData<ValueType>> mMovedOperations;
};

/* @brief Структура адреса */
struct Address
{
    /* Значение адреса */
    std::string mValue;

    /* Уникальный идентификатор адреса */
    size_t mId;

    /* Позиция адреса в списке адресов */
    size_t mPosition;

    bool operator==( const Address& rhs ) const
    {
        return this->mValue == rhs.mValue &&
               this->mId == rhs.mId &&
               this->mPosition == rhs.mPosition;
    }
};

/* Enum определяет направление сдвига */
enum class DIRECTION
{
    UP = -1,
    DOWN = 1,
    NONE = 0,
};

/*
 * @brief Структура, хранящая данные по разнице в расположении элементов в списке.
 */
struct ShiftDiff
{
    /* Разница между расположением элементов */
    int mShift;

    /* Разница между расположением */
    DIRECTION mDir;

    /* Разница между расположением */
    std::size_t mPosition;
};

/*
 * @brief Класс содержит логику по формированию разницы между 2 списками адрессов.
 */
class DifferAddress
{

public:

    /*
     * @brief Сравнивает 2 списка адресов.
     * @param old_addresses Старый список адресов.
     * @param updated_addresses Новый список адресов.
     * @return Результат сравнения.
     */
    CompareResult< Address > Compare( const std::vector< Address >& old_addresses, const std::vector< Address >& updated_addresses );

    /*
     * @brief Распечатать редакционное предписание для результата сравнения.
     * @param compare_result Результат сравнения.
     */
    void PrintEditorialPrescription( const CompareResult< Address >& compare_result );

private:

    /*
     * @brief Сравнивает 2 списка адресов задаваемых итераторами и копирует элементы в новый список, которые есть в 1 списке и нет во 2 списке.
     * @param first1 Начало первого списка.
     * @param last1 Конец первого списка.
     * @param first2 Начало второго списка.
     * @param last2 Конец второго списка.
     * @param d_first - Выходной итератор нового списка.
     * @param comp - Компаратор для сравнения элементов.
     * @param updated_addresses Новый список адресов.
     * @return Результат сравнения.
     */
    template< class InputIt1, class InputIt2, class OutputIt, class Comp >
    OutputIt SetDifference( InputIt1 first1, InputIt1 last1,
        InputIt2 first2, InputIt2 last2, OutputIt d_first, Comp comp );


    /*
     * @brief Формирует список элементов с информацией о сдвигах в порядке элементов в 2 списках.
     * @warning По содержанию оба массива должны быть равны.
     * @param old_addresses Старый список адресов.
     * @param updated_addresses Новый список адресов.
     * @return Результат сравнения.
     */
    std::vector< ShiftDiff > FormShifts( const std::vector< Address >& old_addresses, const std::vector< Address >& updated_addresses );

    /*
     * @brief Проверяет есть ли сдвиги элементов в списке со сдвигами.
     * @param shifts Список со сдвигами элементов.
     * @return true - есть сдвиги, false - нет сдвигов.
     */
    bool HasShifts( const std::vector< ShiftDiff >& shifts );

    /*
     * @brief Сдвигает элемент из списка на заданное количество элементов и направление.
     * @param vec Список элементов.
     * @param position Позиция элемент, который нужно сдвинуть.
     * @param shift - Количество элементов, на которое надо сдвинуть.
     * @param direction - Направление сдвига.
     */
    void MoveElementInVector( std::vector< Address >& vec, size_t position, size_t shift, DIRECTION direction );

};

/* Компаратор для отбора новых элементов */
template< typename InputIt1 >
struct NewElementsAddressComparator
{
    bool operator() ( const Address& a ) const
    {
        return a.mId == this->mIt->mId;
    }

    InputIt1 mIt;
};

/* Компаратор для отбора новых элементов */
struct AddressIdComparator
{
    AddressIdComparator( size_t id )
        : mId( id ) {}

    bool operator() ( const Address& a ) const
    {
        return a.mId == mId;
    }

    size_t mId;
};

/* Компаратор для отбора изменившихся элементов */
template<typename InputIt1>
struct ChangeElementsAddressComparator
{
    bool operator() ( const Address& a ) const
    {
        return a.mId == this->mIt->mId && a.mValue != this->mIt->mValue;
    }

    InputIt1 mIt;
};

CompareResult< Address > DifferAddress::Compare( const std::vector< Address >& old_addresses, const std::vector< Address >& updated_addresses )
{
    std::vector<OperationData<Address>> added_operations;
    added_operations.reserve( updated_addresses.size() );
    std::vector<OperationData<Address>> deleted_operations;
    deleted_operations.reserve( old_addresses.size() );
    std::vector<OperationData<Address>> chanded_operations;
    chanded_operations.reserve( old_addresses.size() );
    std::vector<OperationData<Address>> moved_operations;
    moved_operations.reserve( old_addresses.size() );

    std::vector< Address > deleted_elements, added_elements, changed_elements;
    deleted_elements.reserve( old_addresses.size() );
    added_elements.reserve( updated_addresses.size() );
    changed_elements.reserve( old_addresses.size() );

    NewElementsAddressComparator< decltype ( old_addresses.begin() ) > new_element_comparator;

    /* Находим добавленные элементы */
    SetDifference( old_addresses.begin(), old_addresses.end(), updated_addresses.begin(), updated_addresses.end(), std::back_inserter( deleted_elements ), new_element_comparator );

    /* Находим удаленные элементы */
    SetDifference( updated_addresses.begin(), updated_addresses.end(), old_addresses.begin(), old_addresses.end(), std::back_inserter( added_elements  ), new_element_comparator );

    std::vector< Address > old_copy( old_addresses.begin(), old_addresses.end() );

    for( const auto& elem: added_elements )
    {
        added_operations.push_back( {OPERATION_TYPE::ADDED, elem, std::nullopt, elem.mPosition, std::nullopt} );

        /* Добавляем новые элементы в список, чтобы можно было сравнить порядок элементов */
        old_copy.insert( old_copy.begin() + elem.mPosition, elem );
    }

    for( const auto& elem: deleted_elements )
    {
        deleted_operations.push_back({ OPERATION_TYPE::DELETED, elem, std::nullopt, elem.mPosition, std::nullopt} );
    }

    /* Удаляем старые элементы, чтобы можно было сравнить порядок элементов  */
    old_copy.erase(
        std::remove_if(
            old_copy.begin(),
            old_copy.end(),
            [&]( const Address& elem )
                {
                    return std::find_if( deleted_elements.begin(), deleted_elements.end(), AddressIdComparator( elem.mId ) ) != deleted_elements.end(); }),
             old_copy.end());

    ChangeElementsAddressComparator< decltype ( old_addresses.begin() ) > change_element_comparator;
    /* Находим измененные элементы */
    SetDifference( updated_addresses.begin(), updated_addresses.end(), old_addresses.begin(), old_addresses.end(), std::back_inserter( changed_elements ), change_element_comparator );

    for( const auto& elem: changed_elements )
    {
        auto old_el = std::find_if( old_addresses.begin(), old_addresses.end(), AddressIdComparator( elem.mId ) );
        chanded_operations.push_back( { OPERATION_TYPE::CHANGED, *old_el, elem, elem.mPosition, std::nullopt } );
    }

    /* Формируем сдвиги элементов относительно друг друга */
    auto shifts = FormShifts( old_copy, updated_addresses );

    while( HasShifts( shifts ) )
    {
       int highest = 0;
       for( std::size_t i = 0; i < shifts.size(); ++i )
       {
           if( shifts[ i ].mShift > shifts[ highest ].mShift || (shifts[ i ].mShift == shifts[ highest ].mShift && shifts[ i ].mDir == DIRECTION::UP ) )
           {
               highest = i;
           }
       }

       for( std::size_t j = 0; j < old_copy.size(); ++j)
       {
           if( old_copy[ j ].mId == updated_addresses[ highest ].mId )
           {
               MoveElementInVector( old_copy, j, shifts[ highest ].mShift, shifts[ highest ].mDir );
               size_t position_end =static_cast< size_t >( j + shifts[highest].mShift * static_cast< int >( shifts[ highest ].mDir ) );
               moved_operations.push_back( { OPERATION_TYPE::MOVED, updated_addresses[ highest ], std::nullopt,  j, position_end } );
               break;
           }
        }
        shifts = FormShifts( old_copy, updated_addresses );
    }

    return CompareResult<Address>{ std::move( added_operations ), std::move( deleted_operations ), std::move( chanded_operations ), std::move( moved_operations ),};
}

template< class InputIt1, class InputIt2, class OutputIt, class Comp >
OutputIt DifferAddress::SetDifference( InputIt1 first1, InputIt1 last1,
    InputIt2 first2, InputIt2 last2, OutputIt d_first, Comp comp )
{
    if constexpr( std::is_same< Comp, NewElementsAddressComparator< InputIt1 > >::value )
    {
        if(  first2 == last2 ) return std::copy( first1, last1, d_first );
    }

    for( auto it_1 = first1; it_1 != last1; ++it_1 )
    {
        comp.mIt = it_1;
        auto it = std::find_if( first2, last2, comp );
        if constexpr ( std::is_same< Comp, NewElementsAddressComparator< InputIt1 > >::value )
        {
            if( it == last2 ) *d_first++ = *it_1;
        }

        if constexpr ( std::is_same< Comp, ChangeElementsAddressComparator< InputIt1 > >::value )
        {
            if( it != last2 ) *d_first++ = *it_1;
        }
    }
    return d_first;
}

void DifferAddress::MoveElementInVector( std::vector< Address >& vec, size_t position, size_t shift, DIRECTION direction )
{
    int int_direction = static_cast< int >( direction );
    for( size_t i = position; i != position + shift * int_direction; i += int_direction )
    {
       std::swap( vec[ i ], vec[ i + int_direction ]) ;
    }
}

bool DifferAddress::HasShifts( const std::vector< ShiftDiff >& shifts )
{
    return std::find_if( shifts.begin(), shifts.end(), []( const ShiftDiff& shift_diff ){ return shift_diff.mShift != 0; } ) != shifts.end();
}

std::vector< ShiftDiff > DifferAddress::FormShifts( const std::vector< Address >& old_addresses, const std::vector< Address >& updated_addresses )
{
    std::vector<ShiftDiff> result;

    for( std::size_t i = 0; i < updated_addresses.size(); ++i)
    {
        for( std::size_t j = 0; j < old_addresses.size(); ++j )
        {
            if( updated_addresses[ i ].mId == old_addresses[ j ].mId )
            {
                DIRECTION current_dirrection;
                if( i < j ) current_dirrection = DIRECTION::UP;
                else if( i > j ) current_dirrection = DIRECTION::DOWN;
                else current_dirrection = DIRECTION::NONE;
                result.push_back( { std::abs( int( i - j ) ), current_dirrection, i } );
                break;
            }
        }
    }
    return result;
}
