/**
 * Copyright 2016 Fredrik Åhs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBTYRA_COMPONENTMANAGER_H
#define LIBTYRA_COMPONENTMANAGER_H

#include "../inc/defs.hpp"
#include "../inc/typeid.hpp"

#include <array>
#include <bitset>
#include <unordered_set>
#include <vector>

namespace tyra {

    class Component;

    class ComponentManager {
        private:
            typedef std::array<Component*, MAX_COMPONENT_TYPES>	ComponentArray;
            typedef std::bitset<MAX_COMPONENT_TYPES>            ComponentBitSet;

            std::unordered_set<EntityId>    m_updated;

            std::vector<ComponentArray>		m_components;
            std::vector<ComponentBitSet>	m_components_bits;
            size_t							m_num_registered_components;

            void add(EntityId, TypeId, Component*);
            void remove(EntityId, TypeId);
            void removeAll(EntityId);
            bool valid(EntityId, TypeId) const;
            Component* get(EntityId, TypeId) const;

        public:
            ~ComponentManager();

            template <typename T> bool valid(EntityId) const;
            template <typename T> T& get(EntityId) const;
            template <typename T, typename... Args> void add(EntityId, Args&&...);
            template <typename T> void remove(EntityId);

            std::bitset<MAX_COMPONENT_TYPES>& bits(EntityId); 
            std::unordered_set<EntityId>& updated() { return m_updated; }
            size_t size() const { return m_num_registered_components; }
    };

    template <typename T> bool ComponentManager::valid(EntityId entity_id) const {
        static_assert(std::is_base_of<Component, T>::value, "Entity::has_component: T must be derived from Component");
        return valid(entity_id, Type<Component>::id<T>());
    }

    template <typename T> T& ComponentManager::get(EntityId entity_id) const {
        static_assert(std::is_base_of<Component, T>::value, "Entity::get_component: T must be derived from Component");
        return static_cast<T&>(*get(Type<Component>::id<T>(), entity_id));
    }

    template <typename T, typename... Args>	void ComponentManager::add(EntityId entity_id, Args&&... args)	{
        static_assert(std::is_base_of<Component, T>::value, "Entity::add_component: T must be derived from Component");
        T* ptr = new T{std::forward<Args>(args)...};
        add(entity_id, Type<Component>::id<T>(), ptr);
    }

    template <typename T> void ComponentManager::remove(EntityId entity_id) {
        static_assert(std::is_base_of<Component, T>::value, "Entity::remove_component: T must be derived from Component");
        removeComponent(entity_id, Type<Component>::id<T>());
    }

}

#endif
