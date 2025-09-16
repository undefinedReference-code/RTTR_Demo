#include <rttr/registration>
#include <iostream>

class MyClass {
public:
    MyClass() = default; 
    void greet() { 
        std::cout << "RTTR Static Link Works!" << std::endl; 
    }
};

static void rttr_auto_register_reflection_function_(); namespace {
    struct rttr__auto__register__ {
        rttr__auto__register__() {
            rttr_auto_register_reflection_function_();
        }
    };
} 
static const rttr__auto__register__ auto_register__12; 
static void rttr_auto_register_reflection_function_() {
    rttr::registration::class_<MyClass>("MyClass")
        .constructor<>()  
        .method("greet", &MyClass::greet);
}

int main() {
    
    rttr::type t = rttr::type::get_by_name("MyClass");
    if (!t.is_valid()) {
        std::cerr << "Error: Type 'MyClass' not registered!" << std::endl;
        return 1;
    }

    rttr::variant variable = t.create();
    if (!variable.is_valid()) {
        std::cerr << "Object creation failed!" << std::endl;
        return 1;
    }

    MyClass* obj = variable.get_value<MyClass*>();
    if (obj) {
        obj->greet(); 
    }

    return 0;
}