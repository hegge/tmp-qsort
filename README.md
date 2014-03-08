tmp-qsort
=========

Compile time quicksort


> Metaprogramming allows us to show off our cleverness. Obviously, that is to be avoided.
    -- Bjarne Stroustrup, The C++ Programming Language, fourth edition.


Have you ever started learning Haskell because you wanted to understand C++
template metaprogramming? And then you came across some variant of quicksort in
Haskell, like

    quicksort :: Ord a => [a] -> [a]
    quicksort []     = []
    quicksort (p:xs) = (quicksort lesser) ++ [p] ++ (quicksort greater)
        where
            lesser  = filter (< p) xs
            greater = filter (>= p) xs

and wondered what that would look like in TMP?


References
==========

* Andrei Alexandrescu, Modern C++ Design
* David Abrahams and Aleksey Gurtvoy, C++ Template Metaprogramming
* http://bartoszmilewski.com/2009/10/21/what-does-haskell-have-to-do-with-c/
* https://github.com/beark/ftl
