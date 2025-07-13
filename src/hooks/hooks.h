#pragma once

#include "ProjectileManager/ProjectileManager.h"

namespace Hooks {
	bool Install();

	template <typename T>
	struct ProjectileHook
	{
		static void Install() {
			REL::Relocation<std::uintptr_t> VTABLE{ T::VTABLE[0] };
			_getLinearVelocity = VTABLE.write_vfunc(0x86, &GetLinearVelocity);
			_getGravity = VTABLE.write_vfunc(0xB5, &GetGravity);
		}

		static void GetLinearVelocity(T* a_this, RE::NiPoint3& a_velocity) {
			_getLinearVelocity(a_this, a_velocity);
			if (!a_this) {
				return;
			}
			auto* manager = ProjectileManager::ProjectileManager::GetSingleton();
			if (!manager) {
				return;
			}
			auto factors = manager->ModifyProjectile(a_this);
			a_velocity *= factors.m_modifySpeed;
		}

		static float GetGravity(T* a_this) {
			float response = _getGravity(a_this);
			if (!a_this) {
				return response;
			}
			auto* manager = ProjectileManager::ProjectileManager::GetSingleton();
			if (!manager) {
				return response;
			}
			auto factors = manager->ModifyProjectile(a_this);
			return factors.m_modifyGravity * response;
		}

		inline static REL::Relocation<decltype(GetGravity)> _getGravity;
		inline static REL::Relocation<decltype(GetLinearVelocity)> _getLinearVelocity;
	};
}