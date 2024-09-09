#include "includes.h"

Skins g_skins{};;


#define MAX_WEAPONS	48

class CAttribute_String
{
public:
	virtual ~CAttribute_String();

	int32_t FieldSet;
	char** Value;

private:
	int32_t pad0[2];
};

struct cmd_context_t {
	bool		m_needs_processing{ };
	CUserCmd	m_user_cmd{ };
	int			m_cmd_number{ };
};

template < typename Key, typename Value >
struct Node_t {
	int previous_id;    //0x0000
	int next_id;        //0x0004
	void* _unknown_ptr; //0x0008
	int _unknown;       //0x000C
	Key key;            //0x0010
	Value value;        //0x0014
};

template < typename Key, typename Value >
struct Head_t {
	Node_t< Key, Value >* memory; //0x0000
	int allocation_count;         //0x0004
	int grow_size;                //0x0008
	int start_element;            //0x000C
	int next_available;           //0x0010
	int _unknown;                 //0x0014
	int last_element;             //0x0018
};

union attribute_data_union_t
{
	bool bValue;
	int iValue;
	float fValue;
	vec3_t* vValue;
	CAttribute_String* szValue;
};

class CEconItemAttributeDefinition;
class ISchemaAttributeType
{
public:
	virtual ~ISchemaAttributeType();
	virtual int GetTypeUniqueIdentifier();
	virtual void LoadEconAttributeValue();
	virtual void ConvertEconAttributeValueToByteStream();
	virtual bool BConvertStringToEconAttributeValue(const CEconItemAttributeDefinition*, const char*, attribute_data_union_t*);
};

struct attr_type_t
{
	char* name;
	ISchemaAttributeType* type;
};

struct AlternateIconData_t
{
private:
	int32_t pad0;

public:
	CUtlString icon_path;
	CUtlString icon_path_large;

private:
	CUtlString pad1[2];
};

struct kill_eater_score_type_t
{
	int id;
	char* type_name;
	char* model_attribute;
	char* level_data;
	bool use_level_data;
};

class CEconItemRarityDefinition
{
public:
	int value;
	int color;
	CUtlString name;
	CUtlString loc_key;
	CUtlString loc_key_weapon;
	CUtlString loot_list;
	CUtlString recycle_list;
	CUtlString drop_sound;
	CUtlString next_rarity;

private:
	int32_t pad0[2];

public:
	float weight;
};

class CEconItemQualityDefinition
{
public:
	int value;
	char* name;
	int weight;
	bool canSupportSet;
	char* hexColor;
};

class CEconItemAttributeDefinition
{
public:
	virtual ~CEconItemAttributeDefinition();

	KeyValues* kv;
	uint16_t id;
	ISchemaAttributeType* attribute_type;
	bool hidden;
	bool force_output_description;
	bool stored_as_integer;
	bool instance_data;
	int asset_class_export;
	int asset_class_bucket;
	int effect_type;
	int description_format;
	char* description_string;
	char* description_tag;
	char* armory_desc;
	int score;
	char* name;
	char* attribute_class;

private:
	int32_t pad0;
};

class CEconCraftingRecipeDefinition
{
public:
	virtual ~CEconCraftingRecipeDefinition();

	int definitionindex; // Don't ask me why Valve sometimes uses dword and sometimes word.
	CUtlString name;
	CUtlString n_A;
	CUtlString desc_inputs;
	CUtlString desc_outputs;
	CUtlString di_A;
	CUtlString di_B;
	CUtlString di_C;
	CUtlString do_A;
	CUtlString do_B;
	CUtlString do_C;
	wchar_t name_localized[64];
	wchar_t requires_produces_localized[512];
	bool all_same_class;
	bool all_same_slot;
	int add_class_usage_to_output;
	int add_slot_usage_to_output;
	int add_set_to_output;
	bool always_known;
	bool premium_only;
	int category;
	int filter;

private: // Some vectors and other things
	int32_t pad0[47];
};

struct item_list_entry_t
{
	int item;
	int paintkit;

private:
	int32_t pad0[5];
};

struct itemset_attrib_t;

class CEconItemSetDefinition
{
public:
	virtual ~CEconItemSetDefinition();

	char* key_name;
	char* name;
	char* unlocalized_name;
	char* set_description;
	CUtlVector<item_list_entry_t> items;
	int store_bundle;
	bool is_collection;
	bool is_hidden_set;
	CUtlVector<itemset_attrib_t> attributes;
};

class CEconLootListDefinition
{
public:
	virtual ~CEconLootListDefinition();

private:
	int32_t pad0[5]; // a vector

public:
	char* name;
	CUtlVector<item_list_entry_t> items;
	int hero;
	bool public_list_contents;
	bool contains_stickers_autographed_by_proplayers;
	bool will_produce_stattrak;
	float totalweights;
	CUtlVector<float> weights;

private:
	int32_t pad1[6]; // a vector + something else
};

class CPaintKit
{
public:
	int id;

	CUtlString name;
	CUtlString description_string;
	CUtlString description_tag;
	CUtlString pattern;
	CUtlString normal;
	CUtlString logo_material;

	bool pad0;

	int rarity;
	int style;

	Color color0;
	Color color1;
	Color color2;
	Color color3;

	Color logocolor0;
	Color logocolor1;
	Color logocolor2;
	Color logocolor3;

	float wear_default;
	float wear_remap_min;
	float wear_remap_max;

	uint8_t seed;

	uint8_t phongexponent;
	uint8_t phongalbedoboost;
	uint8_t phongintensity;

	float pattern_scale;
	float pattern_offset_y_start;
	float pattern_offset_y_end;
	float pattern_offset_x_start;
	float pattern_offset_x_end;
	float pattern_rotate_start;
	float pattern_rotate_end;

	float logo_scale;
	float logo_offset_x;
	float logo_offset_y;
	float logo_rotation;

