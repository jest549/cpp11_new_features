
#include <condition_variable>
#include <iostream>
#include <initializer_list>
#include <mutex>
#include <vector>
#include <map>
#include <memory>
#include <array>
#include <forward_list>
#include <thread>
#include <chrono>
#include <future>
#include <cstring>
#include <stdarg.h>

//**************nullptr的使用**************
char *p_test_0 = ((char *)0);
char *p_test = NULL;
char *cpp_test = nullptr;

//**************constexp的使用**************
constexpr int len_foo_constexpr() { return 5; } //函数,C++11只支持一些简单的表达式，如不能调用printf函数、不能定义未初始化的变量。
constexpr int len_2_constexpr = 1 + 2 + 3;      //变量
int aa[len_2_constexpr];
//**************auto**************
class MagicFoo
{
public:
    std::vector<int> vec;
    //**************std::initializer_list参数**************
    MagicFoo(std::initializer_list<int> list)
    {
        for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it)
            vec.push_back(*it);
    }
    void foo(std::initializer_list<int> list)
    {
        for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it)
            vec.push_back(*it);
    }
    //**************auto关键字在循环中应用**************
    void printVal()
    {
        
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            std::cout << *it << ", ";
        }
        std::cout << std::endl;
    }
    void printVal2()
    {
        for (auto element : vec)
        {
            std::cout << element << ", ";
        }
        std::cout << std::endl;
    }
};
////**************函数模板中使用decltype,C++11**************
template <typename T, typename U>
auto myadd11(T x, U y) -> decltype(x + y)
{
    return x + y;
}
int t = 0;
int func(int a, char *s)
{
    printf("a=%d,s=%s\n",a,s);
    return 0;
}

//**************delete default override final关键字**************
namespace hello_world
{
    template <typename T>
    class vector: public std::enable_shared_from_this<vector<T>>
{
public:
    char *p = nullptr;
    int k;
    vector()
    {
        p = new char[1000];
        k = 100;
    };
    vector(int init_value) : vector()
    { //同一个类中一个构造函数调用另一个构造函数
        k = init_value;
    }
    virtual ~vector()
    {
        if (p)
        {
            printf("~vector ,del [cahr *p]\n");
                delete[] p;
                p=nullptr;
        }
            
    }
    vector(const vector &obj) = delete;
    vector &operator=(const vector &obj) = default;
    virtual void push_back(const T &t);
    virtual int func(int a, char *s);
    std::shared_ptr<vector<T>> get_share_ptr();
};

template <typename T>
 void vector<T>::push_back(const T &t) 
    {
        if (p && sizeof(t) < 1000)
        {
            memcpy(p + sizeof(t), &t, sizeof(t));
        }
    }
template <typename T>
int vector<T>::func(int a, char *s){};
template <typename T>
std::shared_ptr<vector<T>> vector<T>::get_share_ptr()
{
    return std::enable_shared_from_this<vector<T>>::shared_from_this();
}
template <typename U> 
class sub_vector final : public vector<U>
{
    private:
        /* data */
    public:
        using vector<U>::vector; //继承构造
        //sub_vector(/* args */){};
        ~sub_vector(){
            printf("~sub_vector\n");
             vector<U>::~vector();
        };
        void push_back(const U &t) override;
        int func(int a, char *s) override;
        std::shared_ptr<sub_vector<U>> get_share_ptr();

};
template <typename U>
void  sub_vector<U>::push_back(const U &t) 
{
    char *_ptr = this->p;
    for (int i = 0; _ptr && (i * sizeof(t) < 1000); i++)
    {
        memcpy(_ptr + (i * sizeof(t)), &t, sizeof(t));
    }
}
template <typename U>
int sub_vector<U>::func(int a, char *s)
{
    printf("sub_vector,a=%d,s=%s\n",a,s);
    return 0;
}
template <typename U>
std::shared_ptr<sub_vector<U>> sub_vector<U>::get_share_ptr()
{
    return std::enable_shared_from_this<sub_vector<U>>::shared_from_this();
}
}    // namespace hello_world

//**************using关键字**************
int (*pfunc)(int a, char *s);
typedef int (*pfunc_type)(int a, char *s);
using cpp_pfunc_type = int (*)(int a, char *s);
using namespace hello_world;
using mystuct = std::map<std::string, std::vector<std::string>>; //c++11支持正确识别>>
mystuct my_map1;
std::map<std::string, std::vector<std::string>> m_map3=my_map1;



mystuct my_map2=my_map1;
//**************枚举类型**************
//枚举类型并非类型安全，枚举类型会被视作整数，则会让两种完全不同的枚举类
//型可以进行直接的比较（虽然编译器给出了检查，但并非所有），甚至同一个命名空间中的不同枚举类型
//的枚举值名字不能相同，
enum class old_enum : unsigned char
{ //指定unsigned char类型
    value1,
    value2,
    value3 = 100,
    value4 = 100
};

