#ifndef LEGACY_PARSE_H
#define LEGACY_PARSE_H

#include "dry.h"

#include "protocol/data_container.h"

#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_gltf.h"

class LegacyParse : public Object {
	GDCLASS(LegacyParse, Object);

protected:
	static void _bind_methods();

public:
	static Ref<DataContainer> card_set(const PackedStringArray &buf, const Ref<GameMode> &vanilla_default, const PackedByteArray &p1_spoiler_guard = PackedByteArray(), const PackedByteArray &p2_spoiler_guard = PackedByteArray());
private:
	static LazyPredefined<GameMode> VANILLA_1_0_5;
	static LazyPredefined<GameMode> VANILLA_1_1;
	static LazyPredefined<GameMode> VANILLA_1_1_1;
	static LazyPredefined<GameMode> VANILLA_1_2_1;

	static LazyPredefined<JigsawParameterFileIDOpus> SOUND_ATK_SUCCESS;
	static LazyPredefined<JigsawParameterFileIDOpus> SOUND_ATK_FAIL;
	static LazyPredefined<JigsawParameterFileIDOpus> SOUND_NULL;

	static LazyPredefined<JigsawParameterFileIDGLTF> METAL_ISLAND_AUDITORIUM;
	static LazyPredefined<JigsawParameterFileIDGLTF> GOLDEN_HILLS;
	static LazyPredefined<JigsawParameterFileIDGLTF> FORSAKEN_LANDS;
	static LazyPredefined<JigsawParameterFileIDGLTF> UNDERGROUND_TAVERN;
	static LazyPredefined<JigsawParameterFileIDGLTF> BROODMOTHER;
	static LazyPredefined<JigsawParameterFileIDGLTF> ABANDONED_TENT;
	static LazyPredefined<JigsawParameterFileIDGLTF> ANT_KINGDOM_MINE;
	static LazyPredefined<JigsawParameterFileIDGLTF> ANT_KINGDOM_PLAZA;

	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_GENERIC;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TOURNEY_PLAYER;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_JANET;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_MENDER_SPAM_MOTHIVA;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_MENDER_SPAM_KALI;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_MENDER_SPAM_RANDOM;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_GENERIC;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_JANET;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_BU_GI;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_JOHNNY;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_KAGE;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_RITCHEE;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_SERENE;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_CARMINA;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_CHUCK;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_ARIE;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_SHAY;
	static LazyPredefined<JigsawProcedureNPCBuildDeck> LEGACY_NPC_BUILD_DECK_TP2_CROW;
	static LazyPredefined<JigsawProcedureNPCDecideChoice> LEGACY_NPC_DECIDE_CHOICE_GENERIC;
	static LazyPredefined<JigsawProcedureNPCDecideChoice> LEGACY_NPC_DECIDE_CHOICE_MENDER_SPAM;
	static LazyPredefined<JigsawProcedureNPCDecideChoice> LEGACY_NPC_DECIDE_CHOICE_TP2;

	static enums::RankDef::Rank card_rank(enums::CardDef::Card id);
	static String card_name(enums::CardDef::Card id);

	static bool game_mode(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, Ref<GameMode> &vanilla);
	static bool field(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant, Ref<GameMode> &vanilla);
	static bool field_metadata(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_banned_cards(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_game_rules(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_summon_card(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_variant(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, Ref<GameMode> &vanilla);
	static bool field_unfilter_card(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_deck_limit_filter(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_timer(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_turn_0_effect(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_vanilla_version(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, Ref<GameMode> &vanilla);
	static bool field_card_group(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);
	static bool field_special_flags(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant);

	static bool card(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh);
	static bool card_v0_v1(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, int64_t format_version);
	static bool card_v2_v4(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, int64_t format_version);
	static bool card_v5(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, int64_t format_version);

	static bool effect_v0_v1(const Ref<DataContainer> &container, const Ref<EffectInstance> &effect, const Ref<FormatHelper> &fh, int64_t format_version);
	static bool effect_v2_v4(const Ref<DataContainer> &container, const Ref<EffectInstance> &effect, const Ref<FormatHelper> &fh, int64_t format_version);
	static bool effect_v5(const Ref<DataContainer> &container, const Ref<EffectInstance> &effect, const Ref<FormatHelper> &fh, int64_t format_version);
	static bool extension_v5(const Ref<DataContainer> &container, const Ref<EffectInstance> &extension, const Ref<FormatHelper> &fh, int64_t format_version);

	static Ref<JigsawParameterAmount> read_amount_v0_v1(const Ref<FormatHelper> &fh);
	static Ref<JigsawParameterAmount> read_amount_v2_v4(const Ref<FormatHelper> &fh, bool negate);
	static Ref<JigsawParameterAmount> read_amount_v5(const Ref<FormatHelper> &fh, bool infinite);
	static Ref<CardFilter> read_card_filter_v2_v4(const Ref<FormatHelper> &fh, bool generic);
	static Ref<CardFilter> read_card_filter_v5(const Ref<FormatHelper> &fh);

	static enums::IconDef::Icon custom_portrait(const Ref<DataContainer> &container, const PackedByteArray &data, bool external);
	static enums::TribeDef::Tribe custom_tribe(const Ref<DataContainer> &container, const Color &color, const String &name);
	static enums::NPCDef::NPC custom_npc(const Ref<DataContainer> &container, const String &code);

	static enums::NPCDef::NPC add_legacy_npc(const Ref<DataContainer> &container, const String &code, const String &display_name, const String &character, const Ref<JigsawProcedureNPCBuildDeck> &build_deck, const Ref<JigsawProcedureNPCDecideChoice> &decide_choice);
	static enums::NPCDef::NPC legacy_npc_generic(const Ref<DataContainer> &container, const String &code);
	static enums::NPCDef::NPC legacy_npc_tourney_player(const Ref<DataContainer> &container, const String &name);
	static enums::NPCDef::NPC legacy_npc_janet(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_card_master(const Ref<DataContainer> &container, const String &code, const String &name, const String &deck);
	static enums::NPCDef::NPC legacy_npc_mender_spam(const Ref<DataContainer> &container, const String &code);
	static enums::NPCDef::NPC legacy_npc_tp2_generic(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_janet(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_bu_gi(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_johnny(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_kage(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_ritchee(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_serene(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_carmina(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_chuck(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_arie(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_shay(const Ref<DataContainer> &container);
	static enums::NPCDef::NPC legacy_npc_tp2_crow(const Ref<DataContainer> &container);

	static void finalize_effect_param(const Ref<DataContainer> &container, const Ref<JigsawParameter> &param);
	static void finalize_card_filter(const Ref<DataContainer> &container, const Ref<CardFilter> &filter);
public:
	static Ref<DataContainer> card_recording(const PackedByteArray &buf);
	static Ref<DataContainer> arcade_recording(const PackedByteArray &buf);
};

#endif // LEGACY_PARSE_H