	bool ignore_weapon_size_scale;
	int view_model_exponent_override_size;
	bool only_first_material;
	bool use_normal;

	CUtlString vmt_path;
	KeyValues* vmt_overrides;
};

class CStickerKit
{
public:
	int id;

	int item_rarity;

	CUtlString name;
	CUtlString description_string;
	CUtlString item_name;
	CUtlString sticker_material;
	CUtlString sticker_material_nodrips;
	CUtlString image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool cannot_trade;

	float rotate_start;
	float rotate_end;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	CUtlString image_inventory2;
	CUtlString image_inventory_large;

	KeyValues* kv;
};

class CEconColorDefinition
{
public:
	char* name;
	char* color_name;
	char* hex_color;
};

class CEconGraffitiTintDefinition
{
public:
	int id;
	char* name;
	char* hex_color;

	uint8_t b;
	uint8_t g;
	uint8_t r;
};

class CEconMusicDefinition
{
public:
	int id;
	char* name;
	char* loc_name;
	char* loc_description;
	char* pedestal_display_model;
	char* image_inventory;

private:
	int32_t pad0[2];
};

class CEconQuestDefinition
{
public:
	int id;
	char* name;
	char* mapgroup;
	char* map;
	char* gamemode;
	char* quest_reward;
	char* expression;
	char* expr_bonus;
	CCopyableUtlVector<unsigned int> points;

	enum
	{
		is_an_event = 1,
		event_allow_individual_maps
	} flags;

	int difficulty;
	int operational_points;
	int xp_reward;
	int xp_bonus_percent;
	int target_team;
	int required_event;

private:
	int32_t pad0;

public:
	int gametype; // gamemode & 15 | ( mapgroup << 8 )
	char* loc_name;
	char* loc_shortname;
	char* loc_description;
	char* loc_huddescription;
	KeyValues* string_tokens;
	char* loc_bonus;
	char* quest_icon;
};

class CEconCampaignDefinition
{
public:
	int id;

private:
	int32_t pad0;

public:
	char* loc_name;
	char* loc_description;

	class CEconCampaignNodeDefinition
	{
	public:
		class CEconCampaignNodeStoryBlockDefinition
		{
		public:
			char* content_file;
			char* character_name;
			char* expression;
			char* description;
		};
		CUtlVector<CEconCampaignNodeStoryBlockDefinition*> story_blocks;

		int id;
		int quest_index;
		int parentid;
		CUtlVector<unsigned int> questunlocks;
	};
	CUtlHashMapLarge<int, CEconCampaignNodeDefinition> quests;
	CUtlHashMapLarge<int, CEconCampaignNodeDefinition> quests2; // ???

	int season_number;
};

class CSkirmishModeDefinition
{
public:
	int id;
	char* name;
	char* gamemode;
	int igamemode;
	char* server_exec;
	char* loc_name;
	char* loc_rules;
	char* loc_description;
	char* loc_details;
	char* icon;
};

class CProPlayerData
{
public:
	KeyValues* kv;
	int id;
	CUtlString name;
	CUtlString code;
	int dob;
	CUtlString geo;
};

class CItemLevelingDefinition
{
public:
	int id;
	int score;
	char* rank_name;

private:
	int32_t pad0[2];
};

class CPipRankData
{
public:
	class PipRankInfo
	{
	private:
		int32_t pad0;

	public:
		int8_t id;
		int8_t pips;
		int8_t winstreak;
		int8_t loss;
	};

	CUtlVector<PipRankInfo> rank_info;
};

struct static_attrib_t // Get a CEconItemAttributeDefinition* with m_Attributes[static_attrib_t.id]
{
	uint16_t id;
	attribute_data_union_t value;
	bool force_gc_to_generate;
};

struct StickerData_t
{
	char viewmodel_geometry[128];
	char viewmodel_material[128];
	vec3_t worldmodel_decal_pos;
	vec3_t worldmodel_decal_end;
	char worldmodel_decal_bone[32];
};

struct codecontrolledbodygroupdata_t;
struct attachedmodel_t;
struct attachedparticle_t;
class CEconStyleInfo;
class AssetInfo
{
private:
	int32_t pad0;

public:
	CUtlMap<const char*, int, unsigned short> player_bodygroups;
	CUtlMap<const char*, codecontrolledbodygroupdata_t, unsigned short> code_controlled_bodygroup;
	int skin;
	bool use_per_class_bodygroups;
	CUtlVector<attachedmodel_t> attached_models;
	CUtlVector<attachedparticle_t> attached_particles;
	char* custom_sounds[10];
	char* material_override;
	int muzzle_flash;

private:
	int32_t pad1;

public:
	char* particle_effect;
	char* particle_snapshot;
	char* sounds[18];
	char* primary_ammo;
	char* secondary_ammo;
	char* weapon_type;
	char* addon_location;
	char* eject_brass_effect;
	char* muzzle_flash_effect_1st_person;
	char* muzzle_flash_effect_1st_person_alt;
	char* muzzle_flash_effect_3rd_person;
	char* muzzle_flash_effect_3rd_person_alt;
	char* heat_effect;
	char* player_animation_extension;
	vec3_t grenade_smoke_color;
	int vm_bodygroup_override;
	int vm_bodygroup_state_override;
	int wm_bodygroup_override;
	int wm_bodygroup_state_override;
	bool skip_model_combine;
	CUtlVector<const char*> animation_modifiers;
	CUtlVector<CEconStyleInfo*> styles;
};

struct WeaponPaintableMaterial_t
{
	char Name[128];
	char OrigMat[128];
	char FolderName[128];
	int ViewmodelDim;
	int WorldDim;
	float WeaponLength;
	float UVScale;
	bool BaseTextureOverride;
	bool MirrorPattern;
};

