#include "flat_hash_map.hpp"
#include "khash.h"
#include <cstdlib>
#include <cstring>
#include <random>
#include <chrono>
using namespace ska;
#define get_tp std::chrono::high_resolution_clock::now
KHASH_SET_INIT_INT64(64)

#define to_ns(start, end) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()

int main(int argc, char *argv[]) {
    flat_hash_set<uint64_t> set;
    khash_t(64) khset;
    std::memset(&khset, 0, sizeof(khset));
    std::mt19937_64 mt;
    size_t nelem = argc == 1 ? size_t(1000000): size_t(std::strtoull(argv[1], nullptr, 10));
    std::vector<uint64_t> vals;
    set.reserve(nelem);
    vals.reserve(nelem);
    kh_resize(64, &khset, nelem);
    while(vals.size() < vals.capacity())
        vals.push_back(mt());
    auto start = get_tp();
    for(const auto v: vals)
        set.emplace(v);
    auto end = get_tp();
    std::fprintf(stderr, "Time to insert all into reserved ska hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    uint64_t fhstime = to_ns(start, end);
    int khr;
    start = get_tp();
    for(const auto v: vals)
        kh_put(64, &khset, v, &khr);
    end = get_tp();
    uint64_t khtime = to_ns(start, end);
    std::fprintf(stderr, "Time to insert all into reserved khash hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    std::fprintf(stderr, "khash is %lf as fast as ska for reserved insertions\n", double(fhstime) / khtime);
    uint64_t sum = 0;
    start = get_tp();
    for(const auto v: vals)
        sum += *set.find(v);
    end = get_tp();
    fhstime = to_ns(start, end);
    std::fprintf(stderr, "Time to find present keys in ska hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    sum = 0;
    start = get_tp();
    for(const auto v: vals)
        sum += khset.keys[kh_get(64, &khset, v)];
    end = get_tp();
    khtime = to_ns(start, end);
    std::fprintf(stderr, "Time to find present keys in khash hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    std::fprintf(stderr, "khash is %lf as fast as ska for correct lookups\n", double(fhstime) / khtime);
    for(auto &v: vals)
        v = mt();
    start = get_tp();
    for(const auto v: vals)
        (void)kh_get(64, &khset, v);
    end = get_tp();
    khtime = to_ns(start, end);
    std::fprintf(stderr, "Time to look for absent keys in khash hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    start = get_tp();
    for(const auto v: vals)
        (void)set.find(v);
    end = get_tp();
    fhstime = to_ns(start, end);
    std::fprintf(stderr, "Time to look for absent keys in ska hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    std::fprintf(stderr, "khash is %lf as fast as ska for missing lookups\n", double(fhstime) / khtime);
    {
        flat_hash_set<uint64_t> tmp;
        std::swap(tmp, set);
        std::free(khset.keys);
        std::free(khset.vals);
        std::free(khset.flags);
        std::memset(&khset, 0, sizeof(khset));
    }
    start = get_tp();
    for(const auto v: vals)
        kh_put(64, &khset, v, &khr);
    end = get_tp();
    khtime = to_ns(start, end);
    std::fprintf(stderr, "Time to insert all into unreserved khash hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    start = get_tp();
    for(const auto v: vals)
        set.insert(v);
    end = get_tp();
    fhstime = to_ns(start, end);
    std::fprintf(stderr, "Time to insert all into unreserved ska hashset: %zd/%lf\n", to_ns(start, end), to_ns(start, end) / 1.e9);
    std::fprintf(stderr, "khash is %lf as fast as ska for unreserved inserts\n", double(fhstime) / khtime);
    std::free(khset.keys);
    std::free(khset.vals);
    std::free(khset.flags);
}
