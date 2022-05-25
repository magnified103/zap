#ifndef ENTITY_WEAPON_HPP
#define ENTITY_WEAPON_HPP

#include <stdexcept>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include "entity/inventory.hpp"

struct map3d;
struct player3d;
struct monster;

struct melee_weapon : inventory_item {
    float melee_damage;

    void use(map3d &map, player3d &player) override;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("inventory_item", (cereal::base_class<inventory_item>(this))),
                CEREAL_NVP(melee_damage));
    }
};

CEREAL_REGISTER_TYPE(melee_weapon);

struct ranged_weapon : inventory_item {
    int projectile_type;
    bool activated;
    int number_of_rounds;
    int max_number_of_rounds;

    ranged_weapon(){};

    ranged_weapon(int projectile_type, bool activated, int number_of_rounds,
                  int max_number_of_rounds)
        : projectile_type{projectile_type}, activated{activated},
          number_of_rounds{number_of_rounds}, max_number_of_rounds{max_number_of_rounds} {}

    void use(map3d &map, player3d &player) override;

    void use(map3d &map, monster &monster, player3d &player) override;

    int get_count() override;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("inventory_item", (cereal::base_class<inventory_item>(this))),
                CEREAL_NVP(projectile_type), CEREAL_NVP(activated), CEREAL_NVP(number_of_rounds),
                CEREAL_NVP(max_number_of_rounds));
    }
};

CEREAL_REGISTER_TYPE(ranged_weapon);

#endif // ENTITY_WEAPON_HPP