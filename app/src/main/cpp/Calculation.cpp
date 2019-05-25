//
// Created by dfzq on 2019-05-23.
//

#include <string>
#include "Calculation.h"
#include "Log.h"
#include "JavaScriptCore/JavaScript.h"

// calculation.testStatic() 调用时对应的 c++ 函数钩子
// 定义静态全局对象->函数结构->回调
static JSValueRef TestStaticCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    // 获取 JS 引擎对象中关联的 c++ 对象
    Calculation *calculation = static_cast<Calculation *>(JSObjectGetPrivate(thisObject));
    // 通过关联的this指针对象去调用 c++ 中已实现的回调函数
    calculation->TestStatic();
    return JSValueMakeNull(ctx);
}

// console.log(calculation.num) 调动时对应触发的 c++ 函数钩子
// 定义静态全局对象->变量结构->getter回调
static JSValueRef GetNumCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
    Calculation *calculation = static_cast<Calculation *>(JSObjectGetPrivate(object));
    int num = calculation->num();
    return JSValueMakeNumber(ctx, num);
}

// calculation.num = 99 时会触发的 c++ 函数钩子
// 定义静态全局对象->变量结构->setter回调
static bool SetNumCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
    Calculation *calculation = static_cast<Calculation *>(JSObjectGetPrivate(object));
    calculation->set_num(JSValueToNumber(ctx, value, NULL));
    return true;
}

// calculation.testDynamic() 调用时对应的 c++ 函数钩子
static JSValueRef TestDynamicCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    Calculation *calculation = static_cast<Calculation *>(JSObjectGetPrivate(thisObject));
    calculation->TestDynamic();
    return JSValueMakeNull(ctx);
}

// 定义静态全局对象->函数结构
static JSStaticFunction s_calculation_function_[] = {
        {"testStatic", TestStaticCallback, kJSClassAttributeNone},
        {0,            0,                  0}
};

// 定义静态全局对象->变量结构
static JSStaticValue s_calculation_values_[] {
        {"num", GetNumCallback, SetNumCallback, kJSPropertyAttributeDontDelete},
        {0,     0,              0,              0}
};

void Calculation::Initialize() {
    // 初始化 JSC 引擎

    // 创建js执行上下文组：它可以将不同的JSContext关联起来，在同一个组的JSContext可共享js对象
    JSContextGroupRef context_group = JSContextGroupCreate();

    // 首先创建一个类：
    // 创建全局 global 类的定义
    JSClassDefinition global_class_definition = kJSClassDefinitionEmpty;
    // 创建 global 对象的类
    JSClassRef global_class = JSClassCreate(&global_class_definition);

    // 在js执行上下文环境(Group)中创建一个全局的js执行上下文
    context = JSGlobalContextCreateInGroup(context_group, global_class);
    // 获取js执行上下文的全局对象
    global = JSContextGetGlobalObject(context);

    // 创建 calculation 类的定义
    JSClassDefinition calculation_class_definition = kJSClassDefinitionEmpty;
    // 设置类的对应函数名和参数名的结构
    calculation_class_definition.staticFunctions = s_calculation_function_;
    calculation_class_definition.staticValues = s_calculation_values_;
    // 设置类名
    calculation_class_definition.className = "Calculation";
    // 创建 JS 引擎的类
    JSClassRef calculation_class_ref = JSClassCreate(&calculation_class_definition);
    JSObjectRef calculation = JSObjectMake(context, calculation_class_ref, NULL);
    // 关联 c++ 对象和 JS 引擎对象
    JSObjectSetPrivate(calculation, this);
    JSClassRelease(calculation_class_ref);

    // 动态注册函数钩子
    JSStringRef dynamic_test_func_name = JSStringCreateWithUTF8CString("testDynamic");
    JSObjectRef dynamic_test_func = JSObjectMakeFunctionWithCallback(context,
                                                                     dynamic_test_func_name,
                                                                     TestDynamicCallback);
    // 为 Calculation 类添加一个原型属性
    JSObjectSetProperty(context,
                        calculation,
                        dynamic_test_func_name,
                        dynamic_test_func,
                        kJSPropertyAttributeDontDelete,
                        NULL);

    // 最后向 global 全局对象中加入 Calculation 对象
    JSStringRef calculation_str_ref = JSStringCreateWithUTF8CString("calculation");
    JSObjectSetProperty(context,
                        global,
                        calculation_str_ref,
                        calculation,
                        kJSPropertyAttributeDontDelete,
                        NULL);
    JSStringRelease(calculation_str_ref);
}

Calculation::Calculation() {
    LOGD("Crixusshen-C++ Calculation Constructor");
    // 初始化构建 JavaScript 引擎
    Initialize();
}

Calculation::~Calculation() {
    LOGD("Crixusshen-C++ ~Calculation");
}

void Calculation::TestStatic() {
    LOGD("Crixusshen-C++ call TestStatic");
}

void Calculation::TestDynamic() {
    LOGD("Crixusshen-C++ call TestDynamic");
}

int Calculation::num() {
    LOGD("Crixusshen-C++ call num: %d", numValue);
    return numValue;
}

void Calculation::set_num(int num) {
    numValue = num;
    LOGD("Crixusshen-C++ call set_num: %d", num);
}

JSValueRef Calculation::runScript(const char *buffer) {
    JSStringRef script = ::JSStringCreateWithUTF8CString(buffer);
    JSValueRef exception = nullptr;
    JSValueRef ret = JSEvaluateScript(context, script, global, 0, 0, &exception);
    JSStringRelease(script);
    if (nullptr != exception) {
        JSStringRef errorStr = ::JSValueToStringCopy(context, exception, nullptr);
        if (nullptr != errorStr) {
            std::string lineError = "";
            lineError.resize(::JSStringGetMaximumUTF8CStringSize(errorStr));
            lineError.resize(::JSStringGetUTF8CString(errorStr, &lineError[0], lineError.length()));
            ::JSStringRelease(errorStr);
            LOGE("Crixusshen-C++ runScript error: %s", lineError.c_str());
        }
    }
    return ret;
}