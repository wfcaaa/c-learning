#include<iostream>

using namespace std;

int main()
{
    //************ rule1:make sure initiate built-in variable
    int a = 10;
    int *p = NULL;
    //*********** rule2: try to use member initialization list to initiate class member variable
    class Stu
    {
        public:
            Stu(const string& s1,int num = 0 ):name(s1),age(num){}
        private:
            string name;
            int age;
    };

    //******** rule3: to avoid problem that initalization sequence of compile in different translation unit, it is better to use local static element to take place non-local static element!
    class FileSystem
    {
        FileSystem& tfs()
        {
            static FileSystem fs;
            return fs;
        }
    };

    class File
    {
        File& tmpFile()
        {
            static File f;
            return f;
        }
    };



}