enum class new_enum : unsigned int
{ //指定unsigned int类型
    value1,
    value2,
    value3 = 100,
    value4 = 100
};
new_enum test_enum = new_enum::value2;
//**************变长参数**************
//CPP
template <typename T0>
void printf1(T0 value)
{
    std::cout << value;
}
template <typename T, typename... Ts>
void printf1(T value, Ts... args)
{

    std::cout << value << " sizeof=" << sizeof...(args) << " ";
    printf1(args...);
}

//C
void c_printf(const char *fmt, ...)
{
    char buf[1024] = {0};
    char *p_buf = buf;
    u_char *p;
    va_list args;

    va_start(args, fmt);
    printf("input=%s\n", fmt);
    while (*fmt)
    {

        if (*fmt == '%')
        {
            printf("input=%c\n", *fmt);
            switch (*++fmt)
            {

            case 'd': //%u，这个u表示无符号
            {
                int i64 = (int64_t)va_arg(args, int); //va_arg():遍历可变参数，var_arg的第二个参数表示遍历的这个可变的参数的类型
                int len = sprintf(p_buf, "%d", i64);
                if (len > 0)
                {
                    p_buf += len;
                }
            }
            break;
            case 'x': //%x，
            {
                int i64 = (int64_t)va_arg(args, int);
                int len = sprintf(p_buf, "0x%x", i64);
                if (len > 0)
                {
                    p_buf += len;
                }
            }

            break;
            case 'f':
            {
                double d = (double)va_arg(args, double);
                int len = sprintf(p_buf, "%f", d);
                if (len > 0)
                {
                    p_buf += len;
                }
            }

            break;
            case 's':
            {
                char *str = (char *)va_arg(args, u_char *); //va_arg():遍历可变参数，var_arg的第二个参数表示遍历的这个可变的参数的类型
                int len = sprintf(p_buf, "%s", str);
                if (len > 0)
                {
                    p_buf += len;
                }
            }

            break;
            default:
                break;
            }
            fmt++;
        }
        else
        {
            *p_buf++ = *fmt++;
        }
    }
    va_end(args);
    printf("%s", buf);
}


//**************左值与右值  完美转发**************
void my_test_func(int v1,int &v2,int &&v3)
{
    v2=v1+v3;
    std::cout<<"v1="<<v1<<",v2="<<v2<<",v3="<<v3<<std::endl;v1=0;v3=0;
}
template <typename F,typename T,typename U,typename W>
void my_temp_test_func_error1(F func_,T t,U u,W w)//转发与原定义函数不匹配1
{
    func_(t,u,std::move(w));
}
template <typename F,typename T,typename U,typename W>
void my_temp_test_func_error2(F func_,T &&t,U &&u,W &&w)//转发与原定义函数不匹配2
{
    func_(t,u,w);
}
template <typename F,typename T,typename U,typename W>
void my_temp_test_func_error3(F func_,T &&t,U &&u,W &&w)//转发与原定义函数匹配,//typename  W &&w 失去了万能引用的功能了
{
    func_(t,u,std::move(w));  
}
template <typename F,typename T,typename U,typename W>
void my_temp_test_func_true(F func_,T &&t,U &&u,W &&w)
{
    func_(std::forward<T>(t),std::forward<U>(u),std::forward<W>(w));
}


//**************线程类*************
std::once_flag ccall_one_flag;
class CCall_Once
{
 
public:
    static CCall_Once *get_instance()
    {
        std::call_once(ccall_one_flag,create_instance);
        return p_call_once;
    }

    class Delete
    {
        public:
        ~Delete()
        {
            if(CCall_Once::p_call_once!=nullptr)
            {
                delete p_call_once;
            }
        }
    };
private:
    static CCall_Once *p_call_once;
    CCall_Once(){};
    static Delete del;   
    static void create_instance()
    {
        if(nullptr==p_call_once)
        {
            p_call_once=new CCall_Once();
        }
    }
};
CCall_Once * CCall_Once::p_call_once=nullptr;//定义

