#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class Integer{
public:
    int i;
    // Default constructor
    Integer(int integer) {
        std::cout << "Integer default constructor" << std::endl;
        i = integer;
        printObj("this");
    }

    // Copy constructor
    Integer(Integer const& integer) {
        std::cout << "Integer copy constructor" << std::endl;
        i = integer.i;
        printObj("this");
        printObj("other", integer);
    }

    // Move constructor
    Integer(Integer && integer)
    {
        std::cout << "move constructor" << std::endl;
        i = std::move(integer.i);
        printObj("this");
        printObj("other", integer);
    }

    // destructor
    ~Integer()
    {
        std::cout << "Integer destructor" << std::endl;
        printObj("this");
    }

    // Assign by moving
    Integer& operator=(Integer&& o)
    {
        std::cout << "assign by moving" << std::endl;
        i = std::move(o.i);
        printObj("this");
        printObj("other", o);
        return *this;
    }

    // Assign by coping
    Integer& operator=(const Integer& o)
    {
        std::cout << "assign by coping" << std::endl;
        i = o.i;
        printObj("this");
        printObj("other", o);
        return *this;
    }

    bool operator==(Integer const& o)
    {
        std::cout << "Comparison operator" << std::endl;
        return (this->i == o.i);
    }

    void printObj(std::string const& name) {
        printObj("this", *this);
    }

    static void printObj(std::string const& name, Integer const& obj) {
        std::cout << "\tname=" << name << ", @=Ox" << std::hex << size_t(&obj) << ", @i=Ox" << size_t(&obj.i) << ", i=" << std::dec << obj.i << std::endl;
    }
};

template<typename T>
class CollectionType {
    std::vector<T> _vec;

public:
    CollectionType() {
        std::cout << "default constructor" << std::endl;
        _vec.reserve(2);
        printVec(_vec);
    }

    CollectionType(CollectionType<T> const& collection) {
        std::cout << "copy constructor" << std::endl;
        _vec = collection._vec;
        printVec(_vec);
    }

    static void printVec(const std::vector<T> & _vec)
    {
        int cap = _vec.capacity();
        int sz = _vec.size();
        std::cout << "\tvec size=" << sz << ", cap=" << cap << std::endl;
        for(int i = 0; i < cap; i++)
        {
            const T & o = _vec[i];
            std::string ss("\t\t@vec[");
            ss = ss + std::to_string(i) + "]";

            std::cout << ss << " \t @hex=0x" << std::hex << size_t(&o) << ", sizeof= " << std::dec << sizeof(o) << " - value = " << o.i << std::endl;
        }
    }

    void add(T const& value) {
        _vec.push_back(value);
        printVec(_vec);
    }

    /*
    void remove(T const& value) {
        auto it = std::find(_vec.cbegin(), _vec.cend(), value);
        if(it != _vec.cend()) {
            _vec.erase(it);
        }
        printVec(_vec);
    }
    */
};

void * operator new(size_t sz)
{
    void * mem = malloc(sz);
    std::cout << "\tnew @" << std::hex << mem << std::dec << " sz = " << sz << std::endl;
    return mem;
}

void operator delete(void * mem)
{
    free(mem);
    std::cout << "\tdelete @" << std::hex << mem << std::dec << std::endl;
}

void printLine(const char * cmd, int line) {
    std::cout << "[LINE " << line << "] : " << cmd << std::endl;
}

#define L(cmd) printLine(#cmd, __LINE__); cmd

int main(void) {
    L(CollectionType<Integer> collection);
    L(collection.add(pow(2, 1)));
    L(collection.add(pow(2, 2)));
    L(collection.add(pow(2, 3)));
    L(collection.add(pow(2, 4)));
    L(collection.add(pow(2, 5)));
    //L(collection.remove(2));

    return 0;
}