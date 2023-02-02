#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <optional>
#include <cassert>
#include <address_differ.h>

/*
 * @brief Сортирует массив адресов, если он не сортирован. Сортировка прводится по порядковому номеру в списке
 * @param vector Исходный вектор.
*/
void SortIfNot( std::vector< Address >& vector )
{
    auto less_address_sort = []( const Address& a, const Address& b ) { return a.mPosition < b.mPosition; } ;
    if( !std::is_sorted( vector.begin(),vector.end(), less_address_sort ) )
    {
        std::sort( vector.begin(), vector.end(), less_address_sort );
    }
}

void test_full_delete_address()
{
    std::cout << "test_full_delete_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>{};

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{
        {OPERATION_TYPE::DELETED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt},
        {OPERATION_TYPE::DELETED, { "second", 2, 1 }, std::nullopt, 1, std::nullopt},
        {OPERATION_TYPE::DELETED, { "third", 3, 2 }, std::nullopt, 2, std::nullopt}, };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_address_from_the_beginning()
{
    std::cout << "test_delete_address_from_the_beginning" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "second", 2, 1 },
        { "third", 3, 2 }
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_address_from_the_end()
{
    std::cout << "test_delete_address_from_the_end" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 }
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "third", 3, 2 }, std::nullopt, 2, std::nullopt}, };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_address_from_the_middle()
{
    std::cout << "test_delete_address_from_the_middle" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "third", 3, 2 }
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "second", 2, 1 }, std::nullopt, 1, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_address_from_the_begining_and_end()
{
    std::cout << "test_delete_address_from_the_middle" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "second", 2, 1 },
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{
        {OPERATION_TYPE::DELETED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt},
        {OPERATION_TYPE::DELETED, { "third", 3, 2 }, std::nullopt, 3, std::nullopt}, };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_full_add_address()
{
    std::cout << "test_full_add_address" <<std::endl;
    auto old = std::vector<Address>{};
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };

    std::vector<OperationData<Address>> added_operations{
        {OPERATION_TYPE::ADDED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt},
        {OPERATION_TYPE::ADDED, { "second", 2, 1 }, std::nullopt, 1, std::nullopt},
        {OPERATION_TYPE::ADDED, { "third", 3, 2 }, std::nullopt, 2, std::nullopt}, };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_add_address_at_the_beginning()
{
    std::cout << "test_add_address_at_the_beginning" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "new_el", 4, 0 },
        { "first", 1, 1 },
        { "second", 2, 2 },
        { "third", 3, 3 }
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "new_el", 4, 0 }, std::nullopt, 0, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_add_address_at_the_end()
{
    std::cout << "test_add_address_at_the_end" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 },
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 },
        { "new_el", 4, 3 }
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED,  { "new_el", 4, 3 }, std::nullopt, 3, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_add_address_at_the_middle()
{
    std::cout << "test_add_address_at_the_middle" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 },
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "new_el", 4, 1 },
        { "second", 2, 2 },
        { "third", 3, 3 },
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED,  { "new_el", 4, 1 }, std::nullopt, 1, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_change_address()
{
    std::cout << "test_change_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "HI", 2, 1 },
        { "third", 3, 2 }
    };

    SortIfNot( old );
    SortIfNot( updated );

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "second", 2, 1 }, Address{ "HI", 2, 1 }, 1, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_moved_address()
{
    std::cout << "test_moved_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "third", 3, 0 },
        { "first", 1, 1 },
        { "second", 2, 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "third", 3, 0 }, std::nullopt,  2, 0 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_no_changes()
{
    std::cout << "test_no_changes" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_repeat_string_address()
{
    std::cout << "test_repeat_string_address" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 },
        { "third", 4, 3 }
    };

    std::vector<OperationData<Address>> added_operations { {OPERATION_TYPE::ADDED,  { "third", 4, 3 }, std::nullopt, 3, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_and_add_element()
{
    std::cout << "run_test_delete_and_add_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "first", 1, 0 },
        { "new", 4, 1 }
    };

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "new", 4, 1 }, std::nullopt, 1, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "second", 2, 1 }, std::nullopt, 1, std::nullopt}, {OPERATION_TYPE::DELETED, { "third", 3, 2 }, std::nullopt, 2, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_and_add_element2()
{
    std::cout << "run_test_delete_and_add_element2" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "new", 4, 0 },
        { "second", 2, 1 },
    };

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "new", 4, 0 }, std::nullopt, 0, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations{
        {OPERATION_TYPE::DELETED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt},
        {OPERATION_TYPE::DELETED, { "third", 3, 2 }, std::nullopt, 2, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_and_change_element()
{
    std::cout << "test_delete_and_change_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "second", 2, 0 },
        { "new", 3, 1 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "third", 3, 2 }, Address{  "new", 3, 1 }, 1, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_and_move_element()
{
    std::cout << "test_delete_and_move_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "third", 3, 0 },
        { "second", 2, 1 },
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "first", 1, 0 }, std::nullopt, 0, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations;
    /* Почему в moved_operations position_start = 1, хотя в old элемент third имеет позицию - 2?
     * Ответ: Если следовать редакционному предписанию (т.е последовательности действий, которые позволят из массива 1, сделать массив 2,
     * то сначала будут выполнять операции добавления и удаления. Соответственно, после этих операций элемент "third" будет на позиции 1.
     * С которой он будет перемещен на позицию 0.*/
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "third", 3, 0 }, std::nullopt,  1, 0 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_add_and_change_element()
{
    std::cout << "test_add_and_change_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "HI", 1, 0 },
        { "new_el", 4, 1 },
        { "second", 2, 2 },
        { "third", 3, 3 }
    };

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "new_el", 4, 1 }, std::nullopt, 1, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "first", 1, 0 }, Address{ "HI", 1, 0 }, 0, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations;

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_add_and_move_element()
{
    std::cout << "test_add_and_move_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "new_el", 4, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 },
        { "first", 1, 3 }
    };

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "new_el", 4, 0 }, std::nullopt, 0, std::nullopt} } ;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations;
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "first", 1, 3 }, std::nullopt,  1, 3 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_change_and_move_element()
{
    std::cout << "test_change_and_move_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "HI", 2, 0 },
        { "third", 3, 1 },
        { "first", 1, 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "second", 2, 1 }, Address{ "HI", 2, 0 }, 0, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "first", 1, 2 }, std::nullopt,  0, 2 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_change_and_move_element2()
{
    std::cout << "test_change_and_move_element2" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 },
        { "third", 3, 2 }
    };
    auto updated = std::vector<Address>
    {
        { "HI", 2, 0 },
        { "first", 1, 1 },
        { "third", 3, 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "second", 2, 1 }, Address{ "HI", 2, 0 }, 0, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "HI", 2, 0 }, std::nullopt,  1, 0 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_change_and_move_element()
{
    std::cout << "test_delete_change_and_move_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 }, // заменяем
        { "second", 2, 1 }, // удаляем
        { "third", 3, 2 },
        { "fourth", 4, 3 } // передвигаем
    };
    auto updated = std::vector<Address>
    {
        { "fourth", 4, 0 },
        { "first", 1, 1 },
        { "Hi", 3, 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "second", 2, 1 }, std::nullopt, 1, std::nullopt}  };
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "third", 3, 2 }, Address{ "Hi", 3, 2 }, 2, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, {  "fourth", 4, 0 }, std::nullopt,  2, 0 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_delete_change_and_move_element2()
{
    std::cout << "test_delete_change_and_move_element2" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 },
        { "second", 2, 1 }, // удаляем
        { "third", 3, 2 },
        { "fourth", 4, 3 } // заменяем и передвигаем
    };
    auto updated = std::vector<Address>
    {
        { "fourth", 4, 0 },
        { "first", 1, 1 },
        { "Hi", 3, 2 }
    };

    std::vector<OperationData<Address>> added_operations;
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, { "second", 2, 1 }, std::nullopt, 1, std::nullopt}  };
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "third", 3, 2 }, Address{ "Hi", 3, 2 }, 2, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, {  "fourth", 4, 0 }, std::nullopt,  2, 0 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_add_change_and_move_element()
{
    std::cout << "test_add_change_and_move_element" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 }, // заменяем
        { "second", 2, 1 },
        { "third", 3, 2 },  // передвигаем
    };
    auto updated = std::vector<Address>
    {
        { "HI", 1, 0 },
        { "third", 3, 1 },
        { "second", 2, 2 },
        { "fourth", 4, 3 }
    };

    std::vector<OperationData<Address>> added_operations{ {OPERATION_TYPE::ADDED, { "fourth", 4, 3 }, std::nullopt, 3, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations;
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, {  "first", 1, 0 }, Address{ "HI", 1, 0 }, 0, std::nullopt} };
    std::vector<OperationData<Address>> moved_operations{ { OPERATION_TYPE::MOVED, { "third", 3, 1 }, std::nullopt,  2, 1 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);
}

