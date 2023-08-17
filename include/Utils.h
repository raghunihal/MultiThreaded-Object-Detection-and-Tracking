#define get_ticks cv::getTickCount
#define get_freq  cv::getTickFrequency
/**********************************************************/
double dt(int64 t) 
{ 
    return double(t*1000/get_freq())/1000.0; 
}
/**********************************************************/
struct Profile
{
    string name;
    int64 t; // accumulated time
    int64 c; // function calls

    Profile(const string & name) 
        : name(name)
        , t(0) 
        , c(0)
    {

    }   

    ~Profile() 
    {
        cerr << format("%-24s %8u ",name.c_str(),c);
        cerr << format("%13.6f ",dt(t/c)); 
        cerr << format("%13.6f ",dt(t));
        cerr << format("%14u",t);
        cerr << endl;
    }

    struct Scope
    {
        Profile & p;
        int64 t;

        Scope(Profile & p) 
            : p(p) 
            , t(get_ticks()) 
        {}

        ~Scope() 
        { 
            int64 t1 = get_ticks();
            if ( t1 > t )
            {
                p.t += t1 - t;
                p.c ++;
            }
         }
    }; 
};
/**********************************************************/
#define PROFILEX(s) static Profile _a_rose(s); Profile::Scope _is_a_rose_is(_a_rose);
#define PROFILE PROFILEX(__FUNCTION__)
/**********************************************************/
/*
void foo() {
   PROFILE;
   int k=10;
   while (k) {
       PROFILEX("foo while loop");
       k --;
   }
}
void bar() {
   PROFILE;
}

int main() {
    PROFILE;
    {
        PROFILEX("useless scope");
        bar();
        bar();
    }
    foo();
    return 0;
}*/