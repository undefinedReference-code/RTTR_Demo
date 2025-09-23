#include <rttr/registration>
#include <rttr/type>
#include <iostream>
#include <string>
#include <vector>

template<typename T>
class MyTemplateClass
{
public:
    void setValue(const T& value) { m_value = value; }
    MyTemplateClass(const T& value) : m_value(value) {}
    MyTemplateClass() : m_value(0) {}

    T getValue() const { return m_value; }
private:
    T m_value;
};

namespace rttr
{
    template<typename T>
    struct wrapper_mapper<MyTemplateClass<T>>
    {
        using wrapped_type = T;
        using type = MyTemplateClass<T>;

        static RTTR_INLINE wrapped_type get(const type& obj)
        {
            return obj.getValue();
        }

        static RTTR_INLINE type create(const wrapped_type& value)
        {
            return type(value);
        }
    };
}

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

namespace myns {
    class Foo
    {
    public:
        Foo() = default;
        Foo(int v) : value(v) {}
        int get_value() const { return value; }
        void set_value(int v) { value = v; }
    private:
        int value = 0;
    };
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Person>("Person")
        .constructor<>()                     // constructor()
        .constructor<std::string,int>()      // constructor(string,int)
        .property("name", &Person::get_name, &Person::set_name)
        .property("age",  &Person::get_age,  &Person::set_age)
        .method("greet",  &Person::greet);
        
    registration::class_<myns::Foo>("myns::Foo")
        .constructor<>()
        .constructor<int>()
        .property("value", &myns::Foo::get_value, &myns::Foo::set_value);

    registration::class_<MyTemplateClass<int>>("MyTemplateClass<int>")
        .constructor<>()
        .method("setValue", &MyTemplateClass<int>::setValue)
        .method("getValue", &MyTemplateClass<int>::getValue);
}

void printType(rttr::type t) {
    std::cout << "get_raw_type: " << t.get_raw_type().get_name() << std::endl
        << "get_wrapped_type: " << t.get_wrapped_type().get_name() << std::endl;
    // get_raw_array_type is private, you should modify it to public if you want to run the follow for test;
    std::cout << "get_raw_array_type: " << t.get_raw_array_type().get_name() << std::endl;

}

int main()
{
    std::cout << "======= demo how to obtain rttr::type ========" << std::endl;
    rttr::type person_type = rttr::type::get<Person>();
    rttr::type foo_type = rttr::type::get<myns::Foo>();
    rttr::type int_type = rttr::type::get<int>();
    std::cout << person_type.get_name() << " " << foo_type.get_name() << " " << int_type.get_name() << std::endl;

    Person person("Alice", 25);
    rttr::type person_instance_type = rttr::type::get(person);
    myns::Foo foo(42);
    rttr::type foo_instance_type = rttr::type::get(foo);
    int i = 2;
    rttr::type int_instance_type = rttr::type::get(i);
    std::cout << person_instance_type.get_name() << " " << foo_instance_type.get_name() << " " << int_instance_type.get_name() << std::endl;

    rttr::type person_by_name = rttr::type::get_by_name("Person");
    rttr::type foo_by_name = rttr::type::get_by_name("myns::Foo");
    rttr::type int_by_name = rttr::type::get_by_name("int");
    std::cout << person_by_name.get_name() << " " << foo_by_name.get_name() << " " << int_by_name.get_name() << std::endl;

    std::cout << "======= demo get_raw_type, get_wrapped_typ, get_raw_array_type ========" << std::endl;

    const int _const_int_val = 0;
    rttr::type const_int_val_instance_type = rttr::type::get(_const_int_val);
    printType(const_int_val_instance_type);

    int _int_array_val[100];
    rttr::type _int_array_type = rttr::type::get(_int_array_val);
    printType(_int_array_type);

    std::shared_ptr<int> si;
    rttr::type _si_type = rttr::type::get(si);
    printType(_si_type);

    MyTemplateClass<int> _int_template;
    rttr::type _int_template_type = rttr::type::get(_int_template);
    printType(_int_template_type);
}