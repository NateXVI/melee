#include <melee/ft/fighter.h>

#include <dolphin/mtx/vec.h>
#include <dolphin/os/os.h>
#include <melee/cm/camera.h>
#include <melee/db/db_unknown_001.h>
#include <melee/ef/efasync.h>
#include <melee/ft/chara/ftCrazyHand/ftcrazyhand.h>
#include <melee/ft/chara/ftKirby/ftkirby.h>
#include <melee/ft/chara/ftMasterHand/ftMasterHand_02.h>
#include <melee/ft/code_8007C114.h>
#include <melee/ft/code_8007C630.h>
#include <melee/ft/code_80081938.h>
#include <melee/ft/code_80081B38.h>
#include <melee/ft/ft_unknown_005.h>
#include <melee/ft/ft_unknown_006.h>
#include <melee/ft/ftaction.h>
#include <melee/ft/ftanim.h>
#include <melee/ft/ftcamera.h>
#include <melee/ft/ftcoll.h>
#include <melee/ft/ftcommon.h>
#include <melee/ft/ftdata.h>
#include <melee/ft/ftdrawcommon.h>
#include <melee/ft/ftlib.h>
#include <melee/ft/ftparts.h>
#include <melee/gm/code_801601C4.h>
#include <melee/if/ifmagnify.h>
#include <melee/it/code_80266F3C.h>
#include <melee/it/item.h>
#include <melee/it/item2.h>
#include <melee/lb/lbarchive.h>
#include <melee/lb/lbmthp.h>
#include <melee/lb/lbshadow.h>
#include <melee/lb/lbunknown_001.h>
#include <melee/lb/lbunknown_002.h>
#include <melee/mp/mpcoll.h>
#include <melee/mp/mplib.h>
#include <melee/pl/pl_unknown_001.h>
#include <melee/text_2.h>
#include <melee/text_4.h>
#include <MSL/trigf.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjobject.h>
#include <sysdolphin/baselib/lobj.h>
#include <sysdolphin/baselib/mtx.h>

/// @todo Move elsewhere.
#define HALF_PI (1.5707963267948966)

// external vars from asm/melee/ft/ft_unknown_005.s
typedef void (*ft_callback)(HSD_GObj* gobj);
typedef void (*fn_ptr_t)(void);

extern ft_callback
    ft_OnLoad[FTKIND_MAX]; // One load  callback for every character.
extern ft_callback
    ft_OnDeath[FTKIND_MAX]; // One death callback for every character.
extern ft_callback ft_OnAbsorb[FTKIND_MAX];
extern ft_callback lbl_803C1DB4[FTKIND_MAX]; // probably ft_OnSomething
extern ft_callback ft_OnUserDataRemove[FTKIND_MAX];

extern fn_ptr_t lbl_803C10D0[FTKIND_MAX];

extern struct UnkCostumeList CostumeListsForeachCharacter[FTKIND_MAX];

extern ftData* gFtDataList[FTKIND_MAX];
extern ActionState ActionStateList[341];
extern ActionState* ActionStateTableByCharacter[FTKIND_MAX];

extern s8 lbl_803C26FC[FTKIND_MAX];

extern HSD_PadStatus HSD_PadRumbleData[4];

extern StageInfo stage_info; // from asm/melee/text_2.s

extern u8 lbl_804D7849; // asm/sysdolphin/baselib/gobj.s

// ==== fighter.c variables ====
// =============================

const Vec3 lbl_803B7488 = { 0.0f, 0.0f, 0.0f };
const Vec3 vec3_803B7494 = { 0.0f, 0.0f, 0.0f };

HSD_ObjAllocData fighter_alloc_data;
HSD_ObjAllocData lbl_80458FFC;
HSD_ObjAllocData fighter_bones_alloc_data;
HSD_ObjAllocData lbl_80459054;
HSD_ObjAllocData lbl_80459080;
HSD_ObjAllocData lbl_804590AC;

// TODO: verify that this is really a spawn number counter, then rename this var
// globally
u32 lbl_804D64F8 = 0;
#define g_spawnNumCounter lbl_804D64F8

// the following seems to be an array, initialized in reverse in
unk_t lbl_804D64FC = NULL;
unk_t lbl_804D6500 = NULL;
unk_t lbl_804D6504 = NULL;
unk_t lbl_804D6508 = NULL;
unk_t lbl_804D650C = NULL;
unk_t lbl_804D6510 = NULL;
unk_t lbl_804D6514 = NULL;
unk_t lbl_804D6518 = NULL;
unk_t lbl_804D651C = NULL;
unk_t lbl_804D6520 = NULL;
unk_t lbl_804D6524 = NULL;
unk_t lbl_804D6528 = NULL;
unk_t lbl_804D652C = NULL;
unk_t lbl_804D6530 = NULL;
unk_t lbl_804D6534 = NULL;
unk_t lbl_804D6538 = NULL;
unk_t lbl_804D653C = NULL;
s32** lbl_804D6540 = NULL;
FighterPartsTable** ftPartsTable = NULL;
unk_t lbl_804D6548 = NULL;
unk_t lbl_804D654C = NULL;
unk_t lbl_804D6550 = NULL;
ftCommonData* p_ftCommonData;

void Fighter_800679B0(void)
{
    s32 i;

    /// @warning don't hardcode the allocation sizes
    HSD_ObjAllocInit(&fighter_alloc_data, sizeof(Fighter), /*align*/ 4);
    HSD_ObjAllocInit(&lbl_80458FFC, /*size*/ 0x424, /*align*/ 4);
    func_800852B0();
    Fighter_LoadCommonData();
    func_8008549C();
    func_8009F4A4();
    func_800C8064();
    func_800C8F6C();
    // @TODO: &fighter_alloc_data+2, +3, +4 are not defined in the fighter.s
    // data section, how does this work?
    HSD_ObjAllocInit(&fighter_bones_alloc_data, /*size*/ 0x8c0, /*align*/ 4);
    HSD_ObjAllocInit(&lbl_80459054, /*size*/ 0x1f0, /*align*/ 4);
    HSD_ObjAllocInit(&lbl_80459080, /*size*/ 0x80, /*align*/ 4);

    g_spawnNumCounter = 1;

    for (i = 0; i < FTKIND_MAX; i++) {
        if (lbl_803C10D0[i])
            lbl_803C10D0[i]();
    }
}

void Fighter_FirstInitialize_80067A84(void)
{
    Fighter_800679B0();
    HSD_ObjAllocInit(&lbl_804590AC, 0x8000, 0x20);
}

void Fighter_LoadCommonData(void)
{
    void** pData;
    func_80016C64("PlCo.dat", (unk_t) &pData, "ftLoadCommonData", 0);

    // copy 23 4-byte chunks from pData to p_ftCommonData in reverse order,
    // equivalent to this: for(i=0; i<23; i++)
    //   (&lbl_804D64FC)[23-1-i] = pData[i];
    // loop unrolling doesn't work (only up to 8 elements)
    p_ftCommonData = pData[0]; // p_ftCommonData
    lbl_804D6550 = pData[1];
    lbl_804D654C = pData[2];
    lbl_804D6548 = pData[3];
    ftPartsTable = pData[4];
    lbl_804D6540 = pData[5];
    lbl_804D653C = pData[6];
    lbl_804D6538 = pData[7];
    lbl_804D6534 = pData[8];
    lbl_804D6530 = pData[9];
    lbl_804D652C = pData[10];
    lbl_804D6528 = pData[11];
    lbl_804D6524 = pData[12];
    lbl_804D6520 = pData[13];
    lbl_804D651C = pData[14];
    lbl_804D6518 = pData[15];
    lbl_804D6514 = pData[16];
    lbl_804D6510 = pData[17];
    lbl_804D650C = pData[18];
    lbl_804D6508 = pData[19];
    lbl_804D6504 = pData[20];
    lbl_804D6500 = pData[21];
    lbl_804D64FC = pData[22];
}

void Fighter_UpdateModelScale(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    HSD_JObj* jobj = GET_JOBJ(fighter_gobj);
    Vec3 scale;
    f32 modelScale = Fighter_GetModelScale(fp);

    if (fp->x34_scale.z != 1.0f)
        scale.x = fp->x34_scale.z;
    else
        scale.x = modelScale;

    scale.y = modelScale;
    scale.z = modelScale;

    HSD_JObjSetScale(jobj, &scale);
}

void Fighter_UnkInitReset_80067C98(Fighter* fp)
{
    Vec3 player_coords;
    f32 x, y, z;

    fp->x8_spawnNum = Fighter_NewSpawn_80068E40();
    Player_LoadPlayerCoords(fp->xC_playerID, &player_coords);
    fp->facing_dir = Player_GetFacingDirection(fp->xC_playerID);

    player_coords.x = fp->facing_dir * func_800804EC(fp) + player_coords.x;
    x = player_coords.x;
    fp->xB0_pos.x = x;
    fp->xBC_prevPos.x = x;

    y = player_coords.y;
    fp->xB0_pos.y = y;
    fp->xBC_prevPos.y = y;

    z = player_coords.z;
    fp->xB0_pos.z = z;
    fp->xBC_prevPos.z = z;

    fp->x30_facingDirectionRepeated = fp->facing_dir;
    fp->x34_scale.y = fp->x34_scale.x;

    fp->x2220_flag.bits.b5 = 0;
    fp->x2220_flag.bits.b6 = 0;

    fp->x200C = 0;
    fp->x2010 = 0;
    fp->x2008 = 0;

    fp->x2219_flag.bits.b1 = 0;
    fp->x2219_flag.bits.b2 = 0;
    fp->x2219_flag.bits.b3 = 0;
    fp->x2219_flag.bits.b4 = 0;
    fp->x221A_flag.bits.b5 = 0;
    fp->x221A_flag.bits.b6 = 0;
    fp->x221D_flag.bits.b2 = 0;
    fp->x221E_flag.bits.b7 = 0;
    fp->x2220_flag.bits.b7 = 0;
    fp->x2221_flag.bits.b4 = 0;
    fp->x2221_flag.bits.b5 = 0;
    fp->x2221_flag.bits.b6 = 1;
    fp->x2221_flag.bits.b7 = 0;

    fp->x61D = 255;

    fp->xC8_pos_delta.z = 0.0f;
    fp->xC8_pos_delta.y = 0.0f;
    fp->xC8_pos_delta.x = 0.0f;
    fp->x894_currentAnimFrame = 0.0f;
    fp->x898_unk = 0.0f;

    fp->x89C_frameSpeedMul = 1.0f;
    fp->x8A0_unk = 1.0f;
    fp->dmg.x1850_forceApplied = 0.0f;
    fp->dmg.x18A4_knockbackMagnitude = 0.0f;
    fp->dmg.x18A8 = 0.0f;
    fp->dmg.x18ac_time_since_hit = -1;
    fp->dmg.x18B0 = 0.0f;
    fp->dmg.x18B4_armor = 0.0f;
    fp->x1828 = 0;

    fp->x221C_flag.bits.b6 = 0;

    fp->dmg.x18a0 = 0.0f;
    fp->x1968_jumpsUsed = 0;
    fp->x1969_walljumpUsed = 0;
    fp->x196C_hitlag_mult = 0.0f;
    fp->x2064_ledgeCooldown = 0;

    fp->dmg.x1830_percent = Player_GetDamage(fp->xC_playerID);

    fp->dmg.x1838_percentTemp = 0.0f;

    fp->dmg.x183C_applied = 0;
    fp->dmg.x18C0 = 0;

    fp->dmg.x18c4_source_ply = 6;
    fp->dmg.x18C8 = -1;
    fp->dmg.x18F0 = 0;
    fp->dmg.x18CC = 0;
    fp->dmg.x18D0 = -10;

    fp->x221F_flag.bits.b5 = 0;
    fp->x2221_flag.bits.b1 = 0;

    fp->dmg.x18F4 = 0;
    fp->dmg.x18F8 = 1;
    fp->dmg.x18fa_model_shift_frames = 0;
    fp->dmg.x18FD = 0;
    fp->dmg.x18FC = 0;
    fp->dmg.x1834 = 0.0f;

    fp->x2222_flag.bits.b2 = 0;

    fp->dmg.x1840 = 0;

    fp->x2219_flag.bits.b5 = 0;
    fp->x2219_flag.bits.b6 = 0;
    fp->x2219_flag.bits.b7 = 0;
    fp->x221A_flag.bits.b0 = 0;
    fp->x221A_flag.bits.b1 = 0;

    fp->dmg.x1954 = 0.0f;
    fp->dmg.x1958 = 0.0f;

    fp->x221A_flag.bits.b2 = 0;

    fp->dmg.x195c_hitlag_frames = 0.0f;

    fp->x221A_flag.bits.b3 = 0;
    fp->x1960_vibrateMult = 1.0f;
    fp->x1964 = 0.0f;
    fp->dmg.x189C_unk_num_frames = 0.0f;

    fp->x2220_flag.bits.b3 = 0;
    fp->x2220_flag.bits.b4 = 0;

    fp->dmg.x1914 = 0;
    fp->dmg.x1918 = 0;
    fp->dmg.x191C = 0.0f;
    fp->dmg.x1924 = 0;
    fp->dmg.x1928 = 0.0f;

    fp->x2223_flag.bits.b5 = 0;

    fp->dmg.x1950 = 0;
    fp->dmg.x1948 = 0;

    fp->x2223_flag.bits.b4 = 0;

    fp->xF8_playerNudgeVel.y = 0.0f;
    fp->xF8_playerNudgeVel.x = 0.0f;
    fp->x100 = -1.0f;

    fp->x2222_flag.bits.b7 = 0;
    fp->x2223_flag.bits.b0 = 0;
    fp->x221A_flag.bits.b4 = 0;
    fp->x2219_flag.bits.b0 = 0;

    fp->x20A0_accessory = 0;
    fp->x2210_ThrowFlags.flags = 0;
    fp->x2214 = 0.0f;
    fp->x1974_heldItem = 0;
    fp->x1978 = 0;

    fp->x221E_flag.bits.b3 = 1;

    fp->x1984_heldItemSpec = 0;
    fp->x1988 = 0;
    fp->x198C = 0;

    fp->x2221_flag.bits.b0 = 0;

    fp->x1990 = 0;
    fp->x1994 = 0;

    fp->x221D_flag.bits.b6 = 0;
    fp->x221B_b5 = 0;

    fp->x1A58_interactedFighter = 0;
    fp->x1A5C = 0;

    fp->x221B_b6 = 0;

    fp->x1A60 = 0;
    fp->x1A64 = 0;

    fp->x221B_b7 = 0;
    fp->x221C_flag.bits.b0 = 0;

    fp->x1064_thrownHitbox.owner = NULL;
    fp->x221C_u16_y = 0;
    fp->x20AC = NULL;
    fp->x221C_flag.bits.b5 = 0;

    fp->x2150 = fp->x2154 = fp->x2158 = fp->x215C = fp->x2160 = fp->x2144 =
        fp->x2148 = fp->x214C = -1;

    fp->x2168 = 0;
    fp->x2164 = 0;
    fp->x208C = 0;
    fp->x2090 = 0;
    fp->x2098 = 0;
    fp->x2092 = 0;
    fp->x2094 = 0;
    fp->x1998_shieldHealth = p_ftCommonData->x260_startShieldHealth;

    fp->x221A_flag.bits.b7 = 0;
    fp->x221B_b0 = 0;
    fp->x221B_b1 = 0;
    fp->x221B_b3 = 0;
    fp->x221B_b4 = 0;
    fp->x221C_flag.bits.b3 = 0;
    fp->x221C_flag.bits.b1 = 0;
    fp->x221C_flag.bits.b2 = 0;

    fp->x19A0_shieldDamageTaken = 0;
    fp->x19A4 = 0;
    fp->x199C_shieldLightshieldAmt = 0.0f;
    fp->x19A8 = 0;
    fp->x19B4_shieldUnk = 0.0f;
    fp->x19B8_shieldUnk = 0.0f;
    fp->x19BC_shieldDamageTaken3 = 6;

    fp->x221F_flag.bits.b6 = 0;
    fp->x2218_flag.bits.b3 = 0;
    fp->x2218_flag.bits.b4 = 0;
    fp->ReflectAttr.x1A3C_damageOver = 0;
    fp->ReflectAttr.x1A2C_reflectHitDirection = 0.0f;
    fp->x2218_flag.bits.b6 = 0;
    fp->x2218_flag.bits.b7 = 0;

    fp->AbsorbAttr.x1A40_absorbHitDirection = 0.0f;

    fp->AbsorbAttr.x1A44_damageTaken = 0;
    fp->AbsorbAttr.x1A48_hitsTaken = 0;

    fp->x68C_transNPos.z = 0.0f;
    fp->x68C_transNPos.y = 0.0f;
    fp->x68C_transNPos.x = 0.0f;
    fp->x6A4_transNOffset.z = 0.0f;
    fp->x6A4_transNOffset.y = 0.0f;
    fp->x6A4_transNOffset.x = 0.0f;
    fp->x6BC_inputStickangle = 0.0f;

    fp->x6C0.z = 0.0f;
    fp->x6C0.y = 0.0f;
    fp->x6C0.x = 0.0f;

    fp->x6D8.z = 0.0f;
    fp->x6D8.y = 0.0f;
    fp->x6D8.x = 0.0f;

    fp->x209C = 0;
    fp->x2224_flag.bits.b1 = 0;
    fp->cb.x21E4_callback_OnDeath2 = 0;
    fp->x2100 = -1;
    fp->x2101_bits_0to6 = 0;
    fp->cb.x21B4_callback_Accessory2 = 0;
    fp->cb.x21B8_callback_Accessory3 = 0;
    fp->cb.x21E0_callback_OnDeath = 0;
    fp->cb.x21E8_callback_OnDeath3 = 0;
    fp->x221E_flag.bits.b4 = 1;
    fp->x197C = 0;
    fp->x2223_flag.bits.b7 = 0;
    fp->x2028 = 0;
    fp->x202C = 0;

    func_800C88A0(fp);

    fp->x2227_flag.bits.b3 = 0;
    fp->x2034 = 0;
    fp->x2038 = 0;
    fp->x1980 = 0;

    fp->x2224_flag.bits.b2 = fp->x2224_flag.bits.b3 = 0;

    fp->x2224_flag.bits.b4 = 0;
    fp->x2108 = 0;
    fp->x2224_flag.bits.b5 = 0;
    fp->x1A53 = 0;
    fp->x1A52 = 0;
    fp->x210C_walljumpInputTimer = 254;
    fp->dmg.x1910 = 0;
    fp->x2225_b0 = 0;
    fp->x2225_b2 = 1;
    fp->x2225_b4 = 0;
    func_800DEEA8(fp->x0_fighter);
    fp->dmg.x18BC = 0.0f;
    fp->dmg.x18B8 = 0.0f;
    fp->x2226_flag.bits.b2 = 0;
    fp->x2170 = 0.0f;
    fp->x2225_b6 = fp->x2225_b5;
    fp->dmg.x1908 = -1;
    fp->dmg.x190C = 0;
    fp->x2227_flag.bits.b4 = 0;
    fp->x2114_SmashAttr.x2138_smashSinceHitbox = -1.0f;
    fp->x213C = -1;
    fp->x2227_flag.bits.b5 = 0;
    fp->x2228_flag.bits.b1 = 0;
    fp->x2140 = 0.0f;
    fp->x2227_flag.bits.b6 = 0;
    fp->x2180 = 6;
    fp->x2229_b4 = 1;
}

