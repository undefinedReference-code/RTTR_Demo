#include <rttr/registration>
#include <iostream>

class MyClass {
public:
    MyClass() = default; 
    void greet() { 
        std::cout << "RTTR Static Link Works!" << std::endl; 
    }
};

RTTR_REGISTRATION {
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