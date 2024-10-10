
#include <iostream>


/*Exercise 1
const int i;                            // const int with value of i;
int const i;                            // const int, same as the 1st one;
int const *i;                           // pointer to const int i
struct X *const p;                      // constant pointer to struct x;
struct X const x1;                      // constant struct x
const struct X *p;                      // pointer to constant struct x;
const struct X* *const p;               // constant pointer to pointer to constant struct x;
int const * (*const f)(int*)=foo;       // constant pointer to a function taking a pointer to int and returns pointer to constant int



Exercise 2
******************************************************************
(a)What changed with ENUMS in Cpp

ENUM in cpp behaves like a stand-alone type.
Compilier cannot do IMPLICIT casting of enum to int (explicit will work).

(b) What is the value of an uninitialized enum var
The value of uninitialized is junk value.(whatever value was there before)

(c) What are the implications
tbbd
**********************************************************************************************
Exercise 3

(a)What is the size of a Boolean variable ? is it guaranteed ?

The size of a boolean variable, if declared as boolean is 1.
However, if we reasign int to boolean(which can be done) or other datatypes,
it takes the size of this data type.

(b) What is the value of an uninitialized bool variable ?

The value of an uninitialized boolean variable is 0 if static / global;
if on the sack - garbage value.

(c) what are the implications ?
the summary of the last 2 question.


(d) Given b2 = i;  some compilers will give a 'performance warning'. why ? how is this different from assigning an int to a character ?

This warning is about whether of not there was an intention to change the value from integer to boolean.
Besides that, the integer value, if changed to boolean, remains the size of 4 bytes, which leads to waste of memory.

++ there are 2 assembly commands on b2 = i make it less efficient.


Exercise 4
****************************************************************************************************************
(a) What will the results of this mistake be ?

There are multiple mistakes in the provided code. 
1st of all, ll_find was not declared anywhere.
2nd Person_t has incomplete type.
3rd using (Person_t *) cast on ll which is typedef for List struct.
4th using Person_t struct inside of List_t struct while List_t is declared first;

If we know that List_t has Person_t inside of it, however, the compiler will just use the casting as expected, but will cause an error
on p->age, that it has no member age.

--There will be a change in the first pointer instead the asked one (e.g third).


(b) Why didnt the compiler protect us ?
The compiler is not supposed to protect us from this kind of mistakes, since the syntax of this is okay.
C compiler is not supposed to protect us from the "playing" with casting. 

(c) Add struct and function header so the code will compile. then replace the expression (Person_t*)ll
with the new C++ cast experssion " static_cast<Person_t*>"
try compiling the erroneous code with the new casting style, fix the mistake and compile again. what does static_cast mean ?

Static cast protect us from casting wrong type. It is a compile time cast, that makes explicit type conversion.
It's safer than a C casting because it makes only "reasonable casts", however it's still doesnt protect us from mistakes
with "reasonable" wrong casts.
can cast void * to anything, and anything to void *. Doesn't allow Casting between 2 different types of pointer.
It does not plays with const - cannot make const int - int.

***************************************************************************************************************************

investigate and research const_cast and reinterpret_cast. what and how would you use each one ?

reinterpret_cast - used to convert a pointer of some data into a pointer of another data, even if the data types are different.
It doesn't check if the pointer type and data pointer by the pointer is same or not. (It's basically C Cast)

reinterpret_cast used when we want to work with bits. It is suggested to not be used unless required.

const cast is not changing the memory layout, it only converts the 'const' flag. Also can change volatile. 
****************************************************************************************************************************

what is the type / side-effect / value of the different casting expression ?
THE TYPE IS THE TYPE WE ARE CASTING TO, the value is the value of the expression. there's a side event if we are making an assignment.

***************************************************************************************************************************

can you still use the old C style cast in C++ ? why would you want to ?

c-style casts are not checked by the compiler, and can fail at runtine. It doesn't gives us any protection, and basically 
when a programmer uses them "he knows better than the compiler". C style casts ignore access control. non l-value.

why - only for backward compatability. 


*******************************************************************************************************************************
what is a function-style cast, and when will you use it ?

function style cast syntax goes - type(expression) which will cast the expression with the type cast.

TBD - when to use it;

*****************************************************************************************************************************


exercise 5

*******************************************************************************************************************
(a)
what is the difference in behavior (function declarations in c++ are mandatory. test c++ v c )

In C - implicit declaration warning, conflicting types warning. Compilation allowed.

in CPP - foo was not declared in this scope. Compilation Error.

(b)
Explain how every word in the warning the C compiler gives while allowing this behavior.

Implicit declaration of function foo warning - the program does not know that foo exists, but allows the compiling.
conflicting types for foo - foo() call inside the main function looks like the declaration of it.

(c)
Calling underclared functions that are present in the header file or in a shared library, that is in the standard path.
TBD for else.

(d)
TBD

*************************************************************************************************************************
Exercise 6
Local variables can be defined anywhere in a block.

(a) What are the advantages ?
    Shorter code, more clear, local variables belong to the scope with the same name as the global.
    also, for each scope, the scope determines when the variables are created and destroyed in the memmory.
    means, memory usage will be more efficient.
    

(b) What bugs can we prenet and never see again thanks to this change ?
    Re-definition of variables (unless we redefine in the same scope for some reason).

(c) What is required from our coding style in order to prevent these bugs ?
    TBD

(d) Does this mean that allocating variables on the stack is inefficient (requiring multiple changes to the SP) ?
    The compiler  will work harder in this situation to calculate the stack frame.

***************************************************************************************************************************
Exercise 7
Operator 'new' and 'delete' replace the library functions malloc() and free(). 

float *f = new float(12.6);
delete f;

f = new float[15];
delete[] f;

(a) How do new/delete prevent bugs (common with malloc / free)? Examples.
    Free uses 'Free Store' which is another dynamic memory area.
    Free store objects can have memory allocated without being immediately initialized, and can be destroyed without deallocating immidiately. 
    New - returns a fully typed pointer. Doesnt return NULL (throws an error instead); Size calculated by the compiler. 

(b) If 'new' is an operator, what is the type, side-effect and value of the expression created with it ?
    TBD

(c) It is safe to delete 0? what is this good for ?
    Yes, it is safe. delete operator designed to handle nullptr/NULL.
    TBD - what it is good for

(d) Was it safe to free 0?
    free(NULL) has no operation. (safe).

(e) Try mixing the old and new ways. what happens ? why ? will it always behave in the same way ?

int *ptr = (int *)malloc(1000 * sizeof(int));
delete ptr;
int *ptr2 = new int[1000];
free(ptr);

    TBD

**************************************************************************************************************************
Exercise 8 - For input and output <iostream> (no .h) replaces <stdio.h> 

char *i = new char;
std::cout << "something\n" << i << std::endl;
std::cin >>i;
std::cerr << "somethig else\n" << i;

*******************************************************************************************************************************
Exercise 9 - Default params - a function declared like void foo(int, int i=3, float = 4); can be called in these ways:

void foo(int, int i=3, float = 4);
foo(3);
foo(6, 5);
foo(4,7,8.7)

(a) What is this feature good for ?

It is agood if we want some value to be default on our API / which will suit most cases.
Having a default value can prevent bugs, mistakes, on the other hand it can give the user more flexibility if he wants to change smt in the program.
It also provides simplicity in function calls, making the code more readable.

(b) Create some working code using this feature.
void foo(int num=5, int num2=7)
{
    int result = num + num2;
    std::cout << "First num is " << num << std::endl
    << "Second num is " << num2 << std::endl
    << "Result is " << result << std::endl;
}

(c) How does the function know whether to use the defaults or the parameters on stack ?

    The compiler determines whather to use default or provided parameters at compile-time based on the number
    of arguments passed by the caller. If fewer arguments passed than the number of params, 
    the missing arguments filled using the default values specified. This is resolved before the function is called,
    so there's no runtime-overhead for using default params.


(d) Can we call only first and third parameters ? Why is this a problem ? 

    No. Default arguments cannot be in the middle or the start.
    They must be the trailing parameters always. This has to do with how the compiler read the arguments of a function
    (Function calling conventions, from right to left, means for us, from the last to the first).

    that's a problem because of how the compiler assigns the default values, based on the calling conventions.
    whhen we specify arguments, the compiler reads from left to right, but assigns from right to left. 

*******************************************************************************************************************************************
Exercise 10 //Comments

(a) How are C++ comments different from c comments ?

        c++ comments has only opening '//' and their closing is a newline,
        opposed to c comments that has '/*oppening and * 'slash' closing. 
        // style comments are used for inline commenting.

(b) Can we still use c comments ? why would we ? 

        Yes. For multi-line commenting it is easier and better to use C comments.
        Also there's code portability reasoning behind this.

*************************************************************************************************************************************************
Exercise 11 - Refamilliarize with the build process and shared objects (implicit and explicit) before moving on.
*************************************************************************************************************************************************
Exercise 12 - Function overloading
*/

