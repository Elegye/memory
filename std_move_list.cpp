#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <cmath>

class ObjectType
{
public:
// default constructor
    ObjectType(const int & s) {
        std::cout << "default constructor" << std::endl;
        i_ = s;
        printObj("this");
    }

    // copy constructor
    ObjectType(const ObjectType & o)
    {
        std::cout << "copy constructor" << std::endl;
        i_ = o.i_;
        printObj("this");
        printObj("other", o);
    }

    // move constructor
    ObjectType(ObjectType && o)
    {
        std::cout << "move constructor" << std::endl;
        i_ = std::move(o.i_);
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
        i_ = std::move(o.i_);
        printObj("this");
        printObj("other", o);
        return *this;
    }

    // assign by coping
    ObjectType& operator=(const ObjectType& o)
    {
        std::cout << "assign by coping" << std::endl;
        i_ = o.i_;
        printObj("this");
        printObj("other", o);
        return *this;
    }

public:
    int i_;
    void printObj(const char * name) const
    {
        printObj(name, *this);
    }
    
    static void printObj(const char * name, const ObjectType & o)
    {
        std::cout << "\t@" << name << " = 0x" << std::hex << size_t(&o) << " .@i_ = 0x" << size_t(o.i_) << std::dec << " .i_ = '" << o.i_ << "'" << std::endl;
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

void printList(const std::list<ObjectType> & list) {
    int max_size = list.max_size();
    int size = list.size();
    std::cout << "list size=" << size << ", max_size=" << max_size << ", sizeof(int)=" << sizeof(int) << std::endl;

    std::list<ObjectType>::const_iterator it = list.cbegin();
    int i = 0;

    for(auto value: list) {
        std::advance(it, i);
        std::string ss("@list[");
        ss = ss + std::to_string(i) + "]";
        it->printObj(ss.c_str());
        i++;
    }
}

void printLine(const char * cmd, int line) {
    std::cout << "[STEP " << line << "] : " << cmd << std::endl;
}

#define L(cmd) printLine(#cmd, __LINE__); cmd

int main(void)
{
    L(ObjectType o1(pow(2, 2) + 1));
    L(ObjectType oa(pow(2, 4) + 1));
    L(ObjectType ob(pow(2, 8) + 1));
    L(std::list<ObjectType> list);
    L(printList(list));
    L(list.push_back(o1));
    L(o1.printObj("o1"));
    L(list.push_back(oa));
    L(o1.printObj("oa"));
    L(list.push_back(ob));
    L(o1.printObj("ob"));
    L(printList(list));
    L(list.push_back(std::move(o1)));
    L(o1.printObj("o1"));
    L(printList(list));
    L(ObjectType o2 = std::move(list.front()));
    L(printList(list));
    L(list.erase(list.begin()));
    L(printList(list));
    return 0;
}

/**
[STEP 127] : ObjectType o1(pow(2, 2) + 1)
default constructor
	@this = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'                        i_ vaut 5 : 2^2 + 1 ... o1 est hébergé à l'adresse 0x16dceb5c8
[STEP 128] : ObjectType oa(pow(2, 4) + 1)
default constructor
	@this = 0x16dceb5b0 .@i_ = 0x11 .i_ = '17'                      i_ vaut 17. oa est hébergé à l'adresse 0x16dceb5b0
[STEP 129] : ObjectType ob(pow(2, 8) + 1)
default constructor
	@this = 0x16dceb5a8 .@i_ = 0x101 .i_ = '257'                    idem ici avec 257 dans une autre case mémoire.
[STEP 130] : std::list<ObjectType> list                             Pas d'allocation mémoire ici : pas de new.
[STEP 131] : printList(list)                                    
list size=0, max_size=-1431655766, sizeof(int)=4                    sizeof(int) vaut 4. Un entier est stocké sur 4 bytes (1 octet, 8 bits).
[STEP 132] : list.push_back(o1)                                     Allocation mémoire 
	new @0x6000036351c0 sz = 24
copy constructor
	@this = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
	@other = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
[STEP 133] : o1.printObj("o1")
	@o1 = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
[STEP 134] : list.push_back(oa)
	new @0x6000036351e0 sz = 24
copy constructor
	@this = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
	@other = 0x16dceb5b0 .@i_ = 0x11 .i_ = '17'
[STEP 135] : o1.printObj("oa")
	@oa = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
[STEP 136] : list.push_back(ob)
	new @0x600003635200 sz = 24
copy constructor
	@this = 0x600003635210 .@i_ = 0x101 .i_ = '257'
	@other = 0x16dceb5a8 .@i_ = 0x101 .i_ = '257'
[STEP 137] : o1.printObj("ob")
	@ob = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
[STEP 138] : printList(list)
list size=3, max_size=-1431655766, sizeof(int)=4
copy constructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
	@other = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
	@@list[0] = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
	@other = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
	@@list[1] = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
	@other = 0x600003635210 .@i_ = 0x101 .i_ = '257'
	@@list[2] = 0x16dceb598 .@i_ = 0x3 .i_ = '3'
destructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
[STEP 139] : list.push_back(std::move(o1))
	new @0x600003635220 sz = 24
move constructor
	@this = 0x600003635230 .@i_ = 0x5 .i_ = '5'
	@other = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
[STEP 140] : o1.printObj("o1")
	@o1 = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
[STEP 141] : printList(list)
list size=4, max_size=-1431655766, sizeof(int)=4
copy constructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
	@other = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
	@@list[0] = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
	@other = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
	@@list[1] = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
	@other = 0x600003635210 .@i_ = 0x101 .i_ = '257'
	@@list[2] = 0x600003635230 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
	@other = 0x600003635230 .@i_ = 0x5 .i_ = '5'
	@@list[3] = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
[STEP 142] : ObjectType o2 = std::move(list.front())
move constructor
	@this = 0x16dceb584 .@i_ = 0x5 .i_ = '5'
	@other = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
[STEP 143] : printList(list)
list size=4, max_size=-1431655766, sizeof(int)=4
copy constructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
	@other = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
	@@list[0] = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
	@other = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
	@@list[1] = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
	@other = 0x600003635210 .@i_ = 0x101 .i_ = '257'
	@@list[2] = 0x600003635230 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
	@other = 0x600003635230 .@i_ = 0x5 .i_ = '5'
	@@list[3] = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
[STEP 144] : list.erase(list.begin())
destructor
	@this = 0x6000036351d0 .@i_ = 0x5 .i_ = '5'
	delete @0x6000036351c0
[STEP 145] : printList(list)
list size=3, max_size=-1431655766, sizeof(int)=4
copy constructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
	@other = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
	@@list[0] = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb504 .@i_ = 0x11 .i_ = '17'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
	@other = 0x600003635210 .@i_ = 0x101 .i_ = '257'
	@@list[1] = 0x600003635210 .@i_ = 0x101 .i_ = '257'
destructor
	@this = 0x16dceb504 .@i_ = 0x101 .i_ = '257'
copy constructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
	@other = 0x600003635230 .@i_ = 0x5 .i_ = '5'
	@@list[2] = 0x16dceb598 .@i_ = 0x3 .i_ = '3'
destructor
	@this = 0x16dceb504 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x16dceb584 .@i_ = 0x5 .i_ = '5'
destructor
	@this = 0x6000036351f0 .@i_ = 0x11 .i_ = '17'
	delete @0x6000036351e0
destructor
	@this = 0x600003635210 .@i_ = 0x101 .i_ = '257'
	delete @0x600003635200
destructor
	@this = 0x600003635230 .@i_ = 0x5 .i_ = '5'
	delete @0x600003635220
destructor
	@this = 0x16dceb5a8 .@i_ = 0x101 .i_ = '257'
destructor
	@this = 0x16dceb5b0 .@i_ = 0x11 .i_ = '17'
destructor
	@this = 0x16dceb5c8 .@i_ = 0x5 .i_ = '5'
*/