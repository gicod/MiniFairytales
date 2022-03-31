#pragma once
#include <Arduino.h>

class CodeWithError
{
public:
    enum CODE_STATE {
        NONE,
        FAIL,
        CORRECT,
        DISABLE
    };
    CodeWithError(const String code, size_t errorCount, const String* codesError):
        _code(code), _errorCount(errorCount), _codesError(new String[_errorCount])
    {
        for (size_t i = 0; i < _errorCount; i++)
        {
            _codesError[i] = codesError[i];
        }        
    }
    void reset()
    {
        _input = "";
        _count = 0;
    }
    CODE_STATE check(String value)
    {
        if (_state != NONE)
        {
            _state = DISABLE;
            return _state;            
        }
        
        _count++;
        _input += value;
        if (_count < _codeSize)
            return _state;     
        
        if (_input == _code)
        {
            _state = CORRECT;
            return _state;            
        }

        for (size_t i = 0; i < _errorCount; i++)
        {
            if (_input == _codesError[i])
            {
                _state = FAIL;
                return _state;            
            }
        }       
        reset(); 
        return _state;         
    }
    ~CodeWithError();
private:
    String _code;
    size_t _errorCount;
    String* _codesError;
    String _input = "";
    size_t _count = 0;
    const size_t _codeSize = 6;
    CODE_STATE _state = NONE;
};