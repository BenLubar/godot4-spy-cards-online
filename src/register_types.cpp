#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "godot_squirrel_register_types.h"

#include "util/rng.h"
#include "util/base32.h"
#include "util/why_isnt_this_in_godot.h"

#include "protocol/dedicated_server_rpc.h"
#include "protocol/spy_cards_client.h"
#include "protocol/spy_cards_lobby_connection.h"

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	initialize_squirrel_module(p_level);

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(RNG);
	GDREGISTER_CLASS(Base32);
	GDREGISTER_ABSTRACT_CLASS(WhyIsntThisInGodot);

#ifdef UNIX_ENABLED
	GDREGISTER_CLASS(DedicatedServerRPC);
	GDREGISTER_INTERNAL_CLASS(DedicatedServerRPCConnection);
#endif
	GDREGISTER_CLASS(SpyCardsClientRequest);
	GDREGISTER_CLASS(SpyCardsClient);
	GDREGISTER_CLASS(SpyCardsLobbyConnection);

	Engine *engine = Engine::get_singleton();
	engine->register_singleton(SpyCardsClient::get_class_static(), memnew(SpyCardsClient));
}

Vector<std::function<void()>> _free_lazy_globals;
void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	uninitialize_squirrel_module(p_level);

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	for (int64_t i = 0; i < _free_lazy_globals.size(); i++) {
		_free_lazy_globals[i]();
	}
	_free_lazy_globals.clear();
}

thread_local bool lazy_globals_must_be_first_used_from_the_main_thread = false;

// Initialization
extern "C" GDExtensionBool GDE_EXPORT spycardsonlinenative_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	lazy_globals_must_be_first_used_from_the_main_thread = true;

	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
