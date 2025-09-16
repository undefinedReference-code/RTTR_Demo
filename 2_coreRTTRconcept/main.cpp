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
        // 1. get type
        rttr::type t = rttr::type::get_by_name("Person");
        if (!t.is_valid()) { std::cout << "Type not found\n"; return 0; }

        // 2. (constructor -> variant)
        rttr::variant obj = t.create({ std::string("Alice"), 20 });
        if (!obj.is_valid()) { std::cout << "Create failed\n"; return 0; }

        // 3. set property value
        rttr::property prop_name = t.get_property("name");
        prop_name.set_value(obj, std::string("Bob"));

        // 4. invoke method
        rttr::method m = t.get_method("greet");
        m.invoke(obj); // Hi, I am Bob, 20

        // 5. get property value
        rttr::property age_prop = t.get_property("age");
        rttr::variant age_v = age_prop.get_value(obj);
        std::cout << "Age = " << age_v.get_value<int>() << "\n";

        rttr::variant age_prop_variant = t.get_property("age");
        rttr::variant age_v_from_variant = age_prop_variant.get_value<rttr::property>().get_value(obj);
        std::cout << "Age Using variant as property = " << age_v_from_variant.get_value<int>() << "\n";

        // class in namespace
        rttr::type t_Foo = rttr::type::get_by_name("myns::Foo");
        if (!t_Foo.is_valid())
        {
            std::cout << "Type not found!" << std::endl;
            return 0;
        }
        std::cout << "Type found: " << t.get_name() << std::endl;
        rttr::variant obj2 = t.create({ 42 });
        rttr::property p = t.get_property("value");
        std::cout << "value = " << p.get_value(obj2).to_int() << std::endl;

        return 0;
    }