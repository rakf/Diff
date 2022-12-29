#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <optional>
#include <cassert>

enum class DIRECTION
{
    UP = -1, DOWN = 1, NONE = 0,
};

struct Diff
{
    int shift;
    DIRECTION dir;
    std::size_t position;
};

enum class OPERATION_TYPE
{
    ADDED, DELETED, CHANGED, MOVED,
};

template < typename ValueType >
struct OperationData
{
    OPERATION_TYPE type;
    ValueType value;
    std::optional< ValueType > new_value;
    size_t position_start;
    std::optional< size_t > position_end;

    bool operator==( const OperationData& rhs ) const
    {
        return this->type == rhs.type &&
               this->value == rhs.value &&
               this->new_value == rhs.new_value &&
               this->position_start == rhs.position_start &&
               this->position_end == rhs.position_end;
    }
};

template < typename ValueType >
struct CompareResult
{
    std::vector<OperationData<ValueType>> added_operations;
    std::vector<OperationData<ValueType>> deleted_operations;
    std::vector<OperationData<ValueType>> chanded_operations;
    std::vector<OperationData<ValueType>> moved_operations;
};

struct Address
{
    std::string value;
    int id;

    bool operator==( const Address& rhs ) const
    {
        return this->value == rhs.value &&
               this->id == rhs.id;
    }
};

bool hasShifts(std::vector<Diff> shifts) {
    for( auto& diff : shifts )
    {
        if( diff.shift != 0 )
        {
            return true;
        }
    }
    return false;
}

// Получить список позиций, на которых поставлены новые элементы в массиве update, относительно массива old.
template < typename ValueType >
std::vector< size_t > get_new_elements_positions( const std::vector<ValueType>& old, const std::vector<ValueType>& updated )
{
    std::vector< size_t > result;
    result.reserve( updated.size() );

    for( size_t i = 0; i < updated.size(); ++i )
    {
        if( auto it = std::find( old.begin(), old.end(), updated[ i ] );
            it == old.end() )
        {
            result.push_back( i );
        }
    }

    return result;
}


// Сформировать список отличий списка updated от списка old
template < typename ValueType >
std::vector<Diff> diffs( const std::vector<ValueType>& old, const std::vector<ValueType>& updated)
{
    std::vector<Diff> result;

    for( std::size_t i = 0; i < updated.size(); ++i)
    {

        for( std::size_t j = 0; j < old.size(); ++j )
        {
            if( updated[i] == old[j] )
            {
                DIRECTION current_dirrection;
                if( i < j ) current_dirrection = DIRECTION::UP;
                else if( i > j ) current_dirrection = DIRECTION::DOWN;
                else current_dirrection = DIRECTION::NONE;
                result.push_back( { std::abs(int(i - j)), current_dirrection, i } );
                break;
            }
        }
    }
    return result;
}

template < typename ValueType >
void move(std::vector<ValueType>& vec, int position, int shift, DIRECTION dir)
{
    for( int i = position; i != position + shift * static_cast<int>(dir); i += static_cast<int>(dir))
    {
       std::swap(vec[i], vec[i+int(dir)]) ;
    }
}

template < typename ValueType >
CompareResult<ValueType> compare( std::vector<ValueType> old, std::vector<ValueType> updated )
{
    std::vector<OperationData<ValueType>> added_operations;
    added_operations.reserve(updated.size());
    std::vector<OperationData<ValueType>> deleted_operations;
    deleted_operations.reserve(old.size());
    std::vector<OperationData<ValueType>> chanded_operations;
    chanded_operations.reserve(old.size());
    std::vector<OperationData<ValueType>> moved_operations;
    moved_operations.reserve(old.size());


    // Добавление новых элементов в старый список, которые есть в обновленном.
    for( auto new_elem_position : get_new_elements_positions( old, updated ) )
    {
        old.insert( old.begin() + new_elem_position, updated[new_elem_position] );
        added_operations.push_back({OPERATION_TYPE::ADDED, updated[new_elem_position], std::nullopt, new_elem_position, std::nullopt});
    }

    // Удаление элементов старого списка, которых нет в обновленном.
    auto deleted_elem = get_new_elements_positions( updated, old );
    for( auto new_elem_position : deleted_elem )
    {
        if( auto it = std::find_if( added_operations.begin(), added_operations.end(), [&new_elem_position] (const OperationData<ValueType>& el) { return el.position_start == new_elem_position - 1; } );
            it !=  added_operations.end()  )
        {
            chanded_operations.push_back( { OPERATION_TYPE::CHANGED, old[new_elem_position], it->value, new_elem_position - 1, std::nullopt } );
            added_operations.erase(it);
        }
        else
        {
            deleted_operations.push_back({ OPERATION_TYPE::DELETED, old[new_elem_position], std::nullopt, new_elem_position, std::nullopt});
        }
    }

    old.erase(
        std::remove_if(
            old.begin(),
            old.end(),
            [&]( auto& elem )
                {
                    auto idx = distance(begin(old), find(begin(old), end(old), elem));
                    return std::find( deleted_elem.begin(), deleted_elem.end(), idx ) != deleted_elem.end();
                }),
             old.end());

    auto shifts = diffs(old, updated);

    while( hasShifts(shifts) )
    {
       int highest = 0;
       for( std::size_t i = 0; i < shifts.size(); ++i )
       {
           if( shifts[i].shift > shifts[highest].shift || (shifts[i].shift == shifts[highest].shift && shifts[i].dir == DIRECTION::UP) )
           {
               highest = i;
           }
       }

        for( std::size_t j = 0; j < old.size(); ++j)
        {
            if(old[j] == updated[highest])
            {
                moved_operations.push_back( { OPERATION_TYPE::MOVED, old[j], std::nullopt,  j, size_t( j + shifts[highest].shift * int(shifts[highest].dir) ) } );
                move(old, j, shifts[highest].shift,shifts[highest].dir );
                break;
            }
        }
        shifts = diffs(old, updated);
    }


    /*
    for( const auto& val : added_operations )
    {
        std::cout << val.value << "  added in position " << val.position_start << std::endl;
    }
    for( const auto& val : deleted_operations )
    {
        std::cout << val.value << "  deleted " << std::endl;
    }
    for( const auto& val : chanded_operations )
    {
        std::cout << val.value << " changed to " << val.new_value.value() << std::endl;
    }
    for( const auto& val : moved_operations )
    {
        std::cout << val.value << "  moved to position " << val.position_end.value() << std::endl;
    }*/
    return CompareResult<ValueType>{ std::move( added_operations ), std::move( deleted_operations ), std::move( chanded_operations ), std::move( moved_operations ),};
}

