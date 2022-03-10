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

    size_t _countLed;
    int* _indexHintForLed;
public:
    Hint(const size_t size, const size_t countLed):
        _size(size), 
        _state(new HINT_STATE[size]), 
        _countLed(countLed),
        _indexHintForLed(new int[_countLed])
    {
        for (size_t i = 0; i < _size; i++)
        {
            _state[i] = NONE;
        }
        for (size_t i = 0; i < _countLed; i++)
            _indexHintForLed[i] = -1;
    }

    HINT_STATE getState(int index)
    {
        return _state[index];
    }

    int getIndexLed(int indexHint)
    {
        for (size_t i = 0; i < _countLed; i++)
            if (_indexHintForLed[i] == indexHint)
                return i;        

        return -1;
    }

    int getIndexHint(size_t indexLed)
    {
        return _indexHintForLed[indexLed];
    }

    bool newHint()
    {
        if (_count >= _countLed)
            _count = 0;
        for (size_t i = 0; i < _size; i++)
        {
            if (_state[i] == NONE)
            {
                for (size_t k = _count; k < (_count + _countLed); k++)
                {
                    size_t count = k % _countLed;
                    if (_indexHintForLed[count] == -1)
                    {
                        _indexHintForLed[count] = i;
                        _count = count;                        
                        _state[i] = ACTIVATE;
                        return true;            
                    }
                }                
            }            
        }        
        return false;            
    }

    void isDone(size_t indexHint)
    {
        _state[indexHint] = DONE;
        _indexHintForLed[getIndexLed(indexHint)] = -1;
    }
    ~Hint();
};