void Fighter_UnkProcessDeath_80068354(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    Fighter_UnkInitReset_80067C98(fp);
    HSD_JObjSetTranslate(GET_JOBJ(fighter_gobj), &fp->xB0_pos);

    func_800D105C(fighter_gobj);
    func_800C09B4(fighter_gobj);
    func_8007E2FC(fighter_gobj);
    func_80088A50(fp);
    func_800890BC(fp);
    func_800892D4(fp);
    func_80081B38(fighter_gobj);
    func_80081938(fighter_gobj);

    if (fp->x2114_SmashAttr.x2135 == -1) {
        if (func_80082A68(fighter_gobj) && !fp->x2229_b6)
            func_8007D6A4(fp);
        else
            func_8007D5D4(fp);
    } else
        func_8007D5D4(fp);
    func_80076064(fp);

    HSD_JObjSetTranslate(GET_JOBJ(fighter_gobj), &fp->xB0_pos);
    Fighter_UnkApplyTransformation_8006C0F0(fighter_gobj);
    Fighter_UpdateModelScale(fighter_gobj);

    func_800BFFAC(fp);
    func_800C0074(fp);
    func_800C8438(fighter_gobj);
    func_800C89A0(fighter_gobj);
    func_800C8FC4(fighter_gobj);
    func_8007AFF8(fighter_gobj);
    func_8007B0C0(fighter_gobj, 0);

    if (ft_OnDeath[fp->x4_fighterKind])
        ft_OnDeath[fp->x4_fighterKind](fighter_gobj);

    func_800A101C(fp, Player_GetCpuType(fp->xC_playerID),
                  Player_GetCpuLevel(fp->xC_playerID), 0);

    func_80067688(&fp->x60C);
    func_8007C17C(fighter_gobj);
    func_8007C630(fighter_gobj);
}

void Fighter_UnkUpdateCostumeJoint_800686E4(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    HSD_JObj* jobj;

    fp->x108_costume_joint = CostumeListsForeachCharacter[fp->x4_fighterKind]
                                 .costume_list[fp->x619_costume_id]
                                 .joint;
    func_80074148();
    jobj = HSD_JObjLoadJoint(fp->x108_costume_joint);
    func_80074170();
    func_80073758(jobj);

    func_80390A70(fighter_gobj, lbl_804D7849, jobj);
}

void Fighter_UnkUpdateVecFromBones_8006876C(Fighter* fp)
{
    Vec3 vec;
    Vec3 vec2;
    HSD_JObj* jobj = fp->x5E8_fighterBones[func_8007500C(fp, 2)].x0_jobj;

    HSD_JObjGetTranslation(jobj, &vec);

    fp->x1A6C = (vec.y / 8.55f);

    func_8000B1CC(jobj, 0, &vec);
    func_8000B1CC(fp->x5E8_fighterBones[func_8007500C(fp, 1)].x0_jobj, 0,
                  &vec2);
    fp->x1A70.x = vec2.x - vec.x;
    fp->x1A70.y = vec2.y - vec.y;
    fp->x1A70.z = vec2.z - vec.z;
}

void Fighter_ResetInputData_80068854(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    fp->input.x620_lstick_x = fp->input.x624_lstick_y =
        fp->input.x628_lstick_x2 = fp->input.x62C_lstick_y2 = 0.0f;

    fp->input.x644_lsubStick_y2 = 0.0f;
    fp->input.x640_lsubStick_x2 = 0.0f;
    fp->input.x63C_lsubStick_y = 0.0f;
    fp->input.x638_lsubStick_x = 0.0f;

    fp->input.x654 = 0.0f;
    fp->input.x650 = 0.0f;

    fp->input.x660 = 0;
    fp->input.x66C = 0;
    fp->input.x668 = 0;
    fp->input.x65C_heldInputs = 0;

    fp->x672_input_timer_counter = 0xFE;
    fp->x671_timer_lstick_tilt_y = 0xFE;
    fp->x670_timer_lstick_tilt_x = 0xFE;

    fp->x675 = 0xFE;
    fp->x674 = 0xFE;
    fp->x673 = 0xFE;

    fp->x678 = 0xFE;
    fp->x677_y = 0xFE;
    fp->x676_x = 0xFE;

    fp->x67B = 0xFE;
    fp->x67A_y = 0xFE;
    fp->x679_x = 0xFE;

    fp->x68B = 0xFF;
    fp->x68A = 0xFF;
    fp->x689 = 0xFF;
    fp->x688 = 0xFF;
    fp->x687 = 0xFF;
    fp->x686 = 0xFF;
    fp->x685 = 0xFF;
    fp->x684 = 0xFF;
    fp->x683 = 0xFF;

    fp->x680 = 0xFF;
    fp->x67F = 0xFF;

    fp->x682 = 0xFF;
    fp->x681 = 0xFF;

    fp->x67E = 0xFF;
    fp->x67D = 0xFF;
    fp->x67C = 0xFF;
}

static void Fighter_UnkInitLoad_80068914_Inner1(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    fp->input.x650 = fp->input.x654 = fp->input.x638_lsubStick_x =
        fp->input.x63C_lsubStick_y = fp->input.x640_lsubStick_x2 =
            fp->input.x644_lsubStick_y2 = fp->input.x620_lstick_x =
                fp->input.x624_lstick_y = fp->input.x628_lstick_x2 =
                    fp->input.x62C_lstick_y2 = 0.0f;

    fp->input.x660 = 0;
    fp->input.x66C = 0;
    fp->input.x668 = 0;
    fp->input.x65C_heldInputs = 0;

    fp->x679_x = fp->x67A_y = fp->x67B =

        fp->x676_x = fp->x677_y = fp->x678 =

            fp->x673 = fp->x674 = fp->x675 =

                fp->x670_timer_lstick_tilt_x = fp->x671_timer_lstick_tilt_y =
                    fp->x672_input_timer_counter = 0xFE;

    fp->x67C = fp->x67D = fp->x67E = fp->x681 = fp->x682 = fp->x67F = fp->x680 =
        fp->x683 = fp->x684 = fp->x685 = fp->x686 = fp->x687 = fp->x688 =
            fp->x689 = fp->x68A = fp->x68B = 0xFF;
}

void Fighter_UnkInitLoad_80068914(HSD_GObj* fighter_gobj,
                                  struct S_TEMP1* argdata)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    s32 costume_id;
    fp->x4_fighterKind = argdata->fighterKind;
    fp->xC_playerID = argdata->playerID;

    fp->x221F_flag.bits.b4 = argdata->flags.bits.b0;

    fp->x34_scale.x = Player_GetModelScale(fp->xC_playerID);
    fp->x61C = argdata->unk5;
    fp->x618_player_id = Player_GetPlayerId(fp->xC_playerID);
    fp->x61A_controller_index = Player_GetControllerIndex(fp->xC_playerID);
    fp->x2223_flag.bits.b6 = Player_GetFlagsBit5(fp->xC_playerID);
    fp->x2226_flag.bits.b3 = Player_GetFlagsBit6(fp->xC_playerID);
    fp->x2226_flag.bits.b6 = Player_GetFlagsBit7(fp->xC_playerID);
    fp->x2225_b5 = Player_GetMoreFlagsBit1(fp->xC_playerID);
    fp->x2225_b7 = Player_GetMoreFlagsBit2(fp->xC_playerID);
    fp->x2228_flag.bits.b3 = Player_GetMoreFlagsBit6(fp->xC_playerID);
    fp->x2229_b1 = Player_GetFlagsAEBit0(fp->xC_playerID);

    if (fp->x61A_controller_index > 4) {
        OSReport("fighter sub color num over!\n");
        __assert(__FILE__, 0x33C, "0");
    }

    if (fp->x61A_controller_index != 0) {
        struct RGBA* color =
            &p_ftCommonData->x6DC_colorsByPlayer[fp->x61A_controller_index - 1];
        fp->x610_color_rgba[0].r = (color->r * color->a) / 0xff;
        fp->x610_color_rgba[0].g = (color->g * color->a) / 0xff;
        fp->x610_color_rgba[0].b = (color->b * color->a) / 0xff;
        fp->x610_color_rgba[0].a = color->a;
    }

    costume_id = Player_GetCostumeId(fp->xC_playerID);
    if (costume_id >=
        CostumeListsForeachCharacter[fp->x4_fighterKind].numCostumes)
    {
        costume_id = 0;
    }

    fp->x619_costume_id = costume_id;
    fp->x61B_team = Player_GetTeam(fp->xC_playerID);
    fp->x0_fighter = fighter_gobj;
    fp->x10C_ftData = gFtDataList[fp->x4_fighterKind];
    func_800D0FA0(fighter_gobj);
    fp->x2CC = 0;
    fp->x2D0 = 0;
    fp->x18 = 0x155;
    fp->x1C_actionStateList = ActionStateList;
    fp->x20_actionStateList = ActionStateTableByCharacter[fp->x4_fighterKind];
    fp->x24 = fp->x10C_ftData->xC;
    fp->x28 = fp->x10C_ftData->x10;

    fp->input.x634 = 0.0f;
    fp->input.x630 = 0.0f;
    fp->input.x64C = 0.0f;
    fp->input.x648 = 0.0f;
    fp->input.x658 = 0.0f;
    fp->input.x664 = 0;

    Fighter_UnkInitLoad_80068914_Inner1(fighter_gobj);

    fp->x594_s32 = 0;
    fp->x21FC_flag.u8 = 1;

    fp->x221E_flag.bits.b0 = 0;
    fp->x221E_flag.bits.b1 = 0;
    fp->x221E_flag.bits.b2 = 0;
    fp->x221F_flag.bits.b1 = 0;
    fp->x221F_flag.bits.b2 = 0;

    fp->x209A = 0;
    fp->x221E_flag.bits.b5 = 0;
    fp->x221F_flag.bits.b0 = 0;
    fp->cb.x21EC_callback = 0;

    fp->x221D_flag.bits.b3 = 0;
    fp->x221D_flag.bits.b4 = 0;

    fp->x221F_flag.bits.b3 = 0;

    fp->x2220_flag.bits.b0 = 0;

    fp->x2221_flag.bits.b2 = 0;

    fp->x2229_b5_no_normal_motion = 0;
    fp->x2229_b6 = 0;
    fp->x2229_b7 = 0;

    fp->x222A_flag.bits.b0 = 0;
    fp->x222A_flag.bits.b1 = 0;

    fp->x2228_flag.bits.b5 = 0;
    fp->x2228_flag.bits.b6 = 0;

    fp->x2221_flag.bits.b3 = 0;

    fp->x2222_flag.bits.b0 = 0;
    fp->x2222_flag.bits.b1 = 0;
    fp->x2222_flag.bits.b4 = 0;
    fp->x2222_flag.bits.b5 = 0;
    fp->x2222_flag.bits.b6 = 0;

    fp->x2223_flag.bits.b1 = 0;

    fp->x40 = 0.0f;

    fp->x2224_flag.bits.b7 = 0;

    fp->x60C = 0;

    fp->x2225_b3 = 0;
    fp->x2228_flag.bits.b2 = 0;

    fp->x2226_flag.bits.b0 = 0;
    fp->x2226_flag.bits.b1 = 0;

    fp->x2227_flag.bits.b0 = 0;
    fp->x2224_flag.bits.b0 = 0;

    fp->x2114_SmashAttr.x2135 = -1;
    fp->x2184 = NULL;

    fp->x2229_b3 = 0;
}