enum ClassID
{
	CAI_BaseNPC,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBreakableProp,
	CBreakableSurface,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMaterialModifyControl,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPlantedC4,
	CPlasma,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWorld,
	CWorldVguiText,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail,
};
class IEconTool;
class CCStrike15ItemSystem;
class CCStrike15ItemDefinition
{
public:
	virtual uint16_t GetDefinitionIndex() const = 0;
	virtual const char* GetPrefabName() const = 0;
	virtual const char* GetItemBaseName() const = 0;
	virtual const char* GetItemTypeName() const = 0;
	virtual const char* GetItemDesc() const = 0;
	virtual const char* GetInventoryImage() const = 0;
	virtual const char* GetBasePlayerDisplayModel() const = 0;
	virtual const char* GetWorldDisplayModel() const = 0;
	virtual const char* GetExtraWearableModel() const = 0;
	virtual int GetLoadoutSlot() const = 0;
	virtual KeyValues* GetRawDefinition() const = 0;
	virtual int GetHeroID() const = 0;
	virtual int GetRarity() const = 0;
	virtual CUtlVector<int>* GetItemSets() const = 0;
	virtual int GetBundleItemCount() const = 0;
	virtual void* GetBundleItem(int) const = 0;
	virtual bool IsBaseItem() const = 0;
	virtual bool IsPublicItem() const = 0;
	virtual bool IsBundle() const = 0;
	virtual bool IsPackBundle() const = 0;
	virtual bool IsPackItem() const = 0;
	virtual void* BInitVisualBlockFromKV(KeyValues*, void*, void*) = 0;
	virtual void* BInitFromKV(KeyValues*, void*, void*) = 0;
	virtual void* BInitFromTestItemKVs(int, KeyValues*, void*) = 0;
	virtual void* GeneratePrecacheModelStrings(bool, void*) const = 0;
	virtual void* GeneratePrecacheSoundStrings(void*) const = 0;
	virtual void* GeneratePrecacheEffectStrings(void*) const = 0;
	virtual void* CopyPolymorphic(const CCStrike15ItemDefinition*) = 0;
	virtual int GetItemTypeID() const = 0;
	virtual bool IsDefaultSlotItem() const = 0;
	virtual bool IsPreviewableInStore() const = 0;
	virtual int GetBundleItemPaintKitID(int) const = 0;
	virtual const char* GetWorldDroppedModel() const = 0;
	virtual const char* GetHolsteredModel() const = 0;
	virtual const char* GetZoomInSound() const = 0;
	virtual const char* GetZoomOutSound() const = 0;
	virtual const char* GetIconDisplayModel() const = 0;
	virtual const char* GetBuyMenuDisplayModel() const = 0;
	virtual const char* GetPedestalDisplayModel() const = 0;
	virtual const char* GetMagazineModel() const = 0;
	virtual const char* GetScopeLensMaskModel() const = 0;
	virtual const char* GetUidModel() const = 0;
	virtual const char* GetStatTrakModelByType(unsigned int) const = 0;
	virtual int GetNumSupportedStickerSlots() const = 0;
	virtual const char* GetStickerSlotModelBySlotIndex(unsigned int) const = 0;
	virtual void* GetStickerSlotWorldProjectionStartBySlotIndex(unsigned int) const = 0;
	virtual void* GetStickerSlotWorldProjectionEndBySlotIndex(unsigned int) const = 0;
	virtual void* GetStickerWorldModelBoneParentNameBySlotIndex(unsigned int) const = 0;
	virtual void* GetStickerSlotMaterialBySlotIndex(unsigned int) const = 0;
	virtual const char* GetIconDefaultImage() const = 0;
	virtual void* GetParticleFile() const = 0;
	virtual void* GetParticleSnapshotFile() const = 0;
	virtual bool IsRecent() const = 0;
	virtual bool IsContentStreamable() const = 0;
	virtual void* IgnoreInCollectionView() const = 0;
	virtual void* GeneratePrecacheModelStrings(bool, void*) = 0;

public:
	KeyValues* kv;
	uint16_t id;
	CUtlVector<uint16_t> associated_items;

private:
	int32_t pad0[2];

public:
	uint8_t min_ilevel;
	uint8_t max_ilevel;
	uint8_t item_rarity;
	uint8_t item_quality;
	uint8_t forced_item_quality;
	uint8_t default_drop_quality;
	uint8_t default_drop_quantity;
	CUtlVector<static_attrib_t> attributes;
	uint8_t popularity_seed;
	KeyValues* portraits;
	char* item_name;
	bool propername;
	char* item_type_name;
	int type_id; // crc32( item_type_name )
	char* item_description;
	int expiration_date;
	int creation_date;
	char* model_inventory;
	char* image_inventory;
	CUtlVector<const char*> image_inventory_overlay;
	int image_inventory_pos_x;
	int image_inventory_pos_y;
	int image_inventory_size_w;
	int image_inventory_size_h;
	char* model_player;

private:
	bool pad1;

public:
	bool hide_bodygroups_deployed_only;
	char* model_world;
	char* model_dropped;
	char* model_holstered;
	char* extra_wearable;

private:
	int32_t pad2[20];

public:
	CUtlVector<StickerData_t> stickers;
	char* icon_default_image;
	bool attach_to_hands;
	bool attach_to_hands_vm_only;
	bool flip_viewmodel;
	bool act_as_wearable;
	CUtlVector<int> item_sets;
	AssetInfo* visuals;

private:
	int32_t pad3;

public:
	bool allow_purchase_standalone;
	char* brass_eject_model;
	char* zoom_in_sound;
	char* zoom_out_sound;

	IEconTool* tool;

private:
	int32_t pad4[3];

public:
	int sound_material;
	bool disable_style_selector;

private:
	int32_t pad5[8];

public:
	char* particle_file;
	char* particle_snapshot;
	char* loot_list_name;

private:
	int32_t pad6[5];

public:
	CUtlVector<WeaponPaintableMaterial_t> paint_data;

