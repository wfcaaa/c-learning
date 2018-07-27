#include<iostream>
#include<cstring>
using namespace std;
class String
{
    public:
        String(const String &s);
        String(const char* p);
        String operator=(const String &s);
        const char * dis() const;
        ~String();
    private:
        char *ptr;


};

String::String(const String &s)
{
    cout<<"copy constructor fun called"<<endl;
    ptr = new char(strlen(s.ptr)+1);
    if(!ptr)
        cout<<"new fail"<<endl;
    strcpy(ptr,s.ptr);
}
String::String(const char* p)
{
    cout<<"c style fun called"<<endl;
    if( !p || p&&!strlen(p))
    {
        cout<<"input null"<<endl;
        ptr = new char(1);
    }
    else
    {       
        ptr = new char(strlen(p)+1);
        strcpy(ptr,p);
    }

}

String String:: operator=(const String& s)
{
    cout<<"copy assignment fun called"<<endl;
    if(!ptr)
        delete ptr;
     ptr = new char(strlen(s.ptr)+1);
    if(!ptr)
        cout<<"new fail"<<endl;
    strcpy(ptr,s.ptr);
    return *this;

}

const char* String::dis() const
{
    return ptr;
}
String::~String()
{
    cout<<"destructor fun called"<<endl;
    if(!ptr)
        delete ptr;
}

int main()
{
    //String s1; // please rember that when you define your constructor fun, compiler will not help you define constructor fun
    String s2("this is a test");
    String s3(s2);
    String s4("hi");
    s4 = s2;
    cout<<s4.dis()<<endl;
}