// increments the spawn number, returns the spawn number value before
// incrementing
u32 Fighter_NewSpawn_80068E40(void)
{
    u32 spawnNum = g_spawnNumCounter++;
    if (g_spawnNumCounter == 0)
        g_spawnNumCounter = 1;
    return spawnNum;
}

void Fighter_80068E64(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (stage_info.internal_stage_id == 0x1B) {
        fp->x34_scale.z = p_ftCommonData->x7E4_scaleZ;
    } else {
        fp->x34_scale.z = 1.0f;
    }
}

static void Fighter_Create_Inline2(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    if (!fp->x2229_b5_no_normal_motion) {
        fp->x2EC = func_8001E8F8(func_80085E50(fp, 0x23));
        if (!fp->x2228_flag.bits.b2) {
            fp->x2DC = func_8001E8F8(func_80085E50(fp, 7));
            fp->x2E0 = func_8001E8F8(func_80085E50(fp, 8));
            fp->x2E4 = func_8001E8F8(func_80085E50(fp, 9));
            fp->x2E8 = func_8001E8F8(func_80085E50(fp, 0x25));
        }
    }
}

HSD_GObj* Fighter_Create(struct S_TEMP1* input)
{
    HSD_GObj* fighter_gobj;
    Fighter* fp;
    HSD_JObj* jobj;

    fighter_gobj = GObj_Create(4, 8, 0);
    GObj_SetupGXLink(fighter_gobj, &func_80080E18, 5U, 0U);
    fp = HSD_ObjAlloc(&fighter_alloc_data);
    fp->x2D8_specialAttributes2 = HSD_ObjAlloc(&lbl_80458FFC);
    GObj_InitUserData(fighter_gobj, 4U, &Fighter_Unload_8006DABC, fp);
    func_8008572C(input->fighterKind);
    Fighter_UnkInitLoad_80068914(fighter_gobj, input);
    func_8006737C(lbl_803C26FC[fp->x4_fighterKind]);
    func_80085820(fp->x4_fighterKind, fp->x619_costume_id);

    Fighter_UnkUpdateCostumeJoint_800686E4(fighter_gobj);

    func_80085B10(fp);
    func_80074E58(fp);
    Fighter_SetupParts(fighter_gobj);
    func_80070308(fighter_gobj);
    func_800C884C(fighter_gobj);

    Fighter_80068E64(fighter_gobj);

    func_800749CC(fighter_gobj);
    func_8007077C(fighter_gobj);
    func_8009CF84(fp);
    func_8006FE48(fighter_gobj);

    Fighter_UnkUpdateVecFromBones_8006876C(fp);

    func_8009F578(fp);

    if (ft_OnLoad[fp->x4_fighterKind]) {
        ft_OnLoad[fp->x4_fighterKind](fighter_gobj);
    }

    Fighter_Create_Inline2(fighter_gobj);

    func_8007B320(fighter_gobj);
    fp->x890_cameraBox = func_80029020();

    jobj = GET_JOBJ(fighter_gobj);
    func_8000ED54(&fp->x20A4, jobj);
    func_8038FD54(fighter_gobj, &Fighter_8006A1BC, 0);
    func_8038FD54(fighter_gobj, &Fighter_8006A360, 1);
    func_8038FD54(fighter_gobj, &Fighter_8006ABA0, 2);
    func_8038FD54(fighter_gobj, &Fighter_Spaghetti_8006AD10, 3);
    func_8038FD54(fighter_gobj, &Fighter_procUpdate, 4);
    func_8038FD54(fighter_gobj, &Fighter_8006C27C, 6);
    func_8038FD54(fighter_gobj, &Fighter_8006C5F4, 7);
    func_8038FD54(fighter_gobj, &Fighter_CallAcessoryCallbacks_8006C624, 8);
    func_8038FD54(fighter_gobj, &Fighter_8006C80C, 9);
    func_8038FD54(fighter_gobj, &Fighter_UnkProcessGrab_8006CA5C, 0xC);
    func_8038FD54(fighter_gobj, &Fighter_8006CB94, 0xD);
    func_8038FD54(fighter_gobj, &Fighter_UnkProcessShieldHit_8006D1EC, 0xE);
    func_8038FD54(fighter_gobj, &Fighter_8006D9AC, 0x10);
    func_8038FD54(fighter_gobj, &Fighter_UnkCallCameraCallback_8006D9EC, 0x12);
    func_8038FD54(fighter_gobj, &Fighter_8006DA4C, 0x16);
    Fighter_UnkProcessDeath_80068354(fighter_gobj);

    if (fp->x4_fighterKind == 0x1B) {
        func_8014FE10(fighter_gobj);
    } else if (fp->x4_fighterKind == 0x1C) {
        func_80155FCC(fighter_gobj);
    } else if (input->flags.bits.b1 != 0) {
        func_800BFD04(fighter_gobj);
    } else if (Player_GetFlagsBit3(fp->xC_playerID) != 0) {
        func_800C61B0(fighter_gobj);
    } else {
        if (!fp->x2229_b5_no_normal_motion) {
            func_8007D92C(fighter_gobj);
        } else {
            OSReport("ellegal flag fp->no_normal_motion\n");
            __assert(__FILE__, 1065, "0");
        }
    }
    func_800867E8(fighter_gobj);
    return fighter_gobj;
}