/*
#include <iostream>
#include <dlfcn.h>

void Foo(){
    std::cout << "Foo() called w.o params" << std::endl;
}
void Foo(int i) {
    std::cout << "Foo(int) called with an integer %d: "<<i<<std::endl;
};
void Foo(char c){
    std::cout << "Foo(char) called with char %c: "<<c<<std::endl;
};

typedef void (*voidFunc)();
typedef void (*intFunc)(int);

int main() {
    void* handle = dlopen("./libfoo.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }
    voidFunc foo = (voidFunc)dlsym(handle, "Foo");
    intFunc fooInt = (intFunc)dlsym(handle, "FooInt");

    if (foo) foo();
    if (fooInt) fooInt(42);

    dlclose(handle);
    return 0;
}
*/


/*
#include <iostream>
#include "../include/foo.h"
int main()
{
    Foo();
    FooInt(12);
    return 0;
}

*/



/*
(a) Add main() and try calling Foo() with different parameter types. which one is called for each type ?

    For each type the right function is called (the one that has this type in the arguments).

(b) What is this good for ? what are the dangers ?
    Function overloading improves Code readablity, allowing the same name to be used can make the names more intuative.
    Similar actions can be done with the same function names, making the creation or the API more intuative.
    There's a danger of Ambiguity - if the parameter lists of the functions are very similar,
    the compiler may not be able to determine which version to call.
    There might be an accidental overloading with types that are not distict , e.g int and unsigned int.


(c) Try a pointer, float, null, unsigned char. Explain the different behaviors and error messages.
        Foo(float) - call of overloaded Foo(float) is ambigious.
        Foo(NULL) - call of the overloaded Foo(NULL) is ambiguous.
        Foo(int *) - No matching function to call Foo(int *&)
        Foo(unsigned char) - called with Foo(int)

(d) Create a shared object with some functions in C++.
        cppc -fPIC -shared -o libfoo.so foo.cpp

(e) Look at the nm ouput. how is this problem for expicitly loading this shared object ?
        Overloaded functions in C++ have some different mangled names, and it makes them hard to reference
        explicitly in a dlopen and dlsym calls. The mangling changes function names
        to encode info about parameter types, making it non trivial to match.

(f)Connect to it explicitly (dlopen, dlsym) and call the functions from main () compiled in C and C++

(g)How did you sovle the problem ? find a better solution.
    We can use the mangled name using 'nm -C' option, and pass that name to dylsym();
    We also can use 'extern "C"' in the C++ code. the function names exported in a c compatiable format. 

(h)Create a shared object in C and explicitly connect to it from C++;
    "gcc -fPIC -shared -o libfoo.so foo.c"

(i)Create a shared object in C and implicitly connect to it from C and C++ using the samer header file.
        Foo header used in both, but for C++ needed to give a directives if __cplusplus defined then use 
        extern C {}. this makes sure C++ compiler treats the functions as C.
        The linkage make symbols compatible with C++ and C (ensuring -L. and -lfoo flags)

*********************************************************************************************************************************
Exercise 13 - Namespaces let us divie the global namespace into sub-namespaces.
Look at the following example to get familiar with the feature. Compile it and look at the output of nm.

#include <iostream>

namespace useless
{
    unsigned int g_wasteful[400];
    extern int g_notthere;
    void Foo(){};

    namespace wasteoftyme
    {
        void Foo(){};
    }//wasteoftyme
}//uselesss

namespace stupid
{
    void Foo(){};
    void Bar(){};
    void DoNothing(unsigned int){};
}//stupid

namespace useless
{
    void DoNothing(int){}
}//namespace useless


using namespace useless;
void DoStuff()
{
    stupid::Bar();
    Foo();
    using stupid::Foo;
    Foo();
    DoNothing(g_wasteful[3]+1);
}

void Foo(){};
using namespace stupid;
namespace comeon = useless::wasteoftyme;

void DoMoreStuff()
{
    std::cout << "\nInside DoMoreStuff()" << std::endl;
    comeon::Foo();          // Calls `useless::wasteoftyme::Foo()`
    useless::Foo();         // Specify `useless::Foo()`
    stupid::Foo();          // Specify `stupid::Foo()`
    Bar();                  // Calls `stupid::Bar()`
    DoNothing(g_wasteful[3] + 1);
}
namespace useless
{
    void DoUseLessStuff()
    {
        DoNothing(g_wasteful[3]+1);
    }
} // useless

int main()
{
    std::cout << "Calling DoStuff():" << std::endl;
    DoStuff();

    std::cout << "\nCalling DoMoreStuff():" << std::endl;
    DoMoreStuff();

    std::cout << "\nCalling DoUseLessStuff():" << std::endl;
    useless::DoUseLessStuff();

    return 0;
}


(a) What is the scope of a using declaration ?
    Using declaration introduce a name from specific namespace in the current scope, making
    it accessible without needing to qualify it with the namespace prefix.
    the scope of 'using' declaration is the block or scope in which its DECLARED.

(b) What namespaces did C support ?
    C does not support namespaces. Every in c is part of the global namespace, 
    this can cause conflicts in large projects. 

(c) what is "name hiding" in C ? How do namespaces help solve this problem ?
    Name hiding refers to the issue where different identifiers can conflict,
    especially if multiple libraries define function with the same name.
    Namespaces solve this by allowing to separate different functions or variables
    into different logical groups, even if they share the same name.

(d) What does 'using' do ?
    The 'using' keyword brings a single name from namespace into current scope.
    e.g using std::cout; -> cout can be used without the prefix now.
    Also, using directive 'using namespace std;' brings ALL names from namespace
    into the current scope. 

(e) What will happen if we use 'using' declarations in header file ?
    this can cause name conflics when different source files include the same header.
    This can lead to unexpected behavior in parts of the code.

(f)The function Foo() is called in different places in the code. Explain which version is called, and why.\
    - In DoStuff() - Foo() - calls useless::Foo() at first, then stupid::Foo(); because of the declaration of 'using'.
    - In DoMoreStuff() - calls useles::wasteoftyme::Foo() - comeon is an alias for useless:wasteoftyme

(g) Do the same for DoNothing();
    There are two version of DoNothing().
    - in DoStuff() - g_wastefull[3]+1 is unsigned int so stupid::DoNothing(int) is called.
    - in DoMoreStuff() - Also calls stupid::DoNothing(unsigned int) - using namespace stupid make stupid::DoNothing(unsigned int).
    - in DoUselesStuff() - calls the local useless:stupid::DoNothing(int) because using namespaces is not used.

(h)Uncomment the call to Foo() in DoMoreStuff(), explain EVERY WORD in the error message.
    error: call of overloaded ‘Foo()’ is ambiguous
    - Call of overloaded 'Foo()' means that the compiler found multiple Foo() definitions
    in the current context.
    -Is ambiguous indicates that it cannot decide which one to call due the ambiguity.

(i)Fix the line to call ALL version of Foo()
    -To call all version of Foo() we just need to call each version
    by specifying the fully namespace of each
    useless:Foo();
    stupid::Foo();
    useless::wasteoftyme::Foo();

(j)How do namespaces play with preprocessor directives ? 
    Preprocessor directives are processed BEFORE namespaces come into play.
    Howeber, they can affect defitnitions.
    #include inside namespace will work as expected - it will be part of the current namespace.
    #define inside a namespace will still be seen globaly.

**************************************************************************************************88
Exercise 14 - Const and literals: try compiling and running the follow code as C and as C++. Try using different compilers.

#include "stdio.h"
int main()
{
    const int i = 3;
    int *ip = (int*)&i;
    *ip = 5;
    //int arr[i];

    printf("%d %d\n", i, *ip);
    return 0;
}

(a)Explain the different behaviors.
    -This depends on the command running in c, the gd will producd 5,5 the gc will produce 3,5.
    for C++ cppc and cppd will both produce 3,5.
    in C the const qualifier is not under protection again inderect modifications.
    C++ treats const more strictly. 

(b)Move the definition of i to the global scope. explain the result.
    -gd, gc - segmentation fault. global constans reside in RO segment, modyfing them causes a fault.
    -cppc, cppd - const treated as RO in global context, attemps to modify it ignored.



(c)Add definitions of global const and non-const variables. investigte with nm.
    -C++ treated them as local readonly (r) data, and name mangling presented.
    -in C they are treated both as global readonly (R)

(d)Uncomment the commented line, compile with c and C++ and explain the result.
    -Cause a compilation error in c89, size of an array must be a compile-time constant.
    -CPP - allows variable-length arrays.

(e)What feature of C can we stop using now ? what are the advantages ?
    We can stop using const that can be bypassed by casting.
    in CPP it will give us more safety, knowing that the value will not change.
    also it can affect the compiler optimization knowing that value is const.

***************************************************************************************************************
Exercise 15 - Inline functions. Compile this function and look at the nm output.

#include <iostream>
using namespace std;

inline void foo()
{
    cout << "Hello-World";
}

int main() {
    foo();
    return 0;
}


(a)Add a call to the function and see if the nm output changes.
    -When there's no call for the Foo function, there's no symbol at all for it,
    nor mangling, and no symbol for Cout.
    -When there is a function call i get 2 more lines in NM, one for cout@GLIBC
    and other unresovled of GLIBC(basic stream).
    However, Foo is not present in any of the files.

(b)Now add the -o option, (capital O), and look at nm again.
    got the same output.

(c)What does inline mean? what does -O do ? Explain the differences in the nm output.
    -the inline specifier suggests the compiler that the function can be expanded in line
    at the point of call, instead invoking it through strandard call convention.
    this can reduce overhead, but does not guarantee inlining, the compiler chooses himself.
    -The -O option activates optimization in the compiler.
    -Without -O, nm MAY show the function symbol for foo() if its defined but not inlined. 
    With the Optimization, the function may node appear at all if inlined. 

(d)Put the function definition in a header file and call the function from 2 different compilation units. Look at nm, 
and look at the linker output. Try the same without the 'inline' reserved word.
    -without the inline keyword, i can see in the symbol table the foo symbol (foov),
    with the inline keyword it is not present.

***********************************************************************************************************************
Exercise 16 - Template functions.

#include <iostream>

template <typename T>
T Max(T t1, T t2) {
    return (t1 > t2) ? t1 : t2;
}

int main() {
    int arr[12];

    std::cout << Max<int>(3, 5) << " " << Max<double>(3.7, 12.9) << " ";
    std::cout << Max<int *>(arr, arr + 8) << std::endl;
    std::cout << Max('a', 'b') << std::endl; 

    return 0;
}


(a)Look at nm. Remove all the calls to the function, and look again. what is the difference ?
    -Removing the calls, nm output doesnt show any symbols for Max.
    -adding Max calls, output of NM shows more symbols, that probably related to instances of Max.


(b) Try adding other calls with different types. What happens ?
    -Then compiler creates new version of Max for those types.

(c)Try putting the template function definition in a header file and calling it with the same type from different C++ files.
Look at nm.
    -Well..it works in the different files. the NM looks the same for the same trype.

(d)Look at the line with 'a' and 'b' :
    -How is it different from the others ?
        This line uses char literals directly w.o specific template type. 
    -How does it work ?
        The compiler generates version of Max function for char, it uses template argument deduction.
    -What is the motivation behind this feature ?
        type deduction simplifies the syntax for calling tmeplate functions. It makes the code cleaner.
    -what is required for it to work ?
        The types that are used in the function call must be compatible with the operations
        that are defined in the template (in our case the > operator).

(e)Remove all 'calls' to the function, and then remove the return keyword from Max.
    -if you dont call a template function, is it being compiled ?
        There is no symbol for this function, so look like no.
    -add to Max() an error line x=5(x without a type). what happens ?
        A compilation error occurs of x not being declared.
    -if Max() is not being compiled, why do you get an error ?
        The definition of Max is processed for correctness, as all the other undeclared variables.
        The errors occurs during the parsing phase, before instantiation occurs for Max.
    -remove the x=5 and restore the calls to Max() to the way they were.

(f)In order for the code generated from the template to compile, what does the type represented by T have to look like and what operatios does it have to support ?
    The type that is represented by T must support the operations in the template function.
    in our case, Max, T must support - ">".


(g)What happens to a template function declaration and definition call at every stage of the build process?
    -Declaration - the combiler encounters declaration, no code generation.
    -Definition - the template function definition is processed, but no code is need untill
        it is instantiated.
    -Instantiation - when the template function is called with a specific type, the compiler
        generates the function code for that type.
    -Linking - the linker combines object files, if multiple translation units use the same
        template instantiation, it resolves them to a single isntance in the final executable, preventing
        multiple definitions.

*******************************************************************************************************************************
Exercise 17 - overloading regular function and function temples. something the generic solution is not good enough for a specific
tpe, and we want to replace it just for that specific type.

(a)run this code:


#include <iostream>
#include <typeinfo>

// Primary template
template<typename T>
void foo(T t) {
    std::cout << t << " Generic foo for " << typeid(t).name() << std::endl;
}

// Specialization for int
template<>
void foo<int>(int t) {
    std::cout << t << " Specialized foo (for int param)\n";
}

// Specialization for double
template<>
void foo<double>(double t) {
    std::cout << t << " Specialized foo (for double param)\n";
}

int main() {
    foo(4.2);
    foo(5); 
    foo<int>(5);    

    return 0;
}



(b)What is the problem ? 
    The compiler treats foo<int>(5) as a separate function from the non-template
    overload foo(int), resulting in extra instantiation.

(c)Research and investigate template specializations and solve the problem.
    Template specialization allows to define behaviours for certain types when using
    temples. There are 2 kinds in C++:
    1)Full specialization - the behavior defined for each specific type.
    2)Partial specialization - specialize a template for subset of types,
    pointer, or types derived from specific class.
    It is different from overloading that the specialization modifies the behavior of a template for specific types.
    The compiler first looks at overload match, and if not found, it checks template matches
    and then specialization matches.

(d) in what file should a template specialization be implemented ?
    Template specialization should be implemented in the header file where the tempalte
    is declared to ensure they are visible.


****************************************************************************************************************************
Exercise 18 - C++ has a new type of variables that are aliases to other variables.

#include <iostream>
#include <typeinfo>
#include <string>

void Increment(int& n) {
    n++; 
}

int& GetReference(int& n) {
    return n;
}

struct Example {
    int& refField;
    Example(int& r) : refField(r) {}
};

int main() {
    int i = 8;
    int& ip = i; 
    ip = 9; 
    std::cout << "i: " << i << std::endl; 

    double d = 3.14;
    double& dp = d;
    dp = 2.71; 
    std::cout << "d: " << d << std::endl; 

    std::string s = "Hello";
    std::string& sp = s; 
    sp = "World"; 
    std::cout << "s: " << s << std::endl; 

    int x = 5;
    Increment(x);
    std::cout << "x after increment: " << x << std::endl; 

    int& ref = GetReference(x);
    ref = 20; 
    std::cout << "x after modification: " << x << std::endl; 

    Example ex(x);
    ex.refField = 40; 
    std::cout << "x after modification via struct: " << x << std::endl; 

    std::cout << "Size of reference ip: " << sizeof(ip) << " bytes" << std::endl;

    std::cout << "Address of i: " << &i << std::endl;
    std::cout << "Address of reference ip: " << &ip << std::endl;

    return 0;
}


(a) try references to other types.

(b)Experiment with references as variables, parameters, return vallues and struct fields.

(c)What is the size of a reference variable ?
    The size of a reference variable is the same as the size of a pointer, however
    references do not consume memory like pointers, because they do not hold a separate
    memory adress. they are aliases to the existing variable.

(d)Try printing the adress of a reference variable. Explain.
    The adress of the reference variable that is printed is the adress of the variable it refers to.
    The reference does not own its adress, its only refers the adress of the variable, alias.

(e)in C , what are all the different reasons to use pointers ?
    -Dynamic memory allocation. 
    -Array manipulation
    -Function parameters(modify outside scope)
    -Data structres like trees and linked lists.
    -Multiple return values by passing pointer.

(f)Can and should references replace all our uses of pointers ? How and why is this ?(provide for each of the different reasons per question 17e.)
    -Dynamic memory allocation - reference CANNOT replace pointers.
    -Array manipulation - CANNOT perform pointer arithmetic (can be used for iterating)
    -Function parameters - Can pass NULL, reference cannot pass 'no object'.
    -Data Structures - not suitable for implementing. cannot be reasigned.
    -return values - can be used, but cannt represent absence of value.

(g)Non const reference parameters are evil. why ?
    -Allows to modify the argument that passed in. can lead to unintentional side effects.
    -Safety - for the same reason taht allows to modify the original value.
    -Const references allows RO access, remaining the original variable unchanged.

***********************************************************************************************************************************
Exercise 19 - reference parameters: With the following declarations, answer the questions:

#include <iostream>

void Foo(const double& d) {
    std::cout << "Foo called with value: " << d << " at address: " << &d << std::endl;
}

void Bar(double& d) {
    std::cout << "Bar called with value: " << d << " at address: " << &d << std::endl;
}

int main() {
    int i = 42;

    Foo(reinterpret_cast<double&>(i));
    Bar(reinterpret_cast<double&>(i));
    
    //Foo(i); 
    //Bar(i);  // compilation error

    return 0;
}


(a) try calling both functions with a variable of type int. why is there a difference ?
        Foo accepts reference to a constant double. Passing int, the compiler implicitly convert the int
        to a double, and bind the result to the const double& references. 
        const references can bind to a temporary object created by a type conversion.

        Bar on the other hand, expects a non-const reference to a double. This require the exact type match
        and cannot bind to a temporary object. Since i pass an in, the compiler cannot bind double& reference to it
        and results in compilation error "cannot bind non-const lvalue reference of type ‘double&’ to a value of type ‘int’"

(b) use reinterpret_cast to force the code to compile.

(c) what does it actually do ? Do we want this behavior ?
        it changes the way the compiler interprets the memory layout of the variable i.
        The compiler reads the bits of i as if they were double.
        This can cause undefined behavior, just like i got, some "junk" value;
        This behavior leads to false results so we do not want it, it violates safety and correctness.


(d) implement the functions to print their parameter`s addresses and values, and explain the behavior.
        The addresses are the same in all cases. This is because Foo and Bar were operating on the same memory location,
        of their alias, the original i value.

*********************************************************************************************************************************************8
Exercise 20 - Ctors. 
In C many structs were accompanied by initialization and deinitialization functions that the user of the struct had to call before and after using it.
Sometimes it was necessary to provide special copy fuinctions instead of the standard copy initialization and assignment.
Many bugs in C are a result of failing to call these functions every time a variable was initialized, copied, and released. C++ solves this problem 
by removing the responsibility for calling these function from the programmers that use the structures.
The programmer writing the struct use a special syntax to "teach" the compiler which functions must be called when a struct is created, destroyed
or copied - and they will be called automatically whenever the users do any of those actions.

try the following:
*/

