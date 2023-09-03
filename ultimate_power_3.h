#pragma once

// Include libs individually for VS compatibility (no stdc++.h in MSVC)
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <array>
#include <chrono>
#include <forward_list>
#include <initializer_list>
#include <random>
#include <regex>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <cassert>

// Main macro definition for all debugging features
#define echo(...) {istringstream str(#__VA_ARGS__); __echo(str, __VA_ARGS__); cerr << clc::reset;}
using namespace std;


/* -------------------- Color codes in constants --------------------------- */
namespace clc
{                                           
    const char* black = "\033[30m",   // 0
        * red = "\033[31m",   // 1
        * green = "\033[32m",   // 2
        * yellow = "\033[33m",   // 3
        * blue = "\033[34m",   // 4
        * magenta = "\033[35m",   // 5
        * cyan = "\033[36m",   // 6
        * white = "\033[37m",   // 7

        * br_black = "\033[90m",   // 8
        * br_red = "\033[91m",   // 9
        * br_green = "\033[92m",   // 10
        * br_yellow = "\033[93m",   // 11
        * br_blue = "\033[94m",   // 12
        * br_magenta = "\033[95m",   // 13
        * br_cyan = "\033[96m",   // 14
        * br_white = "\033[97m",   // 15

        * reset = "\033[0m";
}
/* ------------------------------------------------------------------------- */


/* -------- Structures for storing debug printing configuration ------------ */
struct Range { int a, b; };

// Config is used to set up how values should be displayed with 'echo' macro. 
/* 
   Example code: 
    int test_idx = 4, cnt = 10, result = 42; vector vec {4, 0, -3};

    echo("In test no. ", test_idx -nb, ", result is ", result -cc(6) -nn);
    echo("Extra info: ", vec -rng(0, 5) -nn, cnt);

   Displays:
    In test no. 4, result is 42; 
    Extra info: {4, 0, -3}; cnt = 10;
*/
template<class T>
struct Config
{
    T data{};
    T what() const
    {
        return data;
    }
    bool noName{ false };
    pair<bool, string> customSeparator{ false, "" };
    pair<bool, const char*> customColor{ false, "" };
    pair<bool, Range> customRange = { false, {-1, -1} };
};

// This struct is a placeholder for Config<T>, stores only config without data.
struct Whatever {};

// Just like 'cin' and 'cout' are already defined istream and ostream objects...
// Here are the Config objects, ready to use:           // [display with]:
Config<Whatever> nn{ {}, {true}, {}, {}, {} };          //     no name
Config<Whatever> ns{ {}, {}, {true, ""}, {}, {} };      //   no separator
Config<Whatever> nb{ {}, {true}, {true, ""}, {}, {} };  // ^ without both ^
Config<Whatever> cc(int code)                           // in custom color
{
    Config<Whatever> config; 
    config.customColor = { true, *(&clc::black + code) }; 
    return config; // dirty hack ^^^^^^^^^^^^^^^^^^^^^ but here it's not important :)
}
Config<Whatever> rng(int a, int b)                     // custom [a, b) range 
{                                                   // (works for vector & array)
    Config<Whatever> config;
    config.customRange = { true, {a, b} };
    return config;
}
/* ------------------------------------------------------------------------- */


/* ---------------- Helper functions used only in this file ---------------- */
namespace
{
    template<class T>
    bool _char_const(const T& x) { return false; }
    bool _char_const(const char* x) { cerr << x; return true; }
    template<class T>
    bool isCharConst(const T& x) { return false; }
    bool isCharConst(const char* x) { return true; }
    string trim(string x) {
        int i = 0;
        while (i < (int)x.size() && x[i] == ' ')
            i++;
        int j = (int)x.size() - 1;
        while (j >= 0 && x[j] == ' ')
            j--;
        return x.substr(i, max(0, j - i + 1));
    }

    template<size_t>
    struct Int {};

    template<class Tuple, size_t Pos>
    void print_tuple(const Tuple& x, Int<Pos>)
    {
        cerr << get<tuple_size<Tuple>::value - Pos>(x) << ", ";
        print_tuple(x, Int<Pos - 1>());
    }

    template<class Tuple>
    void print_tuple(const Tuple& x, Int<1>)
    {
        cerr << get<tuple_size<Tuple>::value - 1>(x);
    }

    template<class... Args>
    void print_tuple(const tuple<Args...>& x)
    {
        print_tuple(x, Int<sizeof... (Args)>());
    }
}
/* ------------------------------------------------------------------------- */

/* ------ Declarations and implementations of the printing functions ------- */

/* merge Configs (eg. 'dist -rng(0, 5) -nn' -> print first 5, without 'dist' */
template<class T, class TR>
Config<T> operator-(const Config<T>& x, const Config<TR>& mod);

/* add print config to normal type (eg. print 'counter' value in blue color) */
template<class T, class TR>
Config<T> operator- (T x, const Config<TR>& mod);

/* array<T, N> */
template<class T, size_t N>
void __echo(const array<T, N>& x, string sep = ", ");

/* vector<T> */
template<class T>
void __echo(const vector<T>& x, string sep = ", ");

/* pair(T1, T2) */
template<class T1, class T2>
void __echo(const pair<T1, T2>& x, string sep = ", ");

/* "string" */
void __echo(const string& x, string sep = ", ");

/* 'char' */
void __echo(const char& x, string sep);

