#include "papyrus.h"

#include "ProjectileManager/ProjectileManager.h"

namespace Papyrus
{
	static std::vector<int> GetVersion(STATIC_ARGS) {
		return { Plugin::VERSION[0], Plugin::VERSION[1], Plugin::VERSION[2] };
	}

	static bool AddDeadZone(STATIC_ARGS,
		RE::TESObjectREFR* a_deadZone,
		float a_zoneStrength,
		float a_zoneRadius)
	{
		if (!a_deadZone) {
			a_vm->TraceStack("AddDeadZone called with NONE akDeadZone.", a_stackID);
			return false;
		}
		if (a_zoneRadius < 0.0f) {
			a_vm->TraceStack("AddDeadZone called with negative afZoneRadius.", a_stackID);
		}

		auto* manager = ProjectileManager::ProjectileManager::GetSingleton();
		if (!manager) {
			a_vm->TraceStack("Internal error when fetching Projectile Manager.", a_stackID);
			return false;
		}

		return manager->AddDeadZone(a_deadZone, a_zoneStrength, a_zoneRadius);
	}

	static bool AddKillZone(STATIC_ARGS, RE::TESObjectREFR* a_deadZone, float a_zoneRadius) {
		if (!a_deadZone) {
			a_vm->TraceStack("AddKillZone called with NONE akDeadZone.", a_stackID);
			return false;
		}
		if (a_zoneRadius < 0.0f) {
			a_vm->TraceStack("AddKillZone called with negative afZoneRadius.", a_stackID);
		}

		auto* manager = ProjectileManager::ProjectileManager::GetSingleton();
		if (!manager) {
			a_vm->TraceStack("Internal error when fetching Projectile Manager.", a_stackID);
			return false;
		}

		return manager->AddKillZone(a_deadZone, a_zoneRadius);
	}

	static bool RemoveRefAsZone(STATIC_ARGS, RE::TESObjectREFR* a_zone) {
		if (!a_zone) {
			a_vm->TraceStack("AddKillZone called with NONE akDeadZone.", a_stackID);
			return false;
		}
		
		auto* manager = ProjectileManager::ProjectileManager::GetSingleton();
		if (!manager) {
			a_vm->TraceStack("Internal error when fetching Projectile Manager.", a_stackID);
			return false;
		}
		return manager->ClearZone(a_zone);
	}

	static void ClearAllZones(STATIC_ARGS) {
		auto* manager = ProjectileManager::ProjectileManager::GetSingleton();
		if (!manager) {
			a_vm->TraceStack("Internal error when fetching Projectile Manager.", a_stackID);
			return;
		}
		manager->ClearAllZones();
	}

	void Bind(VM& a_vm) {
		logger::info("  >Binding GetVersion..."sv);
		BIND(GetVersion);
		logger::info("  >Binding AddDeadZone..."sv);
		BIND(AddDeadZone);
		logger::info("  >Binding AddKillZone..."sv);
		BIND(AddKillZone);
		logger::info("  >Binding RemoveRefAsZone..."sv);
		BIND(RemoveRefAsZone);
		logger::info("  >Binding ClearAllZones..."sv);
		BIND(ClearAllZones);
	}

	bool RegisterFunctions(VM* a_vm) {
		SECTION_SEPARATOR;
		logger::info("Binding papyrus functions in utility script {}..."sv, script);
		Bind(*a_vm);
		logger::info("Finished binding functions."sv);
		return true;
	}
}
