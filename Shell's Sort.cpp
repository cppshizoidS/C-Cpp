template<typename RandomAccessIterator, typename Compare> //сортировка Шелла
void shell_sort( RandomAccessIterator first, RandomAccessIterator last, Compare comp )
{
    for( auto d = ( last - first ) / 2; d != 0; d /= 2 )
//нужен цикл для first = a[0..d-1]
        for( auto i = first + d; i != last; ++i )
            for( auto j = i; j - first >= d && comp( *j, *( j - d ) ); j -= d )
                std::swap( *j, *( j - d ) );
}