/* -default- */
template<class T>
void __echo(const T& x, string sep = ", ");

template<class... Args>
void __echo(const tuple<Args...>& x, string sep = ", ");

template<class T>
void __echo(const Config<T>& x, string sep = ", ")
{
    if (!x.customRange.first) {
        __echo(x.what(), sep);
    }
    else {
        cerr << clc::red << "error: unsupported type for range extraction" << __LINE__ << clc::reset;
    }
}

template<class vecT>
void __echo(const Config<vector<vecT>>& x, string sep = ", ")
{
    if (!x.customRange.first) {
        __echo(x.what(), sep);
    }
    else {
        auto [a, b] = x.customRange.second;
        a = max(a, 0);
        b = min(b, (int)size(x.data));
        a = min(a, b);
        __echo(vector<vecT>(x.data.begin() + a, x.data.begin() + b), sep);
    }
}

template<class arrT, size_t N>
void __echo(const Config<array<arrT, N>>& x, string sep = ", ")
{
    if (!x.customRange.first) {
        __echo(x.what(), sep);
    }
    else {
        auto [a, b] = x.customRange.second;
        a = max(a, 0);
        b = min(b, (int)size(x.data));
        a = min(a, b);
        __echo(vector<arrT>(x.data.begin() + a, x.data.begin() + b), sep);
    }
}

/* g, (x y), {{a, b, c}, {d, e}}, h */
void __echo(istringstream& stream) {
    cerr << "\n";
}
template<class T, class... Args>
void __echo(istringstream& stream, const T& x, Args... args) {
    static int cnt[200] = { 0 };
    string name;
    do {
        string s;
        getline(stream, s, ',');
        for (auto c : s)
            cnt[(int)c]++;
        name += s + ',';
    } while (not(cnt['('] == cnt[')']
        &&
        cnt['['] == cnt[']']
        &&
        cnt['{'] == cnt['}']
        &&
        cnt['\"'] % 2 == 0
        &&
        cnt['\''] % 2 == 0));
    name.pop_back(); // remove ',' from the end

    if (!_char_const(x)) {
        cerr << trim(name) << " = ";
        __echo(x, "; ");
    }

    __echo(stream, args...);
}
template<class T, class... Args>
void __echo(istringstream& stream, const Config<T>& x, Args... args) {
    static int cnt[200] = { 0 }; 
    string name;
    do {
        string s;
        getline(stream, s, ',');
        for (auto c : s)
            cnt[(int)c]++;
        name += s + ',';
    } while (not(cnt['('] == cnt[')']
        &&
        cnt['['] == cnt[']']
        &&
        cnt['{'] == cnt['}']
        &&
        cnt['\"'] % 2 == 0
        &&
        cnt['\''] % 2 == 0));
    name.pop_back();//usuwamy ','

    //cerr << clc::br_red << "[CONFIG]" << clc::reset;

    // name
    string xPref = "";
    if (!x.noName && !isCharConst(x.data)) {
        //cerr << trim(name) << " = ";
        xPref = trim(name) + " = ";
    }
    // separator
    string xSep = isCharConst(x.data) ? "" : "; ";
    if (x.customSeparator.first)
        xSep = x.customSeparator.second;
    // color
    if (x.customColor.first)
        xPref += x.customColor.second,
        xSep = clc::reset + xSep;

    cerr << xPref;
    __echo(x, xSep);

    // color-reset
    if (x.customColor.first)
        cerr << clc::reset;

    __echo(stream, args...);
}


























/* --------------------- Definitons ----------------------- */
template<class T, class TR>
Config<T> operator- (const Config<T>& x, const Config<TR>& mod)
{
    Config<T> config = x;
    // overwrite 'config' with extra settings from 'mod' 
    if (mod.customColor.first)
        config.customColor = mod.customColor;
    if (mod.customRange.first)
        config.customRange = mod.customRange;
    if (mod.customSeparator.first)
        config.customSeparator = mod.customSeparator;
    if (mod.noName)
        config.noName = mod.noName;
    return config;
}

template<class T, class TR>
Config<T> operator- (T x, const Config<TR>& mod)
{
    return Config<T>{x} - mod;
}

void __echo(const string& x, string sep) {
    cerr << "\"" << x << "\"" << sep;
}

void __echo(const char& x, string sep = ", ") {
    cerr << "\'" << x << "\'" << sep;
}

template<class T>
void __echo(const vector<T>& x, string sep) {
    if (x.size() == 0)
        cerr << "{}";
    else {
        cerr << "{";
        for (const auto& xi : x)
            __echo(xi);
        cerr << "\b\b}";
    }
    cerr << sep;
}

template<class T, size_t N>
void __echo(const array<T, N>& x, string sep) {
    if (x.size() == 0)
        cerr << "{}";
    else {
        cerr << "{";
        for (const auto& xi : x)
            __echo(xi);
        cerr << "\b\b}";
    }
    cerr << sep;
}

template<class... Args>
void __echo(const tuple<Args...>& x, string sep)
{
    cerr << "(";
    print_tuple(x);
    cerr << ")" << sep;
}


template<class T1, class T2>
void __echo(const pair<T1, T2>& x, string sep) {
    cerr << "("; __echo(x.first, ", "); __echo(x.second, ""); cerr << ")";
    cerr << sep;
}

template<class T>
void __echo(const T& x, string sep) {
    cerr << x << sep;
}