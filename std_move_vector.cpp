#include <iostream>
#include <vector>
#include <string>

class ObjectType
{
public:
    // default constructor
    ObjectType(const std::string & s) {
        std::cout << "default constructor" << std::endl;
        s_ = s;
        printObj("this");
    }

    // copy constructor
    ObjectType(const ObjectType & o)
    {
        std::cout << "copy constructor" << std::endl;
        s_ = o.s_;
        printObj("this");
        printObj("other", o);
    }

    // move constructor
    ObjectType(ObjectType && o)
    {
        std::cout << "move constructor" << std::endl;
        s_ = std::move(o.s_);
        printObj("this");
        printObj("other", o);
    }

    // destructor
    ~ObjectType()
    {
        std::cout << "destructor" << std::endl;
        printObj("this");
    }

    // assign by moving
    ObjectType& operator=(ObjectType&& o)
    {
        std::cout << "assign by moving" << std::endl;
        s_ = std::move(o.s_);
        printObj("this");
        printObj("other", o);
        return *this;
    }

    // assign by coping
    ObjectType& operator=(const ObjectType& o)
    {
        std::cout << "assign by coping" << std::endl;
        s_ = o.s_;
        printObj("this");
        printObj("other", o);
        return *this;
    }

public:
    std::string s_;
    void printObj(const char * name) const
    {
        printObj(name, *this);
    }
    