	struct
	{
		ang_t* camera_angles;
		vec3_t* camera_offset;
		float camera_fov;
		LightDesc_t* lights[4];

	private:
		int32_t pad0;
	} *inventory_image_data;

	char* item_class;
	char* item_logname;
	char* item_iconname;
	char* name;
	bool hidden;
	bool show_in_armory;
	bool baseitem;
	bool default_slot_item;
	bool import;
	bool one_per_account_cdkey;

private:
	int32_t pad7;

public:
	char* armory_desc;
	CCStrike15ItemDefinition* armory_remapdef;
	CCStrike15ItemDefinition* store_remapdef;
	char* armory_remap;
	char* store_remap;
	char* class_token_id;
	char* slot_token_id;
	int drop_type;
	int holiday_restriction;
	int subtype;

private:
	int32_t pad8[4];

public:
	CUtlMap<unsigned int, const char*, unsigned short> alternate_icons;

private:
	int32_t pad9[9];

public:
	bool not_developer;
	bool ignore_in_collection_view;

	// This is where CCStrike15ItemDefinition begins
	int item_sub_position;
	int item_gear_slot;
	int item_gear_slot_position;
	int anim_slot;
	char* model_player_per_class[4];
	int class_usage[4];

private:
	int32_t pad10[2];
};

class CCStrike15ItemSchema
{
public:
	virtual ~CCStrike15ItemSchema();

	CEconItemAttributeDefinition* GetAttributeDefinitionByName(const char* Name) const;

private:
	int32_t pad0[11];

public:
	KeyValues* kv;

private:
	int32_t pad1;

public:
	uint16_t first_valid_class;
	uint16_t last_valid_class;
	uint16_t first_valid_item_slot;
	uint16_t last_valid_item_slot;
	int num_item_presets;
	int item_level_min;
	int item_level_max;

private:
	int32_t pad2;

public:
	CUtlVector<attr_type_t> attribute_types;
	CUtlHashMapLarge<int, CEconItemRarityDefinition> rarities;
	CUtlHashMapLarge<int, CEconItemQualityDefinition> qualities;

private:
	int32_t pad3[18];

public:
	CUtlHashMapLarge<int, CCStrike15ItemDefinition*> items;
	CCStrike15ItemDefinition* m_DefaultItem; // The first item
	CUtlVector<CEconItemAttributeDefinition*> attributes;
	CUtlHashMapLarge<int, CEconCraftingRecipeDefinition*> recipes;
	CUtlMap<const char*, CEconItemSetDefinition> item_sets;
	CUtlHashMapLarge<int, const char*> revolving_loot_lists;
	CUtlHashMapLarge<int, const char*> quest_reward_loot_lists;
	CUtlMap<const char*, CEconLootListDefinition> client_loot_lists;

private:
	int32_t pad4[14];

public:
	CUtlHashMapLarge<int, AlternateIconData_t> alternate_icons2;

private:
	int32_t pad5[18];

public:
	CUtlHashMapLarge<int, CPaintKit*> paint_kits;
	CUtlHashMapLarge<int, CStickerKit*> sticker_kits;
	CUtlMap<const char*, CStickerKit*> sticker_kits_namekey;

private:
	int32_t pad6[24];

public:
	CUtlMap<const char*, KeyValues*> prefabs;
	CUtlVector<CEconColorDefinition*> colors;
	CUtlVector<CEconGraffitiTintDefinition*> graffiti_tints;

private:
	int32_t pad7[20];

public:
	CUtlHashMapLarge<int, CEconMusicDefinition*> music_definitions;
	CUtlHashMapLarge<int, CEconQuestDefinition*> quest_definitions;
	CUtlHashMapLarge<int, CEconCampaignDefinition*> campaign_definitions;

private:
	int32_t pad8[27];

public:
	CUtlHashMapLarge<int, CSkirmishModeDefinition*> skirmish_modes;
	CUtlHashMapLarge<unsigned int, CProPlayerData*> pro_players;

private:
	int32_t pad9[53];

public:
	CUtlMap<unsigned int, kill_eater_score_type_t, unsigned short> kill_eater_score_types;
	CUtlMap<const char*, CUtlVector<CItemLevelingDefinition>*> item_levels;
	CPipRankData skirmish_rank_info;

private:
	int32_t pad10[43];

public: // This is where CCStrike15ItemSchema begins
	CUtlVector<const char*> m_ClassUsabilityStrings;
	CUtlVector<const char*> m_LoadoutStrings;
	CUtlVector<const char*> m_LoadoutStringsSubPositions;
	CUtlVector<const char*> m_LoadoutStringsForDisplay;

private:
	int32_t pad[5];

public:
	CUtlVector<const char*> m_LoadoutStringsOrder; // idk how to name this. 1st = primary, 2nd = secondary, 3rd = melee, 4th = grenade, 5th = item
};



