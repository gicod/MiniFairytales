#pragma once 
#include <Arduino.h>

enum HINT_STATE{
    NONE,
    ACTIVATE,
    DONE
};

class Hint
{
private:
    size_t _size;
    HINT_STATE* _state;

    size_t _count = 0;

    const size_t _countMax = 6;
    int* _indexHint;
public:
    Hint(const size_t size):
        _size(size), 
        _state(new HINT_STATE[size]), 
        _indexHint(new int[_countMax])
    {
        for (size_t i = 0; i < _size; i++)
            _state[i] = NONE;
        for (size_t i = 0; i < _countMax; i++)
            _indexHint[i] = -1;
    }

    size_t getCount()
    {
        return _count;
    }

    int getIndexLed(int index)
    {
        return _indexHint[index];

        // for (size_t i = 0; i < _countMax; i++)
        //     if (_indexHint[i] == index)
        //         return i;        

        // return -1;
    }

    size_t getIndexHint(size_t index)
    {
        return _indexHint[index];
    }

    void newHint()
    {
        if (_count >= _countMax)
            _count = 0;
        for (size_t i = 0; i < _size; i++)
        {
            if (_state[i] == NONE)
            {
                _state[i] = ACTIVATE;
                _indexHint[_count] = i;
                _count++;    
                return;            
            }            
        }        
    }

    void isDone(size_t index)
    {
        _state[index] = DONE;
        

        // _count = 0;
        // for (size_t i = 0; i < _size; i++)
        // {
        //     if (_state[i] == ACTIVATE)
        //     {
        //         _indexHint[_count] = i;
        //         _count++;
        //     }            
        // }
    }
    ~Hint();
};
