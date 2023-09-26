// Set 1 -> enable local debugging output ('ultimate_power_3.h' needed)
#define DEBUG 1
// Set 1 -> input is given in tests sets
#define TESTSETS 0

// Depending on the config, include extra libs or undefine macros
#if DEBUG
    #include "ultimate_power_3.h"
#else 
    #include "bits/stdc++.h"
    #define echo(...) {}
#endif

// Include necessary names
using std::vector, std::array, std::get, std::function, 
      std::sort, std::deque, std::tuple, std::reverse, 
      std::queue, std::swap, std::stack, std::priority_queue, 
      std::pair, std::cerr, std::string, std::set, std::unordered_map, 
      std::list, std::unordered_set, std::multiset, 
      std::unordered_multiset, std::istream, 
      std::min, std::max, std::cin, std::cout, std::map;
using llong = long long;
using ullong = unsigned llong;

// Extra macros for more concise code
#define debug   if(DEBUG)
#define pow2(x) (1LL<<(x))
#define all(x)  (x).begin(), (x).end()
#define len(x)  (int)(x).size()
#define dfor(i, a, b) for(int i = (a), i##e = (b); i >= i##e; i--)
#define forx(i, a, b) for(int i = (a), i##e = (b); i < i##e; i++) 
#define until(condition) while(not(condition))

// Helpful constants
constexpr llong Inf = pow2(62) - 1;
constexpr int iInf = pow2(30) - 1;
int Mod = 1000 * 1000 * 1000 + 7; // modulo given in a problem, used by 'mint' 

// Short functions
int clamp(int x, int a, int b) { assert(a <= b); return min(max(x, a), b); }
template<class T> llong sum(const vector<T>& vector) { return accumulate(vector.begin(), vector.end(), 0LL); }
template<class T> T max(const vector<T>& vector) { return *max_element(vector.begin(), vector.end()); }
template<class T> T min(const vector<T>& vector) { return *min_element(vector.begin(), vector.end()); }
bool in_range(int value, int beg, int end) { return beg <= value && value < end; }
int randx(int a, int b) { return rand() % (b - a) + a; } // random in [a, b) range
llong iceil(llong a, llong b) { assert(a > 0); return (a - 1) / b + 1; } // ceil a / b
llong fpow(llong a, llong b) // a^b % Mod in O(logb) time
{
    if (b == 0) return 1;
    if (b % 2 == 1) return fpow(a, b - 1) * a % Mod;
    llong p = fpow(a, b / 2);
    return p * p % Mod;
}
template<class T>
T up2(T x) { // first 2^k such that x <= 2^k, useful for setting segtree size
    T res = T(1);
    while (res < x)
        res <<= 1;
    return res;
}

// Operator overloads
template<class T, class U> pair<T, U> operator+(const pair<T, U>& a, const pair<T, U>& b) { return { a.st + b.st, a.nd + b.nd }; }
template<class T, class U> pair<T, U> operator+=(pair<T, U>& a, const pair<T, U>& b) { return a = a + b; }
template<class T, class U>
istream& operator>>(istream& stream, pair<T, U>& p) { return stream >> p.first >> p.second; }