void Skins::load() {
	// update model indexes on mapload.
	m_knife_data[knives_t::BAYONET].setup(KNIFE_BAYONET,
		XOR("models/weapons/v_knife_bayonet.mdl"),
		XOR("models/weapons/w_knife_bayonet.mdl"),
		XOR("bayonet"));

	m_knife_data[knives_t::BOWIE].setup(KNIFE_BOWIE,
		XOR("models/weapons/v_knife_survival_bowie.mdl"),
		XOR("models/weapons/w_knife_survival_bowie.mdl"),
		XOR("knife_survival_bowie"));


	m_knife_data[knives_t::BUTTERFLY].setup(KNIFE_BUTTERFLY,
		XOR("models/weapons/v_knife_butterfly.mdl"),
		XOR("models/weapons/w_knife_butterfly.mdl"),
		XOR("knife_butterfly"));

	m_knife_data[knives_t::FALCHION].setup(KNIFE_FALCHION,
		XOR("models/weapons/v_knife_falchion_advanced.mdl"),
		XOR("models/weapons/w_knife_falchion_advanced.mdl"),
		XOR("knife_falchion"));

	m_knife_data[knives_t::FLIP].setup(KNIFE_FLIP,
		XOR("models/weapons/v_knife_flip.mdl"),
		XOR("models/weapons/w_knife_flip.mdl"),
		XOR("knife_flip"));

	m_knife_data[knives_t::GUT].setup(KNIFE_GUT,
		XOR("models/weapons/v_knife_gut.mdl"),
		XOR("models/weapons/w_knife_gut.mdl"),
		XOR("knife_gut"));

	m_knife_data[knives_t::HUNTSMAN].setup(KNIFE_HUNTSMAN,
		XOR("models/weapons/v_knife_tactical.mdl"),
		XOR("models/weapons/w_knife_tactical.mdl"),
		XOR("knife_tactical"));

	m_knife_data[knives_t::KARAMBIT].setup(KNIFE_KARAMBIT,
		XOR("models/weapons/v_knife_karam.mdl"),
		XOR("models/weapons/w_knife_karam.mdl"),
		XOR("knife_karambit"));

	m_knife_data[knives_t::M9].setup(KNIFE_M9_BAYONET,
		XOR("models/weapons/v_knife_m9_bay.mdl"),
		XOR("models/weapons/w_knife_m9_bay.mdl"),
		XOR("knife_m9_bayonet"));

	m_knife_data[knives_t::DAGGER].setup(KNIFE_SHADOW_DAGGERS,
		XOR("models/weapons/v_knife_push.mdl"),
		XOR("models/weapons/w_knife_push.mdl"),
		XOR("knife_push"));

	// update glove model indexes on mapload.
	m_glove_data[gloves_t::BLOODHOUND].setup(5027,
		XOR("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"),
		XOR("models/weapons/w_models/arms/glove_bloodhound/w_glove_bloodhound.mdl"));

	m_glove_data[gloves_t::SPORTY].setup(5030,
		XOR("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"),
		XOR("models/weapons/w_models/arms/glove_sporty/w_glove_sporty.mdl"));

	m_glove_data[gloves_t::DRIVER].setup(5031,
		XOR("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"),
		XOR("models/weapons/w_models/arms/glove_slick/w_glove_slick.mdl"));

	m_glove_data[gloves_t::HANDWRAP].setup(5032,
		XOR("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"),
		XOR("models/weapons/w_models/arms/glove_handwrap_leathery/w_glove_handwrap_leathery.mdl"));

	m_glove_data[gloves_t::MOTO].setup(5033,
		XOR("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"),
		XOR("models/weapons/w_models/arms/glove_motorcycle/w_glove_motorcycle.mdl"));

	m_glove_data[gloves_t::SPECIALIST].setup(5034,
		XOR("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"),
		XOR("models/weapons/w_models/arms/glove_specialist/w_glove_specialist.mdl"));

	m_update_time = 0.f;
}