#include <iostream>

struct X {
    explicit X();
    explicit X(int a_, int b_ = 8);
    ~X();
    X(const X& other_);
    X& operator=(const X& other_);

    int m_a;
    const int m_b;
};

X::X() : m_a(3), m_b(4) {
    std::cout << "X default Ctor. this: " << this << " m_a: " << m_a << " m_b: " << m_b << std::endl;
}

X::X(int a_, int b_) : m_a(a_), m_b(b_) {
    std::cout << "X int int Ctor. this: " << this << " m_a: " << m_a << " m_b: " << m_b << std::endl;
}

X::X(const X& other_) : m_a(other_.m_a), m_b(other_.m_b) {
    std::cout << "X copy Ctor. this: " << this << " m_a: " << m_a << " m_b: " << m_b << std::endl;
}

X& X::operator=(const X& other_) {
    if (this != &other_) {
        m_a = other_.m_a;
    }
    std::cout << "X assignment operator. this: " << this << " m_a: " << m_a << " does not change m_b: " << m_b << std::endl;
    return *this;
}

X::~X() {
    std::cout << "X Dtor. this: " << this << " m_a: " << m_a << " m_b: " << m_b << std::endl;
}

struct Y {
    X m_x;
    int m_i;
};

int main() {
    Y y1;
    y1.m_x.m_a = 250;
    Y y2(y1);
    Y y3;
    y3 = y1;

    return 0;
}

