#include <iostream>
#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <iterator>
#include <memory>
#include <format>
#include <sstream>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <thread>
#include <numeric>
#include <functional>

using namespace std::literals::string_literals;

template<class T, size_t N>
std::vector<T> get_vector(const T (&arr)[N]) {
    return std::vector<T>(std::begin(arr), std::end(arr));
}

template<class T>
std::unique_ptr<T[]> get_array(const std::vector<T>& vec) {
    try {
        std::unique_ptr<T[]> arr(new T[vec.size()]);
        std::copy(vec.begin(), vec.end(), arr.get());
        return arr;
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        // Handle memory allocation failure (e.g., return nullptr or rethrow)
        throw;
    }
}

template<class T>
void printArray(const std::unique_ptr<T[]>& arr, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i];
        if (i < size - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

std::istream& streamPrint(std::istream& is) {
    std::string buf;
    while (is >> buf) {
        std::cout << buf << "\n";
    }
    is.clear(); // 清除错误标志
    return is;
}

bool isUpperString(const std::string& s) {
    for (const auto sElement: s) {
        if (std::isupper(static_cast<unsigned char>(sElement))) { // char 可能会被解释成带符号类型
            return true;
        }
    }
    return false;
}

template <class T1, class T2>
bool isEqual(const T1& container_L, const T2& container_R) {
    if (container_L.size() != container_R.size()) {
        return false;
    }
    auto L_Iterator = container_L.begin();
    auto R_Iterator = container_R.begin();

    while (L_Iterator != container_L.end() && R_Iterator != container_R.end()) {
        if (*L_Iterator != *R_Iterator) {
            return false;
        }
        ++L_Iterator;
        ++R_Iterator;
    }
    return true;
};

// 确保谓词只接受一个与迭代器指向的元素类型兼容的参数
bool isUpperString_A(const std::string& s) {
    return std::ranges::any_of(s.begin(), s.end(),[](const unsigned char c) {
        return std::isupper(c);
    }); // 此处的lambda函数是一个谓语函数
} // lambda被应用到容器或范围中每一个迭代器所指向的元素

void transToLow(std::string& s) {
    for (auto& sElenmrnt:s) {
        sElenmrnt = std::tolower(static_cast<unsigned char>(sElenmrnt));
    }
}

// 确保谓词只接受一个与迭代器指向的元素类型兼容的参数
void transToLow_A(std::string &s) {
    std::ranges::transform(s.begin(), s.end(), s.begin(), [](const unsigned char c) {
        return std::tolower(c);
    });
} // lambda被应用到容器或范围中每一个迭代器所指向的元素

bool compare(const std::vector<int>& v1, const std::vector<int>& v2);
std::vector<int>::iterator change_val(std::vector<int>::iterator it, const int i);

// 使用流迭代器
template <class InputIter, class OutputIter> // 更加合适命名为读取流与写入流
void myCopy(InputIter begin, InputIter end, OutputIter target) {
    for (auto iter {begin}; iter != end; ++iter, ++target) {
        *target = * iter;
    }
}

// 比较变量与指针指的变量
template <class T>
T& compareValuePointer(const T &a, const T *&b) {
    return  a > *b ? a : *b; // 不考虑相等的情况，如果相等，输出*b
}

// 交换两个int指针
template <class T>
void swapPointer(T *&a, T *&b) {
    T *tempPoint = a;
    a = b;
    b = tempPoint;
}

// 阶乘
int factorial(const int val) {
    if (val > 1) {
        return factorial(val - 1) * val;
    }
    return 1;
}

class Person {
public:
    std::string firstName;
    std::string lastName;

    Person(const std::string& fName, const std::string& lName) : firstName(fName), lastName(lName) {}

    const std::string& getFirstName() const {
        return firstName;
    }
};

std::string getFirstNameOfPerson(const Person& person) {
    return person.firstName;
}

template <class T>
std::ostream& operator<<(std::ostream& ostr, const std::list<T>& list)
{
    for (auto& i : list)
        ostr << ' ' << i;

    return ostr;
}

std::list<std::string> getTotalEnrollment (const std::vector<std::list<std::string>>& courseStudents,
        const std::list<std::string>& droppedStudents) {
    std::list<std::string> allStudents;

    for (auto& lst : courseStudents) {
        allStudents.insert(std::cend(allStudents), std::cbegin(lst), std::cend(lst));
    }
    allStudents.sort();
    allStudents.unique();
    for (auto& str : droppedStudents) {
        allStudents.remove(str);
    }
    return allStudents;
}


namespace CoreGuidelines {

    // std::function<void(std::istream&, int&)> read = [](auto& input, auto& value)
    auto read = [](auto& input, auto& value) { // auto 保证了类型安全
        input >> value;
    }; // 返回值类型为 void

    void print(auto& output, const auto& value) {
        output << value << "\n";
    }

}
int main() {
    // constexpr char cstring_example[]{'C', '+', '+', '\0'};
    // const char* p = cstring_example;
    // const std::string string{"C++"};
    // /*
    //  * strlen 返回字符串长度
    //  * strcmp 返回字符串比较结果
    //  * strcat 返回字符串连接结果
    //  * strcpy 返回字符串拷贝结果
    //  */
    // std::cout << std::format("{:28}", "cstring address:") << &cstring_example << "\n";
    // std::cout << std::format("{:28}", "cstring length:") << std::strlen(cstring_example) << "\n"; // 返回字符串长度，空字符不计算在内
    // std::cout << std::format("{:28}", "p -> cstring address:") << static_cast<const void *>(p) << '\n'; // 返回字符串首地址
    // std::cout << std::format("{:28}", "p -> cstring first address:") << p << '\n'; // 返回字符串首地址，但是一输出字符串
    // std::cout << std::format("{:28}", "p address:") << &p << '\n'; // 返回指针p的地址
    // std::cout << std::format("{:^28}", "___ Vector ___") << '\n';
    // const int myArray[] = {1, 2, 3, 4, 5};
    // const auto intVector = get_vector(myArray);
    // const auto strVector = get_vector(cstring_example);
    // for (auto const&i: intVector) {
    //     std::cout << i << ' ';
    // }
    // std::cout << std::endl;
    // for (auto const&i: strVector) {
    //     std::cout << i << ' ';
    // }
    // std::cout << std::endl;
    // const auto intarray = get_array(intVector);
    // const auto strarray = get_array(strVector);
    // printArray(intarray, intVector.size());
    // printArray(strarray, strVector.size());
    //
    // constexpr int ia[3][4]{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    // const int (*pia)[4] = ia; // 指向数组的指针  也可以理解成指向指针的指针  []可直接用于指针
    // // const int *ppia[4];       // 指针数组
    // std::cout << std::format("{:28}", "ia address:") << ia << '\n';
    // std::cout << std::format("{:28}", "ia[0]:") << *ia[0] << '\n';
    // std::cout << std::format("{:28}", "pia address:") << pia << '\n';
    // std::cout << std::format("{:28}", "pia[0] -> :") << *pia[0] << '\n';
    // std::cout << std::format("{:28}", "pia[1] -> :") << *pia[1] << '\n';
    // std::cout << std::format("{:28}", "pia[2] -> :") << *pia[2] << '\n';
    // std::cout << std::format("{:28}", "pia[0] address -> :") << pia[0] << '\n';
    // std::cout << std::format("{:28}", "pia increment -> :") << *(pia + 1)[0] << '\n';
    // std::cout << std::format("{:28}", "pia[1] address -> :") << pia[1] << '\n';
    // std::cout << std::format("{:28}", "pia[2] address -> :") << pia[2] << '\n';
    // std::cout << std::format("{:28}", "pia[] (-) element size -> :") << (pia[2] - pia[1]) << '\n';
    // std::cout << std::format("{:28}", "pia size :") << sizeof(*pia) << '\n'; // 静态数组大小 -> 16
    // std::cout << std::format("{:28}", "pia size :") << std::size(*pia) << '\n'; // 静态数组长度 -> 4 单个元素 -> 4
    // // std::cout << std::format("{:28}", "pia -> :") << *pia[3]<< '\n'; // 未定义行为
    //
    //
    // constexpr int rowCount = std::size(ia); // 这里得到的是3
    // constexpr int colCount = std::size(ia[0]); // 这里得到的是4
    // // 使用引用遍历数组的元素（推荐）
    // // 这种方式代码简洁，直观，并且正确地使用了引用来防止数组退化成指针，这可以提高代码的可读性和执行效率
    // for (const int (&i)[colCount]: ia) {
    //     // 引用数组
    //     for (const int j: i) {
    //         std::cout << j << ' ';
    //     }
    // }
    // // 使用数组指针遍历
    // // 给出了一种更底层的数据访问方法，展示了如何手动控制数组指针，对理解指针操作很有帮助
    // //
    // std::cout << std::endl;
    // for (const int (*arr)[colCount] = ia; arr != ia + rowCount; ++arr) {
    //     // 数组指针
    //     for (const int* q = *arr; q != *arr + colCount; ++q) {
    //         std::cout << *q << ' ';
    //     }
    // }
    // std::cout << std::endl;
    // // 使用普通索引访问二维数组
    // for (int i = 0; i != rowCount; ++i) {
    //     // 整型指针
    //     for (int j = 0; j != colCount; ++j) {
    //         std::cout << ia[i][j] << ' ';
    //     }
    // }
    // std::cout << std::endl;
    // // exercise 5.14
    // const std::string stringWords = "how now now now brown cow cow cow cow cow cow";
    // // const std::string stringWords = "how now cow";
    // std::istringstream stringStream(stringWords);
    // std::string stringWord;
    // std::string stringWordCache{};
    // std::string maxStringWord{};
    // std::size_t count = 1;
    // std::size_t maxCount = 0;
    // while (stringStream >> stringWord) {
    //     if (stringWord == stringWordCache) {
    //         ++count;
    //         if (count > maxCount) {
    //             maxCount = count;
    //             maxStringWord = stringWordCache;
    //         }
    //     }
    //     else {
    //         count = 1;
    //         stringWordCache = stringWord;
    //     }
    // }
    // maxCount == 0
    //     ? (std::cout << "No duplicate words found\n")
    //     : (std::cout << maxStringWord << " -> " << maxCount << '\n');
    //
    // std::vector<int> v = {1, 2, 3};
    // for (decltype(v.size()) i = 0; i != v.size(); ++i) {
    //     std::cout << v[i] << ' ';
    // }
    // std::cout << std::endl;
    // // exercise 5.17
    // std::vector<int> longVector = {1, 2, 3, 4, 5, 6};
    // std::vector<int> shortVector = {1, 2, 3};
    //
    // bool flag = false;
    // if (shortVector.size() <= longVector.size()) {
    //     decltype(shortVector.size()) i = 0;
    //     for (; i < shortVector.size() && shortVector[i] == longVector[i]; ++i) {
    //     }
    //     if (i == shortVector.size()) {
    //         flag = true;
    //     }
    // }
    // flag
    //     ? (std::cout << "shortVector is a prefix of longVector\n")
    //     : (std::cout << "shortVector is not a prefix of longVector\n");

    // exercise 5.19
    // std::string stringInput1;
    // std::string stringInput2;
    // std::string rsp;
    // do {
    //     std::cout << "Please input two strings: ";
    //     std::cin >> stringInput1;
    //     std::cin >> stringInput2;
    //     std::cout << (stringInput1 <= stringInput2 ? stringInput1 : stringInput2)
    //               << " is less than the other." << "\n\n"
    //               << "More? Enter yes(y) or no: ";
    //     std::cin >> rsp;
    // }
    // while (!rsp.empty() && tolower(rsp[0]) == 'y');

    // exercise 8.1
    // std::istream& is = streamPrint(std::cin);
    // std::cout << is.rdstate() << std::endl;

    // exercise 8.9
    // std::istringstream iStringstreamIn("Amazon 1 2 3");;
    // std::istream& iStringstreamOut = streamPrint(iStringstreamIn);;
    // std::cout << iStringstreamOut.rdstate() << std::endl;

    //exercise 8.10
    // std::ifstream ifStreamIn("../stringStream", std::ios_base::in);
    // std::string line;
    // std::vector<std::string> stringVector;
    // std::cout << "Vecotr Size:" << stringVector.size() << std::endl;
    // std::getline(ifStreamIn, line);
    // std::cout << ifStreamIn.rdstate() << std::endl;
    // ifStreamIn.close(); // 及时释放资源
    // std::istringstream iVectorstringStream;
    // for (const auto& stringelem : stringVector) {
    //     iVectorstringStream.str(stringelem);
    //     std::istream& iVectorstringStreamOut = streamPrint(iVectorstringStream);
    //     std::cout << iVectorstringStream.rdstate() << std::endl;
    // }
    // std::cout << "Vecotr Size:" << stringVector.size() << std::endl;

    // 使用ostream_iterator单行打印元素
    // std::vector<int> myVector {1, 2, 3, 4, 5, 6, 7};
    // auto myVector = std::views::iota(1, 5)
    //      | std::views::transform([](auto const v_element){ return v_element * 2; })
    //      | std::ranges::to<std::vector>();
    // myCopy(std::cbegin(myVector), std::cend(myVector), std::ostream_iterator<int> {std::cout, " "});


    // // 使用传统STL处理Vector，选择所有偶数，偶数翻倍，选择前1000个元素
    // std::vector<int> data(10000);
    // std::iota(data.begin(), data.end(), 1); // Fill with 1 to 10000
    //
    // const auto start = std::chrono::high_resolution_clock::now();
    //
    // // Step 1: Filter even numbers
    // std::vector<int> evens;
    // std::ranges::copy_if(data.begin(), data.end(), std::back_inserter(evens), [](const int x) { return x % 2 == 0; });
    //
    // // Step 2: Double the values
    // std::vector<int> doubled;
    // std::ranges::transform(evens.begin(), evens.end(), std::back_inserter(doubled), [](const int x) { return x * 2; });
    //
    // // Step 3: Select the first 1000 elements
    // std::vector<int> first1000(doubled.begin(), doubled.begin() + 1000);
    //
    // const auto end = std::chrono::high_resolution_clock::now();
    // const std::chrono::duration<double> elapsed = end - start;
    // std::cout << "Traditional STL processing time: " << elapsed.count() * 1000 << " milli seconds." << std::endl;
    //
    // // 使用传统STL优化处理Vector的，选择前2000个元素选择所有偶数，偶数翻倍，
    // std::vector<int> data_optimized(10000);
    // std::iota(data_optimized.begin(), data_optimized.end(), 1);  // Fill with 1 to 10000
    //
    // const auto start_optimized = std::chrono::high_resolution_clock::now();
    //
    // // Step 1: Take the first 2000 elements
    // std::vector<int> first2000_optimized(data_optimized.begin(), data_optimized.begin() + 2000);
    //
    // // Step 2: Filter even numbers
    // std::vector<int> evens_optimized;
    // std::ranges::copy_if(first2000_optimized.begin(), first2000_optimized.end(), std::back_inserter(evens_optimized), [](const int x) { return x % 2 == 0; });
    //
    // // Step 3: Double the values
    // std::vector<int> doubled_optimized;
    // std::ranges::transform(evens_optimized.begin(), evens_optimized.end(), std::back_inserter(doubled_optimized), [](const int x) { return x * 2; });
    //
    // // Step 4: Select the first 1000 elements, if there are at least 1000
    // std::vector<int> first1000_optimized;
    // const auto count_to_take = std::min(static_cast<size_t>(1000), doubled_optimized.size());
    // std::copy_n(doubled_optimized.begin(), count_to_take, std::back_inserter(first1000_optimized));
    //
    // const auto end_optimized = std::chrono::high_resolution_clock::now();
    // const std::chrono::duration<double> elapsed_optimized = end_optimized - start_optimized;
    // std::cout << "Optimized STL processing time: " << elapsed_optimized.count() * 1000 << " milli seconds." << std::endl;
    //
    // 使用Ranges处理Vector
    std::vector<int> data_ranges(10000);
    std::iota(data_ranges.begin(), data_ranges.end(), 1);  // Fill with 1 to 10000

    const auto start_ranges = std::chrono::high_resolution_clock::now();

    auto result_ranges = data_ranges
        | std::views::take(2000)  // Take only the first 2000 elemens
        | std::views::filter([](const int x) { return x % 2 == 0; })  // Filter even numbers
        | std::views::transform([](const int x) { return x * 2; })  // Double the value
        | std::ranges::to<std::vector>();

    // Convert to vector (only now the computations happen)
    const auto end_ranges = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_ranges = end_ranges - start_ranges;
    std::cout << "Ranges processing time: " << elapsed_ranges.count() * 1000 << " milli seconds." << std::endl;

    std::vector<Person> people = {
        {"John", "Doe"},
        {"Jane", "Smith"},
        {"Alice", "Johnson"},
        {"Bob", "Brown"}
    };

    // 使用 ranges::sort 对 people 按 firstName 排序
    // ranges::sort 第三个参数期待一个可以提供排序依据的函数或者函数对象
    // std::ranges::sort(people, {}, [](const Person& element) {return element.getFirstName();});
    // std::ranges::sort(people, {}, getFirstNameOfPerson);
    struct FirstNameCompare {
        bool operator()(const Person& a, const Person& b) const {
            return a.getFirstName() < b.getFirstName();
        }
    };
    std::ranges::sort(people, FirstNameCompare{});
    // 输出排序结果
    for (const Person& p : people) {
        std::cout << p.firstName << " " << p.lastName << std::endl;
    }

    std::vector<double> stockPrices = {150.5, 153.0, 152.5, 155.0, 154.5};

    const auto increasedPrices = stockPrices | std::views::transform([](const double price) { return price * 1.05; });

    // 通常限于存储一个函数指针和几个指向原始数据的迭代器
    std::cout << "Original prices: ";
    std::ranges::for_each (stockPrices, [](const double &price) {
        std::cout << price << " ";
    });
    std::cout << std::endl;

    std::cout << "Increased prices by 5%: ";
    std::ranges::for_each(increasedPrices, [](const double &price) {
        std::cout << price << " ";
    });
    std::cout << std::endl;

    std::vector<std::string> stringVector = {"Hello", "World", "C++20"};
    // 使用std::ranges::for_each遍历并向每个字符串添加" there "
    std::ranges::for_each(stringVector, [](std::string &str){ str.append(" there "); });

    // 在vector中存储引用
    std::string str1 {"Hello"};
    std::string str2 {"World"};

    std::vector<std::reference_wrapper<std::string>> vec{ std::ref(str1) };
    vec.push_back(std::ref(str2));
    vec[1].get() += "!";
    std::cout << str1 << " " << str2 << std::endl;

    // pop_back()不会返回已删除的元素，如果需要则需要通过back()获得这个元素，insert()可用在任意插入元素

    // Learn how to use list.splice
    std::list<std::string> dictionary {"aardvark", "ambulance"};
    std::list<std::string> bWords {"bathos", "balderdash"};
    dictionary.push_back("canticle");
    dictionary.push_back("consumerism");
    if (!bWords.empty()) {
        const auto it { std::end(dictionary)};
        dictionary.splice(it, bWords);
    }
    std::cout << "dictionary:" << dictionary << '\n';

    std::forward_list<int> list1 {5, 6};
    std::forward_list<int> list2 {1, 2, 3, 4};
    std::forward_list<int> list3 {7, 8};

    list1.splice_after(list1.before_begin(), list2);// [-1,^ 0, 1, 2]
    list1.push_front(0);
    // 获取最后一个元素的迭代器
    auto it = list1.before_begin(); // 先获取到链表的前向迭代器（表示链表开始之前）
    if (list1.empty()) {
        std::cout << "The list is empty.\n";
    } else {
        auto iterTemp {it};
        while (++iterTemp != end(list1)) {
            ++it;
        }
    }
    list1.insert_after(it, std::cbegin(list3), std::cend(list3));

    std::vector<decltype(factorial)*> vectorFunction;
    std::vector<int(*)(int, int)> vectorFunctionP;

    // 判定两个vector<int>是否相等
    std::vector<int> vectorCompare1 {1, 2, 3, 4};
    std::vector<int> vectorCompare2 {1, 2, 3, 4, 5};
    std::list<int> listCompare1 {1, 2, 3, 4};
    std::list<int> listCompare2 {1, 2, 3, 4, 5};
    std::cout << "vector 1 Equals vector 2 ? : " << (vectorCompare1 == vectorCompare2) << '\n';
    std::cout << "vector 1 Equals list 1 ? " << isEqual<std::vector<int>, std::list<int>>(vectorCompare1, listCompare1) << '\n';
    std::cout << "vector 1 Equals list 2 ? " << isEqual<std::vector<int>, std::list<int>>(vectorCompare1, listCompare2) << '\n';

    return 0;
}