void Skins::think() {
	std::vector< Weapon* > weapons{};

	if (!g_menu.main.skins.enable.get())
		return;

	if (!g_csgo.m_engine->IsInGame())
		return;

	if (!g_cl.m_local || !g_cl.m_local->alive())
		return;

	player_info_t info;
	if (!g_csgo.m_engine->GetPlayerInfo(g_cl.m_local->index(), &info))
		return;

	static uintptr_t MUHAHAHAHA = pattern::find(g_csgo.m_client_dll, XOR("E8 ? ? ? ? FF 76 0C 8D 48 04 E8")).as<uintptr_t>();

	// store knife index.
	KnifeData* knife = &m_knife_data[g_menu.main.skins.knife.get()];

	if (!knife || !knife->m_id)
		return;

	// Skip the opcode, read rel32 address
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(MUHAHAHAHA + 1);

	// Add the offset to the end of the instruction
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem * (*)()>(MUHAHAHAHA + 5 + item_system_offset);

	// Skip VTable, first member variable of ItemSystem is ItemSchema
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));

	if (!item_schema)
		return;

	// Skip the instructions between, skip the opcode, read rel32 address
	const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(MUHAHAHAHA + 11 + 1);

	// Add the offset to the end of the instruction
	const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(MUHAHAHAHA + 11 + 5 + get_paint_kit_definition_offset);

	if (!get_paint_kit_definition_fn)
		return;

	// The last offset is start_element, we need that

		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]

		// Skip instructions, skip opcode, read offset
	const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

	// Calculate head base from start_element's offset ( 0x28C )
	const auto head_offset = start_element_offset - 12;

	const auto map_head = reinterpret_cast<Head_t< int, CPaintKit* >*>(std::uintptr_t(item_schema) + head_offset);

	if (!map_head)
		return;

	for (int i{ 1 }; i <= g_csgo.m_entlist->GetHighestEntityIndex(); ++i) {
		Entity* ent = g_csgo.m_entlist->GetClientEntity(i);
		if (!ent)
			continue;

		// run knifechanger.
		if (g_menu.main.skins.knife.get() > 0 && ent->is(HASH("CPredictedViewModel"))) {
			// get weapon entity from predicted viewmodel.
			Weapon* weapon = ent->as< Weapon* >()->GetWeapon();

			// no need to be here anymore.
			if (!weapon || !weapon->IsKnife())
				continue;

			// this weapon does not belong to us, we are done here.
			// should barely happen unless you pick up someones knife.
			// possible on servers with 'mp_drop_knife_enable 1'.
			if (info.m_xuid_low != weapon->m_OriginalOwnerXuidLow() || info.m_xuid_high != weapon->m_OriginalOwnerXuidHigh())
				continue;

			// get and validate world model handle from the weapon
			Weapon* weapon_world_model = weapon->GetWeaponWorldModel();
			if (!weapon_world_model)
				continue;

			// set model index of the predicted viewmodel.
			ent->m_nModelIndex() = knife->m_model_index;

			// set the world model index.
			// do this to have knifechanger in third person. verry p.
			weapon_world_model->m_nModelIndex() = knife->m_world_model_index;

			// correct m_nSequence and m_flCycle.
			UpdateAnimations(ent);
		}

		else if (ent->IsBaseCombatWeapon()) {
			// cast to weapon class.
			Weapon* weapon = ent->as< Weapon* >();

			// this is not our gun.
			if (!weapon || info.m_xuid_low != weapon->m_OriginalOwnerXuidLow() || info.m_xuid_high != weapon->m_OriginalOwnerXuidHigh())
				continue;

			// we found a weapon that we own.
			weapons.push_back(weapon);

			if (weapon->IsKnife()) {
				// if this weapon is a knife, set some additional stuff.
				if (g_menu.main.skins.knife.get() > 0) {
					// set the item id, this is for the hud.
					weapon->m_iItemDefinitionIndex() = knife->m_id;

					// not needed. but everyone does this, try without.
					weapon->m_nModelIndex() = knife->m_model_index;

					// not needed. but everyone does this, try without.
					weapon->m_iViewModelIndex() = knife->m_model_index;

					// not needed. but everyone does this, try without.
					weapon->m_iWorldModelIndex() = knife->m_world_model_index;

					// set the little star thing.
					weapon->m_iEntityQuality() = 3;
				}

				else if (g_menu.main.skins.knife.get() == 0) {
					// fix definition.
					if (g_cl.m_local->m_iTeamNum() == TEAM_TERRORISTS)
						weapon->m_iItemDefinitionIndex() = KNIFE_T;

					else if (g_cl.m_local->m_iTeamNum() == TEAM_COUNTERTERRORISTS)
						weapon->m_iItemDefinitionIndex() = KNIFE_CT;

					// reset.
					weapon->m_iEntityQuality() = 0;
					weapon->m_nFallbackPaintKit() = 0;
					weapon->m_nFallbackStatTrak() = 0;
				}
			}

			switch (weapon->m_iItemDefinitionIndex()) {
			case DEAGLE:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_deagle.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_deagle.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_deagle.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_deagle.get()) / 100.f) + FLT_EPSILON;
				break;
			case ELITE:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_elite.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_elite.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_elite.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_elite.get()) / 100.f) + FLT_EPSILON;
				break;
			case FIVESEVEN:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_fiveseven.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_fiveseven.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_fiveseven.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_fiveseven.get()) / 100.f) + FLT_EPSILON;
				break;
			case GLOCK:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_glock.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_glock.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_glock.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_glock.get()) / 100.f) + FLT_EPSILON;
				break;
			case AK47:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_ak47.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_ak47.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_ak47.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_ak47.get()) / 100.f) + FLT_EPSILON;
				break;
			case AUG:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_aug.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_aug.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_aug.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_aug.get()) / 100.f) + FLT_EPSILON;
				break;
			case AWP:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_awp.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_awp.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_awp.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_awp.get()) / 100.f) + FLT_EPSILON;
				break;
			case FAMAS:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_famas.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_famas.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_famas.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_famas.get()) / 100.f) + FLT_EPSILON;
				break;
			case G3SG1:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_g3sg1.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_g3sg1.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_g3sg1.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_g3sg1.get()) / 100.f) + FLT_EPSILON;
				break;
			case GALIL:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_galil.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_galil.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_galil.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_galil.get()) / 100.f) + FLT_EPSILON;
				break;
			case M249:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_m249.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_m249.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_m249.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_m249.get()) / 100.f) + FLT_EPSILON;
				break;
			case M4A4:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_m4a4.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_m4a4.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_m4a4.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_m4a4.get()) / 100.f) + FLT_EPSILON;
				break;
			case MAC10:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_mac10.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_mac10.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_mac10.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_mac10.get()) / 100.f) + FLT_EPSILON;
				break;
			case P90:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_p90.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_p90.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_p90.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_p90.get()) / 100.f) + FLT_EPSILON;
				break;
			case UMP45:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_ump45.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_ump45.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_ump45.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_ump45.get()) / 100.f) + FLT_EPSILON;
				break;
			case XM1014:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_xm1014.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_xm1014.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_xm1014.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_xm1014.get()) / 100.f) + FLT_EPSILON;
				break;
			case BIZON:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_bizon.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_bizon.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_bizon.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_bizon.get()) / 100.f) + FLT_EPSILON;
				break;
			case MAG7:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_mag7.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_mag7.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_mag7.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_mag7.get()) / 100.f) + FLT_EPSILON;
				break;
			case NEGEV:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_negev.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_negev.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_negev.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_negev.get()) / 100.f) + FLT_EPSILON;
				break;
			case SAWEDOFF:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_sawedoff.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_sawedoff.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_sawedoff.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_sawedoff.get()) / 100.f) + FLT_EPSILON;
				break;
			case TEC9:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_tec9.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_tec9.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_tec9.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_tec9.get()) / 100.f) + FLT_EPSILON;
				break;
			case P2000:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_p2000.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_p2000.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_p2000.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_p2000.get()) / 100.f) + FLT_EPSILON;
				break;
			case MP7:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_mp7.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_mp7.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_mp7.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_mp7.get()) / 100.f) + FLT_EPSILON;
				break;
			case MP9:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_mp9.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_mp9.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_mp9.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_mp9.get()) / 100.f) + FLT_EPSILON;
				break;
			case NOVA:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_nova.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_nova.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_nova.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_nova.get()) / 100.f) + FLT_EPSILON;
				break;
			case P250:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_p250.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_p250.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_p250.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_p250.get()) / 100.f) + FLT_EPSILON;
				break;
			case SCAR20:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_scar20.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_scar20.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_scar20.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_scar20.get()) / 100.f) + FLT_EPSILON;
				break;
			case SG553:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_sg553.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_sg553.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_sg553.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_sg553.get()) / 100.f) + FLT_EPSILON;
				break;
			case SSG08:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_ssg08.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_ssg08.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_ssg08.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_ssg08.get()) / 100.f) + FLT_EPSILON;
				break;
			case M4A1S:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_m4a1s.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_m4a1s.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_m4a1s.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_m4a1s.get()) / 100.f) + FLT_EPSILON;
				break;
			case USPS:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_usps.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_usps.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_usps.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_usps.get()) / 100.f) + FLT_EPSILON;
				break;
			case CZ75A:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_cz75a.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_cz75a.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_cz75a.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_cz75a.get()) / 100.f) + FLT_EPSILON;
				break;
			case REVOLVER:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_revolver.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_revolver.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_revolver.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_revolver.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_BAYONET:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_bayonet.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_bayonet.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_bayonet.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_bayonet.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_FLIP:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_flip.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_flip.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_flip.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_flip.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_GUT:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_gut.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_gut.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_gut.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_gut.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_KARAMBIT:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_karambit.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_karambit.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_karambit.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_karambit.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_M9_BAYONET:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_m9.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_m9.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_m9.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_m9.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_HUNTSMAN:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_huntsman.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_huntsman.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_huntsman.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_huntsman.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_FALCHION:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_falchion.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_falchion.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_falchion.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_falchion.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_BOWIE:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_bowie.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_bowie.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_bowie.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_bowie.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_BUTTERFLY:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_butterfly.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_butterfly.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_butterfly.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_butterfly.get()) / 100.f) + FLT_EPSILON;
				break;
			case KNIFE_SHADOW_DAGGERS:
				weapon->m_nFallbackPaintKit() = g_menu.main.skins.id_daggers.get();
				weapon->m_nFallbackStatTrak() = g_menu.main.skins.stattrak_daggers.get() ? 1337 : 0;
				weapon->m_nFallbackSeed() = g_menu.main.skins.seed_daggers.get();
				weapon->m_flFallbackWear() = ((100.f - g_menu.main.skins.quality_daggers.get()) / 100.f) + FLT_EPSILON;
				break;

			default:
				break;
			}

			if (g_menu.main.skins.custom_paintkit.get() && map_head)
			{
				for (auto i = 0; i < map_head->last_element; ++i)
				{
					const auto paint_kit = map_head->memory[i].value;
					if (!paint_kit)
						continue;

					switch (weapon->m_iItemDefinitionIndex())
					{
					case SSG08:
						paint_kit->color0 = g_menu.main.skins.ssg08_color.get();
						paint_kit->color1 = g_menu.main.skins.ssg08_color1.get();
						paint_kit->color2 = g_menu.main.skins.ssg08_color2.get();
						paint_kit->color3 = g_menu.main.skins.ssg08_color3.get();
						break;
					case AWP:
						paint_kit->color0 = g_menu.main.skins.awp_color.get();
						paint_kit->color1 = g_menu.main.skins.awp_color1.get();
						paint_kit->color2 = g_menu.main.skins.awp_color2.get();
						paint_kit->color3 = g_menu.main.skins.awp_color3.get();
						break;
					case SCAR20:
						paint_kit->color0 = g_menu.main.skins.scar20_color.get();
						paint_kit->color1 = g_menu.main.skins.scar20_color1.get();
						paint_kit->color2 = g_menu.main.skins.scar20_color2.get();
						paint_kit->color3 = g_menu.main.skins.scar20_color3.get();
						break;
					case G3SG1:
						paint_kit->color0 = g_menu.main.skins.g3sg1_color.get();
						paint_kit->color1 = g_menu.main.skins.g3sg1_color1.get();
						paint_kit->color2 = g_menu.main.skins.g3sg1_color2.get();
						paint_kit->color3 = g_menu.main.skins.g3sg1_color3.get();
						break;
					}
				}
			}

			// fix stattrak ownership.
			weapon->m_iAccountID() = info.m_xuid_low;

			// fix stattrak in hud.
			if (weapon->m_nFallbackStatTrak() >= 0)
				weapon->m_iEntityQuality() = 9;

			if (weapon->m_nFallbackPaintKit() < 0)
				weapon->m_nFallbackPaintKit() = 0;

			if (weapon->m_nFallbackSeed() < 0)
				weapon->m_nFallbackSeed() = 0;
			
			if (weapon->m_nFallbackStatTrak() < 0)
				weapon->m_nFallbackStatTrak() = 0;

			// force use fallback values.
			weapon->m_iItemIDHigh() = -1;
		}
	}

	if (g_menu.main.skins.glove.get() > 0) {
		CBaseHandle* wearables = g_cl.m_local->m_hMyWearables();
		if (wearables) {
			// get pointer to entity from wearable handle 0.
			// glove is at wearable 0, valve will likely add more wearables like hats and boots. thats why.
			Weapon* glove = g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >(wearables[0]);

			// there is no glove entity yet.
			if (!glove) {
				// attempt to get our old glove.
				Weapon* old = g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >(m_glove_handle);

				// this glove is still valid.
				if (old) {

					// write back handle to wearables.
					wearables[0] = m_glove_handle;

					// set glove pointer.
					glove = old;
				}
			}

			// if we at this point still not have a glove entity.
			// we should create one.
			if (!glove) {
				ClientClass* list{ g_csgo.m_client->GetAllClasses() };

				if (list) {

					// iterate list.
					for (; list != nullptr; list = list->m_pNext) {

						// break if we found wearable
						if (list->m_ClassID == g_netvars.GetClientID(HASH("CEconWearable")))
							break;
					}

					if (list) {
						// create an ent index and serial no for the new glove entity.
						int index = g_csgo.m_entlist->GetHighestEntityIndex() + 1;
						int serial = g_csgo.RandomInt(0xA00, 0xFFF);

						// call ctor on CEconWearable entity.
						Address networkable = list->m_pCreate(index, serial);

						// get entity ptr via index.
						glove = g_csgo.m_entlist->GetClientEntity< Weapon* >(index);

						static Address offset = g_netvars.get(HASH("DT_EconEntity"), HASH("m_iItemIDLow"));

						// m_bInitialized?
						networkable.add(offset.add(0x8)).set< bool >(true);

						// no idea what this is.
						networkable.add(offset.sub(0xC)).set< bool >(true);

						// set the wearable handle.
						wearables[0] = index | (serial << 16);

						// use this for later on.
						m_glove_handle = wearables[0];
					}
				}
			}

			// store glove data.
			GloveData* data = &m_glove_data[g_menu.main.skins.glove.get()];

			// set default data,
			glove->m_nFallbackSeed() = 0;
			glove->m_nFallbackStatTrak() = 0;
			glove->m_iItemIDHigh() = -1;
			glove->m_iEntityQuality() = 4;
			glove->m_iAccountID() = info.m_xuid_low;

			// set custom data.
			glove->m_nFallbackPaintKit() = 10000 + g_menu.main.skins.glove_id.get();
			glove->m_iItemDefinitionIndex() = data->m_id;
			glove->SetGloveModelIndex(data->m_model_index);

			// update the glove.
			glove->PreDataUpdate(DATA_UPDATE_CREATED);
		}
	}

	// only force update every 1s.
	if (m_update && g_csgo.m_globals->m_curtime >= m_update_time) {
		for (auto& w : weapons)
			UpdateItem(w);

		m_update = false;
		m_update_time = g_csgo.m_globals->m_curtime + 1.f;
	}
}