// Longer functions
vector<llong> factorize(llong x) {
    debug assert(x > 0);
    vector<llong> f;
    while (x % 2 == 0)
        f.push_back(2),
        x /= 2;
    for (llong d = 3; d * d <= x; d += 2)
        while (x % d == 0)
            f.push_back(d),
            x /= d;
    if (x > 1)
        f.push_back(x);
    return f;
}
template<class T>
vector<pair<T, int>> compress(const vector<T>& v) {
    debug assert(std::is_sorted(all(v)));
    vector<pair<T, int>> p;
    for (int i = 0, j = 0; i < len(v); i = j) {
        for (j = i + 1; j < len(v) && v[i] == v[j]; j++);
        p.emplace_back(v[i], j - i);
    }
    return p;
}
template<class T>
vector<pair<T, int>> sort_and_compress(vector<T> v) {
    sort(all(v));
    return compress(v);
}
vector<pair<int, int>> all_adjacent(int i, int j) { return { {i - 1, j}, {i, j + 1}, {i + 1, j}, {i, j - 1} }; };
template<class It, class T>
int scale(It beg, It end, T start)
{
    map<T, T> m{ beg, end };
    for (auto& [x, y] : m)
        y = start++;
    while (beg != end)
        *beg++ = m[*beg];
    return len(m);
}
template<class T>
void sort_and_unique(vector<T>& t) {
    sort(all(t));
    t.resize(unique(all(t)) - t.begin());
}
int modulo(int a, int b) {
    int res = a % b;
    if (res < 0) res += b;
    return res;
}
vector<int> divisors(int n) {
    vector<int> s;
    for (int i = 1; i * i <= n; i++)
        if (n % i == 0)
            s.push_back(n / i),
            s.push_back(i);
    sort_and_unique(s);
    return s;
}
template<class Int, class UnaryPred>
Int binsearch(Int beg, Int end, UnaryPred pred) // first-true binsearch on [beg, end)
{
    // predicate:
    // 000000001111
    // we want ^
    Int a = beg, b = end;
    while (a < b) {
        Int mid = a + (b - a) / 2;
        if (!pred(mid))
            a = mid + 1;
        else
            b = mid;
    }
    return a;
}


// Helpful structures \(^^)/
struct Trie {
    struct Node {
        array<Node*, 26> _next;
        Node*& next(char c) { return _next[c-'a']; }
        Node* push_back(char c) {
            if(!next(c)) next(c) = new Node();
            return next(c);
        }
        bool is_word = false;
    };
    Node* root = new Node();
    void insert(const string& word) {
        auto node = root;
        for (auto c: word) {
            node = node->push_back(c);
        }
        node->is_word = true;
    }
    bool contains(const string& word) {
        auto node = root;
        for (auto c: word) {
            node = node->next(c);
            if (!node) return false;
        }
        return node->is_word;
    }
};
template<class T>
struct fixed_vector
{
    // Array with *indexes* on range [a, b)
    vector<T> mapped; int offset;
    fixed_vector() = default;
    fixed_vector(int a, int b) : mapped(b - a), offset(-a)
    {
    }
    T& operator[](int idx) {
        if (has(idx))
            return mapped[idx + offset];
        exit(69);
        return mapped[0];
    }
    bool has(int idx) {
        return 0 <= idx + offset && idx + offset < len(mapped);
    }
};
struct mint
{
    // Just set the global modulo constant (int Mod).
    // And you can use it just like a builtin type!
    /* 
       Example code:
        Mod = int(1e9+7);
        mint answer = 999999999;
        answer *= answer;
        answer += 2*7*999999999LL + 49;
        cout << "Answer is " << answer << ".\n"; 
       Prints:
        Answer is 1.
    */ 
    int x;

    int _modulize(int x) const { x %= Mod; if (x < 0) x += Mod; return x; }
    mint(int x = 0) : x(_modulize(x)) {}
    mint(llong x) : x(_modulize(int(x% Mod))) {}
    operator int& () { return x; }

    mint& operator+= (const mint& rhs) { x += rhs.x; if (x >= Mod) x -= Mod; return *this; } //damn that x > MOD was a huge bug
    mint& operator+= (int y) { return *this += mint(y); }
    mint& operator+= (llong y) { return *this += mint(y); }
    mint operator+ (const mint& rhs) const { return mint(*this) += rhs; }
    mint operator+ (int y) const { return *this + mint(y); }
    mint operator+ (llong y) const { return *this + mint(y); }
    friend mint operator+ (int x, const mint& rhs) { return mint(x) + rhs; }
    friend mint operator+ (llong x, const mint& rhs) { return mint(x) + rhs; }

    mint operator- () const { return mint(x == 0 ? 0 : Mod - x); }
    mint& operator-= (const mint& rhs) { return *this += -rhs; }
    mint& operator-= (int y) { return *this -= mint(y); }
    mint& operator-= (llong y) { return *this -= mint(y); }
    mint operator- (const mint& rhs) const { return mint(*this) -= rhs; }
    mint operator- (int y) const { return *this - mint(y); }
    mint operator- (llong y) const { return *this - mint(y); }
    friend mint operator- (int x, const mint& rhs) { return mint(x) - rhs; }
    friend mint operator- (llong x, const mint& rhs) { return mint(x) - rhs; }