void Fighter_ActionStateChange_800693AC(HSD_GObj* fighter_gobj,
                                        s32 new_action_state_index, s32 arg2,
                                        HSD_GObj* otherObj, f32 arg8, f32 arg9,
                                        f32 argA)
{
    HSD_JObj* jobj = GET_JOBJ(fighter_gobj);
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    ActionState* new_action_state;
    struct S_TEMP4* unk_struct_x18;
    s32 bone_index;
    u8* unk_byte_ptr;
    bool animflags_bool;
    union Struct2070 x2070;

    fp->action_id = new_action_state_index;
    fp->x30_facingDirectionRepeated = fp->facing_dir;

    HSD_JObjSetTranslate(jobj, &fp->xB0_pos);
    func_80067624(fighter_gobj, &fp->x60C);

    if ((arg2 & FIGHTER_HIT_NOUPDATE) == 0) {
        if (fp->x2219_flag.bits.b3 != 0) {
            func_8007AFF8(fighter_gobj);
        }
        if (fp->x2219_flag.bits.b4 != 0) {
            func_8007C114(fighter_gobj);
        }
    }

    if ((arg2 & FIGHTER_THROW_EXCEPTION_NOUPDATE) == 0) {
        fp->x1064_thrownHitbox.owner = NULL;
    }

    if ((arg2 & FIGHTER_HITSTATUS_COLANIM_PRESERVE) == 0) {
        if (fp->x1988 != 0) {
            func_8007B62C(fighter_gobj, 0);
        }
        if (fp->x221A_flag.bits.b5 != 0) {
            func_8007B0C0(fighter_gobj, 0);
        }
    }

    if (fp->x221A_flag.bits.b6 != 0) {
        func_8007B4E0(fighter_gobj);
    }

    if (((arg2 & FIGHTER_MODEL_NOUPDATE) == 0) &&
        (fp->x221D_flag.bits.b2 != 0U))
    {
        func_80074A8C(fighter_gobj);
    }

    if (((arg2 & FIGHTER_MATANIM_NOUPDATE) == 0) &&
        ((fp->x221E_flag.bits.b7) != 0))
    {
        func_80070654(fighter_gobj);
    }

    if ((arg2 & FIGHTER_PARASOL_NOUPDATE) == 0) {
        fp->x2221_flag.bits.b4 = 0;
        if ((ftGetParasolStatus(fighter_gobj) != -1) &&
            (ftGetParasolStatus(fighter_gobj) != 6))
        {
            func_8007E83C(fighter_gobj, 6, 0.0f);
        }
    }

    func_80070F28(fighter_gobj);
    func_80070E74(fighter_gobj);
    func_8007ECD4(fp, 7);
    func_8007ECD4(fp, 8);
    func_8007ECD4(fp, 0x24);

    if ((arg2 & FIGHTER_RUMBLE_NOUPDATE) == 0) {
        func_8007ECD4(fp, 1);
        func_8007ECD4(fp, 0x19);
    }

    if (((arg2 & FIGHTER_PART_HITSTATUS_COLANIM_PRESERVE) == 0) &&
        (fp->x2221_flag.bits.b1 != 0U))
    {
        func_8007B6EC(fighter_gobj);
        func_8007B760(fighter_gobj, p_ftCommonData->x134);
        fp->x2221_flag.bits.b1 = 0;
    }
    func_8007E2F4(fp, 0);

    if (fp->dmg.x18F4 != 0) {
        fp->dmg.x18F4 = 0;
        fp->x2220_flag.bits.b4 = 0;
    }

    if ((arg2 & FIGHTER_SKIP_UNK_0x2222) == 0) {
        fp->x2222_flag.bits.b2 = 0;
    }

    if ((arg2 & FIGHTER_METALB_NOUPDATE) == 0) {
        fp->x2223_flag.bits.b4 = 0;
    }

    if ((arg2 & FIGHTER_UNK_0x2227) == 0) {
        fp->x2227_flag.bits.b2 = 0;
    }

    if (((arg2 & FIGHTER_HITSTUN_FLAG_NOUPDATE) == 0) &&
        (fp->x221C_flag.bits.b6 != 0U))
    {
        fp->x221C_flag.bits.b6 = 0;
        fp->x2098 = p_ftCommonData->x4CC;
    }

    fp->x221F_flag.bits.b3 = 0;
    fp->x2219_flag.bits.b1 = 0;
    fp->x2219_flag.bits.b2 = 0;

    fp->dmg.x182c_behavior = 1.0f;
    fp->dmg.x1850_forceApplied = 0.0f;
    fp->dmg.x18A8 = 0.0f;
    fp->dmg.x18B4_armor = 0.0f;
    fp->dmg.x18a0 = 0.0f;

    fp->x221A_flag.bits.b7 = 0;
    fp->x221B_b0 = 0;
    fp->x221C_flag.bits.b3 = 0;

    fp->x19B4_shieldUnk = 0.0f;
    fp->x19B8_shieldUnk = 0.0f;

    fp->x221D_flag.bits.b5 = 0;
    fp->x2218_flag.bits.b3 = 0;
    fp->x2218_flag.bits.b6 = 0;
    fp->x221C_flag.bits.b4 = 0;

    fp->x1A6A = 0;

    fp->x221D_flag.bits.b7 = 0;
    fp->x221E_flag.bits.b0 = 0;
    fp->x221E_flag.bits.b1 = 0;
    fp->x221E_flag.bits.b2 = 0;
    fp->x221F_flag.bits.b1 = 0;
    fp->x221E_flag.bits.b5 = 0;
    fp->x221E_flag.bits.b6 = 0;
    fp->x2220_flag.bits.b3 = 0;
    fp->x2220_flag.bits.b7 = 0;

    fp->x209C = 0;

    fp->x2223_flag.bits.b0 = 0;
    fp->x2222_flag.bits.b3 = 0;
    fp->x2224_flag.bits.b5 = 0;
    fp->x2225_b1 = 0;
    fp->x2225_b4 = 0;

    func_8004CBF4(&fp->x6F0_collData);

    func_800DEEA8(fighter_gobj);

    fp->x2114_SmashAttr.x2138_smashSinceHitbox = -1.0f;
    fp->x2224_flag.bits.b4 = 0;

    if ((arg2 & FIGHTER_ITEMVIS_NOUPDATE) == 0) {
        fp->x221E_flag.bits.b3 = 1;
    } else if (fp->x221E_flag.bits.b3 == 0U) {
        func_8007F578(fighter_gobj);
    }

    if ((arg2 & FIGHTER_MODELPART_VIS_NOUPDATE) == 0) {
        fp->x221E_flag.bits.b4 = 1;
    }

    if ((arg2 & FIGHTER_MODEL_FLAG_NOUPDATE) == 0) {
        fp->x2225_b2 = 1;
    }

    if ((arg2 & FIGHTER_FASTFALL_PRESERVE) == 0) {
        fp->x221A_flag.bits.b4 = 0;
    }

    if ((arg2 & FIGHTER_COLANIM_NOUPDATE) == 0) {
        func_800C0134(fp);
    }

    if (((arg2 & FIGHTER_GFX_PRESERVE) == 0) && (fp->x2219_flag.bits.b0 != 0U))
    {
        func_8007DB24(fighter_gobj);
    }

    if (((arg2 & FIGHTER_ACCESSORY_PRESERVE) == 0) &&
        ((u32) fp->x20A0_accessory != 0U))
    {
        HSD_JObjRemoveAll(fp->x20A0_accessory);
        fp->x20A0_accessory = 0U;
    }

    if (fp->xE0_ground_or_air == 0) {
        if (fp->x4_fighterKind == 9) {
            fp->sa.peach.x222C = 1;
        }
        fp->x2221_flag.bits.b5 = 0;
        fp->x2221_flag.bits.b7 = 1;
        fp->x2221_flag.bits.b6 = 1;
        fp->x2224_flag.bits.b1 = 0;
        fp->x2227_flag.bits.b1 = 0;
        fp->x213C = -1;

        if (fp->x2227_flag.bits.b4 != 0U) {
            func_8003FE1C(fp->xC_playerID, fp->x221F_flag.bits.b4);
            fp->x2227_flag.bits.b4 = 0;
        }
        fp->x2227_flag.bits.b5 = 0;
        func_80040330(fp->xC_playerID, fp->x221F_flag.bits.b4, fp->x2140);
        fp->x2140 = 0.0f;
        fp->x2228_flag.bits.b6 = 0;
        fp->x2180 = 6;
    }

    if ((new_action_state_index != 0xE) && (new_action_state_index != 0xF) &&
        (new_action_state_index != 0x10) && (new_action_state_index != 0x11))
    {
        fp->x196C_hitlag_mult = 0.0f;
    }

    if ((arg2 & FIGHTER_SFX_PRESERVE) == 0) {
        func_80088884(fp);
        func_800888E0(fp);
        func_800887CC(fp);
    }

    if ((arg2 & FIGHTER_SWORDTRAIL_PRESERVE) == 0) {
        fp->x2100 = -1;
    }
    if ((arg2 & FIGHTER_NAMETAGVIS_NOUPDATE) == 0) {
        fp->x209A = 0;
    }

    fp->x2222_flag.bits.b7 = 0;

    if ((arg2 & FIGHTER_PHYS_UNKUPDATE) != 0) {
        fp->x100 = 0.0f;
    } else {
        fp->x100 = -1.0f;
    }

    if ((arg2 & FIGHTER_UNK_0x1000000) == 0) {
        fp->x221C_u16_y = 0;
    }

    fp->x6BC_inputStickangle = 0.0f;

    func_8007592C(fp, 0, 0.0f);
    func_80075AF0(fp, 0, (HALF_PI * fp->facing_dir));
    func_80075CB4(fp, 0, 0.0f);

    if (new_action_state_index >= fp->x18) {
        new_action_state =
            &fp->x20_actionStateList[(new_action_state_index - fp->x18)];
    } else {
        new_action_state = &fp->x1C_actionStateList[new_action_state_index];
    }

    if (fp->xE0_ground_or_air == GA_Ground) {
        if ((arg2 & 0x40) == 0) {
            if (new_action_state->x9_flags.bits.b1 != 0 && fp->dmg.x18C8 == -1)
            {
                if (p_ftCommonData->x814 > 0) {
                    fp->dmg.x18C8 = p_ftCommonData->x814;
                } else {
                    fp->dmg.x18C8 = 1;
                }
            }
        }
    }

    {
        // load in the union.
        x2070 = fp->x2070;
        func_800890D0(fp, new_action_state->move_id);
        func_800895E0(fp, new_action_state->x4_flags);
        fp->x2225_b3 = new_action_state->x9_flags.bits.b0;

        if (fp->x2226_flag.bits.b4 != 0U) {
            if (fp->x2070.x2071_b5 != 0U) {
                func_800C8B2C(fp, 0x7E, 0);
            }
            if (fp->x2070.x2071_b6 != 0U) {
                func_800C8B2C(fp, 0x7F, 0);
            }
        }

        if (fp->cb.x21EC_callback) {
            fp->cb.x21EC_callback(fighter_gobj);
            fp->cb.x21EC_callback = 0U;
        }

        if ((arg2 & FIGHTER_ATTACKCOUNT_NOUPDATE) == 0) {
            func_80037C60(fighter_gobj, x2070.x2070_int);
        }

        fp->anim_id = new_action_state->anim_id;
        fp->x89C_frameSpeedMul = arg9;
        fp->x8A0_unk = arg9;

        fp->x894_currentAnimFrame = (arg8 - fp->x89C_frameSpeedMul);
        fp->x898_unk = 0.0f;

        if ((fp->x594_animCurrFlags1.bits.b0) ||
            (fp->x594_animCurrFlags1.bits.b5))
        {
            animflags_bool = true;
        } else {
            animflags_bool = false;
        }

        if (fp->anim_id != -1) {
            Vec3 translation;
            Quaternion quat;

            bone_index = fp->x596_bits.x7;

            if ((arg2 & FIGHTER_FREEZESTATE) != 0) {
                fp->x2223_flag.bits.b0 = 1;
                fp->x104 = 0x14;
                fp->x89C_frameSpeedMul = 0.0f;
                arg9 = 0.0f;
            }

            if (otherObj != NULL) {
                unk_struct_x18 =
                    &((Fighter*) otherObj->user_data)->x24[fp->anim_id];
                unk_byte_ptr =
                    &((Fighter*) otherObj->user_data)->x28[fp->anim_id << 1];
            } else {
                unk_struct_x18 = &fp->x24[fp->anim_id];
                unk_byte_ptr = &fp->x28[fp->anim_id << 1];
            }
            fp->x594_s32 = unk_struct_x18->x10_animCurrFlags;
            func_8009E7B4(fp, unk_byte_ptr);
            if ((arg2 & FIGHTER_ANIM_NOUPDATE) == 0) {
                if (otherObj != 0U) {
                    func_80085CD8(fp, GET_FIGHTER(otherObj), fp->anim_id);
                    func_8007B8CC(fp, otherObj);
                } else {
                    func_80085CD8(fp, fp, fp->anim_id);
                }
                fp->x3EC = unk_struct_x18->xC;
                fp->x3F0 = 0;

                if (arg8) {
                    if (fp->x590 != 0U) {
                        func_8006EBE8(fighter_gobj, arg8 - arg9, arg9,
                                      (argA == -1.0f) ? 0.0f
                                      : (argA)        ? argA
                                                      : *unk_byte_ptr);
                    }
                    func_8006E9B4(fighter_gobj);
                    if (fp->x594_animCurrFlags1.bits.b0 != 0U) {
                        fp->x6B0.x = fp->x6B0.y = fp->x6B0.z = 0.0F;
                        fp->x6A4_transNOffset.x = fp->x6A4_transNOffset.y =
                            fp->x6A4_transNOffset.z = 0.0F;
                        fp->x698 = fp->x68C_transNPos;
                    }

                    if (fp->x594_animCurrFlags1.bits.b5 != 0U) {
                        fp->x6E4.x = fp->x6E4.y = fp->x6E4.z = 0.0F;
                        fp->x6D8.x = fp->x6D8.y = fp->x6D8.z = 0.0F;
                        fp->x6CC = fp->x6C0;
                    }
                    fp->x3E4 = -arg8;
                } else {
                    if (fp->x590 != 0U) {
                        func_8006EBE8(fighter_gobj, arg8, arg9,
                                      (argA == -1.0f) ? 0.0f
                                      : (argA)        ? argA
                                                      : *unk_byte_ptr);
                    }
                    fp->x3E4 = 0.0f;
                }

                func_8006E9B4(fighter_gobj);
                if ((bone_index != 0) && (*unk_byte_ptr != 0U)) {
                    HSD_JObj* temp_joint =
                        fp->x5E8_fighterBones[bone_index].x4_jobj2;

                    HSD_JObjGetTranslation(temp_joint, &translation);
                    HSD_JObjSetTranslate(
                        fp->x5E8_fighterBones[bone_index].x0_jobj,
                        &translation);
                    HSD_JObjGetRotation(temp_joint, &quat);
                    Fighter_JObjSetRotation(
                        fp->x5E8_fighterBones[bone_index].x0_jobj, &quat);
                }

                if (fp->x594_animCurrFlags1.bits.b0 != 0U) {
                    if (!arg8) {
                        float c = 0.0f;
                        fp->x6B0.x = fp->x6B0.y = fp->x6B0.z = c;
                        fp->x6A4_transNOffset.x = fp->x6A4_transNOffset.y =
                            fp->x6A4_transNOffset.z = c;
                        fp->x698 = fp->x68C_transNPos;
                    } else if (((arg2 & FIGHTER_ANIMVEL_NOUPDATE) == 0) &&
                               (fp->xE0_ground_or_air == GA_Ground))
                    {
                        f32 temp_vel = fp->x6A4_transNOffset.z * fp->facing_dir;
                        fp->x80_self_vel.x = temp_vel;
                        fp->xEC_ground_vel = temp_vel;
                    }
                }

                if (fp->x594_animCurrFlags1.bits.b5 != 0U) {
                    if (!arg8) {
                        float c = 0.0f;
                        fp->x6E4.x = fp->x6E4.y = fp->x6E4.z = c;
                        fp->x6D8.x = fp->x6D8.y = fp->x6D8.z = c;
                        fp->x6CC = fp->x6C0;
                    } else if (((arg2 & FIGHTER_ANIMVEL_NOUPDATE) == 0) &&
                               (fp->xE0_ground_or_air == GA_Ground))
                    {
                        f32 temp_vel = fp->x6D8.z * fp->facing_dir;
                        fp->x80_self_vel.x = temp_vel;
                        fp->xEC_ground_vel = temp_vel;
                    }
                }
                if ((arg2 & FIGHTER_CMD_UPDATE) != 0) {
                    func_8007349C(fighter_gobj);
                } else if (arg8) {
                    func_80073354(fighter_gobj);
                } else {
                    func_800C0408(fighter_gobj);
                    func_80073240(fighter_gobj);
                }
            } else {
                fp->anim_id = -1;
            }
        }

        if (fp->anim_id == -1) {
            fp->x594_s32 = 0;
            func_80070758(jobj);
            func_80070758(fp->x8AC_animSkeleton);
            fp->x3EC = 0;
            fp->x8A4_animBlendFrames = 0;
            fp->x8A8_unk = 0;
        }

        if (animflags_bool) {
            if (!fp->x594_animCurrFlags1.bits.b0 &&
                !fp->x594_animCurrFlags1.bits.b0)
            {
                !fp;
                func_8007CC78(fp, fp->x110_attr.x138_DashrunTerminalVelocity);
            }
        }

        fp->cb.x21A0_callback_Anim = new_action_state->animated;
        fp->cb.x219C_callback_IASA = new_action_state->input_updated;
        fp->cb.x21A4_callback_Phys = new_action_state->physics_updated;
        fp->cb.x21A8_callback_Coll = new_action_state->collided;
        fp->cb.x21AC_callback_Cam = new_action_state->camera_updated;

        fp->cb.x21B0_callback_Accessory1 = 0;
        fp->cb.x21BC_callback_Accessory4 = 0;
        fp->cb.x21C0_callback_OnGiveDamage = 0;
        fp->cb.x21C4_callback_OnShieldHit = 0;
        fp->cb.x21C8_callback_OnReflectHit = 0;
        fp->cb.x21D0_callback_EveryHitlag = 0;
        fp->cb.x21CC_callback = 0;
        fp->cb.x21D8_callback_ExitHitlag = 0;
        fp->cb.x21D4_callback_EnterHitlag = 0;
        fp->cb.x21DC_callback_OnTakeDamage = 0;
        fp->cb.x21F0_callback = 0;
        fp->cb.x21F4_callback = 0;
        fp->cb.x21F8_callback = 0;
        fp->cb.x21E4_callback_OnDeath2 = 0;
    }
}

void Fighter_8006A1BC(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (!fp->x221F_flag.bits.b3) {
        if (fp->dmg.x1954 > 0.0f) {
            fp->dmg.x1954 -= 1.0f;
            if (fp->dmg.x1954 <= 0.0f) {
                fp->dmg.x1954 = 0.0f;
                if (!fp->x221A_flag.bits.b2 && !fp->x2219_flag.bits.b7) {
                    Fighter_8006D10C(fighter_gobj);
                }
            }
        }

        if (fp->x221A_flag.bits.b1) {
            Fighter_8006CFE0(fighter_gobj);
            fp->x221A_flag.bits.b1 = 0;
        }

        if (fp->dmg.x195c_hitlag_frames > 0.0f) {
            fp->dmg.x195c_hitlag_frames -= 1.0f;
            if (fp->dmg.x195c_hitlag_frames <= 0.0f) {
                fp->dmg.x195c_hitlag_frames = 0.0f;
                if (fp->x221A_flag.bits.b3) {
                    func_80090718(fp);
                    fp->x221A_flag.bits.b3 = 0;
                }
                if ((!fp->dmg.x1954) && !fp->x2219_flag.bits.b7) {
                    Fighter_8006D10C(fighter_gobj);
                }
                fp->x221A_flag.bits.b2 = 0;
            }
        }
        func_800C37A0(fighter_gobj);

        while (fp->x200C != 0) {
            func_800D14E4(fighter_gobj);
            fp->x200C--;
        }

        while (fp->x2010 != 0) {
            func_800D1E80(fighter_gobj);
            fp->x2010--;
        }

        func_800819A8(fighter_gobj);
        fp->x2219_flag.bits.b6 = fp->x2219_flag.bits.b5;
    }
}

