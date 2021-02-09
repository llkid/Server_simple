#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <type_traits>
#include <iomanip>
#include <future>
#include <chrono>
#include <functional>
#include <thread>

#include "RBTree.h"

//< 函数模板
template<typename T>
bool equivalent(const T& a, const T& b)
{
	return !(a < b) && !(a > b);
}

//< 类模板
template<typename T = int>
class bignumber {
	T _v;
public:
	bignumber(T a) : _v(a) {}
	inline bool operator<(const bignumber& b) const;
	inline bool operator>(const bignumber& b) const;
};

template<typename T>
inline bool bignumber<T>::operator<(const bignumber& b) const
{
	return this->_v < b._v;
}

template<typename T>
inline bool bignumber<T>::operator>(const bignumber& b) const
{
	return this->_v > b._v;
}

template<typename T1, typename T2> 
class theSameType { 
public: 
	enum { 
		ret = false
	}; 
};

template<typename T>
class theSameType<T, T> {
public:
	enum 
	{
		ret = true
	};
};

template<typename T, int i>
class aTMP
{};

namespace test {
	template<typename T>
	class aTMP {
	public: 
		using reType = const T;
		static void showType() {
			std::cout << typeid(reType).name();
		}
	};

	void f()
	{
		std::cout << "global f()\n";
	}

	template<typename T>
	class Base {
	public:
		template<int N = 99>
		void f()
		{
			std::cout << "member f(): " << N << '\n';
		}
	};

	template<typename T>
	class Derived : public Base<T>
	{
		typename T::reType m;
	public:
		Derived(typename T::reType& a) : m(a) { }
		void df1() { f(); }
		void df2() { this->f(); }
		void df3() { Base<T>::template f<22>(); }
		void df4() { test::f(); }
		void out() { std::cout << m << '\n'; }
	};

	template<typename T, int i = 1>
	class someComputing {
	public:
		typedef volatile T* retType;
		enum {
			retValue = i + someComputing<T, i - 1>::retValue
		};

		static void f() {
			std::cout << "someComputing:i = " << i << '\n';
		}
 	};

	template<typename T>
	class someComputing<T, 0> {
	public:
		enum 
		{
			retValue = 0
		};
	};
}

using F = std::function<int(int, int, int)>;

int testFuc(int a, int b, int c)
{
	return a + b + c;
}

void thread1(std::promise<F>& p)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::cout << "load parameter...\n";
	p.set_value(std::bind(&testFuc, 
		std::placeholders::_1, 
		std::placeholders::_2,
		std::placeholders::_3));
}

template<typename T, typename... Args>
void thread2(std::future<T>& f, Args&&... args)
{
	auto func = f.get();
	auto res = func(std::forward<Args>(args)...);
	std::cout << "received parameters: " << res << '\n';
}

int main(int argc, char** argv)
{
	// [1]
	/*bignumber<> a{ 1 }, b{ 1 };
	std::cout << equivalent(a, b) << '\n';
	std::cout << equivalent<double>(1, 2) << '\n';*/

	// [2]
	//typedef unsigned int uint;
	//typedef uint uint2;
	//
	//std::cout << theSameType<unsigned, uint2>::ret << '\n';
	//// aTMP<uint2, 2> 指类类型
	//std::cout << theSameType<aTMP<unsigned, 2>, aTMP<uint2, 2>>::ret << '\n';
	//std::cout << theSameType<aTMP<unsigned, 2>, aTMP<uint2, 3>>::ret << '\n';

	//test::Derived<test::aTMP<double>> a(10);
	//a.df1();
	//a.df2();
	//a.df3();
	//a.df4();
	//a.out();

	//test::aTMP<double>::showType();

	// [3]
	/*test::someComputing<int>::retType a = 0;
	std::cout << sizeof(a) << '\n';
	test::someComputing<int, 99>::f();

	std::cout << test::someComputing<int, 40>::retValue << '\n';*/

	//std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
	//std::shared_future<void> ready_future(ready_promise.get_future());
	//std::chrono::time_point<std::chrono::high_resolution_clock> start;

	//auto fun1 = [&, ready_future]()->std::chrono::duration<double, std::milli> {
	//	t1_ready_promise.set_value();
	//	ready_future.wait();
	//	return std::chrono::high_resolution_clock::now() - start;
	//};

	//auto fun2 = [&, ready_future]()->std::chrono::duration<double, std::milli> {
	//	t2_ready_promise.set_value();
	//	ready_future.wait();
	//	return std::chrono::high_resolution_clock::now() - start;
	//};

	//auto result1 = std::async(std::launch::async, fun1);
	//auto result2 = std::async(std::launch::async, fun2);

	//t1_ready_promise.get_future().wait();
	//t2_ready_promise.get_future().wait();

	//start = std::chrono::high_resolution_clock::now();

	//ready_promise.set_value();
	//std::cout << "Thread 1 received the signal "
	//	<< result1.get().count() << " ms after start!\n"
	//	<< "Thread 2 received the signal "
	//	<< result2.get().count() << " ms after strat!\n";

	//std::promise<F> pro;
	//std::future<F> future_pro = pro.get_future();

	//std::thread tr1(thread1, std::ref(pro));
	//std::thread tr2(thread2<F, int, int, int>, std::ref(future_pro), 1, 2, 3);

	//tr1.join();
	//tr2.join();

	//{
	//	// 在 promise 带有返回值时，获取该值直接调用 get() 无需调用 wait() 方法，
	//	// 有且只能调用一次
	//	std::promise<int> ready_promise, t1_ready_promise, t2_ready_promise;
	//	std::shared_future<int> ready_future(ready_promise.get_future());
	//	std::chrono::time_point<std::chrono::high_resolution_clock> start;

	//	auto func1 = [&, ready_future]()->std::chrono::duration<double, std::milli> {
	//		t1_ready_promise.set_value(1);
	//		// 多线程调用 cout 出现线程交叉运行 
	//		/// TODO
	//		std::cout << "Thread " << std::this_thread::get_id() << " get value " << ready_future.get() << '\n';
	//		return std::chrono::high_resolution_clock::now() - start;
	//	};

	//	auto func2 = [&, ready_future]()->std::chrono::duration<double, std::milli> {
	//		t2_ready_promise.set_value(2);
	//		std::cout << "Thread " << std::this_thread::get_id() << " get value " << ready_future.get() << '\n';
	//		return std::chrono::high_resolution_clock::now() - start;
	//	};

	//	auto result1 = std::async(std::launch::async, func1);
	//	auto result2 = std::async(std::launch::async, func2);

	//	start = std::chrono::high_resolution_clock::now();

	//	ready_promise.set_value(0);

	//	std::cout << "Thread1 received the signal "
	//		<< result1.get().count() << " ms after start!\nt1_ready_promise get value "
	//		<< t1_ready_promise.get_future().get() << "\nThread2 received the signal "
	//		<< result2.get().count() << "ms after start!\nnt2_ready_promise get value "
	//		<< t2_ready_promise.get_future().get() << '\n';
	//}

	std::unique_ptr<RBTree<int>> tree(new RBTree<int>());
	std::vector<int> vec{ 10,7,8,15,5,6,11,13,12 };
	for (int x : vec) {
		tree->insert(x);
	}

	tree->inOrder();
	std::cout << '\n';
	tree->inorderPrint();
	std::cout << '\n';
	tree->remove(13);
	tree->inOrder();
	std::cout << '\n';
	tree->remove(21);

	return 0;
}
