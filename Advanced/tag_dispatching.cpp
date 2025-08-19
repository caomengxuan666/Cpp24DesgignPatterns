// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Tag Dispatching                                               ┃
// ┃  PURPOSE   ::  Overload resolution based on tag types                        ┃
// ┃  CATEGORY  ::  Metaprogramming                                              ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛


#include <iterator>
#include <print>
#include <vector>
using std::println;


// ─────────────────────────────────────────── 实现函数 ───────────────────────────────────────────

template<typename Iter>
void advance_impl(Iter &it, int n, std::random_access_iterator_tag) {
    it += n;
    println("⚡ 随机访问迭代器：直接跳转");
}

template<typename Iter>
void advance_impl(Iter &it, int n, std::bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n--) ++it;
    } else {
        while (n++) --it;
    }
    println("⏪ 双向迭代器：逐个移动");
}

template<typename Iter>
void advance_impl(Iter &it, int n, std::forward_iterator_tag) {
    while (n--) ++it;
    println("➡️  前向迭代器：只能向前");
}

// ─────────────────────────────────────────── 接口函数 ───────────────────────────────────────────

template<typename Iter>
void advance(Iter &it, int n) {
    using category = typename std::iterator_traits<Iter>::iterator_category;
    advance_impl(it, n, category{});
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testTagDispatching() {
    println("\n🧪 标签分发");

    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    advance(it, 3);// 随机访问
    println("当前值: {}", *it);
}

int main() {
    testTagDispatching();
}