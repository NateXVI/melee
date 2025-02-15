#ifndef MELEE_IT_TYPES_H
#define MELEE_IT_TYPES_H

#include <common_structs.h>
#include <dolphin/mtx/types.h>
#include <math.h>
#include <melee/ft/types.h>
#include <melee/gr/stage.h>
#include <melee/it/forward.h>
#include <melee/it/itCommonItems.h>
#include <melee/it/itkind.h>
#include <melee/it/itPKFlash.h>
#include <melee/it/itPKThunder.h>
#include <melee/it/itYoyo.h>
#include <melee/pl/player.h>
#include <Runtime/platform.h>
#include <sysdolphin/baselib/aobj.h>
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjgxlink.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <sysdolphin/baselib/gobjuserdata.h>
#include <sysdolphin/baselib/jobj.h>
#include <sysdolphin/baselib/mobj.h>
#include <sysdolphin/baselib/random.h>

/// @todo Size unknown.
struct ItemStateTable {
    /// @at{0} @sz{4}
    enum_t asid;

    /// @at{4} @sz{4}
    HSD_GObjPredicate animated;

    /// @at{8} @sz{4}
    HSD_GObjEvent physics_updated;

    /// @at{C} @sz{4}
    HSD_GObjPredicate collided;
};

/// @todo Size unknown.
struct ItemStateContainer {
    /// @at{0}
    ItemStateTable stateTable UNK_SIZE_ARRAY;
};

struct ItemLogicTable {
    /// @at{0} @sz{4}
    ItemStateContainer* states;

    /// @at{4} @sz{4}
    HSD_GObjEvent spawned;

    /// @at{8} @sz{4}
    HSD_GObjEvent destroyed;

    /// @at{C} @sz{4}
    HSD_GObjEvent picked_up;

    /// @at{10} @sz{4}
    HSD_GObjEvent dropped;

    /// @at{14} @sz{4}
    HSD_GObjEvent thrown;

    /// @at{18} @sz{4}
    HSD_GObjPredicate dmg_dealt;

    /// @at{1C} @sz{4}
    HSD_GObjPredicate dmg_received;

    /// @at{20} @sz{4}
    HSD_GObjEvent entered_air;

    /// @at{24} @sz{4}
    HSD_GObjPredicate reflected;

    /// @at{28} @sz{4}
    HSD_GObjPredicate clanked;

    /// @at{2C} @sz{4}
    HSD_GObjPredicate absorbed;

    /// @at{30} @sz{4}
    HSD_GObjPredicate shield_bounced;

    /// @at{34} @sz{4}
    HSD_GObjPredicate hit_shield;

    /// @at{38} @sz{4}
    HSD_GObjInteraction evt_unk;
};

struct CameraBoxFlags {
    struct {
        u8 b01 : 2;
        u8 b2 : 1;
        u8 b3 : 1;
        u8 b4 : 1;
        u8 b5 : 1;
        u8 b6 : 1;
        u8 b7 : 1;
    } bits;
};

struct flag32 {
    union {
        struct {
            u32 x0 : 1;
            u32 x1 : 1;
            u32 x2 : 1;
            u32 x3 : 1;
            u32 x4 : 1;
            u32 x5 : 1;
            u32 x6 : 1;
            u32 x7 : 1;
            u32 x8 : 1;
            u32 x9 : 1;
            u32 xA : 1;
            u32 xB : 1;
            u32 xC : 1;
            u32 xD : 1;
            u32 xE : 1;
            u32 xF : 4;
            u32 x13 : 1;
            u32 x14 : 1;
            u32 x15 : 1;
            u32 x16 : 1;
            u32 x17 : 2;
            u32 x19 : 1;
            u32 x1A : 1;
            u32 x1B : 1;
            u32 x1C : 1;
            u32 x1D : 1;
            u32 x1E : 1;
            u32 x1F : 1;
        } flags;
        u32 word;
    };
};

struct DynamicBoneTable {
    /// @at{0} @sz{190}
    HSD_JObj* bones[100];
};

struct Item_DynamicBones {
    int flags;
    HSD_JObj* skeleton;
    unk_t unk_ptr;
    int count;
    Vec3 unk_vec;
};

struct ECB {
    f32 top;
    f32 bottom;
    f32 right;
    f32 left;
};

