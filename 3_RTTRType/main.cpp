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
    }

    int main()
    {
        
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


    }