void test_all_operations()
{
    std::cout << "test_all_operations" <<std::endl;
    auto old = std::vector<Address>
    {
        { "first", 1, 0 }, // удалили
        { "second", 2, 1 },
        { "third", 3, 2 },
        { "fourth", 4, 3 }
    };
    auto updated = std::vector<Address>
    {
        { "third", 3, 0 }, // переместили
        { "second_new", 2, 1 }, // изменили
        { "fourth", 4, 2 },
        { "fifth", 5, 3 }, // добавили
        { "sixth", 6, 4 } // добавили
    };

    std::vector<OperationData<Address>> added_operations{
        {OPERATION_TYPE::ADDED, {  "fifth", 5, 3 }, std::nullopt, 3, std::nullopt},
        {OPERATION_TYPE::ADDED, { "sixth", 6, 4 }, std::nullopt, 4, std::nullopt} };
    std::vector<OperationData<Address>> deleted_operations{ {OPERATION_TYPE::DELETED, {"first", 1, 0 }, std::nullopt, 0, std::nullopt} };
    std::vector<OperationData<Address>> chanded_operations{ {OPERATION_TYPE::CHANGED, { "second", 2, 1 }, Address{ "second_new", 2, 1 }, 1, std::nullopt} } ;
    std::vector<OperationData<Address>> moved_operations{
        {OPERATION_TYPE::MOVED, {  "fourth", 4, 2 }, std::nullopt,  4, 2 },
        {OPERATION_TYPE::MOVED, {"third", 3, 0  }, std::nullopt,  1, 0 } };

    auto res = DifferAddress().Compare( old, updated );

    assert(res.mAddedOperations == added_operations);
    assert(res.mDeletedOperations == deleted_operations);
    assert(res.mChandedOperations == chanded_operations);
    assert(res.mMovedOperations == moved_operations);

    /* Проверка редакционного предписания
    { "first", 1, 0 },
    { "second", 2, 1 },
    { "third", 3, 2 },
    { "fourth", 4, 3 }

    -----------Добавление элемента fifth, 5 на позицию 3

    { "first", 1, 0 },
    { "second", 2, 1 },
    { "third", 3, 2 },
    {  "fifth", 5, 3 }
    { "fourth", 4, 4 }

    -----------Добавление элемента sixth, 6 на позицию 4

    { "first", 1, 0 },
    { "second", 2, 1 },
    { "third", 3, 2 },
    {  "fifth", 5, 3 }
    { "sixth", 6, 4 }
    { "fourth", 4, 5 }

    -----------Удаление элемента first, 1

    { "second", 2, 0 },
    { "third", 3, 1 },
    {  "fifth", 5, 2 }
    { "sixth", 6, 3 }
    { "fourth", 4, 4 }

     ----------- Изменение элемента second, 2 на second_new

    { "second_new", 2, 0 },
    { "third", 3, 1 },
    {  "fifth", 5, 2 }
    { "sixth", 6, 3 }
    { "fourth", 4, 4 }

     ----------- Перемещение элемента fourth, 4 с позиции 4 на позицию 2


    { "second_new", 2, 0 },
    { "third", 3, 1 },
    { "fourth", 4, 2 }
    {  "fifth", 5, 3 }
    { "sixth", 6, 4 }

    ----------- Перемещение элемента third, 3 с позиции 1 на позицию 0

     { "third", 3, 0 },
    { "second_new", 2, 1 },
    { "fourth", 4, 2 }
    {  "fifth", 5, 3 }
    { "sixth", 6, 4 }

           Совпали

    { "third", 3, 0 },
    { "second_new", 2, 1 },
    { "fourth", 4, 2 },
    { "fifth", 5, 3 },
    { "sixth", 6, 4 }

    */
}

void run_simple_tests()
{
    test_full_delete_address();
    test_delete_address_from_the_beginning();
    test_delete_address_from_the_end();
    test_delete_address_from_the_middle();
    test_full_add_address();
    test_add_address_at_the_beginning();
    test_add_address_at_the_end();
    test_change_address();
    test_moved_address();
    test_no_changes();
    test_repeat_string_address();
}

void run_complex_tests()
{
    test_delete_and_add_element();
    test_delete_and_add_element2();
    test_delete_and_change_element();
    test_delete_and_move_element();
    test_add_and_change_element();
    test_add_and_move_element();
    test_change_and_move_element();
    test_change_and_move_element2();
    test_delete_change_and_move_element();
    test_delete_change_and_move_element2();
    test_add_change_and_move_element();
    test_all_operations();
}

int main()
{
    run_simple_tests();
    run_complex_tests();
}