void Fighter_8006A360(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (!fp->x221F_flag.bits.b3) {
        fp->xC8_pos_delta.x = fp->xB0_pos.x - fp->xBC_prevPos.x;
        fp->xC8_pos_delta.y = fp->xB0_pos.y - fp->xBC_prevPos.y;
        fp->xC8_pos_delta.z = fp->xB0_pos.z - fp->xBC_prevPos.z;

        fp->xBC_prevPos = fp->xB0_pos;

        if (fp->dmg.x18C8 != -1 && fp->dmg.x18C8 > 0) {
            fp->dmg.x18C8--;
            if (fp->dmg.x18C8 == 0) {
                fp->dmg.x18c4_source_ply = 6;
                fp->dmg.x18C8 = -1;
                fp->dmg.x18D0 = -0xA;
            }
        }

        if (fp->x1990) {
            fp->x1990--;
            if (fp->x1990 == 0 && !fp->x2221_flag.bits.b0) {
                fp->x198C = fp->x1994 != 0 ? 1 : 0;

                if (func_800C0694(fp) == 9) {
                    func_800C0200(fp, 9);
                }
            }
        }

        if (fp->x1994) {
            fp->x1994--;
            if (fp->x1994 == 0) {
                fp->x198C = (fp->x2221_flag.bits.b0 || fp->x1990 != 0) ? 2 : 0;

                if (func_800C0694(fp) == 9) {
                    func_800C0200(fp, 9);
                }
            }
        }

        if (fp->x221D_flag.bits.b6) {
            if (fp->x2004) {
                fp->x2004--;
                if (fp->x2004 == 0) {
                    fp->x221D_flag.bits.b6 = 0;
                    if (func_800C0694(fp) == 0x6B) {
                        func_800C0200(fp, 0x6B);
                    }
                } else if (fp->x2004 == func_8026B588()) {
                    func_800880D8(fp);
                }
            }
        }

        if (fp->x2220_flag.bits.b5 || fp->x2220_flag.bits.b6) {
            if (fp->x2008)
                fp->x2008--;

            if (fp->x2008 == 0) {
                if (fp->x2220_flag.bits.b5) {
                    func_800D1A8C(fighter_gobj);
                } else if (fp->x2220_flag.bits.b6) {
                    func_800D237C(fighter_gobj);
                }
            }
        }

        if (fp->x197C) {
            if (fp->x2014) {
                fp->x2014--;
                if (fp->x2014 == 0) {
                    func_8007F8E8(fighter_gobj);
                    func_8026A8EC(fp->x197C);
                    func_8007F9B4(fighter_gobj);
                }
            }
        }

        if (fp->x2223_flag.bits.b7) {
            if (fp->x2028) {
                fp->x2028--;
                if (!fp->x2028 || fp->x202C <= 0) {
                    func_800C8540(fighter_gobj);
                }
            }
        }

        if (fp->x2227_flag.bits.b3) {
            if (fp->x2034) {
                fp->x2034--;
                if (!fp->x2034 || fp->x2038 <= 0) {
                    func_800C9034(fighter_gobj);
                    return;
                }
            }
        }

        if (fp->x1980) {
            fp->x2018--;
            fp->x201C--;

            if (fp->x201C == 0) {
                Fighter_TakeDamage_8006CC7C(fp, p_ftCommonData->x6F4_unkDamage);
                fp->x201C = p_ftCommonData->x6F8;
            }

            if (fp->x2018 <= 0) {
                Vec3 vec = lbl_803B7488;

                func_8007F8E8(fighter_gobj);
                func_8026ABD8(fp->x1980, &vec, 0.0f);
                func_8007FF74(fighter_gobj);
            }
        }

        if (fp->x2226_flag.bits.b4) {
            if (fp->x2030) {
                fp->x2030--;
                if (fp->x2030 == 0) {
                    func_800C8A64(fighter_gobj);
                    return;
                }
                if (!fp->x2226_flag.bits.b3 &&
                    fp->x2030 == p_ftCommonData->x7D0 &&
                    func_800C8B2C(fp, 0x7D, 0))
                {
                    fp->x2226_flag.bits.b7 = 1;
                }
            }
        }

        if (fp->x2221_flag.bits.b4) {
            if (fp->x2104) {
                fp->x2104--;
                if (fp->x2104 == 0) {
                    fp->x2221_flag.bits.b4 = 0;

                    if (fp->x1974_heldItem &&
                        itGetKind(fp->x1974_heldItem) == 0x67)
                    {
                        fp->x2221_flag.bits.b5 = 1;
                        func_800968C8(fighter_gobj);
                    } else {
                        func_80095744(fighter_gobj, 0);
                    }
                }
            }
        }

        if (!fp->x221F_flag.bits.b4 && func_80031144() == 1.0f) {
            if (fp->dmg.x1830_percent < p_ftCommonData->x7B0) {
                if (func_802FC998(fp->xC_playerID) &&
                    (Player_GetMoreFlagsBit3(fp->xC_playerID) != 0))
                {
                    fp->dmg.x1910++;
                } else {
                    fp->dmg.x1910 = 0;
                }

                if (fp->dmg.x1910 >= p_ftCommonData->x7AC) {
                    Fighter_TakeDamage_8006CC7C(fp,
                                                p_ftCommonData->x7B4_unkDamage);
                    fp->dmg.x1910 = 0;
                }
            }
        }

        if (fp->dmg.x18F0) {
            fp->dmg.x18F0--;
            if (fp->dmg.x1830_percent > 0.0f) {
                fp->dmg.x1830_percent--;
                func_80088640(fp, 0x7D, 0x7F, 0x40);
                Player_SetHPByIndex(fp->xC_playerID, fp->x221F_flag.bits.b4,
                                    fp->dmg.x1830_percent);
                func_80040B8C(fp->xC_playerID, fp->x221F_flag.bits.b4, 1);
            }

            if (fp->dmg.x1830_percent <= 0.0f) {
                fp->dmg.x1830_percent = 0.0f;
                fp->dmg.x18F0 = 0;
            }

            if (fp->dmg.x18F0 == 0) {
                if (func_800C0694(fp) == 8) {
                    func_800C0200(fp, 8);
                }
                func_8007ECD4(fp, 2);
            }
        }

        if (fp->x1974_heldItem) {
            if (itGetKind(fp->x1974_heldItem) != 0x1C) {
                !fp;
            } else {
                func_800C511C(fighter_gobj);
            }
        }

        if (fp->dmg.x18fa_model_shift_frames) {
            fp->dmg.x18fa_model_shift_frames--;
            fp->dmg.x18FC++;
            if (fp->dmg.x18FC >= fp->dmg.x18FD) {
                fp->dmg.x18FC = 0;
            }
        }

        if (lbl_803C1DB4[fp->x4_fighterKind]) {
            lbl_803C1DB4[fp->x4_fighterKind](fighter_gobj);
        }

        if (fp->cb.x21CC_callback) {
            fp->cb.x21CC_callback(fighter_gobj);
        }

        if (!fp->x2219_flag.bits.b5) {
            if (fp->x209A > 1 && !fp->x221D_flag.bits.b4) {
                fp->x209A--;
            }
            if (fp->x2223_flag.bits.b0) {
                if (fp->x104 == 0x14U) {
                    func_8006F0FC(fighter_gobj, 0.0f);
                } else {
                    fp->x89C_frameSpeedMul += fp->x8A0_unk;
                }
                fp->x104--;
                if (fp->x104 == 0) {
                    func_8006F0FC(fighter_gobj, fp->x89C_frameSpeedMul);
                    fp->x104 = 0x14U;
                }
            }

            if (fp->x2114_SmashAttr.x2138_smashSinceHitbox != -1.0f) {
                fp->x2114_SmashAttr.x2138_smashSinceHitbox++;
            }

            if (fp->dmg.x18ac_time_since_hit != -1) {
                fp->dmg.x18ac_time_since_hit++;
            }
            func_8006EBA4(fighter_gobj);
            func_800D71D8(fighter_gobj);
            func_800764DC(fighter_gobj);

            if (!fp->x221C_flag.bits.b6) {
                func_800411C4(fp->xC_playerID, fp->x221F_flag.bits.b4);
            }
            func_800DEF38(fighter_gobj);

            if (fp->cb.x21A0_callback_Anim) {
                fp->cb.x21A0_callback_Anim(fighter_gobj);
            }
        }

        func_8007E0E4(fighter_gobj);
        func_800C0408(fighter_gobj);
    }
}

void Fighter_8006ABA0(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    if (!fp->x221F_flag.bits.b3 && func_800A2040(fp)) {
        func_800B3900(fighter_gobj);
    }
}

// https://decomp.me/scratch/A7CgG
void Fighter_UnkIncrementCounters_8006ABEC(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (func_800CAE80(fighter_gobj)) {
        fp->x68A = fp->x685;
        fp->x685 = 0;
    } else if (fp->x685 < 0xFF) {
        fp->x685++;
    }

    if (func_800D6928(fp)) {
        fp->x68B = fp->x686;
        fp->x686 = 0;
    } else if (fp->x686 < 0xFF) {
        fp->x686++;
    }

    if (func_800D688C(fp)) {
        fp->x687 = 0;
    } else if (fp->x687 < 0xFF) {
        fp->x687++;
    }
    if (func_800964FC(fp)) {
        fp->x688 = 0;
    } else if (fp->x688 < 0xFF) {
        fp->x688++;
    }

    if (func_800D67C4(fp)) {
        fp->x689 = 0;
    } else if (fp->x689 < 0xFF) {
        fp->x689++;
    }
}

// the stick pairs seen in input structs might make more sense as an array of 2,
// or a struct of 2 floats.. if it still matches.
#define SET_STICKS(stickXPtr, stickYPtr, x, y)                                 \
    do {                                                                       \
        f32* stickX = (f32*) &stickXPtr;                                       \
        f32* stickY = (f32*) &stickYPtr;                                       \
        *stickX = x;                                                           \
        *stickY = y;                                                           \
    } while (0)

static void Fighter_Spaghetti_8006AD10_Inner1(Fighter* fp)
{
    s32 temp0_loc_1;
    s32 temp0_loc_0;

    temp0_loc_0 = (fp->input.x65C_heldInputs &
                   (fp->input.x660 ^ fp->input.x65C_heldInputs));
    temp0_loc_1 =
        (fp->input.x660 & (fp->input.x660 ^ fp->input.x65C_heldInputs));

    if (fp->x2219_flag.bits.b5) {
        fp->input.x668 |= temp0_loc_0;
        fp->input.x66C |= temp0_loc_1;
    } else {
        fp->input.x668 = temp0_loc_0;
        fp->input.x66C = temp0_loc_1;
    }
}

