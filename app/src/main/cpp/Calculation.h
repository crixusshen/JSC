//
// Created by dfzq on 2019-05-23.
//

#ifndef MYJNIAPPLICATION_CALCULATION_H
#define MYJNIAPPLICATION_CALCULATION_H

#include "JavaScriptCore/JavaScript.h"

class Calculation {
public:
    // 成员变量
    JSContextRef context;
    JSObjectRef global;
    int numValue;

    // 构造函数
    Calculation();
    // 析构函数
    ~Calculation();

    void TestStatic();
    void TestDynamic();
    int num();
    void set_num(int num);
    JSValueRef runScript(const char* buffer);

private:
    void Initialize();
};


#endif //MYJNIAPPLICATION_CALCULATION_H
