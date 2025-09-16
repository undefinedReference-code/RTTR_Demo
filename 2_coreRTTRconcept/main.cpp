#include <rttr/registration>
#include <rttr/type>
#include <iostream>
#include <string>

class Person
{
public:
    Person() = default;
    Person(std::string name, int age) : name_(std::move(name)), age_(age) {}
    void greet() const { std::cout << "Hi, I am " << name_ << ", " << age_ << "\n"; }
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& n) { name_ = n; }
    int get_age() const { return age_; }
    void set_age(int a) { age_ = a; }
private:
    std::string name_;
    int age_ = 0;
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Person>("Person")
        .constructor<>()                     // constructor()
        .constructor<std::string,int>()      // constructor(string,int)
        .property("name", &Person::get_name, &Person::set_name)
        .property("age",  &Person::get_age,  &Person::set_age)
        .method("greet",  &Person::greet);
}

int main()
{
    // 1. 获取类型
    rttr::type t = rttr::type::get_by_name("Person");
    if (!t.is_valid()) { std::cout << "Type not found\n"; return 0; }

    // 2. 构造对象 (constructor -> variant)
    rttr::variant obj = t.create({ std::string("Alice"), 20 });
    if (!obj.is_valid()) { std::cout << "Create failed\n"; return 0; }

    // 3. 访问属性
    auto prop_name = t.get_property("name");
    prop_name.set_value(obj, std::string("Bob"));

    // 4. 调方法
    auto m = t.get_method("greet");
    m.invoke(obj); // Hi, I am Bob, 20

    // 5. 读取属性
    auto age_prop = t.get_property("age");
    rttr::variant age_v = age_prop.get_value(obj);
    std::cout << "Age = " << age_v.get_value<int>() << "\n";

    return 0;
}