/*
    (a)Investigate and understand all the syntax. Use nm.

    (b)Use the debugger to understand what's happening. what does 'this' mean ?
        'this' is a pointer to the CURRENT OBJECT on which the member function is called.
        it holds the memory address of the object.

    (c)How does the existence of these functions influence the size of a struct ?
        The member functions does not directly influence the size of a struct.
        (if  there were VIRTUAL FUNCTIONS), the struct would have a vtable pointer, which would increase its size.

    (d) Notice the calling order of Ctors and Dtors of local variables.
        the order of constructor calls based on the order in Main().
        The destructors called in reverse order when variables go out of scope
        e.g X1 is constructed first, but destroyed last.

    (e)Why can we have multiple constructors ? How does the compiler know which constructor it should call ?
        Multiple constructor allowed due to function overloading.
        the compiler selects the corrent constructor based on the number of arguments and their types.

    (f)Can the default Ctor have parameters? why it is called the "default Ctor?"
        A default constructor can have default params. 
        Its called default when i can be called w.o params (X()) (or has all default)

    (g)Look at the commented lines in the Ctor. how are they different from using the initialization list?
        Using assigment in the constructor is different from using initialization list.
        The initialization list initialize member variables BEFORE the constructor body is executed.
        Assignment in the constructor overwrite initialized values, and doesnt work for CONST or REFERECE members.

    (h)Try to use them instead of the initialization list. Explain the result.
        for CONST members like m__b assignment will not work, const variables must be initialized once.
        Using assignment will result in a compilation error if m_b is tried to be modified in constructor body.

    (i)Why do we need the initialization list ? should we split initialization between it and the body ?
        It's needed to directly initialize const and reference members.
        splitting initialization between the list and constructor is not recommended, it leads to inefficiency and confusion.

    (j)How is the memory for x1, x2, x3,x4 allocated ? How is it released ?
        Memory is allocated on the stack for the local variables.
        They are released automatically when they go out of scope.

    (k)When is it allocated and deallocated ? How can you find out ?
        Allocation happens when the variables are created(X x1);
        Decallocation happens at the end of main() function.
        we can observe this in a debugger by adding print statements in the constructor and destructor.

    (l)Where is this done ?
        This is managed by compiler and runtime system, they handle stack mem allocation and deallocation.

    (m)When is the memory allocated for px ?
        Memory for PX allocated when new X(x2) is called, and release when delete px is called.
    
    (n)Describe the Ctor's part in allocating memory for the object.
        The constructor does not allocate memory for the object, but i can allocate
        additional resources if these are needed.
    
    (o)What does the Dtor in the above code do ?
        the destructor prints the m_a and m_b values and is used to clean up resources.
        its automatically called when the object goes out of scope or delete is used. 

    **************************************************************************************************************************************************88
    Exercise 21 - Add the following lines to main (of previous exercise);
    X* xp= newx[10];
    delete[]xp;

    (a)use the debugger to investigate the order of construction and destruction.
        The constructors called in order from xp[0] to xp[9]
        the destructors called from xp[9] to xp[0].

    (b)Remove the square brackes [] from the delete line. Try running it again. Why do we need them ?
        If we remove the square brack, only one destructor is called. Delete w.o brackers deallocates 
        the first object and calls its destructor. the brackets tells the runtime to calld estructor for all the object in the array
        before deallocating memory, w.o leak we will have a memory leak.

    (c)Replace X with double and try the same thing. does it behave the same way ? Should it ? Why? can we trust it ?
        It works the same. Double is build-in type, and safe to call with destructors and constructors.


    (d)What type of new and delete (with and without brackets) could we manage without ? why do we have both ?
        Yes, we could allocate manually block of memory for the new objects, and manually call the destructors.
        But this can lead to potential bugs and problems. Having both allows us safer memory management
        especially for arrays.

    (e)What constructor type does X have to have tu use new[] ?
        To use new[], X must have a default constructor. New need to be able to construct multiple objects w.o arguments.


    *********************************************************************************************************************************************************
    Exercise 22 - add the following struct definition after the definition of struct X:

    struct Y
    {
        X m_x;
        int m_i
    };

    (a)Add the following lines to main(before the return).
    Y y1;
    y1.m_x.m_a=250
    Y y2(y1);
    Y y3;

    y3=y1;

    (b)Look at the output and user the debugger toi investigate. Look at the nm output. what was added ?
        The NM output shows symbols for Y implicitly defined constructor functions.
        (constructor, copy, destruct, assignment).

    (c)Investigating the object file with nm, change main to use / not use the Cctor, Ctor, assignment operator and Dtor. What happens ?

    
    (d) Does Y have constructors ?
        Y have constructors that defiend by the compiler. it has a default constructor, copy and assignment operator.
    
    (e)Remove X's default Ctor and try to compile. what is the problem? be able to explain EVERY WORD in the error message.
        Removing X's default we get compilation error - no matching function for call to X::X();
        this occurs because Y implicit default constractur trying to initialize m_x using X's default constructor that doesnt exist.

    (f)Make the definition of y1 compile and work without reinserting X's default Ctor, and without removing the non-default Ctor.
        Y y1 = {X(1,2), 0};
        This will use X's non default constructor.

    (g) Remove the line x1=x3 and remove X's assignment operator. be able to explain every word in the error message.
        removing x1=x3 and x's assignment , we will get an error assigning Y objects.
        'use of deleted function X& X::operator=(const x&)
        this error occurs because Ys implicit assignment operator tries to use Xs assigment operator that is deleted.

    (h)can this problem be solved in a similar way the previous one ? Should it ?
        It can be solved, but not reccomended as it breaks the expected behavior of assignment.
        A better solution is to keep x's assigment operator.



    ************************************************************************************************************************************************
    Exercise 23 - use nm, the previous examples and some of your own to thoroughly investigate the generated functions.

    (a) which 4 functions can the compiler generate automatically ?
        -Default constructor.
        -Copy constructor
        -Copy assignment operator
        -Destructor.

        c++11 version it also can -move constructor, -move assignment operator.

    (b)Find two more operations which exist automatically and may be overloaded by the programmer
        -Adress of operator &
        -Comma operator

    (c)Are there situations in which the generated functions will not be generated ? which? in which situations ?
        -If constructor is declared, the default is not generated.
        -Declaring a move operation, the copy and assignment constructors are not generated.
        -eclaring esctuctor, copy constructor/copy assignment operator, move constructor, move assignment operator not generated.
        -Declaring a copy constructor or destructor, the copy assignment operator is not generated.


    (d)In what compilation unit are the functions generated ?
        They generated where they are first neede. Typicaly in the cpp file where classes are used, not the header where eclared.


    (e)What potential problems does it create? how are they solved ?
        -different compilation units might generate different version of the same function
        leading to violations. The solution is to define special member functions in the class
        definition or explicitly default/delete them.
        -implicit generation might not be appropriate for all types, especially for resource managing types.
        -Generated functions might have unexpected behavior with non-trivial members. 
        Explicit definition of the function should ensure proper behaviour.

    ****************************************************************************************************************************************************
    Exercise 24 - in C we apply functions on structs in order to manipulate them (by implementing functions that recieve a pointer to the struct).
    in C++ we send messages to objects by calling member functions.

    (a)Investigate(nm, debugger, play)
    Struct X
    {
        X(int a_, int b_);
        void Inc();
        void Dec();
        int m_a;
        int m_b;

    };

    X::X(int a_, int b_): m_a(a_), m_b(b_)
    {}

    void X::Inc()
    {
        ++m_a;
        ++m_b;
    }

    void X::Dec()
    {
        --m_a;
        --m_b;
    }

    void Inc(){}
     
    int main()
    {
        X x1(7, -55);
        X x2(x1);

        Inc();
        x1.Inc();
        x1.Inc();
        x2.Dec();

        return 0;
    }


    (b)Explain all the ways in which the two version of Inc are different (usage, nm, etc...) How do the coexist ?

    (c)Can the non member Inc() change the values of m_a and m_b ? why?

    (d)How is a member function different from having a data member that is a pointer to a function ?

    (e) How does adding multiple member function influence the size of instances of the struct ?

    (f)in the method (also known as member function) Dec(), which m_a is decremented ? How?

    (g)in GDB perform the command p X::Inc what parameters does it recieve ?

    (h)Add the line cout << "this:"<< this << endl; to both methods and investigate.

    (i)Where and how can we use 'this'?

    (j) what will you use 'this'm for ?

    (k)Where does 'this' come from ?

    (i)Can you change this ?

    (m)What is the type of the expression 'this ?

    ******************************************************************************************************************************
    Exercise 25 - Const.
    

    (a) Investigate the C++ example:
    struct X
    {
    explicit X (int);
    ~X();
    void Foo();
    void Bar() const;
    };
    int m_a;
    int *m_p;
    X::X(int_a_): m_a( a_ ), m_p(new int(a_)) {}
    X::~X() { delete m_p; m_p=0; }
    void X::Foo() { ++m_a; --(*m_p); }
    void X::Bar() const
    {
    std::cout << m_a << std::endl;
    std::cout << *m_p << std::endl;
    std::cout << m_p << std::endl;
    //m_a = 0; //---1--- //m_p = 0; //---2---
    //*m_p = 0; //---3---
    //Foo(); //---5---
    }


    void Fifi (const X& x_)
    {
    //x_.Foo(); //---4--- x_. Bar();
    }
    int main()
    {
    X x1 (1);
    x1.Foo();
    Fifi (x1);
    }
    return 0;
    (b) Explain the meaning of const on a struct in C

    (c) Explain the different meanings of const on a pointer in C.

    (d) Uncomment the commented lines (one-at-a-time) and investigate the compiler messages.

    (e) How is commented line number 3 different from the previous ones? What is wrong with the compiler?

    (f) What is the meaning of const in the declaration and definition of the method Bar () ?

    (g) What is the type of this within Bar() ? (You can use GDB to find out!)

    (h) Why is this feature important?

    (i) What is the new habit we need to adopt ?

    (j) What horrible mistakes exist in struct X (nothing to do with const) ?
    
    **************************************************************************************************************************
    Exercise 26 - Classes


    #include<cstdio>
    struct X
    {
    public:
    explicit X(int a_);
    void Foo();
    private:
    int m_a;
    };
    X::X(int a_)
    : m_a(a_)
    {
    }
    void X::Foo()
    {
    printf("%d\n", m_a);
    }
    void Foo (const X &x_)
    {
    printf("%d\n", x_.m_a);
    }
    int main()
    

    }
    X x1;
    x1. Foo();
    printf("%d\n", x1.m_a);
    Foo (x1);
    return 0;
    (a) What happens when you compile? Why?
    (b) What happens if you remove the private keyword from the above code?
    (c) Suggest method(s) to add to struct x that will allow us to fix the above code without removing private
    (d) Where in the above code can m_a be used?
    (e) How can you find all the code that will be influenced by changing the type of m_a to unsigned char?
    (f) How would this be different if we did not use private ?
    (g) What is the purpose of the private keyword?
    (h) How did we achieve this in C?
    (i) Why isn't the C way of achieving this good enough?
    (j) What happens if we don't write the public keyword above?
    (k) How can this feature help us with the problem of unwanted generated functions?
    (l) What must you decide about generated functions with every new struct you write?
    Note: C++ adds a class keyword that can be used in most places a struct is used.
    (m) Replace the struct above with class. What changed?
    (n) What happens if we now remove the public keyword from the code above?
    (o) What happens if we remove the private keyword?
    (p) What are all the differences between class and struct?
    (q) Why do we have both?
    (r) Why is our convention to put public first?

    ************************************************************************************************************************************************
    Exercise 27 - Time for the simple string Exercise
    *************************************************************************************************************************************************
    Exercise 28 - Syntatic sugar / operator overloading: C++ lets us define operator for user defined types


    (a) Investigate the following code: #include <iostream>
    using namespace std;
    class X;
    int operator+ (const X& x1_, const X& x2_)
    {
    return 7;
    }
    ostream & operator<<(ostream& os_, const X& x_);
    class X
    {
    public:
    bool operator== (const X& o_) const { return m_a == o_.m_a; } //Java style inline – not allowed in our coding standard private:
    friend ostream& operator<< (ostream& os_, const X& x_); int m_a;
    };

    int main()
    {
    }
    X x1;
    x x2;
    cout<<" x1+x2:" << x1 + x2 <<
    " x1==x2:" << (x1 == x2) <<
    " x1:" << x1 <<
    " x2:" << x2 << endl;
    return 0;
    ostream & operator<<(ostream& os_, const X& x_)
    {
    return os_ << x_.m_a;
    }

    (b) Note the differences between member operators vs. global operators. When will we use each?
    (c) Investigate all the syntax used in this example. Write the type / side-effect/value of every expression in main()
    (d) Why do we need the line class x; at the beginning?
    (e) What is the purpose of the line starting with friend ? Remove it to investigate.
    (f) Will non-member operators always be friend s? Why?
    (g) Find a way to implement operator<<<< without it being a friend.
    (h) How does friend *play* with public and private ? Does it break encapsulation?
    (i) In order for the operator<<() function to be able to access m_a in this case, should we use a friend attribute, or supply a GetA() getter method? Why?
    (j) Return the 'friend' line and remove the forward declaration of operator<<. what happens? why?
    (k) Where in the class body should the 'friend' declaration be ? what does this depend on ?
    (l) Implement some more operators. What did you learn ?
    (m) When should you use operators vs. regular functions ?
    (n) Why do we call this "syntatic sugar" ?

    **********************************************************************************************************************************************************
    Exercise 29 - conversions


    (a) Investigate the following code (use all your tools nm, debugger, brain, etc.). #include <iostream>
    class X
    {
    public:
    explicit X();
    explicit X(int a_);
    explicit X(int a_, int b_);
    operator int() const;
    void print() const;
    private:
    int m_a;
    };
    X::X() m_a(0) { }
    X::X(int a_) : m_a(a_) { }
    X::X(int a_, int b_) : m_a(a_ + b_) { }
    X::operator int() const
    {
    return m_num;
    }
    void X:: Print()
    {
    std::cout << "X::Print() " << m_a << std::endl;
    }

    void Fifi (X x_)
    {
    std::cout << "Fifi() " << x_ << std::endl; x_.Print();
    }
    int main()
    {
    x x1 (7);
    Fifi (x1);
    // (1)
    Fifi (X (7));
    // (2)
    //Fifi (9);
    // (3)
    // Fifi (3, 4);
    // (4)
    }
    return x1 * 3; //(5)
    (b) Explain the syntax of line (2) including type/value/side-effect.
    (c) Uncomment line (3). What happens? What did the writer of line (3) expect would happen?
    • Remove the explicit keyword from explicit X(int a_); and try again.
    (d) What is different between calling rifi in (1) (2) and (3)? Use the debugger.
    (e) What happens if you uncomment (4)? Can you make it work?
    (f) What does the explicit kevword mean on the default Ctor?
    (g) What does the explicit keyword mean on a Ctor with one argument?
    (h) What does the explicit keyword mean on a Ctor with two arguments or more?
    (i) What Ctors do you need to prefix with explicit? Why?
    (j) Find examples where you would want conversion Ctors.
    (k) There are 3 different ways to write the explicit temporary creation in line (2). Find the other two.
    (l) Why do we have 3 ways? Which one should we use? Why?
    (m) Explain the syntax of line (5) including type/value/side-effect.
    (n) Why doesn't the method operator int() have a return type?
    (o) Find examples of where you would like conversion operators.
    (p) Why is this syntactic sugar?
    (q) Change Fifi() to accept a const x&. Does the code still work? How? Why?
    (r) Change Fifi () to accept a x&. Does the code still work? How? Why?
    Note: There are many ways to accept parameters, (x x_), (X& x_), (const X& x_), (const X x_), (X* x_), (const X* x_), (X* const_x_)
    (s) Find all the differences in meaning, usage, misusage, readability, conversions, performance, etc.
    (t) When should you use each?

******************************************************************************************************************************************************
Exercise 30 - static members

    {
    class X
    public:
    X() : m_id(++s_cntr) {}
    int GetId() { std::cout << m_id << std::endl; return m_id;}
    private:
    int m_id;
    static int s_cntr;
    };
    int X::s_cntr = 0;
    int main()
    {
    }
    X x1;
    X x2;
    x1.GetId();
    x2.GetId();
    return 0;
    }

    (a) What does static on a data member imply?
    (b) What is the meaning of the line starting with int x:: ? Why do we need it?(Try removing.) Where should we put this line?
    (c) Notice our coding convention for static data members.
    (d) Why "static" again? What do global static variables, local static, and class static have in common?
    (e) Investigate and research static member functions.
    (f) What can you do with a static member function that is impossible with a regular method?
    (g) What does a regular member function do that a static member function cannot?
    (h) What are the two different syntaxes used to call static member functions?
    (i) Can static member functions access private member variables/functions of an object?

*************************************************************************************************************************************************************
Exercise 31 - TIME FOR COMPLEX EXERCISE
**************************************************************************************************************************************************************
Exercise 32 - Inheritance


Compile-time polymorphism in C++ is implemented with templates, but we also have runtime polymorphism implemented via inheritance and virtual functions. a. Investigate the code, look at the output, investigate with the debugger, check the sizeof, concentrate on Ctors, Dtors and virtual functions. class B
[
public:
B(int a = 8):m_a(a_){ cout << "B::Ctor" << endl; }
virtual ~B(){ cout << "B::Dtor" << endl; }
//Java style is not allowed in our coding convention
virtual void Printl() const;
virtual void Print2() const;
virtual void Print3() const;
private:
int m_a;
};
void B:: Print1() const
{
cout << "B::Printl B::m_a - " << m_a << endl;
}
void B:: Print2() const
{
}
cout << "B::Print2" << endl;


void B:: Print3() const
{
cout << "B::Print3" << endl;
}
class X: public B
public:
X():m_b(0){ cout<< "X::Ctor" << endl; }
~X(){ cout << "X::Dtor" << endl;}
virtual void Printl() const
{
cout << "X::Printl X::m_b - " << m_b << endl;
B:: Print1();
cout << "X::Printl end" << endl;
}
void Print2() const { cout << "X::Print2" << endl; }
private:
int m_b;
};


int main()
{
B *b1= new B;
B *b2= new X;
cout<< endl << "main bl:" << endl;
bl->Print1();
b1->Print2();
b1->Print3();
cout << endl << "main b2:" << endl;
b2->Print1();
b2->Print2();
b2->Print3();
X *xx= static_cast<X*>(b2);
cout << endl << "main xx:" << endl;
xx->Print2();
b2->Print2();
delete bl;
delete b2;
return 0;
}


(b) Look at main. How come we can initialize a B✶ with an address of x? What is it good for?
(c) What is called when the line b2->Print3 () is executed?
(d) What new implicit conversions does public inheritance add?
(e) What can static_cast do?
(f) Investigate the implementation of x::Print1 (). What is interesting?
(g) Remove the default parameter value from the class Ctor. What happens? Solve the problem.
(h) Remove the word virtual from the declaration of Print2() in class B. What happens?
(i) Look at the output of the lines xx->Print2(); and b2->Print2(); . What is wrong with this behavior?
(j) Would we ever want this behavior?
(k) Remove the word virtual from the declaration of B's Dtor. What happens? What is wrong with this behavior?
(l) What do the differences in the declarations of X:: Printl() and X:: Print2() change?
(m) How is the virtual mechanism implemented in C++?

***************************************************************************************************************************************************************************88
Exercise 33 - Exeptions


In C, every function that might fail returns a status or sets errno (or a similar global flag). The caller of such a function has to check the status / flag to know if it failed. When it fails, the calling function does not really handle the error, it just returns the error status to its caller.
For example, if function A (that can handle a failure) calls function B that calls c, and so on - until function z that returns an error status, all the functions B - Y contain similar lines of code that check the status and pass it on. If even one function fails to pass the error status, A will not know an error occurred.
The solution is exception handling. There are only a few functions where a decision can be made about what to do for rarely-expected failures ('exceptions'). Only in those functions (e.g., function A) is a new syntax used to notify the compiler that exceptions are handled, and how to handle them. In the function that used to return a status (function z ), we use a new syntax to throw an exception instead of return a value. All the functions along the way (e.g., functions B - Y ) are clear of cumbersome error-handling code.
(a) Investigate the following code, and note the new reserved words, try throw catch:
#include <iostream>
#include <exception>
#include <string>
#include <cstdlib>
using namespace std;
struct BadDog: public runtime_error
{
};
BadDog (const string& s_ "This is a bad dog"): runtime_error(s_) {}
void Fifi()
{
throw BadDog ("bad pup"); cerr << "Fifi() after throw" << endl;
}

void Foo()
{
// X x1;
Fifi();
cerr << "Foo() after Fifi()" << endl;
}
void Bar()
{
Foo();
cerr << "Bar() after Foo()" << endl;
}
int main() {
try
{
Bar();
}
catch (bad_alloc&)
{
cerr << "Out of memory! exiting."; exit(2);
}
catch (BadDog& b)
{
cerr << "Bad dog exception: " << b.what(); exit(3);
}
catch(exception& r )
{
cerr << "unknown exception: " << r.what();
exit (4);
}
return 0;
}


(b) Use the debugger to investigate Ctors and Dtors.
(c) Note what gets printed out, and what does not. Why?
(d) What is exception ?
(e) What is logical_error?
(f) What is bad_alloc ?
(g) Remove the catch (BadDog&) and its block. Run it this way and explain the output.
(h) Change the order of the catch blocks. Does the order have any significance?
(i) Add a class that prints in its Ctor and Dtor, and add a local variable of that class to the function Foo() before the call to Fifi (). Use the debugger to investigate the results. What did you learn?
(j) Change the throw line to throw something else. Add a catch(...) block. Why do the catch blocks take the exception objects by reference? Experiment and research.
******************************************************************************************************************************************************************
Exercise 34 - research and experiment with the following:
    -dynamic exception specification(also known as throw list)
    -set_terminate and terminate.
    -set_unexpected and unexpected

***************************************************************************************************************************************************************************
Exercise 35 - Practive investigating a crashed program - create a program that throws an unhandlel /unexpected exeption. open the core file with GDB and investigate.

********************************************************************************************************************************************************************8
Exercise 36 - execption safety - resources

void Foo()
{
     int *ip = new int;
     Bar(ip);
     Fishi();
     Fifi(ip);
     DOdo();
     delete ip;
}

(a) What is the problem ?
(b) How can you solve the problem? find two solutions.
(c) Which one is better ?
(d) How do we avoid having this problem ever again ? What is the new habit ?

************************************************************************************************************************************************************************************
Exercise 37 - Exception safety - Dtors


int Foo (int) throw (bad_alloc);
void Bar() throw (bad_cast);
X::~X()
{
cerr << Foo (m_a) << endl;
}
void Fishi ()
{
}
X x1;
Bar();
(a) What might happen if Bar () throws an exception when called by Fishi () ?
(b) Create similar functioning code that actually throws from Foo (). What happens? Use the debugger to open the core file.
(c) What happens exactly?
(d) How can it be avoided? What is the new exception safety rule regarding throwing exceptions from destructors?
(e) What should a Dtor do when calling a function which might throw?


*********************************************************************************************************************************************************************************\
Exercise 38 - eceptiom safety - Ctors


{
struct X
};
X() { cerr << "X Ctor" << endl; throw bad_cast(); }
X (int) { cerr << "X Ctor (int)" << endl; }
~X() { cerr << "X Dtor" << endl; }
struct Y
{
};
Y() { cerr << "Y Ctor" << endl; }
~Y() { cerr << "Y Dtor" << endl; }
class L
{
public:
L() :m_x(2) { cerr << "L Ctor" << endl; }
~L() { cerr << "L Dtor" << endl; }
private:
Y m_y;
X m_x;
};
class M
{
public:
M() { cerr << "M Ctor" << endl; } ~M() { cerr << "M Dtor" << endl; }
private:
X m_x;
Y m_y;
};


class J
{
public:
J(): m_y (new Y), m_x (new X) { cerr << "J Ctor" << endl; } ~J() { cerr << "J Dtor" << endl; delete m_x; delete m_y; }
private:
Y *m_y;
X *m_x;
};
class K
{
public:
K() { cerr << "K Ctor" << endl; m_y= new Y; m_x= new X; } ~K() { cerr << "K Dtor" << endl; delete m_x; delete m_y; }
private:
Y *m_y;
X *m_x;
};
int main()
{
try
{
}
L varl;
catch (exception & e)
{
cerr << "exception caught. what:"<<e.what() << endl;
}
return 0;
}


(a) Replace varl 's type in main by all the other defined types and investigate the results.
(b) Notice which Ctors and Dtors are executed.
(c) Which classes do we encounter a problem with?
(d) What is the problem?
(e) Why aren't the Dtors called?
(f) How can the problem be avoided? What is the new exception safety rule regarding throwing exceptions from constructors?
***********************************************************************************************************************************************************************
Exercise 39 - Time for the Reference Counted String Exercise
**********************************************************************************************************************************************************************
Exercise 40 - inheritance dynamic allocation pitfall

class X
public:
virtual ~X() {}
private:
};
double m_a;
class Y: public X
{
private:
int m_b;
};
int main()
{
X *xp= new Y[5];
delete[] xp;
return 0;
}

(a) Why does it crash? Hint: Try adding printouts of this in Ctors and Dtors, or removing virtual
(b) What was the programmer's mistake? Why does it compile? What is the lesson learned?
(c) [Advanced]: Try changing the m_a member to be of type int. Explain the results.

*****************************************************************************************************************************************************************
Exercise 41 - Slicing - with classes from the previous example :

void Foo(X, x);
int main()
{
    Y y1;
    Foo(y1)
}

(a) What will the function Foo(0 actually recieve ? Investigate and use the debugger.
(b) Why do we have this behavior ?
(c) What existing habit would have protected us with real code ? Think of real examples of inheritance.


********************************************************************************************************************************************************************************8
Exercise 42 - inheritance assignment pitfall


{
class X
public:
X (double d_) :m_a(d_) {}
virtual ~X() {}
private:
double m_a;
};
class Y: public X
{
public:
Y (double d_, int i_): X(d_), m_b(i_) {}
private:
int m_b;
};
int main()
{
Y y1 (0.0, 0), y2 (14.2, 5);
Y y3(0.0, 0), y4 (14.2, 5);
X &x1 = y1;
X &x2 = y2;
x1 = x2;
y3 y4;
return 0;
}

(a) Use the debugger to look at y1 and y3 after the assignments.
(b) What is wrong ?
(c) What did it happen?
(d) Why is it so wrong ?
(e) Who is to blame ?
(f) How can we avoid this situation ? what is the new habit ?
*************************************************************************************************************************************************************************

Exercise 43 -  Virtual static binding
The fact that a function is virtual does not mean all calls are dynamically bound (using the virtual pointer and table at runtime).
(a) In which situations can the function be statically bound (where the compiler knows at compile time which function should be called)?
(b) What does it mean for inline ?
(c) Which reoccurring function is this an issue for?

**********************************************************************************************************************************************************************
Exercise 44 Research and investigate: protected keyword
(a) What will you put in the protected part of the class?
(b) What will never go in the protected part?
(c) Why? Explain in terms of encapsulation.
*****************************************************************************************************************************************************************
Exercise 45 Research and investigate:
• Pure virtual method
• Abstract class
(a) Can you implement a pure virtual method? Why would you want to? When will you have to?
(b) What happens to a class that inherits an abstract class?
(c) What is the meaning of the syntax =0 ? (No, not an emoticon.)
(d) Why would you declare a Dtor pure virtual? Would you implement it?
Exercise 46- Template classes:
• Research and investigate template classes.
• Write a template class representing a square. The side length member type should be defined by the user. Implement the constructor, 
    a getter and a setter for the side length. There is no need to implement the copy constructor, assignment operator, and destructor (why?).

*************************************************************************************************************************************************************************
Exercise 46 - Template classes:

    -Research and investigate template classes.
    -Write a template class representing a square. the side length member type should be defined by the user. implement the constructor,
    a getter and a setter for the size length. there is no nned to implement the copy constructor, assignment operator and destructor (why ?)


********************************************************************************************************************************************************************************
Exercise 47 - Some Finalizers
(a)Why does a string class not have a virtual destructor ?
(b)What is a clone() function used for ? how is it implemented ?(Note:single line!). Research it.
(c)Exeption safety: what are the common exeption guarantees ?
(d)Exeption safety : State which exception guarantee you would normally specify for each of the following - constructors, destructors, const methds, non-const methods.








*/
