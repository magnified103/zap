#ifndef CORE_ECS_HPP
#define CORE_ECS_HPP

#include <bit>
#include <bitset>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace ecs::detail {
template <typename T, typename U, typename... Types>
struct index_helper;

template <typename T>
struct index_helper<T, T> {
    constexpr static std::size_t value{0};
};

template <typename T, typename U, typename... Types>
struct index_helper<T, T, U, Types...> {
    constexpr static std::size_t value{0};
};

template <typename T, typename U, typename V, typename... Types>
struct index_helper<T, U, V, Types...> {
    constexpr static std::size_t value{index_helper<T, V, Types...>::value + 1};
};

template <typename T, typename... Types>
[[nodiscard]] consteval std::size_t index_of() noexcept {
    static_assert((std::is_same_v<T, Types> || ...), "Can't get the index of the required type");
    return index_helper<T, Types...>::value;
}

template <typename... Types>
struct unique_helper {};

template <>
struct unique_helper<> {
    constexpr static bool value{true};
};

template <typename T, typename U>
struct unique_helper<T, U> {
    constexpr static bool value{!std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>};
};

template <typename T, typename... Types>
struct unique_helper<T, Types...> {
    constexpr static bool value{(unique_helper<T, Types>::value && ...) && unique_helper<Types...>::value};
};

template <typename... Types>
[[nodiscard]] consteval bool is_unique() noexcept {
    return unique_helper<Types...>::value;
}

template <std::size_t N, typename Base = std::uint64_t>
class bitset {
public:
    static_assert(std::is_unsigned_v<Base>, "Underlying base type must be unsigned");

    constexpr bitset() noexcept : masks{} {}

    template <typename Func>
    constexpr bitset(Func &&func) noexcept : masks{} {
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            masks[pos >> base_size_shift] |= func(pos) << (pos & base_size_mask);
        }
    }

    template <std::size_t... Ints>
    constexpr bitset(std::index_sequence<Ints...>) noexcept : masks{} {
        static_assert((std::less()(Ints, n) && ...), "Indices must be less than N");
        auto apply_bit = [&](std::size_t pos) { masks[pos >> base_size_shift] |= 1ull << (pos & base_size_mask); };
        (apply_bit(Ints), ...);
    }

    [[nodiscard]] constexpr bool test(std::size_t pos) const {
        if (pos >= n) {
            throw std::out_of_range("Attempted to access a position higher than the maximum allowed");
        }
        return (masks[pos >> base_size_shift] >> (pos & base_size_mask)) & 1;
    }

    template <std::size_t pos>
    [[nodiscard]] constexpr bool get() const noexcept {
        static_assert(pos < n, "Attempted to access a position higher than the maximum allowed");
        return (masks[pos >> base_size_shift] >> (pos & base_size_mask)) & 1;
    }

    [[nodiscard]] constexpr bool all() const noexcept { return count() == n; }

    [[nodiscard]] constexpr bool any() const noexcept {
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            if (masks[pos] != 0) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] constexpr bool none() const noexcept {
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            if (masks[pos] != 0) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept {
        std::size_t result{0};
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            result += std::popcount(masks[pos]);
        }
        return result;
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept { return n; }

    [[nodiscard]] constexpr bitset operator&(const bitset &other) const noexcept {
        auto result{*this};
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            result.masks[pos] &= other.masks[pos];
        }
        return result;
    }

    [[nodiscard]] consteval bitset operator|(const bitset &other) const noexcept {
        auto result{*this};
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            result.masks[pos] |= other.masks[pos];
        }
        return result;
    }

    [[nodiscard]] consteval bitset operator^(const bitset &other) const noexcept {
        auto result{*this};
        for (std::size_t pos{0}; pos != n_base; ++pos) {
            result.masks[pos] ^= other.masks[pos];
        }
        return result;
    }

    bitset &set(std::size_t pos, bool value = true) {
        if (pos >= n) {
            throw std::out_of_range("Attempted to access a position higher than the maximum allowed");
        }
        if (value) {
            masks[pos >> base_size_shift] |= 1ull << (pos & base_size_mask);
        } else {
            masks[pos >> base_size_shift] &= base_mask ^ (1ull << (pos & base_size_mask));
        }
        return *this;
    }

    bitset &reset(std::size_t pos) {
        if (pos >= n) {
            throw std::out_of_range("Attempted to access a position higher than the maximum allowed");
        }
        masks[pos >> base_size_shift] &= base_mask ^ (1ull << (pos & base_size_mask));
        return *this;
    }

    bitset &flip(std::size_t pos) {
        if (pos >= n) {
            throw std::out_of_range("Attempted to access a position higher than the maximum allowed");
        }
        masks[pos >> base_size_shift] ^= 1ull << (pos & base_size_mask);
        return *this;
    }

private:
    using base_type = Base;

    constexpr static std::size_t base_size_bytes{sizeof(Base)};
    constexpr static std::size_t base_size{base_size_bytes * CHAR_BIT};
    constexpr static std::size_t base_size_shift{sizeof(std::size_t) * CHAR_BIT - 1 - std::countl_zero(base_size)};
    constexpr static std::size_t base_size_mask{base_size - 1};
    constexpr static std::size_t n{N};
    constexpr static std::size_t n_base{(n + base_size - 1) / base_size};
    constexpr static base_type base_mask{static_cast<base_type>(-1)};

    base_type masks[n_base];
};
}