    mint& operator*= (const mint& rhs) { x = int((llong)x * rhs.x % Mod); return *this; }
    mint& operator*= (int y) { return *this *= mint(y); }
    mint& operator*= (llong y) { return *this *= mint(y); }
    mint operator* (const mint& rhs) const { return mint(*this) *= rhs; }
    mint operator* (int y) const { return *this * mint(y); }
    mint operator* (llong y) const { return *this * mint(y); }
    friend mint operator* (int x, const mint& rhs) { return mint(x) * rhs; }
    friend mint operator* (llong x, const mint& rhs) { return mint(x) * rhs; }

    mint pow(int e) const {
        mint res = 1;
        for (mint p = *this; e > 0; p *= p, e /= 2)
            if (e % 2)
                res *= p;
        return res;
    }
    mint inv() const { return pow(Mod - 2); }

    mint& operator/= (const mint& rhs) { x *= rhs.inv();  return *this; }
    mint& operator/= (int y) { return *this /= mint(y); }
    mint& operator/= (llong y) { return *this /= mint(y); }
    mint operator/ (const mint& rhs) const { return mint(*this) /= rhs; }
    mint operator/ (int y) const { return *this / mint(y); }
    mint operator/ (llong y) const { return *this / mint(y); }
    friend mint operator/ (int x, const mint& rhs) { return mint(x) / rhs; }
    friend mint operator/ (llong x, const mint& rhs) { return mint(x) / rhs; }
    friend std::ostream& operator<<(std::ostream& stream, const mint& mod_x) { return stream << mod_x.x; }
};
vector<mint> factorial(int n) {
    vector<mint> fact(n);
    fact[0] = 1;
    forx(i, 1, n)
        fact[i] = fact[i - 1] * i;
    return fact;
}
vector<vector<mint>> newton(int n) {
    vector x(n, vector<mint>(n));
    x[0][0] = 1;
    forx(i, 1, n)
        forx(j, 0, i + 1)
        x[i][j] = (j - 1 >= 0 ? x[i - 1][j - 1] : mint(0)) + x[i - 1][j];
    return x;
}

// Helpers for getting input
template<class T>
void init(vector<T>& vec, int size) { vec = vector<T>(size); }
void read() {}
template<class T, class... Args>
void read(T& x, Args&... args) {
    cin >> x;
    read(args...);
}
template<class T>
vector<T> read_vector(int size, const T& delta = T{})
{
    vector<T> res(size);
    for (auto& x : res) cin >> x, x += delta;
    return res;
}
template<class T> 
void read_vector(vector<T>& vec, int n, const T& delta = T()) { 
    vec = read_vector<T>(n, delta); 
}

// Helpers for printing answer (not debugging)
template<class T>
void answer_vector(const vector<T>& v, const T& delta = T{}, bool print_size = true) {
    if (print_size) cout << v.size() << "\n";
    for (auto& vi : v) cout << vi + delta << " ";
    cout << "\n";
}
template<class T>
void answer(const T& x) {
    cout << x << "\n";
}
template<class T, class... Args>
void answer(const T& x, Args... args) {
    cout << x << " "; // auto spacing
    answer(args...);
}

// --- In this function you will write the solution ---
void solve();

int main() 
{
    ios::sync_with_stdio(0); cin.tie(0);
    int t = 1; 
    if(TESTSETS) 
        cin >> t;

    forx(i, 0, t) {
        echo("Test ", clc::green, i+1 -nn -ns);
        solve();
        echo("\n");
    }
}

/* ------------------------------------------------------------------------- */

void solve() {
    // Write your code here...
    vector<pair<int, char>> arr {{0, 't'}, {2, 'm'}, {3, 'c'}, {1, 's'}};
    echo(arr);

    int test_idx = 4, cnt = 10, result = 42; vector vec {4, 0, -3};
    echo("In test no. ", test_idx -nb, ", result is ", result -cc(6) -nn);
    echo("Extra info: ", vec -rng(0, 5) -nn, cnt);

    Mod = int(1e9+7);
    mint answer = 999999999;
    answer *= answer;
    answer += 2*7*999999999LL + 49;
    cout << answer << endl; // Prints: 1
}