struct ItemAttr {
    u8 x0_is_heavy : 1;     // 0x0, bit 0x80, is heavy item (crate)
    u8 x0_78 : 4;           // unk, might be lock-on behavior? (Samus Missile)
    u8 x0_hold_kind : 3;    // defines hand hold behavior
    u8 x1_1 : 2;            // 0x1 0xB0
    u8 x1_3 : 1;            // 0x1 0x20
    u8 x1_4 : 1;            // 0x1 0x10
    u8 x1_5 : 1;            // 0x1 0x08
    u8 x1_67_cam_kind : 2;  // 0x1 0x06, is stored to 0xdcd
    u8 x1_8 : 1;            // 0x1 0x01    char flags3; //0x2
    u8 x3;                  // 0x3
    f32 x4_throw_speed_mul; // 0x4, speed multiplier at which this item is
                            // thrown at
    s32 x8;
    f32 xC_spin_speed;
    f32 x10_fall_speed;     // 0x10
    f32 x14_fall_speed_max; // 0x14
    f32 x18;
    f32 x1C_damage_mul; // collision related? referenced on taking damage @
                        // 80270f90
    ECB x20;
    Vec2 x30_unk;        // 0x34
    Vec2 x38_grab_range; // 0x38
    ECB x40;
    s32 x50;       // 0x50
    s32 x54;       // 0x54
    s32 x58;       // 0x58
    s32 x5c;       // 0x5c
    f32 x60_scale; // 0x60, does not affect hitboxes

    /// @at{64} @sz{4}
    /// @brief GFX to play on destroy.
    enum_t destroy_gfx;

    s32 x68; // 0x68
    s32 x6C; // 0x6c
    s32 x70; // 0x70
    s32 x74; // 0x74

    /// @at{78} @sz{4}
    /// @brief SFX that plays when this item is destroyed
    enum_t destroy_sfx;

    s32 x7C; // 0x7c
    s32 x80; // 0x80
    s32 x84; // 0x84
    s32 x88; // 0x88
    s32 x8C; // 0x8c
    s32 x90; // 0x90
    s32 x94; // 0x94
    s32 x98; // 0x98
    s32 x9C; // 0x9c
};

struct ItemDynamicsDesc {
    int x0_boneID;   // bone index;
    unk_t x4_params; // dynamics params;

    /// @at{8} @sz{4}
    /// @brief Number of children bones to make dynamic.
    int child_count;

    f32 xC;
    f32 x10;
    f32 x14;
};

/// @sz{8}
struct ItemDynamics {
    /// @at{0} @sz{4}
    int count;

    /// @at{4} @sz{4}
    ItemDynamicsDesc* dyn_descs;
};

/// @sz{10}
struct ItemStateDesc {
    /// @at{0} @sz{4}
    HSD_AnimJoint* x0_anim_joint;

    /// @at{0} @sz{4}
    HSD_MatAnimJoint* x4_matanim_joint;

    /// @at{0} @sz{4}
    unk_t x8_parameters;

    /// @at{0} @sz{4}
    unk_t xC_script;
};

struct ItemStateArray {
    struct ItemStateDesc x0_itemStateDesc[8];
};

/// @sz{10}
struct ItemModelDesc {
    /// @at{0} @sz{4}
    HSD_Joint* x0_joint;

    /// @at{4} @sz{4}
    u32 x4_bone_count;

    /// @at{8} @sz{4}
    s32 x8_bone_attach_id;

    /// @at{C} @sz{4}
    int xC_bit_field;
};

struct Article {
    ItemAttr* x0_common_attr;
    unk_t x4_specialAttributes;
    unk_t x8_hurtbox;
    ItemStateArray* xC_itemStates;
    ItemModelDesc* x10_modelDesc;
    ItemDynamics* x14_dynamics;
};

