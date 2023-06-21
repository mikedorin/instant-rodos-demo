//
// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2019 Michael Dorin
 *
 */

// Created by Michael Dorin on 8/7/19.
// Inspired by work of Doug Lea
// http://gee.cs.oswego.edu/dl/classes/EDU/oswego/cs/dl/util/concurrent/Heap.java
//

#pragma GCC optimize("O0")
#ifndef RODOS_KIPU2_TINYHEAP_H
#define RODOS_KIPU2_TINYHEAP_H

#include "LocalDefines.h"
#include "rodos.h"
#include "TinyHeap.h"
#include <stdlib.h>
//#include <cstdlib>


template <class Type, int capacity>  class TinyHeap {

private:

    int count=0;
    int max=0;
    Type * items[capacity];

public:

    TinyHeap()
    {
	    count = 0;
	    clear();
        max = capacity;
    }

    Type * peek(int index)
    {
        if (index < count)
        {
            return items[index];
        }

        return NULL;
    }

    Type * peek() {
        if (count > 0)
        {
            return items[0];
        }

        return NULL;
    }

    int size()
    {
        return count;
    }

    void clear()
    {
        for (int i = 0; i < count; ++i)
        {
            items[i] = NULL;
        }
        count = 0;
    }

    int nodeParent(int node)
    {
        return (node - 1) / 2;
    }

    int nodeLeft(int node)
    {
        return 2 * node + 1;
    }

    int nodeRight(int node)
    {
        return 2 * (node + 1);
    }


    int insert(Type * newNode)
    {

        int k=count;

        if (count == (max -1))
        {
            return -1;
        }

        count++;

        while(k>0)
        {
            int parent = nodeParent(k);

            if (*newNode < *items[parent])
            {
                items[k] = items[parent];
                k = parent;
            }
            else
            {
                break;
            }
        }
        items[k] = newNode;
        return k;
    }

    Type * extract()
    {

        if (count < 1)
        {
            return NULL;
        }

        int cursorNode = 0;
        Type * least = items[cursorNode];
        count--;
        Type * last = items[count];

        items[count] = NULL;

        while(true)
        {
            int left = nodeLeft(cursorNode);
            if (left >= count)
            {
                break;
            }
            else
            {
                int right = nodeRight(cursorNode);
                //* mad note, Types[left] time less than Types[right] time]
                int child = (right >= count || (*items[left]<*items[right])) ? left : right;

                if (*last> *items[child]) //mad note, last time, greater than Types[child] last time
                {
                    items[cursorNode] = items[child];
                    cursorNode = child;
                }
                else
                {
                    break;
                }
            }
        }
        items[cursorNode] = last;
        return least;
    }

};


#endif //RODOS_KIPU2_TINYHEAP_H
