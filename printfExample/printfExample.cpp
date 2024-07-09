#include <iostream>

template<typename T1, typename T2>
struct common_type_two {
    using type = decltype(true ? std::declval<T1>() : std::declval<T2>());
};

template <typename ...Ts>
struct common_type {
};

template <typename T0>
struct common_type <T0> {
    using type = T0;
};

template<typename T0, typename T1>
struct common_type<T0, T1> {
    using type = typename common_type_two<T0, T1>::type;
};

template <typename T0, typename T1, typename ...Ts>
struct common_type<T0, T1, Ts...> {
    using type = typename common_type_two<T0, typename common_type<T1, Ts...>::type>::type;
};

template <typename T0, typename ...Ts>
constexpr auto get_common_type(T0 t0, Ts ...ts) {
    if constexpr (sizeof...(Ts) == 0) {
        return t0;
    } else {
        return true ? t0 : get_common_type(ts...);
    }
}
// template <typename T0, typename ...Ts>
// struct common_type<T0, Ts...> {
//     using type = typename common_type_two<T0, typename common_type<Ts...>::type>::type;
// };

template<typename T>
static void print(T t) {
    std::cout << t << '\n';
}

template<typename... Ts>
auto func(Ts... ts) {
    (print(ts), ...);
    // return (0 + ... + ts);
    // this will expand to:
    // return (ts + ...); // This will not work because the first element is 0
}


int main() {
    func("Hello", 2, 3, 4, 5);
    return 0;
}