struct itHit {
    bool x0_toggle; // Toggles hitbox on/off.
    s32 x4_unk;
    s32 x8_damage;        // Projected damage
    f32 xC_damage_staled; // Staled damage, actually applied
    Vec3 x10_offset;
    f32 x1C_hitbox_size;
    s32 x20_angle;
    s32 x24_knockback_growth;
    s32 x28_wdsk; // Weight-Dependent Set Knockback
    s32 x2C_base_knockback;
    s32 x30_element;       // Normal, fire, electric, etc.
    s32 x34_shield_damage; // If hitbox damage + shield damage is less than 0
                           // (negative), this will effectively restore shield
                           // health
    s32 x38_SFX_severity;  // 0x38. hurtbox interaction. 0 = none, 1 = grounded,
                           // 2 = aerial, 3 = both // What
    s32 x3C_SFX_kind;
    UnkFlagStruct x40_flags; // 0x20 -> check against aerial fighters; 0x10 ->
                             // check against grounded fighters
    UnkFlagStruct x41_flags; // 0x8/0x4/0x2 = timed rehit on
                             // item/fighter/shield; 0x1 = can reflect
    UnkFlagStruct x42_flags; // 0x80 = can absorb; 0x20 = hit only fighters
                             // facing the item; 0x10 = can shield; 0x8 = ignore
                             // reflect/absorb bubbles(?); 0x4 = ignore
                             // hurtboxes; 0x2 = ignore ungrabbable hurtboxes
    UnkFlagStruct
        x43_flags; // 0x80 = interact with items only; 0x20 = interact with all?
    s32 x44;
    HSD_JObj* x48_jobj;
    Vec3 pos;
    Vec3 x58_posPrev;
    Vec3 x64_posColl;      // 0x64   position of hurt collision
    f32 x70_coll_distance; // 0x70   Distance From Collding HurtCapsule (Used
                           // for phantom hit collision calculation)
    HitVictim x74_tipLog[12];
    HitVictim xD4_damageLog[12];
    s32 x134;
    s32 x138;
};

struct Item {
    void* x0;

    /// @at{4} @sz{4}
    HSD_GObj* entity;

    s32 x8;

    /// @at{C} @sz{4}
    enum_t spawn_kind;

    /// @at{10} @sz{4}
    ItemKind kind;

    /// @at{14} @sz{4}
    enum_t hold_kind;

    s32 x18;
    s32 x1C;

    /// @at{20} @sz1
    u8 x20_team_id;

    u8 x21;
    u8 x22;
    u8 x23;

    /// @at{24} @sz{4}
    enum_t asid;

    /// @at{28} @sz{4}
    enum_t anim_id;

    /// @at{2C} @sz{4}
    f32 facing_dir;

    /// @at{30} @sz{4}
    f32 init_facing_dir;

    /// @at{34} @sz{4}
    f32 spin_spd;

    /// @at{38} @sz{4}
    f32 scl;

    /// @at{3C} @sz{4}
    f32 x3C;

    /// @at{40} @sz{C}
    Vec3 x40_vel;

    /// @at{4C} @sz{C}
    Vec3 pos;

    /// @at{58} @sz{C}
    Vec3 x58_vec_unk;

    /// @at{64} @sz{C}
    Vec3 x64_vec_unk2;

    /// @at{70} @sz{C}
    Vec3 x70_nudge;

    u8 padding_x7C[0xB8 - 0x7C];

    ItemLogicTable* xB8_itemLogicTable; // Global item callbacks

    ItemStateContainer* xBC_itemStateContainer;
    GroundOrAir ground_or_air;
    Article* xC4_article_data;
    HSD_Joint* xC8_joint;
    ItemAttr* xCC_item_attr;
    ItemStateDesc* xD0_itemStateDesc;
    Item_DynamicBones xD4_dynamicBones[24];
    s32 x374_dynamicBonesNum;
    CollData x378_itemColl;

    /// @at{518} @sz{4}
    /// @brief Item's current owner
    HSD_GObj* owner;

    s32 x51C;
    CameraBox* x520_cameraBox; // CameraBox
    f32 x524;
    s32 x528;
    void* x52C_item_script; // Script parse?
    u32 x530;
    u32 x534;
    u32 x538;
    u32 x53C;
    u32 x540;
    u32 x544;
    ColorOverlay x548_colorOverlay;
    u8 x5C8;
    u8 x5C9;
    u8 x5CA;
    u8 x5CB;
    f32 x5CC_currentAnimFrame;
    f32 x5D0_animFrameSpeed;
    itHit x5D4_hitboxes[4];
    s32 xAC4_ignoreItemID; // Cannot hit items with this index?
    s32 xAC8_hurtboxNum;   // Number of hurtboxes this item has
    HurtCapsule xACC_itemHurtbox[2];
    u32 xB64;
    u8 xB68;
    u8 xB69;
    u8 xB6A;
    u8 xB6B;
    u32 xB6C;
    u32 xB70;
    u32 xB74;
    u32 xB78;
    u32 xB7C;
    u32 xB80;
    u32 xB84;
    u32 xB88;
    u32 xB8C;
    u32 xB90;
    u32 xB94;
    u32 xB98;
    u32 xB9C;
    u32 xBA0;
    u32 xBA4;
    u32 xBA8;
    u32 xBAC;
    u32 xBB0;
    u32 xBB4;
    u32 xBB8;
    DynamicBoneTable* xBBC_dynamicBoneTable;
    unk_t xBC0;
    u8 xBC4;
    u8 xBC5;
    u8 xBC6;
    u8 xBC7;
    u32 xBC8;
    Vec2 xBCC_unk;
    Vec2 xBD4_grabRange;
    ECB xBDC;
    ECB xBEC;
    ECB xBFC;
    ECB xC0C;
    ECB xC1C;
    u32 xC2C;
    u32 xC30;

