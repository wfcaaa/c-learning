#include<iostream>

using namespace std;

// explicitly disallow the use of compiler-generated functions you do not want
// for example, wo  want the class HomeForSale is unique, we cant declar the copy constructor fun and copy assignment into private and do not implement it , therefore , compiler will not 
// genetat copy constructor fun and copy assignment fun!!!
class HomeForSale
{
    public:
        HomeForSale(){}
    private:
        HomeForSale(const HomeForSale&);
        HomeForSale operator=(const HomeForSale&);
};

//******* we can define a class for upcopy 

class Uncopyable
{
    public:
        Uncopyable(){}
        ~Uncopyable(){}
    private:
        Uncopyable(const Uncopyable&);
        Uncopyable& operator=(const Uncopyable&);
};

class HomeForSale_:public Uncopyable
{
    public:
        HomeForSale_(){}
};
int main()
{
    HomeForSale h1;
    HomeForSale h2;
    //h1 = h2;
    HomeForSale_ h3;
    HomeForSale_ h4;
    //h3 = h4;
}