void test_full_delete_address()
{
    std::cout << "test_full_delete_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };
    auto updated = std::vector<Address>{};

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "first", 1 }, std::nullopt, 0, std::nullopt}, {OPERATION_TYPE::DELETED, { "second", 2 }, std::nullopt, 1, std::nullopt}, {OPERATION_TYPE::DELETED, { "third", 3 }, std::nullopt, 2, std::nullopt}, };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = compare(old, updated);

    assert(res.added_operations == added_operations);
    assert(res.deleted_operations == deleted_operations);
    assert(res.chanded_operations == chanded_operations);
    assert(res.moved_operations == moved_operations);
}

void test_full_add_address()
{
    std::cout << "test_full_add_address" <<std::endl;
    auto old = std::vector<Address>{};
    auto updated = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "first", 1 }, std::nullopt, 0, std::nullopt}, {OPERATION_TYPE::ADDED, { "second", 2 }, std::nullopt, 1, std::nullopt}, {OPERATION_TYPE::ADDED, { "third", 3 }, std::nullopt, 2, std::nullopt}, };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = compare(old, updated);

    assert(res.added_operations == added_operations);
    assert(res.deleted_operations == deleted_operations);
    assert(res.chanded_operations == chanded_operations);
    assert(res.moved_operations == moved_operations);
}

void test_change_address()
{
    std::cout << "test_change_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1 },
        { "HI", 4 },
        { "third", 3 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "second", 2 }, Address{ "HI", 4 }, 1, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations;

    auto res = compare(old, updated);

    assert(res.added_operations == added_operations);
    assert(res.deleted_operations == deleted_operations);
    assert(res.chanded_operations == chanded_operations);
    assert(res.moved_operations == moved_operations);
}

void test_moved_address()
{
    std::cout << "test_moved_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };
    auto updated = std::vector<Address>
    {
        { "third", 3 },
        { "first", 1 },
        { "second", 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "third", 3 }, std::nullopt,  2, 0 } };

    auto res = compare(old, updated);

    assert(res.added_operations == added_operations);
    assert(res.deleted_operations == deleted_operations);
    assert(res.chanded_operations == chanded_operations);
    assert(res.moved_operations == moved_operations);
}

void test_same_address()
{
    std::cout << "test_same_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = compare(old, updated);

    assert(res.added_operations == added_operations);
    assert(res.deleted_operations == deleted_operations);
    assert(res.chanded_operations == chanded_operations);
    assert(res.moved_operations == moved_operations);
}

void test_repeat_string_address()
{
    std::cout << "test_repeat_string_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1 },
        { "second", 2 },
        { "third", 3 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1 },
        { "Hi", 2 },
        { "third", 3 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "second", 2 }, Address{ "Hi", 2 }, 1, std::nullopt} };;
    std::vector<OperationData<Address>> moved_operations;

    auto res = compare(old, updated);

    assert(res.added_operations == added_operations);
    assert(res.deleted_operations == deleted_operations);
    assert(res.chanded_operations == chanded_operations);
    assert(res.moved_operations == moved_operations);
}

void run_tests_address()
{
    test_full_delete_address();
    test_full_add_address();
    test_change_address();
    test_moved_address();
    test_same_address();
    test_repeat_string_address();
}

int main() {
    run_tests_address();
}