    s32 xC34_damageDealt; // Rounded down
    s32 xC38;             // 0xc38
    f32 xC3C;             // 0xc3c
    f32 xC40;             // 0xc40
    f32 xC44;             // 0xc44
    s32 xC48;             // 0xc48
    s32 xC4C; // Something to do with damage. 0x80077464 checks this against
              // reflectors' maximum damage threshold
    s32 xC50; // 0xc50
    f32 xC54; // 0xc54
    f32 xC58; // 0xc58
    s32 xC5C; // 0xc5c
    s32 xC60; // 0xc60
    HSD_GObj* xC64_reflectGObj; // GObj that reflected this item?
    f32 xC68;                   // 0xc68
    f32 xC6C;                   // 0xc6c
    f32 xC70;                   // 0xc70
    s32 xC74;
    Vec2 xC78;                   // 0xc74
    Vec2 xC80;                   // 0xc7c
    s32 xC88;                    // 0xc88
    s16 xC8C;                    // 0xc8c
    HSD_GObj* xC90_absorbGObj;   // 0xc90
    s32 xC94;                    // 0xc94
    s32 xC98;                    // 0xc98
    s32 xC9C;                    // Total damage taken?
    s32 xCA0;                    // Last amount of damage taken?
    s32 xCA4;                    // 0xca4
    s32 xCA8;                    // 0xca8, hitlag related
    s32 xCAC_angle;              // 0xcac
    s32 xCB0_source_ply;         // 0xcb0, damage source ply number
    s32 xCB4;                    // 0xcb4
    f32 xCB8_outDamageDirection; // 0xcb8, updated @ 80078184
    f32 xCBC_hitlagFrames;       // 0xcbc, hitlag frames remaining
    f32 xCC0;                    // 0xcc0
    s32 xCC4;                    // 0xcc4
    f32 xCC8_knockback;          // 0xcc8
    f32 xCCC_incDamageDirection; // Direction from which damage was applied?
    f32 xCD0;                    // 0xcd0
    f32 xCD4;                    // 0xcd4
    f32 xCD8;                    // 0xcd8
    f32 xCDC;                    // 0xcdc
    f32 xCE0;                    // 0xce0
    f32 xCE4;                    // 0xce4
    f32 xCE8;                    // 0xce8
    HSD_GObj* xCEC_fighterGObj;  // 0xcec
    HSD_GObj* xCF0_itemGObj;     // 0xcf0, is a fp GObj, but is the owner of the

    HSD_GObj* xCF4_fighterGObjUnk;

    /// @at{CF8} @sz{4}
    /// @brief The entity that was detected by this item's inert hitbox.
    HSD_GObj* toucher;

    u32 xCFC;

    /// @at{D00} @sz{4}
    /// @brief The entity that got grabbed by this item.
    HSD_GObj* grab_victim;

    /// @at{D04} @sz{4}
    /// @brief The entity that collided with this item's hitbox?
    HSD_GObj* atk_victim;

    u8 xD08;
    u8 xD09;
    u8 xD0A;
    u8 xD0B;

    /// @at{D0C} @sz{4}
    enum_t xD0C;

    s32 xD10;

    /// @at{D14} @sz{4}
    HSD_GObjPredicate animated;

    /// @at{D18} @sz{4}
    HSD_GObjEvent physics_updated;

    /// @at{D1C} @sz{4}
    HSD_GObjPredicate collided;

    /// @at{D20} @sz{4}
    /// @todo What does this mean?
    HSD_GObjEvent on_accessory;

    /// @at{D24} @sz{4}
    /// @brief Runs when an entity is detected by this item's inert hibox.
    HSD_GObjPredicate touched;

    /// @at{D28} @sz{4}
    /// @brief Runs after applying hitlag in damage.
    /// @todo What function is @c 8026a62c?
    HSD_GObjEvent entered_hitlag;

    // 0xd2c, runs after exiting hitlag in hitlag, update proc 8026a200
    HSD_GObjEvent exited_hitlag;