void Fighter_Spaghetti_8006AD10(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    f32 tempf1;
    f32 tempf0;

    if (!fp->x221F_flag.bits.b3) {
        if (!fp->x2224_flag.bits.b2) {
            if (!fp->x221D_flag.bits.b3) {
                SET_STICKS(fp->input.x628_lstick_x2, fp->input.x62C_lstick_y2,
                           fp->input.x630, fp->input.x634);
                SET_STICKS(fp->input.x640_lsubStick_x2,
                           fp->input.x644_lsubStick_y2, fp->input.x648,
                           fp->input.x64C);
                fp->input.x654 = fp->input.x658;
                fp->input.x660 = fp->input.x664;
                fp->x221D_flag.bits.b3 = 1;
            } else {
                SET_STICKS(fp->input.x628_lstick_x2, fp->input.x62C_lstick_y2,
                           fp->input.x620_lstick_x, fp->input.x624_lstick_y);
                SET_STICKS(
                    fp->input.x640_lsubStick_x2, fp->input.x644_lsubStick_y2,
                    fp->input.x638_lsubStick_x, fp->input.x63C_lsubStick_y);
                fp->input.x654 = fp->input.x650;
                fp->input.x660 = fp->input.x65C_heldInputs;
            }

            if (func_800A2040(fp)) {
                SET_STICKS(fp->input.x620_lstick_x, fp->input.x624_lstick_y,
                           func_800A17E4(fp), func_800A1874(fp));
                if (g_debugLevel < 3 && !func_8016B41C()) {
                    SET_STICKS(fp->input.x638_lsubStick_x,
                               fp->input.x63C_lsubStick_y, func_800A1994(fp),
                               func_800A1A24(fp));
                } else {
                    fp->input.x638_lsubStick_x = 0;
                    fp->input.x63C_lsubStick_y = 0;
                }

                tempf0 = func_800A1904(fp);
                tempf1 = func_800A1948(fp);

                fp->input.x650 = (tempf0 > tempf1) ? tempf0 : tempf1;

            } else {
                SET_STICKS(fp->input.x620_lstick_x, fp->input.x624_lstick_y,
                           HSD_PadRumbleData[fp->x618_player_id].nml_stickX,
                           HSD_PadRumbleData[fp->x618_player_id].nml_stickY);
                if (g_debugLevel < 3 && func_8016B41C() == 0) {
                    SET_STICKS(
                        fp->input.x638_lsubStick_x, fp->input.x63C_lsubStick_y,
                        HSD_PadRumbleData[fp->x618_player_id].nml_subStickX,
                        HSD_PadRumbleData[fp->x618_player_id].nml_subStickY);
                } else {
                    fp->input.x638_lsubStick_x = 0;
                    fp->input.x63C_lsubStick_y = 0;
                }

                tempf1 = HSD_PadRumbleData[fp->x618_player_id].nml_analogR;
                tempf0 = HSD_PadRumbleData[fp->x618_player_id].nml_analogL;

                fp->input.x650 = (tempf0 > tempf1) ? tempf0 : tempf1;
            }

            if (fabs_inline(fp->input.x620_lstick_x) <= p_ftCommonData->x0) {
                fp->input.x620_lstick_x = 0.0f;
            }

            if (fabs_inline(fp->input.x624_lstick_y) <= p_ftCommonData->x4) {
                fp->input.x624_lstick_y = 0.0f;
            }

            if (fabs_inline(fp->input.x638_lsubStick_x) <= p_ftCommonData->x0) {
                fp->input.x638_lsubStick_x = 0.0f;
            }

            if (fabs_inline(fp->input.x63C_lsubStick_y) <= p_ftCommonData->x4) {
                fp->input.x63C_lsubStick_y = 0.0f;
            }

            if (fp->input.x650 <= p_ftCommonData->x10) {
                fp->input.x650 = 0.0f;
            }

            if (func_800A2040(fp)) {
                fp->input.x65C_heldInputs = func_800A198C(fp);
            } else {
                fp->input.x65C_heldInputs =
                    HSD_PadRumbleData[fp->x618_player_id].button;
            }

            if (func_8016B0FC()) {
                fp->input.x650 = 0.0f;
                if (func_800A2040(fp)) {
                    fp->input.x65C_heldInputs =
                        (fp->input.x65C_heldInputs & 0xD00);
                } else {
                    fp->input.x65C_heldInputs =
                        (fp->input.x65C_heldInputs & 0x100);
                }
            } else {
                if ((fp->input.x65C_heldInputs & 0x60)) {
                    fp->input.x65C_heldInputs =
                        (s32) (fp->input.x65C_heldInputs | 0x80000000);
                    fp->input.x650 = 1.0f;
                } else if (fp->input.x650) {
                    fp->input.x65C_heldInputs =
                        (fp->input.x65C_heldInputs | 0x80000000);
                }
                if (func_801A45E8(0) == 0) {
                    if ((fp->input.x65C_heldInputs & 0x10)) {
                        fp->input.x65C_heldInputs =
                            (fp->input.x65C_heldInputs | 0x80000000 | 0x100);
                        fp->input.x650 = p_ftCommonData->x14;
                    }
                }
            }

            Fighter_Spaghetti_8006AD10_Inner1(fp);

            // Fighter_ClampSpecificValue
            fp->x676_x++;
            if (fp->x676_x > 0xFE) {
                fp->x676_x = 0xFE;
            }

            if (fp->input.x620_lstick_x >=
                p_ftCommonData->x8_someStickThreshold)
            {
                if (fp->input.x628_lstick_x2 >=
                    p_ftCommonData->x8_someStickThreshold)
                {
                    // Fighter_ClampThreeValues
                    fp->x670_timer_lstick_tilt_x++;
                    if (fp->x670_timer_lstick_tilt_x > 0xFE) {
                        fp->x670_timer_lstick_tilt_x = 0xFE;
                    }
                    fp->x673++;
                    if (fp->x673 > 0xFE) {
                        fp->x673 = 0xFE;
                    }
                    fp->x679_x++;
                    if (fp->x679_x > 0xFE) {
                        fp->x679_x = 0xFE;
                    }
                } else {
                    fp->x676_x = 0;
                    fp->x673 = 0;
                    fp->x670_timer_lstick_tilt_x = 0;
                    fp->x2228_flag.bits.b7 = 1;
                }
            } else if (fp->input.x620_lstick_x <=
                       -p_ftCommonData->x8_someStickThreshold)
            {
                if (fp->input.x628_lstick_x2 <=
                    -p_ftCommonData->x8_someStickThreshold)
                {
                    // Fighter_ClampThreeValues
                    fp->x670_timer_lstick_tilt_x++;
                    if (fp->x670_timer_lstick_tilt_x > 0xFE) {
                        fp->x670_timer_lstick_tilt_x = 0xFE;
                    }
                    fp->x673++;
                    if (fp->x673 > 0xFE) {
                        fp->x673 = 0xFE;
                    }
                    fp->x679_x++;
                    if (fp->x679_x > 0xFE) {
                        fp->x679_x = 0xFE;
                    }
                } else {
                    fp->x676_x = 0;
                    fp->x673 = 0;
                    fp->x670_timer_lstick_tilt_x = 0;
                    fp->x2228_flag.bits.b7 = 0;
                }
            } else {
                fp->x679_x = 0xFEU;
                fp->x673 = 0xFEU;
                fp->x670_timer_lstick_tilt_x = 0xFEU;
            }

            // Fighter_ClampSpecificValue
            fp->x677_y++;
            if (fp->x677_y > 0xFE) {
                fp->x677_y = 0xFE;
            }

            if (fp->input.x624_lstick_y >= p_ftCommonData->xC) {
                if (fp->input.x62C_lstick_y2 >= p_ftCommonData->xC) {
                    // Fighter_ClampThreeValues
                    fp->x671_timer_lstick_tilt_y++;
                    if (fp->x671_timer_lstick_tilt_y > 0xFE) {
                        fp->x671_timer_lstick_tilt_y = 0xFE;
                    }
                    fp->x674++;
                    if (fp->x674 > 0xFE) {
                        fp->x674 = 0xFE;
                    }
                    fp->x67A_y++;
                    if (fp->x67A_y > 0xFE) {
                        fp->x67A_y = 0xFE;
                    }
                } else {
                    fp->x677_y = 0;
                    fp->x674 = 0;
                    fp->x671_timer_lstick_tilt_y = 0;
                    fp->x2229_b0 = 0;
                }
            } else if (fp->input.x624_lstick_y <= -p_ftCommonData->xC) {
                if (fp->input.x62C_lstick_y2 <= -p_ftCommonData->xC) {
                    // Fighter_ClampThreeValues
                    fp->x671_timer_lstick_tilt_y++;
                    if (fp->x671_timer_lstick_tilt_y > 0xFE) {
                        fp->x671_timer_lstick_tilt_y = 0xFE;
                    }
                    fp->x674++;
                    if (fp->x674 > 0xFE) {
                        fp->x674 = 0xFE;
                    }
                    fp->x67A_y++;
                    if (fp->x67A_y > 0xFE) {
                        fp->x67A_y = 0xFE;
                    }
                } else {
                    fp->x677_y = 0;
                    fp->x674 = 0;
                    fp->x671_timer_lstick_tilt_y = 0;
                    fp->x2229_b0 = 1;
                }
            } else {
                fp->x67A_y = 0xFE;
                fp->x674 = 0xFE;
                fp->x671_timer_lstick_tilt_y = 0xFE;
            }

            if (func_8000D148(fp->input.x628_lstick_x2,
                              fp->input.x62C_lstick_y2, fp->input.x620_lstick_x,
                              fp->input.x624_lstick_y, 0.0f, 0.0f,
                              p_ftCommonData->x8_someStickThreshold))
            {
                fp->x67A_y = 0;
                fp->x679_x = 0;
            }

            // Fighter_ClampSpecificValue
            fp->x678++;
            if (fp->x678 > 0xFE) {
                fp->x678 = 0xFE;
            }

            if (fp->input.x650 >= p_ftCommonData->x18) {
                if (fp->input.x654 >= p_ftCommonData->x18) {
                    // Fighter_ClampThreeValues
                    fp->x672_input_timer_counter++;
                    if (fp->x672_input_timer_counter > 0xFE) {
                        fp->x672_input_timer_counter = 0xFE;
                    }
                    fp->x675++;
                    if (fp->x675 > 0xFE) {
                        fp->x675 = 0xFE;
                    }
                    fp->x67B++;
                    if (fp->x67B > 0xFE) {
                        fp->x67B = 0xFE;
                    }
                } else {
                    fp->x67B = 0;
                    fp->x678 = 0;
                    fp->x675 = 0;
                    fp->x672_input_timer_counter = 0;
                }
            } else {
                fp->x67B = 0xFE;
                fp->x675 = 0xFE;
                fp->x672_input_timer_counter = 0xFE;
            }

            if (fp->input.x668 & 0x100) {
                fp->x683 = fp->x67C;
                fp->x67C = 0;
            } else if (fp->x67C < 0xFF) {
                fp->x67C++;
            }

            if (fp->input.x668 & 0x200) {
                fp->x67D = 0;
            } else if (fp->x67D < 0xFF) {
                fp->x67D++;
            }

            if (fp->input.x668 & 0xC00) {
                fp->x67E = 0;
            } else if (fp->x67E < 0xFF) {
                fp->x67E++;
            }

            if (fp->input.x668 & 8) {
                fp->x681 = 0;
            } else if (fp->x681 < 0xFF) {
                fp->x681++;
            }

            if (fp->input.x668 & 4) {
                fp->x682 = 0;
            } else if (fp->x682 < 0xFF) {
                fp->x682++;
            }

            if (fp->input.x668 & 0x80000000) {
                fp->x67F = 0;
            } else if (fp->x67F < 0xFF) {
                fp->x67F++;
            }

            if (fp->input.x668 & 0x60) {
                fp->x684 = fp->x680;
                fp->x680 = 0;
            } else if (fp->x680 < 0xFF) {
                fp->x680++;
            }
        }

        if (fp->x221D_flag.bits.b4 || fp->x2224_flag.bits.b2 ||
            func_801A45E8(2))
        {
            fp->input.x630 = fp->input.x620_lstick_x;
            fp->input.x634 = fp->input.x624_lstick_y;
            fp->input.x648 = fp->input.x638_lsubStick_x;
            fp->input.x64C = fp->input.x63C_lsubStick_y;
            fp->input.x658 = fp->input.x650;
            fp->input.x664 = fp->input.x65C_heldInputs;
            fp->x221D_flag.bits.b3 = 0;

            Fighter_UnkInitLoad_80068914_Inner1(fighter_gobj);
        }

        if (!fp->x2219_flag.bits.b5) {
            if (fp->x1980) {
                func_8007FFD8(fp, p_ftCommonData->x6FC);
            }
            func_800DF0D0(fighter_gobj);
            func_8008031C(fighter_gobj);
            Fighter_UnkIncrementCounters_8006ABEC(fighter_gobj);

            if (fp->cb.x219C_callback_IASA) {
                fp->cb.x219C_callback_IASA(fighter_gobj);
            }
        }
    }
}

//// https://decomp.me/scratch/oFu1o
#define VEC_CLEAR(vec)                                                         \
    do {                                                                       \
        Vec3* vecLocal = (void*) &vec;                                         \
        f32 c = 0;                                                             \
        vecLocal->x = vecLocal->y = vecLocal->z = c;                           \
    } while (0)

void Fighter_procUpdate(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    Vec3 windOffset;

    if (fp->x221F_flag.bits.b3) {
        return;
    }

    if (!fp->x2219_flag.bits.b5) {
        Vec3* p_kb_vel;
        Vec3* pAtkShieldKB;
        Vec3 selfVel;
        float kb_vel_x, kb_vel_y, atkShieldKB_X;

        if (fp->x2064_ledgeCooldown)
            fp->x2064_ledgeCooldown -= 1;

        if (fp->x2108)
            fp->x2108 -= 1;

        func_800C0A98(fighter_gobj);

        if (fp->cb.x21A4_callback_Phys) {
            fp->cb.x21A4_callback_Phys(fighter_gobj);
        }

        p_kb_vel = &fp->x8c_kb_vel;
        if ((kb_vel_x = p_kb_vel->x) != 0 || p_kb_vel->y != 0) {
            if (fp->xE0_ground_or_air == GA_Air) {
                kb_vel_x = p_kb_vel->x;
                kb_vel_y = p_kb_vel->y;

                if (fp->x2228_flag.bits.b2) {
                    p_kb_vel->x = func_8007CD6C(p_kb_vel->x, func_8007CDA4(fp));
                    ;
                    p_kb_vel->y = func_8007CD6C(p_kb_vel->y, func_8007CDF8(fp));
                    ;
                } else {
                    float kb_angle = atan2f(kb_vel_y, kb_vel_x);

                    if (sqrtf(kb_vel_x * kb_vel_x + kb_vel_y * kb_vel_y) <
                        p_ftCommonData->x204_knockbackFrameDecay)
                    {
                        p_kb_vel->x = p_kb_vel->y = 0;
                    } else {
                        p_kb_vel->x -=
                            p_ftCommonData->x204_knockbackFrameDecay *
                            cosf(kb_angle);
                        p_kb_vel->y -=
                            p_ftCommonData->x204_knockbackFrameDecay *
                            sinf(kb_angle);
                    }
                }

                fp->xF0_ground_kb_vel = 0;
            } else {
                Vec3* pNormal = &fp->x6F0_collData.x14C_ground.normal;
                struct attr* pAttr;

                if (fp->xF0_ground_kb_vel == 0)
                    fp->xF0_ground_kb_vel = kb_vel_x;

                pAttr = &fp->x110_attr;
                func_8007CCA0(fp,
                              /*effective friction - ground multiplier is
                                 usually 1. last factor was 1 when I looked*/
                              /*effective friction - ground multiplier is
                                 usually 1. last factor was 1 when I looked*/
                              Stage_GetGroundFrictionMultiplier(fp) *
                                  pAttr->x128_GroundFriction *
                                  p_ftCommonData->x200);

                // set knockback velocity to ground_kb_vel * surfaceTangent
                p_kb_vel->x = pNormal->y * fp->xF0_ground_kb_vel;
                p_kb_vel->y = -pNormal->x * fp->xF0_ground_kb_vel;
            }
        }
        // Now handle the attacker's shield knockback in a similar way
        pAtkShieldKB = &fp->x98_atk_shield_kb;
        if ((atkShieldKB_X = pAtkShieldKB->x) != 0 || pAtkShieldKB->y != 0) {
            if (fp->xE0_ground_or_air == GA_Air) {
                float kb_x = pAtkShieldKB->x;
                float kb_y = pAtkShieldKB->y;
                float atkShieldKBAngle = atan2f(kb_y, kb_x);

                if (sqrtf(kb_x * kb_x + kb_y * kb_y) <
                    p_ftCommonData->x3E8_shieldKnockbackFrameDecay)
                {
                    // BUG IN THE MELEE CODE THAT CAUSES THE INVISIBLE CEILING
                    // GLITCH The next line should be 'pAtkShieldKB->y = 0', but
                    // instead it is:
                    pAtkShieldKB->x = p_kb_vel->y = 0;
                } else {
                    // again, the better implementation would be:
                    // *pAtkShieldKB *= (atkShieldKB_len -
                    // p_stc_ftcommon->x3e8_shield_kb_frameDecay)/atkShieldKB_len
                    // float atkShieldKBAngle = atan2_80022C30(pAtkShieldKB->y,
                    // pAtkShieldKB->x);
                    pAtkShieldKB->x -=
                        p_ftCommonData->x3E8_shieldKnockbackFrameDecay *
                        cosf(atkShieldKBAngle);
                    pAtkShieldKB->y -=
                        p_ftCommonData->x3E8_shieldKnockbackFrameDecay *
                        sinf(atkShieldKBAngle);
                }
                fp->xF4_ground_attacker_shield_kb_vel = 0;
            } else {
                Vec3* pNormal =
                    &fp->x6F0_collData.x14C_ground
                         .normal; // ground_normal offset inside fp is 0x844,
                                  // surface normal points out of the surface.
                struct attr* pAttr;

                if (fp->xF4_ground_attacker_shield_kb_vel == 0)
                    fp->xF4_ground_attacker_shield_kb_vel = atkShieldKB_X;

                pAttr = &fp->x110_attr;

                func_8007CE4C(
                    fp,
                    /* effectiveFriction - the last constant variable differs
                       from the one for the knockback friction above*/
                    Stage_GetGroundFrictionMultiplier(fp) *
                        pAttr->x128_GroundFriction *
                        p_ftCommonData->x3EC_shieldGroundFrictionMultiplier);

                /* effectiveFriction - the last constant variable differs from
                 * the one for the knockback friction above*/
                pAtkShieldKB->x =
                    pNormal->y * fp->xF4_ground_attacker_shield_kb_vel;
                pAtkShieldKB->y =
                    -pNormal->x * fp->xF4_ground_attacker_shield_kb_vel;
            }
        }

        fp->xEC_ground_vel += fp->xE4_ground_accel_1 + fp->xE8_ground_accel_2;
        fp->xE4_ground_accel_1 = fp->xE8_ground_accel_2 = 0;

        // self_vel += anim_vel
        PSVECAdd(&fp->x80_self_vel, &fp->x74_anim_vel, &fp->x80_self_vel);
        VEC_CLEAR(fp->x74_anim_vel);

        // copy selfVel into a stack storage variable
        selfVel = fp->x80_self_vel;

        // TODO: these double_lower_32bit variables are probably integer
        // counters that get decremented each frame, but I was not able to
        // trigger the following condition. The double value construction then
        // is only used as an interpolation tool between selfVel and some
        // UnkVel2.
        if (fp->dmg.x1948 != 0) {
            // The compiler casts an u32 integer 'val' to a double type using
            // double v = *(double*)&(0x43300000_00000000 | val ^ 0x80000000) -
            // *(double*)&43300000_80000000 which is all that happens in the
            // lengthy assembly generated by this
            float C1 = 1.0f;
            float C2 = C1 - (float) fp->dmg.x194C / (float) fp->dmg.x1948;

            selfVel.x = C2 * (fp->x80_self_vel.x - fp->xA4_unk_vel.x) +
                        fp->xA4_unk_vel.x;
            selfVel.y = C2 * (fp->x80_self_vel.y - fp->xA4_unk_vel.y) +
                        fp->xA4_unk_vel.y;

            fp->dmg.x194C--;
            if (fp->dmg.x194C == 0)
                fp->dmg.x1948 = 0;
        }

        // add some horizontal+depth offset to the position? Why is there no
        // vertical component?
        fp->xB0_pos.x += fp->xF8_playerNudgeVel.x;
        fp->xB0_pos.y += 0;
        fp->xB0_pos.z += fp->xF8_playerNudgeVel.y;

        if (fp->x2222_flag.bits.b6 && !fp->x2222_flag.bits.b7) {
            s32 bit;

            // fp->xD4_unk_vel += selfVel
            PSVECAdd(&fp->xD4_unk_vel, &selfVel, &fp->xD4_unk_vel);

            fp->xD4_unk_vel.x += p_kb_vel->x;
            fp->xD4_unk_vel.y += p_kb_vel->y;
            fp->xD4_unk_vel.z += 0.0f;

            if (fp->x2210_ThrowFlags.b2) {
                fp->x2210_ThrowFlags.b2 = 0;
                bit = 1;
            } else
                bit = 0;

            /// @todo @c incompatible-pointer-types because bad headers
            if (bit || func_80070FD0(fp) || fp->x594_animCurrFlags1.bits.b7) {
                // fp->xB0_position += fp->xD4_unk_vel
                PSVECAdd(&fp->xB0_pos, &fp->xD4_unk_vel, &fp->xB0_pos);
                /// @todo We set this velocity to 0 after applying it.
                ///       Is this SDI or ASDI?
                VEC_CLEAR(fp->xD4_unk_vel);
            }
            // fp->xB0_position += *pAtkShieldKB
            PSVECAdd(&fp->xB0_pos, (Vec3*) pAtkShieldKB, &fp->xB0_pos);
        } else {
            // fp@r31.position@0xB0.xyz += selfVel + pAtkShieldKB
            PSVECAdd(&fp->xB0_pos, &selfVel, &fp->xB0_pos);
            fp->xB0_pos.x += p_kb_vel->x;
            fp->xB0_pos.y += p_kb_vel->y;
            fp->xB0_pos.z += 0;

            PSVECAdd(&fp->xB0_pos, (Vec3*) pAtkShieldKB, &fp->xB0_pos);
        }
        // accumulate wind hazards into the windOffset vector
        func_getWindOffsetVec_8007B924(fighter_gobj,
                                       /*result vec3*/ &windOffset);
    } else {
        VEC_CLEAR(windOffset);
    }

    func_80076528(fighter_gobj);

    if (fp->cb.x21D0_callback_EveryHitlag) {
        fp->cb.x21D0_callback_EveryHitlag(fighter_gobj);
    }

    if (fp->xE0_ground_or_air == GA_Ground) {
        Vec3 difference;
        // I think this function always returns r3=1, but it contains two
        // __assert functions. But I guess these just stop or reset the game.
        // result is written to where r5 points to, which is 'difference' in
        // this case
        if (func_800567C0(fp->x6F0_collData.x14C_ground.index, &fp->xB0_pos,
                          &difference))
            // fp->position += difference
            PSVECAdd(&fp->xB0_pos, &difference, &fp->xB0_pos);
    }

    fp->xB0_pos.x += windOffset.x;
    fp->xB0_pos.y += windOffset.y;
    fp->xB0_pos.z += windOffset.z;

    // TODO: do the bitflag tests here tell us if the player is dead?
    func_800D3158(fighter_gobj);

    if (fp->x2225_b0) {
        // if position.y crossed
        // (0.25*stage.blastBottom+0.75*stage.cameraBottom) +
        // stage.crowdReactStart from below...
        if (fp->xBC_prevPos.y <= Stage_CalcUnkCamYBounds() &&
            fp->xB0_pos.y > Stage_CalcUnkCamYBounds())
        {
            fp->x2225_b0 = 0;
        }
    } else {
        if (!fp->x222A_flag.bits.b1 && !fp->x2228_flag.bits.b5) {
            // if position.y crossed 0.5*(stage.blastBottom+stage.cameraBottom)
            // + stage.crowdReactStart from above...
            if (fp->xBC_prevPos.y >= Stage_CalcUnkCamY() &&
                fp->xB0_pos.y < Stage_CalcUnkCamY())
            {
                // plays this sound you always hear when you get close to the
                // bottom blast zone
                func_80088148(fp, 96, 127, 64);
                fp->x2225_b0 = 1;
            }
        }
    }

    if (fp->dmg.x18A4_knockbackMagnitude && !fp->x221C_flag.bits.b6 &&
        !func_80322258(fp->xB0_pos.x))
    {
        fp->dmg.x18A4_knockbackMagnitude = 0.0f;
    }

    func_8007AF28(fighter_gobj);

    if (g_debugLevel >= 3 && (fpclassify(fp->xB0_pos.x) == FP_NAN ||
                              fpclassify(fp->xB0_pos.y) == FP_NAN ||
                              fpclassify(fp->xB0_pos.z) == FP_NAN))
    {
        OSReport("fighter procUpdate pos error.\tpos.x=%f\tpos.y=%f\n",
                 fp->xB0_pos.x, fp->xB0_pos.y);
        __assert(__FILE__, /*line*/ 2517, "0");
    }
}

