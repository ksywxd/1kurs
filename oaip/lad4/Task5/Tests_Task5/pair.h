#ifndef PAIR_H
#define PAIR_H

template<typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair() : first(), second() {}
    Pair(const T1& f, const T2& s) : first(f), second(s) {}
    Pair(T1&& f, T2&& s) : first(std::move(f)), second(std::move(s)) {}

    // Копирование и перемещение (автоматически генерируются, но для явности)
    Pair(const Pair&) = default;
    Pair(Pair&&) = default;
    Pair& operator=(const Pair&) = default;
    Pair& operator=(Pair&&) = default;
};

#endif // PAIR_H
