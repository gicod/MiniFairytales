#pragma once 
#include <Arduino.h>

class Codes
{
private:
    size_t _size;
    String* _codes;
    String _input = "";
    size_t _count = 0;
    const size_t _codeSize = 6;
public:
    Codes(size_t size, const String* code):
        _size(size), 
        _codes(new String[size])
    {
        for (size_t i = 0; i < _size; i++)
        {
            _codes[i] = code[i];
        }
    }
    void reset()
    {
        _input = "";
        _count = 0;
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
                return i;            
            }            
        }        
        reset();
        return -2;     
    }
};