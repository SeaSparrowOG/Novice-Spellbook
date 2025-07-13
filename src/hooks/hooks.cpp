#include "Hooks/hooks.h"

namespace Hooks {
	bool Install() {
		SECTION_SEPARATOR;
		logger::info("Installing hooks..."sv);

		ProjectileHook<RE::BeamProjectile>::Install();
		ProjectileHook<RE::ArrowProjectile>::Install();
		ProjectileHook<RE::GrenadeProjectile>::Install();
		ProjectileHook<RE::MissileProjectile>::Install();
		ProjectileHook<RE::FlameProjectile>::Install();

		return true;
	}
}