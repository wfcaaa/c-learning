#include<iostream>
#include<cstring>
using namespace std;

class Rational
{
    public:
        Rational(int a):num(a){}
        int num;
};
const Rational operator*(const Rational&,const Rational&);

 const Rational operator*(const Rational& Rational1,const Rational& Rational2)
    {
        return Rational1.num*Rational2.num;
    }


int main()
{
    //-------------in this case1 , I execute assignment operation in if statemnet, it is wrong!
#if 0
    int a=1,b=2,c=3;
    if(a*b=c)
        cout<<"sdf"<<endl;
    else
        cout<<"greate"<<endl;
#endif

   
    Rational Rational1(1),Rational2(2),Rational3(3);
    //----------case2: return value type 
    //-------------question: why set return value to const?
    //--------------answer: if customer use in such case: if(Rational1 * Rational2 = Rational3),this will cause big error, you can refer to the case1 error!
    //

    //case 3: parameter
    // when we do not need to change the parameter we should do such as declartion :fun(const string str) / const string s2;const can save us a lot of unneccssary trouble,such you want to type
    //"==",but,type into "="


    //case 4: const member function
    // the fact,actually,that is is very important is the difference of constness between two member function can be overload!!!
     class TextBlock
    {
        public:
            TextBlock(const string& s1):text(s1){}
            const char& operator[](size_t position) const  // operator[] for const element
            {
                return text[position];
            }

            char& operator[](size_t position) //operator[] for non-const element
            {
                return text[position];
            }
        private:
            string text;
    };
    TextBlock t1("abc");
    const TextBlock ct2("qwe");
    t1[0] = 'f'; //call the operator[] for non-const ,right!
    //ct2[0] = 'x'; call the operator[] for const , assignment to const wrong


    //Tip: concept: bitwise constness(the member function who will not chang any bit of class  is "const" ); logical constness
    //how to delaration a "const" member function : char& operator[](size_t position) const , this is "const" member functio, in this function ,we cant not chang the class member
    // sunch as :char& operator[](size_t position) const{text="ttttt";} this is wrong , as we chang the class memeber!!!

    //-----logical constness : A const member function can modify some bits within the object it processes,but only if the client cannot detect them
    class CTextBlock
    {
        public:
            CTextBlock(char* s):pText(s),lenthIsValid(true)
            {
                textLength = strlen(pText);

            }
            size_t lenth() const//"const member function"
            {
                if(!lenthIsValid)
                {
                   // textLength = strlen(pText);// wrong in const member function we can not assign to class member:textLength , lengthIsValid
                    //lengthIsValid = true;
                }
                return textLength;
            }
        private:
            char *pText;
            size_t textLength;
            bool lenthIsValid;

    };
    //now , we want to make the lenth() function to be "const",but we also want to change the value of class member:textLength,lenthIsValid
    //how to? we can use key word: mutable to relase the limition of non-staict member variable!!!
     class CTextBlock_
    {
        public:
            CTextBlock_(char* s):pText(s),lenthIsValid(true)
            {
                textLength = strlen(pText);

            }
            size_t lenth() const//"const member function"
            {
                if(!lenthIsValid)
                {
                    textLength = strlen(pText);// right,as we have add key word mutable before textLength and lenthIsValid!!!
                    lenthIsValid = true;
                }
                return textLength;
            }
        private:
            char *pText;
            mutable size_t textLength;
            mutable bool lenthIsValid;

    };

     // ok!!! now we go through our code , you can find the code is too fat!!! so we do so to replace non-const version 
      class TextBlock_
    {
        public:
            TextBlock_(const string& s1):text(s1){}
            const char& operator[](size_t position) const  // operator[] for const element
            {
                cout<<"in const"<<endl;
                return text[position];
            }

            char& operator[](size_t position) //operator[] for non-const element
            {
                cout<<"in non const"<<endl;
                return const_cast<char&>(static_cast<const TextBlock_&>(*this)[position]);
            }
        private:
            string text;
    };

    TextBlock_ tb1("asdfgh");
    for(size_t i = 0;i < 3;++i)
    {    cout<<tb1[i]<<endl;
        tb1[i]='x';
        cout<<"after assignment: "<<tb1[i]<<endl;
    }
    

}
