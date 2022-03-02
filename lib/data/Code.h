#pragma once 
#include <Arduino.h>

enum CODES {
    NONE, 
    FAIL,
    CORRECT,
    DONE
};

class Codes
{
private:
    size_t _size;
    String* _codes;
    CODES* _states;
    String _input;
    size_t _count;
    const size_t _codeSize = 6;
public:
    Codes(size_t size, const String* code):
        _size(size), 
        _codes(new String[size]), 
        _states(new CODES[size]), 
        _input(""),
        _count(0)
    {
        for (size_t i = 0; i < _size; i++)
        {
            _codes[i] = code[i];
            _states[i] = CODES::NONE;
        }
    }
    void reset()
    {
        _input = "";
        _count = 0;
    }
    CODES getState(size_t index)
    {
        return _states[index];      
    }
    int checkIsCorrect(String value)
    {
        _count++;
        _input += value;
        if (_count < _codeSize)
            return -1;     

        for (size_t i = 0; i < _size; i++)
        {
            if (_input == _codes[i])
            {
                reset();
                _states[i] = CODES::CORRECT;
                return i;            
            }            
        }        
        reset();
        return -2;     
    }
};