void Fighter_UnkApplyTransformation_8006C0F0(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->x34_scale.z != 1.0f) {
        HSD_JObj* jobj = GET_JOBJ(fighter_gobj);
        Mtx mtx1;
        Mtx mtx2;
        Vec3 scale;
        Vec3 translation;
        Quaternion rotation;

        /// @todo This usually implies an inline, but inlines reserve more stack
        ///       (usually in 8 byte increments) than a single allocation
        ///       (removed) like this.
        jobj = GET_JOBJ(fighter_gobj);

        HSD_JObjSetupMatrix(jobj);
        HSD_JObjGetMtx(jobj);
        HSD_MtxInverse(jobj->mtx, mtx1);
        HSD_JObjGetScale(jobj, &scale);

        scale.x = Fighter_GetModelScale(fp);

        HSD_JObjGetRotation(jobj, &rotation);
        HSD_JObjGetTranslation(jobj, &translation);

        HSD_MtxSRT(mtx2, &scale, (Vec3*) &rotation, &translation, 0);
        PSMTXConcat(mtx2, mtx1, fp->x44_mtx);
    }
}

static inline float Fighter_GetPosX(Fighter* fp)
{
    return fp->xB0_pos.x;
}

static inline float Fighter_GetPosY(Fighter* fp)
{
    return fp->xB0_pos.y;
}

void Fighter_8006C27C(HSD_GObj* fighter_gobj)
{
    Fighter* fp = (Fighter*) HSD_GObjGetUserData(fighter_gobj);

    if (!fp->x221F_flag.bits.b3) {
        if (fp->x6F0_collData.x19C) {
            fp->x6F0_collData.x19C--;
            if (!fp->x6F0_collData.x19C) {
                func_8007D5BC(fp);
            }
        }

        fp->x2223_flag.bits.b5 = 0;

        HSD_JObjSetTranslate(fighter_gobj->hsd_obj, &fp->xB0_pos);

        if (fp->cb.x21A8_callback_Coll) {
            fp->cb.x21A8_callback_Coll(fighter_gobj);
            func_800F1D24(fighter_gobj);
        }

        if (fp->xE0_ground_or_air == GA_Ground) {
            func_80041280(fp->xC_playerID, fp->x221F_flag.bits.b4);
        }

        if (g_debugLevel >= 3) {
            if (fpclassify(fp->xB0_pos.x) == FP_NAN ||
                fpclassify(fp->xB0_pos.y) == FP_NAN ||
                fpclassify(fp->xB0_pos.z) == FP_NAN)
            {
                f32 x = Fighter_GetPosX(fp);
                f32 y = Fighter_GetPosY(fp);
                OSReport("fighter procMap pos error.\tpos.x=%f\tpos.y=%f\n", x,
                         y);
                __assert("fighter.c", 2590, "0");
            }
        }

        HSD_JObjSetTranslate(fighter_gobj->hsd_obj, &fp->xB0_pos);
    }
}

void Fighter_8006C5F4(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    if (!fp->x221F_flag.bits.b3)
        func_80089B08(fighter_gobj);
}

void Fighter_CallAcessoryCallbacks_8006C624(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[4];
#endif

    if (!fp->x221F_flag.bits.b3) {
        if (fp->x2219_flag.bits.b5) {
            if (fp->cb.x21B8_callback_Accessory3) {
                fp->cb.x21B8_callback_Accessory3(fighter_gobj);
            }
            return;
        }

        if (fp->cb.x21B4_callback_Accessory2) {
            fp->cb.x21B4_callback_Accessory2(fighter_gobj);
            HSD_JObjSetTranslate(fighter_gobj->hsd_obj, &fp->xB0_pos);
        }

        if (fp->cb.x21B0_callback_Accessory1) {
            fp->cb.x21B0_callback_Accessory1(fighter_gobj);
            HSD_JObjSetTranslate(fighter_gobj->hsd_obj, &fp->xB0_pos);
        }
    }
}

void Fighter_8006C80C(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (!fp->x221F_flag.bits.b3) {
        func_80067624(fighter_gobj, &fp->x60C);
        Fighter_UnkApplyTransformation_8006C0F0(fighter_gobj);

        if (!fp->x2219_flag.bits.b5) {
            if (fp->cb.x21BC_callback_Accessory4) {
                fp->cb.x21BC_callback_Accessory4(fighter_gobj);
            }
        }

        func_8007AE80(fighter_gobj);
        func_8007C224(fighter_gobj);
        func_8007C6DC(fighter_gobj);

        if (fp->x20A0_accessory) {
            HSD_JObjAnimAll(fp->x20A0_accessory);
        }

        if (fp->xE0_ground_or_air == GA_Air &&
            fp->xB0_pos.y < Stage_GetCamBoundsBottomOffset())
        {
            if (func_802FB6E8(fp->xC_playerID) == 3) {
                Vec3 cam_offset;
                Stage_UnkSetVec3TCam_Offset(&cam_offset);

                if (fp->xB0_pos.y + cam_offset.y < fp->x2140) {
                    fp->x2140 = fp->xB0_pos.y + cam_offset.y;
                }
            }
        }
    }
}

void Fighter_UnkProcessGrab_8006CA5C(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (!fp->x221F_flag.bits.b3 && !func_8016B1C4()) {
        func_8007BA0C(fighter_gobj);
        if (fp->x221E_flag.bits.b6) {
            func_80078A2C(fighter_gobj);
            if (fp->x1A58_interactedFighter) {
                if (!fp->x2225_b1) {
                    func_80088148(fp, fp->x10C_ftData->x4C_collisionData->x30,
                                  0x7F, 0x40);
                }
                func_80078754(fighter_gobj, fp->x1A58_interactedFighter, 0);
                fp->cb.x2190_callback_OnGrabFighter_Self(fighter_gobj);
                fp->cb.x2198_callback_OnGrabFighter_Victim(
                    fp->x1A58_interactedFighter, fighter_gobj);
                return;
            }
            func_8007BC90(fighter_gobj);

            if (fp->x1A60) {
                if (!fp->x2225_b1) {
                    func_80088148(fp, fp->x10C_ftData->x4C_collisionData->x30,
                                  0x7F, 0x40);
                }
                func_8027B4A4(fighter_gobj, fp->x1A60);
                if (fp->cb.x2194_callback) {
                    fp->cb.x2194_callback(fighter_gobj);
                }
            }
        }
    }
}

void Fighter_8006CB94(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    f32 func_8007BBCC_float_output;

    if (!fp->x221F_flag.bits.b3 && !fp->x2219_flag.bits.b1) {
        func_800765E0();
        func_80078C70(fighter_gobj);
        func_8007C77C(fighter_gobj);
        func_8007925C(fighter_gobj);
        func_8007BAC0(fighter_gobj);
        func_8007C4BC(fighter_gobj);
        func_8007AB48(fighter_gobj);
        func_8007AB80(fighter_gobj);
        func_8007BBCC_float_output = func_8007BBCC(fighter_gobj);
        if (func_8007BBCC_float_output > 0.0f) {
            func_8007FC7C(fighter_gobj, func_8007BBCC_float_output);
        }
    }
}

void Fighter_UnkTakeDamage_8006CC30(Fighter* fp, f32 arg0)
{
    Fighter_TakeDamage_8006CC7C(fp, arg0);
    func_8007EA90(fp, arg0);
}

void Fighter_TakeDamage_8006CC7C(Fighter* fp, f32 damage_amount)
{
    if (!fp->x2226_flag.bits.b4 || fp->x2226_flag.bits.b3) {
        fp->dmg.x1830_percent += damage_amount;
        if (fp->x2028) {
            fp->x202C -= damage_amount;
        }

        if (fp->x2034) {
            fp->x2038 -= damage_amount;
        }

        if (fp->dmg.x1830_percent > 999.0f) {
            fp->dmg.x1830_percent = 999.0f;
        }
        Player_SetHPByIndex(fp->xC_playerID, fp->x221F_flag.bits.b4,
                            fp->dmg.x1830_percent);
        func_8003EC9C(fp->xC_playerID, fp->x221F_flag.bits.b4,
                      fp->dmg.x1830_percent, damage_amount);
        func_800C8C84(fp->x0_fighter);
    }
}

/// https://decomp.me/scratch/9QvFG
void Fighter_8006CDA4(Fighter* fp, s32 arg1, s32 arg2, s32 arg3)
{
    bool temp_bool;
    bool hold_item_bool = 0;
    Vec3 vec;

    if (fp->x1974_heldItem && !func_8026B2B4(fp->x1974_heldItem)) {
        hold_item_bool = 1;
    }

    temp_bool = !((fp->x2220_flag.bits.b3 || fp->x2220_flag.bits.b4 ||
                   func_8008E984(fp)));
    vec = vec3_803B7494;

    if (fp->action_id != 0x145 && (unsigned) fp->action_id - 0x122 > 1 &&
        fp->dmg.x1860_dealt != 0xAU && !fp->x2226_flag.bits.b2)
    {
        if ( ///// giant if condition
            hold_item_bool && temp_bool &&
            ((HSD_Randi(p_ftCommonData->x418) < arg1) ||
             ((((func_8026B30C(fp->x1974_heldItem) == 3) &&
                func_8026B594(fp->x1974_heldItem))) &&
              !HSD_Randi(p_ftCommonData->x41C))))
        {
            if (fp->x1978) {
                func_8026ABD8(fp->x1978, &vec, 1.0f);
            }
            func_8026ABD8(fp->x1974_heldItem, &vec, 1.0f);
        }
        if (fp->x197C) {
            if (HSD_Randi(p_ftCommonData->x418) < arg1) {
                func_8007F8E8(fp->x0_fighter);
                func_8026ABD8(fp->x197C, &vec, 1.0f);
                func_8007F9B4(fp->x0_fighter);
            }
        }
    }
}

void Fighter_8006CF5C(Fighter* fp, s32 arg1)
{
    if (!fp->x2224_flag.bits.b2) {
        fp->dmg.x18F0 += arg1;
        func_800BFFD0(fp, 8, 0);
        func_8007EBAC(fp, 2, 0);
    }
}