class CMyThread
{
public:
    CMyThread():thread1_(std::bind(&CMyThread::print,this)),thread2_(std::bind(&CMyThread::notify_print,this))
                ,thread3_(std::bind(&CMyThread::print3,this))
    {

    }
     ~CMyThread()
    {
        thread1_.join();
        thread2_.join();
    }
    void print()
    {
          //printf("run ++++++\n");
        for(int i=0;i<100000;i++)
        {
            {
                std::unique_lock<std::mutex> its_lock(mutex1_2_);
                //std::unique_lock<std::mutex> its_lock(mutex1_2_,std::try_to_lock);
                   //这里是有bug的，会一直停留在这
                cond1_.wait(its_lock);

            }
            printf("run i=%d\n",i);

            //std::this_thread::sleep_for(std::chrono::milliseconds(900));
        }
   
    }
    void notify_print()
    {

        for(int i=0;i<100000;i++)
        {
         
            {
                std::lock_guard<std::mutex> its_lock(mutex1_2_);
                // std::unique_lock<std::mutex> its_lock(mutex1_2_);
                //std::unique_lock<std::mutex> its_lock(mutex1_2_,std::try_to_lock);
                         printf("notify ====%d\n",i);
                cond1_.notify_one();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
        }
   
    }
        void print3()
    {
          //printf("run ++++++\n");
        for(int i=0;i<1000;i++)
        {
            for(int j=0;j<10;j++)
            {
                r_mutex3_.lock();
                for(int k=0;k<2;k++)
                {
                    r_mutex3_.lock();
                   
                }
            }
       
            //r_mutex3_.unlock();
            //std::this_thread::sleep_for(std::chrono::milliseconds(900));
            //printf("r_mutex3_ i=%d\n",i);//最多嵌套1000*2*10次
        }
   
    }
private:
    std::thread thread1_;
    std::mutex mutex1_2_;
    std::thread thread2_;
    std::condition_variable cond1_;

    std::thread thread3_;
    std::recursive_mutex r_mutex3_;
    //std::recursive_timed_mutex t_t_mutex4_;//多了for和until方法


};




int async_th(int i)
{
    std::cout<<"async_th run start====i="<<i<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::cout<<"async_th run end,3s===="<<std::endl;
    return 10;
    
}
void async_th_wait(std::future<int> *state)
{
    printf("async_th_wait=====start \n");
    std::future_status w_status=state->wait_for(std::chrono::seconds(2));
    if(w_status==std::future_status::ready)
    {
        printf("async_th_wait===== already wait 2s ，ready \n");
    }
    else if(w_status==std::future_status::timeout)
    {
        printf("async_th_wait===== already wait 2s， timeout\n");
    }
    else if(w_status==std::future_status::deferred)//被延时
    {
        printf("async_th_wait===== already wait 2s ,deferred\n");
    }
       
    printf("=====>get state =%d \n",state->get());
    printf("async_th_wait=====end \n");
}
#if 0
void promise_test(std::promise<vector<int>> &in)
{
    printf("share_th_test=====start \n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    in.set_value(10);
    printf("share_th_test=====done \n");
}
#endif

void share_th_test(std::shared_ptr<vector<int>> in)
{
    std::shared_ptr<vector<int>> p_in=std::move(in);
    printf("share_th_test=====start \n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("share_th_test=====done \n");
}

int main(int argc, char **argv)
{
    //**************内置类型的初始化**************
    int a1(5);
    int a2{5};
    int a3 = {5};
    int array[]{1, 2, 3, 4, 5};

    int *p_begin = std::begin(array);
    int *p_end = std::end(array);
    for (; p_begin != p_end; p_begin++)
    {
        printf("%d ", *p_begin);
    }
    printf("\n");
    MagicFoo magicFoo = {1, 2, 3, 4, 5};
    magicFoo.foo({6, 7, 8, 9});
    magicFoo.printVal2();
    magicFoo.printVal();
    auto i = 5;
    auto x = 1;
    auto arr = new auto(10);
    decltype(x + i) z;
    if (std::is_same<decltype(x), int>::value)
    {
        std::cout << "type x == int" << std::endl;
    }
    auto w = myadd11<int, double>(1, 2.0); //w是一个double类型
    std::cout << "t=" << t << std::endl;

    pfunc_type pf = func;
    cpp_pfunc_type cpp_pf = func;
    pfunc = func;
    sub_vector<int> s_v;
    s_v.push_back(10);
    //sub_vector<int> s_v2(s_v);//error
    printf1(1, 2, "123", 1.1);
    c_printf("hello:%d %s  %f\n", 1, "hello", 72.1);
    printf1("hello:", 1, "hello", 72.1, "\n");
    auto lambda_exp = [] { printf("%u\n", static_cast<unsigned int>(test_enum)); };
    lambda_exp();

    std::array<int, 40> my_arr;
    my_arr[5] = 10;
    std::forward_list<int> for_list;
    for_list.push_front(10); //链首添加元素

    std::vector<int> test_vec(20);
    for (int i = 0; i < test_vec.size(); i++)
		test_vec[i] = i;
    std::cout << "size=" << test_vec.size() <<"capacity="<<test_vec.capacity()<<"test_vec[19]="<<test_vec[19]<< std::endl;
    test_vec.resize(10);
    std::cout << "size=" << test_vec.size() <<"capacity="<<test_vec.capacity()<<"test_vec[19]="<<test_vec[19]<< std::endl;
    test_vec.shrink_to_fit();
    std::cout << "size=" << test_vec.size() <<"capacity="<<test_vec.capacity()<<"test_vec[19]="<<test_vec[19]<< std::endl;//再访问test_vec[19]有风险
    std::bind(func,std::placeholders::_1,std::placeholders::_2)(10,(char*)"hello");
    std::bind(func,20,std::placeholders::_1)((char*)"hello_world");
    auto bin_func= std::bind(func,30,std::placeholders::_1);
    bin_func((char*)"my hello");
    std::function<int (int,char*)>function_func= std::bind(func,std::placeholders::_1,std::placeholders::_2);
    function_func(40,(char*)"my hello");
    function_func= std::bind(&sub_vector<int>::func,&s_v,std::placeholders::_1,std::placeholders::_2);//绑到类的实例化实体时，需要加&取地址符，需要注意实体是否已经销毁。
    function_func(50,(char*)"hello");
    int mv1=1;
    int mv2=2;
    int mv3=3;
    //my_test_func(mv1,mv2,mv3);//error
    my_test_func(mv1,mv2,std::move(mv3));
    std::cout<<"mv1="<<mv1<<",mv2="<<mv2<<",mv3="<<mv3<<std::endl;
    mv1=1;mv2=2;mv3=3;
    my_temp_test_func_error1(my_test_func,mv1,mv2,std::move(mv3));
    std::cout<<"mv1="<<mv1<<",mv2="<<mv2<<",mv3="<<mv3<<std::endl;
    mv1=1;mv2=2;mv3=3;
    //my_temp_test_func_error2(my_test_func,mv1,mv2,std::move(mv3));//error 无法编译
    my_temp_test_func_error3(my_test_func,mv1,mv2,std::move(mv3));//
    std::cout<<"mv1="<<mv1<<",mv2="<<mv2<<",mv3="<<mv3<<std::endl;
    mv1=1;mv2=2;mv3=3;
    my_temp_test_func_true(my_test_func,mv1,mv2,std::move(mv3));
    std::cout<<"mv1="<<mv1<<",mv2="<<mv2<<",mv3="<<mv3<<std::endl;
    std::thread *test_share;
    //************智能指针**********************
    {
        std::shared_ptr<vector<int>> m_sp1(std::make_shared<sub_vector<int>>());
        m_sp1->func(100,(char*)"hello");
        (*m_sp1).get_share_ptr()->func(100,(char*)("hello2"));
       test_share=new std::thread(share_th_test,m_sp1);
    }
    
    test_share->join();
    delete test_share;

    std::shared_ptr<std::vector<int>> sp1(std::make_shared<std::vector<int>>());
    (*sp1).push_back(10);
    (*sp1).push_back(20);
    std::cout<<"*sp1[0]="<<(*sp1)[0]<<",*sp1[1]="<<(*sp1).at(1)<<std::endl;
    {
        std::weak_ptr<std::vector<int>> wp = sp1;
        std::cout<<"*sp1[0]="<<(*wp.lock())[0]<<",*sp1[1]="<<(*wp.lock()).at(1)<<std::endl;
        wp.reset();
    }
    std::unique_ptr<std::vector<int>> up1(new std::vector<int>(2,1));
    sp1=std::move(up1);
    //std::cout<<"*up1[0]="<<(*up1)[0]<<",*up[1]="<<(*up1).at(1)<<std::endl;//error
    std::cout<<"*sp1[0]="<<(*sp1)[0]<<",*sp1[1]="<<(*sp1).at(1)<<std::endl;


    CMyThread c_thread;
    std::thread thread1_(func,1,(char*)("hello"));
    thread1_.join();

    

    //这种初始化方式不行,允许在类的构造函数中对std::thread传入参数初始化
    //std::thread thread1_;
    //thread1_(func,1,(char*)("hello"));

    std::thread *p_thread1_=nullptr;
    p_thread1_=new std::thread(func,2,(char*)("hello"));


    std::future<int >result_=std::async(std::launch::async,async_th,10);
    std::thread p_thread2_(async_th_wait,&result_);



    std::packaged_task<int(int) >my_pt(async_th);
    std::vector<std::packaged_task<int(int)> > v_task;
    //v_task.push_back(std::move(my_pt));
    std::thread p_thread3_(std::ref(my_pt),20);
    std::future<int >result3_=my_pt.get_future();
    std::thread p_thread4_(async_th_wait,&result3_);


    p_thread2_.join();
    p_thread1_->join();
    p_thread4_.join();
    p_thread3_.join();
    delete p_thread1_;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}