void Skins::UpdateItem(Weapon* item) {
	if (!item || !item->IsBaseCombatWeapon())
		return;

	if (g_csgo.m_cl->m_delta_tick == -1)
		return;

	//  if ( sub_106E32D0(v4, "round_start") )
	//		SFWeaponSelection::ShowAndUpdateSelection( v2, 2, 0, v10 );

	// v1 = CHud::FindElement(&g_HUD, "SFWeaponSelection");
	// if( v1 )
	//		SFWeaponSelection::ShowAndUpdateSelection( ( int * )v1, 2, 0, this );

	item->m_bCustomMaterialInitialized() = item->m_nFallbackPaintKit() <= 0;

	item->m_CustomMaterials().RemoveAll();
	item->m_CustomMaterials2().RemoveAll();

	size_t count = item->m_VisualsDataProcessors().Count();
	for (size_t i{}; i < count; ++i) {
		auto& elem = item->m_VisualsDataProcessors()[i];
		if (elem) {
			elem->unreference();
			elem = nullptr;
		}
	}

	item->m_VisualsDataProcessors().RemoveAll();

	item->PostDataUpdate(DATA_UPDATE_CREATED);
	item->OnDataChanged(DATA_UPDATE_CREATED);

	CHudElement* SFWeaponSelection = g_csgo.m_hud->FindElement(HASH("SFWeaponSelection"));

	if (!SFWeaponSelection)
		return;

	g_csgo.ShowAndUpdateSelection(SFWeaponSelection, 0, item, false);
}