    /// @at{D28} @sz{4}
    /// @brief Runs when the item is jumped on.
    /// @todo What function is @c 80269bac?
    HSD_GObjPredicate jumped_on;

    /// @at{D34} @sz{4}
    /// @brief When grabbing a fighter, run this function on self.
    HSD_GObjEvent grab_dealt;

    /** @at{D38} @sz{4}
     * @brief When grabbing a fighter, run this function on them.
     *
     * @p gobj0 - The victim of the grab. \n
     * @p gobj1 - This item's entity.
     */
    HSD_GObjInteraction grabbed_for_victim;

    f32 xD3C_spinSpeed;
    f32 xD40;
    f32 xD44_lifeTimer;
    f32 xD48_halfLifeTimer; // Not radioactive, just the item's original
                            // lifetime halved
    int xD4C;
    s32 xD50_landNum;  // Number of times this item has landed
    s32 xD54_throwNum; // Number of times this item has been thrown
    s32 xD58;
    s32 xD5C;

    /// @at{D60} @sz{4}
    enum_t destroy_type;

    /// @at{D64} @sz{4}
    enum_t sfx_unk1;

    /// @at{D68} @sz{4}
    enum_t sfx_unk2;

    s32 xD6C;
    s32 xD70;
    s32 xD74;
    s32 xD78;

    /// @at{D7C} @sz{4}
    /// @brief SFX that plays when this item is destroyed
    enum_t destroy_sfx;

    s32 xD80;
    s32 xD84;
    s32 xD88_attackID; // ???
    s16 xD8C_attackInstance;
    s16 xD8E;
    s32 xD90;
    Vec2 xD94;
    Vec2 xD9C;
    union {
        UnkFlagStruct xDA4_flag;
        u32 xDA4_word;
    };
    union {
        UnkFlagStruct xDA8_flag;
        u16 xDA8_short;
    };
    union {
        UnkFlagStruct xDAA_flag; // Develop mode stuff?
        u8 xDAA_byte;
    };
    u32 xDAC_itcmd_var0;
    u32 xDB0_itcmd_var1;
    u32 xDB4_itcmd_var2;
    u32 xDB8_itcmd_var3;
    u32 xDBC_itcmd_var4;
    u32 xDC0;
    u32 xDC4;
    flag32 xDC8_word;
    struct {
        u8 b0 : 1;
        u8 b1 : 1;
        u8 b2 : 1;
        u8 b3 : 1;
        u8 b4567 : 4;
    } xDCC_flag;
    CameraBoxFlags xDCD_flag;
    UnkFlagStruct xDCE_flag;
    UnkFlagStruct xDCF_flag;
    UnkFlagStruct xDD0_flag;
    UnkFlagStruct xDD1_flag;
    UnkFlagStruct xDD2_flag;
    UnkFlagStruct xDD3_flag;
    union {
        BobOmbVars BobOmb;
        HeartContainerVars HeartContainer;
        MaximTomatoVars MaximTomato;
        FoodVars Food;
        EggVars Egg;
        WhispyAppleVars WhispyApple;
        PKFlashVars PKFlash;
        PKFlashExplVars PKFlashExpl;
        PKThunderVars PKThunderVars;
        u8 padding[0xFCC - 0xDD4];
    } xDD4_itemVar;
};

typedef struct ItemLink // user_data struct of GObj class 7
{
    struct ItemLink* prev;
    struct ItemLink* next;
    Vec3 x8_vel;
    Vec3 pos;
    Vec3 x20_vec; // Previous position?
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 flag2 : 1;
    u8 flag3 : 1;
    u8 flag4 : 1;
    u8 flag5 : 1;
    u8 flag6 : 1;
    u8 flag7 : 1;
    CollData x30_collData;
    HSD_GObj* x1D0_GObj;
    HSD_GObj* x1D4_GObjLinkNext;

} ItemLink;

struct sdata_ItemGXLink {
    void (*x0_renderFunc)(HSD_GObj*, s32);
};

struct r13_ItemTable {
    s32 filler;
    Article* x0_article[0x9F];
};

typedef struct UnkCommonArticlePtr {
    u32 x0;
} UnkCommonArticlePtr;

typedef struct UnkCommonArticleStruct {
    s32 x0;
    UnkCommonArticlePtr* x4;
} UnkCommonArticleStruct;

typedef struct CommonItemArticles {
    void* article_ptr[0x10];
    UnkCommonArticleStruct* x40;
} CommonItemArticles;

typedef struct UnkItemArticles {
    void* unkptr UNK_SIZE_ARRAY;
} UnkItemArticles;

