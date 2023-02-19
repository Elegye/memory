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
        std::cout << "Integer move constructor" << std::endl;
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
    

public:
    std::vector<T> _vec;

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

    void add_move(T const* value) {
        _vec.push_back(std::move(*value));
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
    L(Integer * i = new Integer(19));
    L(Integer i1 = std::move(*i));     // Ici on va faire appel à std::move
    L(collection.add(i1));
    L(collection.add_move(i));

    return 0;
}


/**

[LINE 143] : CollectionType<Integer> collection
default constructor                                                     On réserve 2 places sur le vecteur.
	new @0x600001df0040 sz = 8
	vec size=0, cap=2
		@vec[0] 	 @hex=0x600001df0040, sizeof= 4 - value = 0
		@vec[1] 	 @hex=0x600001df0044, sizeof= 4 - value = 0
[LINE 144] : collection.add(pow(2, 1))                                  On ajoute un élément au premier emplacement.
Integer default constructor                                             Le constructeur de Integer est appelé. Hébergement à Ox16d5bb590.
	name=this, @=Ox16d5bb590, @i=Ox16d5bb590, i=2
Integer copy constructor                                                Puis la méthode add fait une copie de l'élément à ajouter.
	name=this, @=Ox600001df0040, @i=Ox600001df0040, i=2                 Linker hyper intelligent : il crée l'élement directement au premier emplacement du vecteur.
	name=other, @=Ox16d5bb590, @i=Ox16d5bb590, i=2                      Le other ici c'est l'Integer créé par le pow(2, 1) passé à collection.add()
	vec size=1, cap=2
		@vec[0] 	 @hex=0x600001df0040, sizeof= 4 - value = 2         Le premier élément est bien ajouté.
		@vec[1] 	 @hex=0x600001df0044, sizeof= 4 - value = 0
Integer destructor
	name=this, @=Ox16d5bb590, @i=Ox16d5bb590, i=2                       L'élément "transitoire" est détruit.
[LINE 145] : collection.add(pow(2, 2))                                  ici, même chose.
Integer default constructor
	name=this, @=Ox16d5bb58c, @i=Ox16d5bb58c, i=4
Integer copy constructor
	name=this, @=Ox600001df0044, @i=Ox600001df0044, i=4
	name=other, @=Ox16d5bb58c, @i=Ox16d5bb58c, i=4
	vec size=2, cap=2
		@vec[0] 	 @hex=0x600001df0040, sizeof= 4 - value = 2
		@vec[1] 	 @hex=0x600001df0044, sizeof= 4 - value = 4
Integer destructor
	name=this, @=Ox16d5bb58c, @i=Ox16d5bb58c, i=4
[LINE 146] : collection.add(pow(2, 3))                                  Même processus : On construit l'élément à ajouter dans la collection.
Integer default constructor
	name=this, @=Ox16d5bb588, @i=Ox16d5bb588, i=8
	new @0x600001df0050 sz = 16                                         Là on crée un nouveau vecteur @0x600001df0050 avec une taille de 16 bytes (donc max 4 éléments de 4 bytes).
Integer copy constructor                                                Et là, c'est le drame : On veut mettre un 3ème élément dans un vecteur qui a 2 places.
	name=this, @=Ox600001df0058, @i=Ox600001df0058, i=8                 Donc on va copier toutes les valeurs une à une entre le nouveau vecteur et l'ancien.
	name=other, @=Ox16d5bb588, @i=Ox16d5bb588, i=8                      Il commence par le dernier élément (le nouveau)
Integer copy constructor
	name=this, @=Ox600001df0054, @i=Ox600001df0054, i=4                 2ème
	name=other, @=Ox600001df0044, @i=Ox600001df0044, i=4
Integer copy constructor
	name=this, @=Ox600001df0050, @i=Ox600001df0050, i=2                 1er
	name=other, @=Ox600001df0040, @i=Ox600001df0040, i=2
Integer destructor
	name=this, @=Ox600001df0044, @i=Ox600001df0044, i=4                 Il détruit les éléments du premier vecteur.
Integer destructor
	name=this, @=Ox600001df0040, @i=Ox600001df0040, i=2                 
	delete @0x600001df0040                                              Bye-bye le premier vecteur.
	vec size=3, cap=4
		@vec[0] 	 @hex=0x600001df0050, sizeof= 4 - value = 2
		@vec[1] 	 @hex=0x600001df0054, sizeof= 4 - value = 4
		@vec[2] 	 @hex=0x600001df0058, sizeof= 4 - value = 8
		@vec[3] 	 @hex=0x600001df005c, sizeof= 4 - value = 0
Integer destructor
	name=this, @=Ox16d5bb588, @i=Ox16d5bb588, i=8                       La copie du 3ème élément est supprimée.
[LINE 147] : collection.add(pow(2, 4))                                  Encore un élément à ajouter. Ici tout est normal
Integer default constructor
	name=this, @=Ox16d5bb584, @i=Ox16d5bb584, i=16
Integer copy constructor
	name=this, @=Ox600001df005c, @i=Ox600001df005c, i=16
	name=other, @=Ox16d5bb584, @i=Ox16d5bb584, i=16
	vec size=4, cap=4
		@vec[0] 	 @hex=0x600001df0050, sizeof= 4 - value = 2
		@vec[1] 	 @hex=0x600001df0054, sizeof= 4 - value = 4
		@vec[2] 	 @hex=0x600001df0058, sizeof= 4 - value = 8
		@vec[3] 	 @hex=0x600001df005c, sizeof= 4 - value = 16
Integer destructor
	name=this, @=Ox16d5bb584, @i=Ox16d5bb584, i=16
[LINE 148] : collection.add(pow(2, 5))                                  Le vecteur est plein. Donc on recommence la même copie hyper longue de la ligne 146.
Integer default constructor
	name=this, @=Ox16d5bb580, @i=Ox16d5bb580, i=32
	new @0x600001ff51c0 sz = 32
Integer copy constructor
	name=this, @=Ox600001ff51d0, @i=Ox600001ff51d0, i=32
	name=other, @=Ox16d5bb580, @i=Ox16d5bb580, i=32
Integer copy constructor
	name=this, @=Ox600001ff51cc, @i=Ox600001ff51cc, i=16
	name=other, @=Ox600001df005c, @i=Ox600001df005c, i=16
Integer copy constructor
	name=this, @=Ox600001ff51c8, @i=Ox600001ff51c8, i=8
	name=other, @=Ox600001df0058, @i=Ox600001df0058, i=8
Integer copy constructor
	name=this, @=Ox600001ff51c4, @i=Ox600001ff51c4, i=4
	name=other, @=Ox600001df0054, @i=Ox600001df0054, i=4
Integer copy constructor
	name=this, @=Ox600001ff51c0, @i=Ox600001ff51c0, i=2
	name=other, @=Ox600001df0050, @i=Ox600001df0050, i=2
Integer destructor
	name=this, @=Ox600001df005c, @i=Ox600001df005c, i=16
Integer destructor
	name=this, @=Ox600001df0058, @i=Ox600001df0058, i=8
Integer destructor
	name=this, @=Ox600001df0054, @i=Ox600001df0054, i=4
Integer destructor
	name=this, @=Ox600001df0050, @i=Ox600001df0050, i=2
	delete @0x600001df0050
	vec size=5, cap=8
		@vec[0] 	 @hex=0x600001ff51c0, sizeof= 4 - value = 2
		@vec[1] 	 @hex=0x600001ff51c4, sizeof= 4 - value = 4
		@vec[2] 	 @hex=0x600001ff51c8, sizeof= 4 - value = 8
		@vec[3] 	 @hex=0x600001ff51cc, sizeof= 4 - value = 16
		@vec[4] 	 @hex=0x600001ff51d0, sizeof= 4 - value = 32
		@vec[5] 	 @hex=0x600001ff51d4, sizeof= 4 - value = 0
		@vec[6] 	 @hex=0x600001ff51d8, sizeof= 4 - value = 0
		@vec[7] 	 @hex=0x600001ff51dc, sizeof= 4 - value = 0
Integer destructor
	name=this, @=Ox16d5bb580, @i=Ox16d5bb580, i=32
Integer destructor
	name=this, @=Ox600001ff51d0, @i=Ox600001ff51d0, i=32
Integer destructor
	name=this, @=Ox600001ff51cc, @i=Ox600001ff51cc, i=16
Integer destructor
	name=this, @=Ox600001ff51c8, @i=Ox600001ff51c8, i=8
Integer destructor
	name=this, @=Ox600001ff51c4, @i=Ox600001ff51c4, i=4
Integer destructor
	name=this, @=Ox600001ff51c0, @i=Ox600001ff51c0, i=2
	delete @0x600001ff51c0


A retenir sur cet exemple:
- Réserver la taille adéquate pour le vecteur.

*/