void Skins::UpdateAnimations(Entity* ent) {
	if (!ent)
		return;

	int knife = g_menu.main.skins.knife.get();

	int seq = ent->m_nSequence();

	// check if this knife needs extra fixing.
	if (knife == knives_t::BUTTERFLY || knife == knives_t::FALCHION || knife == knives_t::DAGGER || knife == knives_t::BOWIE) {

		// fix the idle sequences.
		if (seq == sequence_default_idle1 || seq == sequence_default_idle2) {
			// set the animation to be completed.
			ent->m_flCycle() = 0.999f;

			// cycle change, re-render.
			ent->InvalidatePhysicsRecursive(ANIMATION_CHANGED);
		}
	}

	// fix sequences.
	if (m_last_seq != seq) {
		if (knife == knives_t::BUTTERFLY) {
			switch (seq) {
			case sequence_default_draw:
				seq = g_csgo.RandomInt(sequence_butterfly_draw, sequence_butterfly_draw2);
				break;

			case sequence_default_lookat01:
				seq = g_csgo.RandomInt(sequence_butterfly_lookat01, sequence_butterfly_lookat03);
				break;

			default:
				seq++;
				break;
			}
		}

		else if (knife == knives_t::FALCHION) {
			switch (seq) {
			case sequence_default_draw:
			case sequence_default_idle1:
				break;

			case sequence_default_idle2:
				seq = sequence_falchion_idle1;
				break;

			case sequence_default_heavy_miss1:
				seq = g_csgo.RandomInt(sequence_falchion_heavy_miss1, sequence_falchion_heavy_miss1_noflip);
				break;

			case sequence_default_lookat01:
				seq = g_csgo.RandomInt(sequence_falchion_lookat01, sequence_falchion_lookat02);
				break;
			}
		}

		else if (knife == knives_t::DAGGER) {
			switch (seq) {
			case sequence_default_idle2:
				seq = sequence_push_idle1;
				break;

			case sequence_default_heavy_hit1:
			case sequence_default_heavy_backstab:
			case sequence_default_lookat01:
				seq += 3;
				break;

			case sequence_default_heavy_miss1:
				seq = sequence_push_heavy_miss2;
				break;
			}

		}

		else if (knife == knives_t::BOWIE) {
			if (seq > sequence_default_idle1)
				seq--;
		}

		m_last_seq = seq;
	}

	// write back fixed sequence.
	ent->m_nSequence() = seq;
}