typedef struct UnkItemArticles2 {
    void* unkptr UNK_SIZE_ARRAY;
} UnkItemArticles2;

struct UnkItemArticles3 {
    void* unkptr[1];
};

struct BobOmbRain {
    enum_t x0;
    HSD_JObj* x4;
    Vec3 x8_vec;
    enum_t x14;
    s32 x18;
    UnkFlagStruct x1C;
};

struct SpawnItem {
    HSD_GObj* x0_parent_gobj; // Primary owner of the item; usually a fp GObj
    HSD_GObj*
        x4_parent_gobj2; // Secondary owner GObj of the item; e.g. Ness' PK Fire
                         // Pillar has this set to PK Fire Spark's item GObj
    ItemKind kind;       // 0x8, ID of the item to spawn

    /// @at{C} @sz{4}
    /// @brief Defines the behavior of the item, such as thrown and pickup.
    /// @todo 0 = capsule.
    enum_t hold_kind;

    s32 x10;

    /// @at{14} @sz{C}
    Vec3 pos;

    /// @at{20} @sz{C}
    Vec3 prev_pos;

    /// @at{2C} @sz{C}
    Vec3 vel;

    /// @at{38} @sz{4}
    f32 facing_dir;

    s16 x3C_damage;
    s16 x3E;
    s32 x40;                // 0x1 = correct initial position
    UnkFlagStruct x44_flag; // 0x80 = perform initial collision check
    UnkFlagStruct x45_flag;
    UnkFlagStruct x46_flag;
    UnkFlagStruct x47_flag;
    GroundOrAir x48_ground_or_air; // 0x0 = stationary, 0x1 = air (?)
};

struct ItemModStruct {
    s32 x0_unk;
};

struct ItemCommonData {
    u32 x0;
    u32 x4;
    u32 x8;
    u32 xC;
    u32 x10;
    u32 x14;
    u32 x18;
    u32 x1C;
    u32 x20;
    u32 x24;
    u32 x28;
    u32 x2C;
    u32 x30;
    u32 x34;
    s32 x38_float;
    s32 x3C_float;
    s32 x40_float;
    s32 x44_float;
    s32 x48_float;
    s32 x4C_float;
    s32 x50_float;
    s32 x54_float;
    s32 x58_float;
    s32 x5C_float;
    s32 x60_float;
    s32 x64_float;
    s32 x68_float;
    f32 x6C_float;
    f32 x70_float;
    f32 x74_float;
    f32 x78_float;
    f32 x7C_float;
    f32 x80_float[13];
    s32 xB4;
    f32 xB8;
    f32 xBC;
    f32 xC0;
    f32 xC4;
    f32 xC8;
    f32 xCC;
    f32 xD0;
    f32 xD4;
    uint xD8;
    s32 xDC;
    u8 filler[0x148 - 0xE0];
    s32 x148;
    f32 x14C;
    f32 x150;
    f32 x154;
    f32 x158;
    f32 x15C;
};

struct Item_r13_Data {
    ItemCommonData* item_common;
    void** common_items;
    void** adventure_items;
    void** pokeball_items;
    s32 x10;
    s32 x14;
};

struct HSD_ObjAllocUnk2 {
    u8 x0_filler[0x148];
    u32 x148;
    u32 x14C;
    u32 x150;
    UnkFlagStruct x154;
};

struct x1C_struct {
    s32 x1C;
};

struct HSD_ObjAllocUnk {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
    s32 x14;
    u32 x18;
    s32 x1C;
    s32 x20;
    s32 x24;
    s32 x28;
    u32 x2C;
    u32 x30;
    s32 x34;
    s32 x38;
    s32 x3C;
    s32 x40;
    s32 x44;
    s32 x48;
    s32 x4C;
    s32 x50;
    s32 x54;
    s32 x58;
    s32 x5C;
    u32 x60;
    u32 x64;
};

struct HSD_ObjAllocUnk4 {
    u8 x0;
    u8 x1;
    u8 x2;
    u8 x3;
    u8 x4;
    u16 x6;
    u16 x8;
    u16 xA;
    u16 xC;
    u32 x10;
    u8 pad[0x20 - 0x10];
};

struct HSD_ObjAllocUnk5 {
    u8 x0;
    u32 x4;
    u16 x8;
    u16 xA;
    u16 xC;
};

struct HSD_ObjAllocUnk6 {
    u8 x0;
    u32 x4;
    u16 x8;
    u16 xA;
    u16 xC;
};
#endif