// An implementation of entity component system
namespace ecs {
template <typename Alloc>
struct ecs_traits {
    template <typename Type>
    using alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Type>;

    template <typename Type>
    using alloc_traits = typename std::allocator_traits<Alloc>::template rebind_traits<Type>;

    using id_type = std::size_t;

    template <typename Type>
    [[nodiscard]] static alloc_type<Type> &get_alloc() {
        static alloc_type<Type> alloc{};
        return alloc;
    }

    template <typename Type>
    [[nodiscard]] static decltype(auto) allocate() {
        return alloc_traits<Type>::allocate(get_alloc<Type>(), 1);
    }

    template <typename Type, typename... Args>
    static Type &construct(Type *ptr, Args &&...args) {
        alloc_traits<Type>::construct(get_alloc<Type>(), ptr, std::forward<Args>(args)...);
        return *ptr;
    }

    template <typename Type>
    static void deallocate(Type *ptr) {
        alloc_traits<Type>::deallocate(get_alloc<Type>(), ptr, 1);
    }

    template <typename Type>
    struct deleter {
        static_assert(!std::is_void<Type>::value, "Can't delete pointer of incomplete type");
        static_assert(sizeof(Type) > 0, "Can't delete pointer of incomplete type");

        constexpr deleter() noexcept = default;

        template <typename Other, typename = std::enable_if_t<std::is_convertible_v<Other *, Type *>>>
        constexpr deleter(const deleter<Other> &) noexcept {}

        void operator()(Type *ptr) const { deallocate<Type>(ptr); }
    };
};

template <typename Traits, typename... Components>
class basic_entity {
public:
    static_assert(detail::is_unique<Components...>(), "Components must be unique");
    static_assert((std::is_same_v<Components, std::decay_t<Components>> && ...), "Non-decayed types are prohibited");

    template <typename Component>
    [[nodiscard]] auto &get() {
        if (!any_of<Component>()) {
            throw std::runtime_error("Attempted to get a component from an entity that doesn't own it");
        }
        return *get_ptr<Component>();
    }

    template <typename... RequiredComponents>
    [[nodiscard]] bool all_of() const noexcept {
        return (mask & get_masks<RequiredComponents...>()) == get_masks<RequiredComponents...>();
    }

    template <typename... RequiredComponents>
    [[nodiscard]] bool any_of() const noexcept {
        return (mask & get_masks<RequiredComponents...>()).any();
    }

    template <typename Component, typename... Args>
    decltype(auto) emplace(Args &&...args) {
        auto &ptr = get_ptr<Component>();
        if (ptr) {
            throw std::runtime_error("Attempted to assign a component to an entity that already owns it");
        }
        mask.set(index_of<Component>());
        ptr.reset(entity_traits::template allocate<Component>());
        return entity_traits::construct(ptr.get(), std::forward<Args>(args)...);
    }

    template <typename Component, typename... Args>
    decltype(auto) replace(Args &&...args) {
        auto &ptr = get_ptr<Component>();
        if (!ptr) {
            throw std::runtime_error("Attempted to replace a component of an entity that doesn't own it");
        }
        return entity_traits::construct(ptr.get(), std::forward<Args>(args)...);
    }

    template <typename Component, typename... Args>
    decltype(auto) emplace_or_replace(Args &&...args) {
        auto &ptr = get_ptr<Component>();
        if (ptr) {
            return entity_traits::construct(ptr.get(), std::forward<Args>(args)...);
        }
        mask.set(index_of<Component>());
        ptr.reset(entity_traits::template allocate<Component>());
        return entity_traits::construct(ptr.get(), std::forward<Args>(args)...);
    }

    template <typename Component, typename... Func>
    decltype(auto) patch(Func &&...func) {
        auto &ptr = get_ptr<Component>();
        if (!ptr) {
            throw std::runtime_error("Attempted to patch a component of an entity that doesn't own it");
        }
        auto &component{*ptr};
        std::forward<Func>(func)(component), ...;
        return component;
    }

private:
    using entity_traits = Traits;
    using mask_type = detail::bitset<sizeof...(Components)>;
    template <typename Component>
    using component_ptr = std::unique_ptr<Component, typename entity_traits::template deleter<Component>>;

    template <typename Component>
    [[nodiscard]] consteval static auto index_of() {
        return detail::index_of<Component, Components...>();
    }

    template <typename T, typename... Types>
    struct mask_helper;

    template <typename T>
    struct mask_helper<T> {
        constexpr static mask_type value{std::index_sequence<index_of<T>()>()};
    };

    template <typename T, typename U, typename... Types>
    struct mask_helper<T, U, Types...> {
        constexpr static mask_type value{mask_helper<T>::value | mask_helper<U, Types...>::value};
    };

    template <typename Component, typename... Other>
    [[nodiscard]] consteval static auto get_masks() {
        return mask_helper<Component, Other...>::value;
    }

    template <typename Component>
    [[nodiscard]] component_ptr<Component> &get_ptr() noexcept {
        return std::get<component_ptr<Component>>(pool);
    }

    mask_type mask;
    std::tuple<component_ptr<Components>...> pool;
    entity_traits::id_type id;
};

template <typename Traits, typename... Components>
class basic_registry {
public:
    using entity_type = basic_entity<Traits, Components...>;

private:
};

template <typename... Components>
using registry = basic_registry<ecs_traits<std::allocator<char>>, Components...>;

template <typename... Components>
using entity = typename registry<Components...>::entity_type;

}

#endif // CORE_ECS_HPP