    static void printObj(const char * name, const ObjectType & o)
    {
        std::cout << "\t@" << name << " = 0x" << std::hex << size_t(&o) << " .@s_ = 0x" << size_t((char *)o.s_.c_str()) << std::dec << " .s_ = '" << o.s_ << "'" << std::endl;
    }
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

void printVec(const std::vector<ObjectType> & vec)
{
    int cap = vec.capacity();
    int sz = vec.size();
    std::cout << "vec size=" << sz << ", cap=" << cap << ", sizeof(std::string)=" << sizeof(std::string) << std::endl;
    for(int i = 0; i < cap; i++)
    {
        const ObjectType & o = vec[i];
        std::string ss("@vec[");
        ss = ss + std::to_string(i) + "]";
        o.printObj(ss.c_str());
    }
}

void printLine(const char * cmd, int line) {
    std::cout << "[STEP " << line << "] : " << cmd << std::endl;
}

#define L(cmd) printLine(#cmd, __LINE__); cmd

int main(void)
{
    L(ObjectType o1("0123456789123456789"));
    L(std::vector<ObjectType> vec);
    L(vec.reserve(10));
    L(printVec(vec));
    L(vec.push_back(o1));
    L(o1.printObj("o1"));
    L(printVec(vec));
    L(vec.push_back(std::move(o1)));
    L(o1.printObj("o1"));
    L(printVec(vec));
    L(ObjectType o2 = std::move(vec.front()));
    L(printVec(vec));
    L(vec.erase(vec.begin()));
    L(printVec(vec));
    return 0;
}

/**
[STEP 108] : ObjectType o1("0123456789123456789")                           
default constructor
	@this = 0x16f6df5b0 .@s_ = 0x16f6df5b0 .s_ = '0123456789123456789'      
[STEP 109] : std::vector<ObjectType> vec
[STEP 110] : vec.reserve(10)
	new @0x600002e70000 sz = 240
[STEP 111] : printVec(vec)
vec size=0, cap=10, sizeof(std::string)=24
	@@vec[0] = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = ''
	@@vec[1] = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = ''
	@@vec[2] = 0x600002e70030 .@s_ = 0x600002e70030 .s_ = ''
	@@vec[3] = 0x600002e70048 .@s_ = 0x600002e70048 .s_ = ''
	@@vec[4] = 0x600002e70060 .@s_ = 0x600002e70060 .s_ = ''
	@@vec[5] = 0x600002e70078 .@s_ = 0x600002e70078 .s_ = ''
	@@vec[6] = 0x600002e70090 .@s_ = 0x600002e70090 .s_ = ''
	@@vec[7] = 0x600002e700a8 .@s_ = 0x600002e700a8 .s_ = ''
	@@vec[8] = 0x600002e700c0 .@s_ = 0x600002e700c0 .s_ = ''
	@@vec[9] = 0x600002e700d8 .@s_ = 0x600002e700d8 .s_ = ''
[STEP 112] : vec.push_back(o1)
copy constructor
	@this = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = '0123456789123456789'
	@other = 0x16f6df5b0 .@s_ = 0x16f6df5b0 .s_ = '0123456789123456789'
[STEP 113] : o1.printObj("o1")
	@o1 = 0x16f6df5b0 .@s_ = 0x16f6df5b0 .s_ = '0123456789123456789'
[STEP 114] : printVec(vec)
vec size=1, cap=10, sizeof(std::string)=24
	@@vec[0] = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = '0123456789123456789'
	@@vec[1] = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = ''
	@@vec[2] = 0x600002e70030 .@s_ = 0x600002e70030 .s_ = ''
	@@vec[3] = 0x600002e70048 .@s_ = 0x600002e70048 .s_ = ''
	@@vec[4] = 0x600002e70060 .@s_ = 0x600002e70060 .s_ = ''
	@@vec[5] = 0x600002e70078 .@s_ = 0x600002e70078 .s_ = ''
	@@vec[6] = 0x600002e70090 .@s_ = 0x600002e70090 .s_ = ''
	@@vec[7] = 0x600002e700a8 .@s_ = 0x600002e700a8 .s_ = ''
	@@vec[8] = 0x600002e700c0 .@s_ = 0x600002e700c0 .s_ = ''
	@@vec[9] = 0x600002e700d8 .@s_ = 0x600002e700d8 .s_ = ''
[STEP 115] : vec.push_back(std::move(o1))
move constructor
	@this = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = '0123456789123456789'
	@other = 0x16f6df5b0 .@s_ = 0x16f6df5b0 .s_ = ''
[STEP 116] : o1.printObj("o1")
	@o1 = 0x16f6df5b0 .@s_ = 0x16f6df5b0 .s_ = ''
[STEP 117] : printVec(vec)
vec size=2, cap=10, sizeof(std::string)=24
	@@vec[0] = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = '0123456789123456789'
	@@vec[1] = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = '0123456789123456789'
	@@vec[2] = 0x600002e70030 .@s_ = 0x600002e70030 .s_ = ''
	@@vec[3] = 0x600002e70048 .@s_ = 0x600002e70048 .s_ = ''
	@@vec[4] = 0x600002e70060 .@s_ = 0x600002e70060 .s_ = ''
	@@vec[5] = 0x600002e70078 .@s_ = 0x600002e70078 .s_ = ''
	@@vec[6] = 0x600002e70090 .@s_ = 0x600002e70090 .s_ = ''
	@@vec[7] = 0x600002e700a8 .@s_ = 0x600002e700a8 .s_ = ''
	@@vec[8] = 0x600002e700c0 .@s_ = 0x600002e700c0 .s_ = ''
	@@vec[9] = 0x600002e700d8 .@s_ = 0x600002e700d8 .s_ = ''
[STEP 118] : ObjectType o2 = std::move(vec.front())
move constructor
	@this = 0x16f6df558 .@s_ = 0x16f6df558 .s_ = '0123456789123456789'
	@other = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = ''
[STEP 119] : printVec(vec)
vec size=2, cap=10, sizeof(std::string)=24
	@@vec[0] = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = ''
	@@vec[1] = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = '0123456789123456789'
	@@vec[2] = 0x600002e70030 .@s_ = 0x600002e70030 .s_ = ''
	@@vec[3] = 0x600002e70048 .@s_ = 0x600002e70048 .s_ = ''
	@@vec[4] = 0x600002e70060 .@s_ = 0x600002e70060 .s_ = ''
	@@vec[5] = 0x600002e70078 .@s_ = 0x600002e70078 .s_ = ''
	@@vec[6] = 0x600002e70090 .@s_ = 0x600002e70090 .s_ = ''
	@@vec[7] = 0x600002e700a8 .@s_ = 0x600002e700a8 .s_ = ''
	@@vec[8] = 0x600002e700c0 .@s_ = 0x600002e700c0 .s_ = ''
	@@vec[9] = 0x600002e700d8 .@s_ = 0x600002e700d8 .s_ = ''
[STEP 120] : vec.erase(vec.begin())
assign by moving
	@this = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = '0123456789123456789'
	@other = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = ''
destructor
	@this = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = ''
[STEP 121] : printVec(vec)
vec size=1, cap=10, sizeof(std::string)=24
	@@vec[0] = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = '0123456789123456789'
	@@vec[1] = 0x600002e70018 .@s_ = 0x600002e70018 .s_ = ''
	@@vec[2] = 0x600002e70030 .@s_ = 0x600002e70030 .s_ = ''
	@@vec[3] = 0x600002e70048 .@s_ = 0x600002e70048 .s_ = ''
	@@vec[4] = 0x600002e70060 .@s_ = 0x600002e70060 .s_ = ''
	@@vec[5] = 0x600002e70078 .@s_ = 0x600002e70078 .s_ = ''
	@@vec[6] = 0x600002e70090 .@s_ = 0x600002e70090 .s_ = ''
	@@vec[7] = 0x600002e700a8 .@s_ = 0x600002e700a8 .s_ = ''
	@@vec[8] = 0x600002e700c0 .@s_ = 0x600002e700c0 .s_ = ''
	@@vec[9] = 0x600002e700d8 .@s_ = 0x600002e700d8 .s_ = ''
destructor
	@this = 0x16f6df558 .@s_ = 0x16f6df558 .s_ = '0123456789123456789'
destructor
	@this = 0x600002e70000 .@s_ = 0x600002e70000 .s_ = '0123456789123456789'
	delete @0x600002e70000
destructor
	@this = 0x16f6df5b0 .@s_ = 0x16f6df5b0 .s_ = ''

    */