void Fighter_UnkSetFlag_8006CFBC(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->x2219_flag.bits.b7) {
        fp->x221A_flag.bits.b1 = 1;
    }
}

void Fighter_8006CFE0(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->x2219_flag.bits.b7) {
        if (!fp->x221A_flag.bits.b2) {
            if (!fp->dmg.x1954) {
                Fighter_8006D10C(fighter_gobj);
            }
        }
        fp->x2219_flag.bits.b7 = 0;
    }
}

inline void setBit(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    fp->x2219_flag.bits.b7 = 1;
}

void Fighter_UnkRecursiveFunc_8006D044(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->cb.x21D4_callback_EnterHitlag) {
        fp->cb.x21D4_callback_EnterHitlag(fighter_gobj);
    }

    fp->x2219_flag.bits.b5 = 1;

    if (fp->x1A5C && !fp->x2219_flag.bits.b7) {
        HSD_GObj* new_gobj = fighter_gobj;

        // @todo What is going on here?
        setBit(new_gobj = fp->x1A5C);

        Fighter_UnkRecursiveFunc_8006D044(new_gobj);
    }
}

static void Fighter_8006D10C_Inline2(Fighter* fp)
{
    HSD_GObj* gobj = fp->x1A5C;

    if (fp->x1A5C != NULL && !fp->x2219_flag.bits.b7) {
        Fighter_8006CFE0(gobj);
    }
}

static void Fighter_8006D10C_Inline1(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->x2219_flag.bits.b7) {
        if (!fp->x221A_flag.bits.b2 && !fp->dmg.x1954) {
            if (fp->cb.x21D8_callback_ExitHitlag) {
                fp->cb.x21D8_callback_ExitHitlag(fighter_gobj);
            }
            fp->x2219_flag.bits.b5 = 0;
            Fighter_8006D10C_Inline2(fp);
        }
        fp->x2219_flag.bits.b7 = 0;
    }
}

void Fighter_8006D10C(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->cb.x21D8_callback_ExitHitlag) {
        fp->cb.x21D8_callback_ExitHitlag(fighter_gobj);
    }

    fp->x2219_flag.bits.b5 = 0;

    if (fp->x1A5C != NULL && !fp->x2219_flag.bits.b7) {
        Fighter_8006D10C_Inline1(fp->x1A5C);
    }
}

void Fighter_UnkProcessShieldHit_8006D1EC(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    bool bool1 = 0;
    s32 action_state_index = fp->action_id;
    bool bool2 = 0;
    bool bool3 = 0;
    bool bool4 = 0;
    f32 forceAppliedOnHit;

    if (!fp->x221F_flag.bits.b3) {
        if (!fp->x221A_flag.bits.b7) {
            if (fp->x1998_shieldHealth < p_ftCommonData->x260_startShieldHealth)
            {
                fp->x1998_shieldHealth += p_ftCommonData->x27C;
                if (fp->x1998_shieldHealth >
                    p_ftCommonData->x260_startShieldHealth)
                {
                    fp->x1998_shieldHealth =
                        p_ftCommonData->x260_startShieldHealth;
                }
            }
        }

        if (fp->x221A_flag.bits.b7) {
            fp->x1998_shieldHealth -=
                (p_ftCommonData->x284 *
                 ((fp->x19A0_shieldDamageTaken) *
                  (1.0f - ((fp->x199C_shieldLightshieldAmt *
                            (p_ftCommonData->x2E0 - p_ftCommonData->x2DC)) +
                           p_ftCommonData->x2DC)))) +
                p_ftCommonData->x288;
            if (fp->x1998_shieldHealth < 0.0f) {
                bool3 = 1;
                fp->x1998_shieldHealth = p_ftCommonData->x280_unkShieldHealth;
                /// this function is called when shield is broken
                func_8003E058(fp->x19BC_shieldDamageTaken3,
                              fp->x221F_flag.bits.b6, fp->xC_playerID,
                              fp->x221F_flag.bits.b4);
            }
        }

        if (fp->dmg.x189C_unk_num_frames > 0.0f) {
            fp->dmg.x189C_unk_num_frames--;
            if (fp->dmg.x189C_unk_num_frames <= 0.0f &&
                !fp->dmg.x1850_forceApplied)
            {
                fp->dmg.x189C_unk_num_frames = 0.0f;
                func_8007BE3C(fighter_gobj);
            }
        }

        forceAppliedOnHit = fp->dmg.x1850_forceApplied;
        if (forceAppliedOnHit) {
            s32 ground_or_air = fp->xE0_ground_or_air;
            bool damage_bool;

            fp->dmg.x189C_unk_num_frames = 0.0f;
            Fighter_UnkTakeDamage_8006CC30(fp, fp->dmg.x1838_percentTemp);
            func_8008D930(fp);
            func_800F5BA4(fp);

            if (fp->cb.x21F0_callback) {
                fp->cb.x21F0_callback(fighter_gobj);
            }

            if (!fp->x2229_b7) {
                switch (fp->x1828) {
                case 0:
                    func_8008EC90(fighter_gobj);
                    break;
                case 1:
                    func_8007DB58(fighter_gobj);
                    func_8008E908(fighter_gobj, 0.0f);
                    break;
                case 2:
                    func_8007DB58(fighter_gobj);
                    func_8008E9D0(fighter_gobj);
                    break;
                case 3:
                    func_8007DB58(fighter_gobj);
                    func_8008EB58(fighter_gobj);
                    break;
                }

                damage_bool = fp->dmg.x183C_applied;
                bool2 = 1;
                func_80090594(fp, fp->dmg.x1860_dealt, damage_bool,
                              action_state_index, ground_or_air,
                              fp->x1960_vibrateMult);
                func_8007ED50(fp, fp->dmg.x1838_percentTemp);
                bool1 = damage_bool;

            } else {
                switch (fp->x4_fighterKind) {
                case 0x1B:
                    func_8014FE58(fighter_gobj);
                    break;
                case 0x1C:
                    func_80156014(fighter_gobj);
                    break;
                default:
                    OSReport("ellegal flag fp->no_reaction_always\n");
                    __assert(__FILE__, 3085, "0");
                }
                func_8008E9D0(fighter_gobj);
            }
        } else if (fp->dmg.x18a0) {
            bool1 = fp->dmg.x1840;
            bool4 = 1;
        } else if (fp->x19A4) {
            if (bool3) {
                func_80098B20(fighter_gobj);
                func_80088148(fp, 0x82, 0x7F, 0x40);
            } else {
                if (fp->cb.x21C4_callback_OnShieldHit) {
                    fp->cb.x21C4_callback_OnShieldHit(fighter_gobj);
                }
            }
            bool1 = fp->x19A4;
        } else if (fp->dmg.x1918) {
            if ((fp->dmg.x191C) && (!fp->x1A58_interactedFighter) &&
                (!fp->x1A60))
            {
                func_8007DB58(fighter_gobj);
                func_80099D9C(fighter_gobj);
            }
            bool1 = fp->dmg.x1918;
        } else if (fp->dmg.x1914) {
            if (fp->cb.x21C0_callback_OnGiveDamage) {
                fp->cb.x21C0_callback_OnGiveDamage(fighter_gobj);
            }
            bool1 = fp->dmg.x1914;
            if (fp->x2070.x2073 == 0x46U) {
                func_8007EBAC(fp, 0xE, 0);
            } else {
                func_8007EE0C(fp, fp->dmg.x1914);
            }
        } else {
            if (fp->dmg.x1924) {
                bool1 = fp->dmg.x1924;
            } else if (fp->ReflectAttr.x1A3C_damageOver) {
                func_80098C9C(fighter_gobj);
            } else if (fp->ReflectAttr.x1A2C_reflectHitDirection) {
                if (fp->cb.x21C8_callback_OnReflectHit) {
                    fp->cb.x21C8_callback_OnReflectHit(fighter_gobj);
                }
            } else if (fp->AbsorbAttr.x1A40_absorbHitDirection) {
                if (ft_OnAbsorb[fp->x4_fighterKind]) {
                    ft_OnAbsorb[fp->x4_fighterKind](fighter_gobj);
                }
            } else if (fp->x20AC) {
                if (fp->cb.x21F4_callback) {
                    fp->cb.x21F4_callback(fighter_gobj);
                }
            }
        }

        if (!forceAppliedOnHit && fp->dmg.x1838_percentTemp) {
            Fighter_UnkTakeDamage_8006CC30(fp, fp->dmg.x1838_percentTemp);
            func_800F5C34(fp);
            func_800804FC(fp);
        }
        func_800C8D00(fighter_gobj);

        if (bool1) {
            fp->dmg.x195c_hitlag_frames =
                func_8007DA74(bool1, action_state_index, fp->x1960_vibrateMult);
            if (fp->dmg.x195c_hitlag_frames < fp->x1964) {
                fp->dmg.x195c_hitlag_frames = fp->x1964;
            }
            if (fp->dmg.x195c_hitlag_frames > 0.0f) {
                if (fp->dmg.x195c_hitlag_frames >
                    p_ftCommonData->x194_unkHitLagFrames)
                {
                    fp->dmg.x195c_hitlag_frames =
                        p_ftCommonData->x194_unkHitLagFrames;
                }
                fp->x221A_flag.bits.b2 = 1;
                if (bool2) {
                    fp->x221A_flag.bits.b3 = 1;
                }
                if (bool4) {
                    fp->dmg.x189C_unk_num_frames = fp->dmg.x195c_hitlag_frames;
                }
                if (!fp->x2219_flag.bits.b5) {
                    Fighter_UnkRecursiveFunc_8006D044(fighter_gobj);
                }
            }
        } else {
            func_80090718(fp);
        }

        if (fp->x221A_flag.bits.b0 || fp->dmg.x1958) {
            if (!fp->x2219_flag.bits.b5) {
                Fighter_UnkRecursiveFunc_8006D044(fighter_gobj);
            }

            if (fp->x221A_flag.bits.b0) {
                fp->x2219_flag.bits.b7 = 1;
                fp->x221A_flag.bits.b0 = 0;
            } else {
                if (fp->dmg.x1958 > fp->dmg.x1954) {
                    fp->dmg.x1954 = fp->dmg.x1958;
                }
                fp->dmg.x1958 = 0.0f;
            }
        }

        if (fp->dmg.x1928) {
            float eval =
                (fp->dmg.x1928 * p_ftCommonData->x3E0) + p_ftCommonData->x3E4;
            fp->xF4_ground_attacker_shield_kb_vel =
                (fp->dmg.x192c < 0.0f) ? eval : -eval;
            func_8007E2A4(fighter_gobj);
        }

        fp->dmg.x1838_percentTemp = 0.0f;
        fp->dmg.x183C_applied = 0;
        fp->x1828 = 0;
        fp->dmg.x1850_forceApplied = 0.0f;
        fp->dmg.x18a0 = 0.0f;
        fp->dmg.x1840 = 0;
        fp->dmg.x1914 = 0;
        fp->dmg.x1918 = 0;
        fp->dmg.x191C = 0.0f;
        fp->x20AC = NULL;
        fp->x221C_flag.bits.b5 = 0;

        fp->dmg.x1924 = 0;
        fp->dmg.x1928 = 0.0f;
        fp->x19A0_shieldDamageTaken = 0;
        fp->x19A4 = 0;
        fp->x19A8 = 0;
        fp->ReflectAttr.x1A3C_damageOver = 0;
        fp->ReflectAttr.x1A2C_reflectHitDirection = 0.0f;
        fp->AbsorbAttr.x1A40_absorbHitDirection = 0.0f;
        fp->AbsorbAttr.x1A44_damageTaken = 0;
        fp->AbsorbAttr.x1A48_hitsTaken = 0;
        fp->x1960_vibrateMult = 1.0f;
        fp->x1964 = 0.0f;
        fp->dmg.x1950 = 0;

        if (!fp->x2219_flag.bits.b6 || fp->dmg.x18F4) {
            func_800C2FD8(fighter_gobj);
        }
        func_800A0DA4(fp);
    }
}

void Fighter_8006D9AC(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (fp->x221F_flag.bits.b3 || fp->x2219_flag.bits.b5)
        return;

    func_8009E0A8(fighter_gobj);
}

void Fighter_UnkCallCameraCallback_8006D9EC(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (!fp->x221F_flag.bits.b3) {
        func_8008021C(fighter_gobj);
        if (fp->cb.x21AC_callback_Cam) {
            fp->cb.x21AC_callback_Cam(fighter_gobj);
        }
    }
}

void Fighter_8006DA4C(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);

    if (!fp->x221F_flag.bits.b3) {
        Player_80032828(fp->xC_playerID, fp->x221F_flag.bits.b4, &fp->xB0_pos);
        Player_SetFacingDirectionConditional(
            fp->xC_playerID, fp->x221F_flag.bits.b4, fp->facing_dir);
        func_8003FAA8(fp->xC_playerID, fp->x221F_flag.bits.b4, &fp->xB0_pos,
                      &fp->xBC_prevPos);
    }
}

void Fighter_Unload_8006DABC(void* user_data)
{
    /// @remarks This doesn't use #GET_FIGHTER, but since it appears to pass it
    ///          directly it's probably just written directly.
    Fighter* fp = (Fighter*) (user_data);
    int kind = fp->x4_fighterKind;

    if (ft_OnUserDataRemove[kind]) {
        ft_OnUserDataRemove[kind](fp->x0_fighter);
    }

    func_8007B8E8(fp->x0_fighter);
    func_80067688(&fp->x60C);
    func_8026B7F8(fp->x0_fighter);
    func_800290D4(fp->x890_cameraBox);
    func_8009E0D4(fp);
    func_800765AC(fp->x0_fighter);
    func_80088C5C(fp->x0_fighter);
    func_8000EE8C(&fp->x20A4);

    if (fp->x20A0_accessory) {
        HSD_JObjRemoveAll(fp->x20A0_accessory);
    }

    HSD_JObjRemoveAll(fp->x8AC_animSkeleton);
    HSD_JObjUnref(fp->x2184);
    func_800859A8(fp);
    HSD_LObjRemoveAll(fp->x588);
    Player_80031FB0(fp->xC_playerID, fp->x221F_flag.bits.b4);

    HSD_ObjFree(&lbl_804590AC, fp->x59C);
    HSD_ObjFree(&lbl_804590AC, fp->x5A0);
    HSD_ObjFree(&fighter_bones_alloc_data, fp->x5E8_fighterBones);
    HSD_ObjFree(&lbl_80459054, fp->x5EC_dobj_list.data);
    HSD_ObjFree(&lbl_80459080, fp->x2040);
    HSD_ObjFree(&lbl_80458FFC, fp->x2D8_specialAttributes2);
    HSD_ObjFree(&fighter_alloc_data, fp);
}
