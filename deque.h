#include <iostream>
#include <algorithm>
#include <iterator>

template<typename T>
class Deque {
private:
    static const size_t N = 32;
    size_t M; 
    T** arr; 
    size_t sz;
    size_t bgn;

public:
    Deque() {
        M = 2; sz = 0; bgn = M*N/2;
        arr = new T*[M]; 
        for (size_t i = 0; i < M; i++) {
            arr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]); 
        }
    }

    Deque(const Deque& dq) {
        M = dq.M; sz = dq.sz; bgn = dq.bgn; 
        arr = new T*[dq.M];
        for (size_t i = 0; i < M; i++) {
            arr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]);
            std::copy(dq.arr[i], dq.arr[i]+N, arr[i]);
        }
    }

    Deque(int count) {
        M = std::max(static_cast<size_t>(2), 2*count/N); sz = count;
        arr = new T*[M];
        for (size_t i = 0; i < M; i++) {
            arr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]);
        }
        bgn = M*N/2 - count / 2;
        for (int j = 0; j < count; j++) {
            new(arr[(bgn+j)/N]+(bgn+j)%N) T();
        }
    }

    Deque(int count, const T& value) {
        M = std::max(static_cast<size_t>(2), 2*count/N); sz = count;
        arr = new T*[M];
        for (size_t i = 0; i < M; i++) {
            arr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]);
        }
        bgn = M*N/2 - count / 2;
        for (int j = 0; j < count; j++) {
            new(arr[(bgn+j)/N]+(bgn+j)%N) T(value);
        }
    } 

    Deque& operator=(const Deque& dq) {
        for (size_t j = 0; j < sz; j++) {
            arr[(bgn+j)/N][(bgn+j)%N].~T();
        }
        for (size_t i = 0; i < M; i++) {
            delete[] reinterpret_cast<uint8_t*>(arr[i]);
        }
        delete[] arr;
        M = dq.M; sz = dq.sz; bgn = dq.bgn; 
        arr = new T*[dq.M];
        for (size_t i = 0; i < M; i++) {
            arr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]);
            std::copy(dq.arr[i], dq.arr[i]+N, arr[i]);
        }
        return *this;  
    }

    ~Deque() {
        for (size_t j = 0; j < sz; j++) {
            arr[(bgn+j)/N][(bgn+j)%N].~T();
        }
        for (size_t i = 0; i < M; i++) {
            delete[] reinterpret_cast<uint8_t*>(arr[i]);
        } 
        delete[] arr;
    }

    T& operator[](size_t i) {
        return arr[(bgn+i)/N][(bgn+i)%N];
    }

    const T& operator[](size_t i) const {
        return arr[(bgn+i)/N][(bgn+i)%N];
    }

    T& at(size_t i) {
        if (i < sz){
            return arr[(bgn+i)/N][(bgn+i)%N];
        } else {
            throw std::out_of_range("takie dela");
        }
    }

    const T& at(size_t i) const {
        if (i < sz){
            return arr[(bgn+i)/N][(bgn+i)%N];
        } else {
            throw std::out_of_range("takie dela");
        }
    }

    T push_back(const T& item) {
        if (bgn+sz < M*N) {
            new (arr[(bgn+sz)/N]+(bgn+sz)%N) T(item);
            sz++;
        }
        else {
            T** newarr = new T*[2*M]; 
            size_t mb = bgn/N, buckets2 = (M-mb+1)/2;
            for (size_t i = 0; i < M-buckets2-mb; i++) {
                newarr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]); 
            }
            for (size_t i = 0; i < M; i++) {
                newarr[M-buckets2-mb+i] = arr[i];
            }
            for (size_t i = 2*M-buckets2-mb; i < 2*M; i++) {
                newarr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]);
            } 
            delete[] arr;
            bgn = N*(M-buckets2)+bgn%N; M *= 2;
            arr = newarr;
            new (arr[(bgn+sz)/N]+(bgn+sz)%N) T(item);
            sz++;
        }
        return item;
    }

    T push_front(const T& item) {
        if (bgn != 0) {
            new (arr[(bgn-1)/N]+(bgn-1)%N) T(item);
            sz++; bgn--;
        }
        else {
            T** newarr = new T*[2*M]; 
            size_t me = sz/N, buckets2 = (me+1)/2;
            for (size_t i = 0; i < M-buckets2; i++) {
                newarr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]);
            }
            for (size_t i = 0; i < M; i++) {
                newarr[M-buckets2+i] = arr[i];
            } 
            for (size_t i = 2*M-buckets2; i < 2*M; i++) {
                newarr[i] = reinterpret_cast<T*>(new uint8_t[N * sizeof(T)]); 
            }
            delete[] arr;
            bgn = N*(M-buckets2)+bgn%N; M *= 2;
            arr = newarr;
            new (arr[(bgn-1)/N]+(bgn-1)%N) T(item);
            sz++; bgn--;
        }
        return item; 
    }

    T pop_back() {
        T ret = arr[(bgn+sz-1)/N][(bgn+sz-1)%N]; 
        arr[(bgn+sz-1)/N][(bgn+sz-1)%N].~T();
        sz--;
        return ret;
    }

    T pop_front() {
        T ret = arr[bgn/N][bgn%N]; 
        arr[bgn/N][bgn%N].~T();
        bgn++; sz--;
        return ret;
    }

    size_t size() const {
        return sz;
    }

    void printBuckets() const {
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                std::cout << arr[i][j] << " ";
            }
            std::cout << '\n';
        }
    }

    void print() const {
        for (size_t i = 0; i < sz; i++) {
            std::cerr << arr[(bgn+i)/N][(bgn+i)%N] << " ";
        }
    } 

    template<typename PtrType, typename RefType>
    struct Deque_iterator { 
        using iterator = Deque_iterator<T*, T&>;
        using const_iterator = Deque_iterator<const T*, const T&>;   
        using Self = Deque_iterator;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = PtrType;
        using reference = RefType;
        using iterator_category = std::random_access_iterator_tag;

        T** mp;
        T* cur;
        T* first;
        T* last;
    
        void changeBucket(T** newmp) {
            mp = newmp;
            first = *mp;
            last = first + N;
        }

        Deque_iterator() : mp(0), cur(0), first(0), last(0) {}
        Deque_iterator(T** mp, T* cur, T* first, T* last): mp(mp), cur(cur), first(first), last(last) {}
        Deque_iterator(const iterator& it): mp(it.mp), cur(it.cur), first(it.first), last(it.last) {}

        reference operator*() const {
            return *cur;
        }

        pointer operator->() const {
            return cur;
        }

        Self& operator++() {
            cur++;  
            if (cur == last) {
                changeBucket(mp+1); 
                cur = first;
            }
            return *this;
        }

        Self& operator--() {
            if (cur == first) {
                changeBucket(mp-1); 
                cur = last;
            }
            cur--;
            return *this;
        }

        Self& operator++(int) {
            cur++;  
            if (cur == last) {
                changeBucket(mp+1); 
                cur = first;
            } 
            return *this;
        }

        Self& operator--(int) {
            if (cur == first) {
                changeBucket(mp-1); 
                cur = last;
            }
            cur--;
            return *this;
        }

        Self& operator+=(int k) {
            if (last - cur > k) {
                cur += k;
            }
            else {
                int shift = (cur-first+k);
                changeBucket(mp+shift/N);
                cur = first+shift%N;
            }
            return *this;
        }

        Self& operator-=(int k) {
            if (cur - first >= k) {
                cur -= k;
            }
            else {
                int shift = (last-cur+k);
                changeBucket(mp-shift/N);
                cur = last-shift%N;
            }
            return *this;
        }

        Self operator+(int k) {
            Self it = *this;
            it += k;
            return it;
        }

        Self operator-(int k) {
            Self it = *this;
            it -= k;
            return it;
        }

        bool operator==(const Self& b) {
            return cur == b.cur;
        }

        bool operator!=(const Self& b) {
            return !(*this == b);
        }

        bool operator<(const Self& b) {
            if (mp < b.mp) return true;
            else if (mp == b.mp) return (cur < b.cur);
            else return false;
        }

        bool operator>(const Self& b) {
            if (mp > b.mp) return true;
            else if (mp == b.mp) return (cur > b.cur);
            else return false;
        }

        bool operator<=(const Self& b) {
            return !(*this > b);
        }

        bool operator>=(const Self& b) {
            return !(*this < b);
        }

        int operator-(const Self& b) {
            return N*(mp-b.mp) + (cur-first)-(b.cur-b.first);
        }
    };
    
    using iterator = Deque_iterator<T*, T&>;
    using const_iterator = Deque_iterator<const T*, const T&>;

    iterator begin() {
        return iterator(arr+bgn/N, arr[bgn/N]+bgn%N, arr[bgn/N], arr[bgn/N]+N);
    }

    iterator end() {
        return this->begin()+sz;
    }

    const_iterator begin() const {
        return const_iterator(arr+bgn/N, arr[bgn/N]+bgn%N, arr[bgn/N], arr[bgn/N]+N);
    }

    const_iterator end() const {
        return this->begin()+sz;
    }

    const_iterator cbegin() const {
        return const_iterator(arr+bgn/N, arr[bgn/N]+bgn%N, arr[bgn/N], arr[bgn/N]+N);
    }

    const_iterator cend() const {
        return this->cbegin()+sz;
    }

    std::reverse_iterator<iterator> rbegin() {
        return std::reverse_iterator(this->end());
    }

    std::reverse_iterator<iterator> rend() {
        return std::reverse_iterator(this->begin());
    }

    std::reverse_iterator<const_iterator> rbegin() const {
        return std::reverse_iterator(this->end());
    }

    std::reverse_iterator<const_iterator> rend() const {
        return std::reverse_iterator(this->begin());
    }

    std::reverse_iterator<const_iterator> crbegin() const {
        return std::reverse_iterator(this->cend());
    }

    std::reverse_iterator<const_iterator> crend() const {
        return std::reverse_iterator(this->cbegin());
    }

    void insert(iterator iter, const T& value) {
        T mv = *iter;
        *iter = value;
        for (iterator it = iter+1; it != this->end(); it++) {
            T tmp = mv;
            mv = *it;
            *it = tmp;
        }
        this->push_back(mv);
    }

    T erase(iterator iter) {
       T ret = *iter;
       for (iterator it = iter; it + 1 != this->end(); it++) {
            *it = *(it+1);
       }
       this->pop_back();
       return ret;
    }
};
