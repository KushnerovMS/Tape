#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <algorithm>

#include "Tape.h"
#include "Error.h"
#include "Config.h"

namespace Tape {

//const size_t AVAILABLE_MEMORY = 128;

template<typename T, typename Cmp>
void NaturalSorting(Tape<T>& in, Tape<T>& out, Cmp cmp)
{
    if (in.getSize() > out.getSize())
        throw Error("Sorting: Size of output tape less than size of input tape");
    in.toBeg();
    out.toBeg();

    std::filesystem::create_directory("tmp");

{ // scope of tmp tapes


    size_t size = in.getSize();
    Tape<T> secTapes[] = { CreateTape<T>("tmp/0", size),        // help tapes
                           CreateTape<T>("tmp/1", size) };
    Tape<size_t> segSizes = CreateTape<size_t>("tmp/2", size);  // tape, that saves segment sizes

    ///////////////////////////////////////////////////////////////////////////////////////////
    // presorting
    ///////////////////////////////////////////////////////////////////////////////////////////

    size_t bufSize = config.SortAvailableMemory / sizeof(T);
    T* buf = new T[bufSize];
    while (! in.atEnd())
    {
        size_t i;
        for (i = 0; i < bufSize; ++ i, in.Next())
            buf[i] = in.getItem();

        std::sort(buf, buf + i, cmp);

        for (size_t j = 0; j < i; ++ j, out.Next())
            out.setItem(buf[j]);
    }
    delete[] buf;

    bool isSorted = false;
    while (! isSorted)
    {
        out.toBeg();
        secTapes[0].toBeg();
        secTapes[1].toBeg();
        segSizes.toBeg();

    ///////////////////////////////////////////////////////////////////////////////////////////
    //  Separating array by subarrays with already sorted order.
    //  Subarrys stores in secTapes alternatively
    ///////////////////////////////////////////////////////////////////////////////////////////

        T preItem = out.getItem();
        secTapes[0].setItem(preItem);
        secTapes[0].Next();

        size_t segCount = 1;

        int k = 0;
        for (size_t i = 1; ; secTapes[k].Next(), ++ i)
        {
            if (! out.Next())
            {
                segSizes.setItem(i);
                break;
            }

            T item = out.getItem();

            if (cmp(item, preItem))
            {
                segSizes.setItem(i);
                segSizes.Next();
                ++ segCount;
                i = 0;
                k = k ? 0 : 1;
            }

            secTapes[k].setItem(item);
            preItem = item;
        }

        if (segCount <= 2)
            isSorted = true;

    /////////////////////////////////////////////////////////////////////////////////////////

        /*
        std::cout << secTapes[0] << std::endl;
        std::cout << secTapes[1] << std::endl;
        std::cout << segSizes << std::endl;
        std::cout << segCount << std::endl << std::endl;
        */

        out.toBeg();
        secTapes[0].toBeg();
        secTapes[1].toBeg();
        segSizes.toBeg();

    /////////////////////////////////////////////////////////////////////////////////////////
    // Merge subarrays into main array by pairs
    /////////////////////////////////////////////////////////////////////////////////////////

        do
        {
            size_t size0 = segSizes.getItem();
            segSizes.Next();
            -- segCount;
            size_t size1 = 0;
            if (segCount > 0)
            {
                size1 = segSizes.getItem();
                -- segCount;
            }
            segSizes.Next();

            //std::cout << size0 << ' ' << size1 << std::endl;

            T items[] = {secTapes[0].getItem(), secTapes[1].getItem()};
            for (size_t i[2] = {}; ; ++ i[k])
            {
                if (i[0] < size0)
                    if (i[1] < size1)
                        if (cmp(items[0], items[1]))
                            k = 0;
                        else
                            k = 1;
                    else
                        k = 0;
                else
                    if (i[1] < size1)
                        k = 1;
                    else
                        break;

           //     std::cout << i[0] << ' ' << i[1] << " : " << items[0] << ", " << items[1] << " : " << items[k] << std::endl;
                out.setItem(items[k]);
                out.Next();
                secTapes[k].Next();
                items[k] = secTapes[k].getItem();
            }
        }
        while (! out.atEnd());

    /////////////////////////////////////////////////////////////////////////////////////////

        //std::cout << out << std::endl << std::endl;

    }


} // scope of tmp tapes

    std::filesystem::remove_all("tmp");
}

} // namespace Tape
