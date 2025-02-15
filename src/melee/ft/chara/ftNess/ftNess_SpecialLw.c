#include <melee/ft/chara/ftNess/ftNess_SpecialLw.h>

#include <melee/ef/efasync.h>
#include <melee/ef/eflib.h>
#include <melee/ft/chara/ftNess/ftNess.h>
#include <melee/ft/code_80081B38.h>
#include <melee/ft/ft_unknown_006.h>
#include <melee/ft/ftcoll.h>
#include <melee/ft/ftcommon.h>
#include <melee/ft/ftparts.h>
#include <melee/pl/pl_unknown_001.h>

// 0x80119E14 //
// https://decomp.me/scratch/LwTKg //
void ftNess_SpecialLwStart_Action(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet Start Action State
                            // handler //
{
    ftNessAttributes* ness_attr;
    Fighter* temp_fp;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[20];
#endif

    temp_fp = GET_FIGHTER(fighter_gobj);
    ness_attr = temp_fp->x2D4_specialAttributes;
    temp_fp->nessVars.SpecialLw.releaseLag =
        (s32) ness_attr->x74_PSI_MAGNET_RELEASE_LAG;
    temp_fp->nessVars.SpecialLw.isRelease = 0;
    temp_fp->nessVars.SpecialLw.gravityDelay =
        (s32) ness_attr->x84_PSI_MAGNET_FRAMES_BEFORE_GRAVITY;
    temp_fp->x2350_stateVar5 = 0;
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_START, 0,
                                       NULL, 0.0f, 1.0f, 0.0f);
    func_8006EBA4(fighter_gobj);
}

// 0x80119E90 //
// https://decomp.me/scratch/ckNxx //
void ftNess_SpecialAirLwStart_Action(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Start Action State handler //
{
    Fighter* temp_fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[20];
#endif

    temp_fp = GET_FIGHTER(fighter_gobj);
    ness_attr = temp_fp->x2D4_specialAttributes;
    temp_fp->nessVars.SpecialLw.releaseLag =
        (s32) ness_attr->x74_PSI_MAGNET_RELEASE_LAG;
    temp_fp->nessVars.SpecialLw.isRelease = 0;
    temp_fp->nessVars.SpecialLw.gravityDelay =
        (s32) ness_attr->x84_PSI_MAGNET_FRAMES_BEFORE_GRAVITY;
    temp_fp->x2350_stateVar5 = 0;
    temp_fp->x80_self_vel.y = 0.0f;
    temp_fp->x80_self_vel.x /= ness_attr->x88_PSI_MAGNET_MOMENTUM_PRESERVATION;
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_START,
                                       0, NULL, 0.0f, 1.0f, 0.0f);
    func_8006EBA4(fighter_gobj);
}

// 0x80119F20 //
// https://decomp.me/scratch/frKYd //
void ftNess_SpecialLwStart_Anim(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Start Animation callback //
{
    Fighter* fp;
    Fighter* fighter_data2;

    fighter_data2 = GET_FIGHTER(fighter_gobj);

    if (FLAGS_NONE(fighter_data2->input.x65C_heldInputs, HSD_BUTTON_B)) {
        fighter_data2->nessVars.SpecialLw.isRelease = 1;
    }

    if (!ftAnim_IsFramesRemaining(fighter_gobj)) {
        fp = GET_FIGHTER(fighter_gobj);

        if (fp->x2219_flag.bits.b0 == 0) {
            efAsync_Spawn(fighter_gobj, &fp->x60C, 0U, 0x4F0U,
                          fp->x5E8_fighterBones[23].x0_jobj);
            fp->x2219_flag.bits.b0 = 1;
        }
        fp->cb.x21D4_callback_EnterHitlag = efLib_PauseAll;
        fp->cb.x21D8_callback_ExitHitlag = efLib_ResumeAll;
        fighter_data2->x2350_stateVar5 = 0;
        if ((s32) fighter_data2->xE0_ground_or_air == GA_Ground) {
            ftNess_SpecialLwHold_Action(fighter_gobj);
            return;
        }
        ftNess_SpecialAirLwHold_Action(fighter_gobj);
    }
}

// 0x8011A000
// https://decomp.me/scratch/psrOE //
void ftNess_SpecialAirLwStart_Anim(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Start Animation callback //
{
    Fighter* fighter_data2;
    Fighter* fp;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[8];
#endif

    fighter_data2 = fp = GET_FIGHTER(fighter_gobj);

    if ((fighter_data2->input.x65C_heldInputs & HSD_BUTTON_B) == false) {
        fighter_data2->nessVars.SpecialLw.isRelease = 1;
    }

    if (!ftAnim_IsFramesRemaining(fighter_gobj)) {
        fp = GET_FIGHTER(fighter_gobj);

        if (fp->x2219_flag.bits.b0 == 0) {
            efAsync_Spawn(fighter_gobj, &fp->x60C, 0U, 0x4F0U,
                          fp->x5E8_fighterBones[23].x0_jobj);
            fp->x2219_flag.bits.b0 = 1;
        }
        fp->cb.x21D4_callback_EnterHitlag = efLib_PauseAll;
        fp->cb.x21D8_callback_ExitHitlag = efLib_ResumeAll;
        fighter_data2->x2350_stateVar5 = 0;
        if ((s32) fighter_data2->xE0_ground_or_air == GA_Ground) {
            ftNess_SpecialLwHold_Action(fighter_gobj);
            return;
        }
        ftNess_SpecialAirLwHold_Action(fighter_gobj);
    }
}

// 0x8011A0E0 //
void ftNess_SpecialLwStart_IASA(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet Start IASA callback //
{
    return;
}

// 0x8011A0E4 //
void ftNess_SpecialAirLwStart_IASA(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Start IASA callback //
{
    return;
}

// 0x8011A0E8 //
// https://decomp.me/scratch/GCJrl //
void ftNess_SpecialLwStart_Phys(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Start Physics callback //
{
    func_80084F3C(fighter_gobj);
}

// 0x8011A108 //
// https://decomp.me/scratch/4zHBZ //
void ftNess_SpecialAirLwStart_Phys(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Start Physics callback //
{
    Fighter* fp;
    s32 unused[2];
    s32 gravityDelay;
    attr* attr;
    ftNessAttributes* ness_attr;

    fp = GET_FIGHTER(fighter_gobj);
    attr = &fp->x110_attr;
    ness_attr = fp->x2D4_specialAttributes;
    gravityDelay = fp->nessVars.SpecialLw.gravityDelay;
    if (gravityDelay != 0) {
        fp->nessVars.SpecialLw.gravityDelay = gravityDelay - 1;
        goto block_end;
    }

    func_8007D494(fp, ness_attr->x8C_PSI_MAGNET_FALL_ACCEL,
                  attr->x170_TerminalVelocity);
block_end:
    func_8007CF58(fp);
}

// 0x8011A168 //
// https://decomp.me/scratch/S8g3b //
void ftNess_SpecialLwStart_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Start Collision callback //
{
    if (func_80082708(fighter_gobj) == false) {
        ftNess_SpecialLwStart_GroundToAir(fighter_gobj);
    }
}

// 0x8011A1A4 //
// https://decomp.me/scratch/eNYIn //
void ftNess_SpecialAirLwStart_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Start Collision callback //
{
    if (func_80081D0C(fighter_gobj) != false) {
        ftNess_SpecialAirLwStart_AirToGround(fighter_gobj);
    }
}

// 0x8011A1E0 //
// https://decomp.me/scratch/erSzB //
void ftNess_SpecialLwStart_GroundToAir(
    HSD_GObj* fighter_gobj) // Ness's ground->air PSI Magnet Start Action State
                            // handler //
{
    Fighter* fp;

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D5D4(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_START,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
}

// 0x8011A240 //
// https://decomp.me/scratch/153K2 //
void ftNess_SpecialAirLwStart_AirToGround(
    HSD_GObj* fighter_gobj) // Ness's air->ground PSI Magnet Start Action State
                            // handler //
{
    Fighter* fp;

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D7FC(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_START,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
    func_8007D468(fp);
}

// 0x8011A2A8 //
// https://decomp.me/scratch/Wlutx //
void ftNess_SpecialLwHold_Anim(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Hold Animation callback //
{
    Fighter* temp_r31;
    Fighter* fp;

    fp = GET_FIGHTER(fighter_gobj);
    if ((fp->input.x65C_heldInputs & HSD_BUTTON_B) == false) {
        fp->nessVars.SpecialLw.isRelease = 1;
    }

    if (fp->nessVars.SpecialLw.releaseLag > 0) {
        fp->nessVars.SpecialLw.releaseLag--;
    }
    if (((s32) fp->nessVars.SpecialLw.releaseLag <= 0) &&
        ((s32) fp->nessVars.SpecialLw.isRelease != 0))
    {
        if ((s32) fp->xE0_ground_or_air == GA_Ground) {
            ftNess_SpecialLwEnd_Action(fighter_gobj);
        } else {
            ftNess_SpecialAirLwEnd_Action(fighter_gobj);
        }
    }
    temp_r31 = GET_FIGHTER(fighter_gobj);

    temp_r31->x2350_stateVar5--;

    if ((s32) temp_r31->x2350_stateVar5 <= 0) {
        func_80088478(temp_r31, 0x334A1, 0x7F, 0x40);
        temp_r31->x2350_stateVar5 = 0x28;
    }
}

// 0x8011A370 //
// https://decomp.me/scratch/UbQAr //
void ftNess_SpecialAirLwHold_Anim(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Hold Animation callback //
{
    Fighter* temp_r31;
    Fighter* fp;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[8];
#endif

    fp = GET_FIGHTER(fighter_gobj);
    if ((fp->input.x65C_heldInputs & HSD_BUTTON_B) == false) {
        fp->nessVars.SpecialLw.isRelease = 1;
    }

    if (fp->nessVars.SpecialLw.releaseLag > 0) {
        fp->nessVars.SpecialLw.releaseLag =
            (s32) (fp->nessVars.SpecialLw.releaseLag - 1);
    }
    if (((s32) fp->nessVars.SpecialLw.releaseLag <= 0) &&
        ((s32) fp->nessVars.SpecialLw.isRelease != 0))
    {
        if ((s32) fp->xE0_ground_or_air == GA_Ground) {
            ftNess_SpecialLwEnd_Action(fighter_gobj);
        } else {
            ftNess_SpecialAirLwEnd_Action(fighter_gobj);
        }
    }
    temp_r31 = GET_FIGHTER(fighter_gobj);

    temp_r31->x2350_stateVar5 = (s32) (temp_r31->x2350_stateVar5 - 1);

    if ((s32) temp_r31->x2350_stateVar5 <= 0) {
        func_80088478(temp_r31, 0x334A1, 0x7F, 0x40);
        temp_r31->x2350_stateVar5 = 0x28;
    }
}

// 0x8011A438 //
void ftNess_SpecialLwHold_IASA(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet Hold IASA callback //
{
    return;
}

// 0x8011A43C //
void ftNess_SpecialAirLwHold_IASA(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Hold IASA callback //
{
    return;
}

// 0x8011A440 //
// https://decomp.me/scratch/knaIL //
void ftNess_SpecialLwHold_Phys(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Hold Physics callback //
{
    func_80084F3C(fighter_gobj);
    func_8007AF10(fighter_gobj);
}

// 0x8011A474 //
// https://decomp.me/scratch/R46Uj //
void ftNess_SpecialAirLwHold_Phys(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Hold Physics callback //
{
    Fighter* fp;
    s32 unused[6];
    s32 magnetTimer;
    ftNessAttributes* ness_attr;
    attr* attr;

    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    attr = &fp->x110_attr;

    magnetTimer = fp->nessVars.SpecialLw.gravityDelay;
    if (magnetTimer != 0) {
        fp->nessVars.SpecialLw.gravityDelay = magnetTimer - 1;
        goto block_end;
    }
    func_8007D494(fp, ness_attr->x8C_PSI_MAGNET_FALL_ACCEL,
                  attr->x170_TerminalVelocity);
block_end:
    func_8007CF58(fp);
    func_8007AF10(fighter_gobj);
}

// 0x8011A4E8 //
// https://decomp.me/scratch/SNaVN //
void ftNess_SpecialLwHold_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Hold Collision callback //
{
    if (func_80082708(fighter_gobj) == false) {
        ftNess_SpecialLwHold_GroundToAir(fighter_gobj);
    }
}

// 0x8011A524 //
// https://decomp.me/scratch/dS3Sn //
void ftNess_SpecialAirLwHold_Coll(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Hold Collision callback
{
    if (func_80081D0C(fighter_gobj) != false) {
        ftNess_SpecialAirLwHold_AirToGround(fighter_gobj);
    }
}

// 0x8011A560 //
// https://decomp.me/scratch/noqpv //
void ftNess_SpecialLwHold_GroundToAir(
    HSD_GObj* fighter_gobj) // Ness's ground->air PSI Magnet Hold Action State
                            // handler //
{
    Fighter* fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[16];
#endif

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D5D4(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_HOLD,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    ftColl_CreateAbsorbHit(fighter_gobj, &ness_attr->x98_PSI_MAGNET_ABSORPTION);
}

// 0x8011A5D4 //
// https://decomp.me/scratch/PCAft //
void ftNess_SpecialAirLwHold_AirToGround(
    HSD_GObj* fighter_gobj) // Ness's air->ground PSI Magnet Hold Action State
                            // handler //
{
    Fighter* fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[16];
#endif

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D7FC(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_HOLD,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
    func_8007D468(fp);
    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    ftColl_CreateAbsorbHit(fighter_gobj, &ness_attr->x98_PSI_MAGNET_ABSORPTION);
}

// 0x8011A650 //
// https://decomp.me/scratch/YoMqy //
void ftNess_SpecialLwHold_Action(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Hold Action State handler //
{
    Fighter* fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[8];
#endif

    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_HOLD,
                                       FIGHTER_GFX_PRESERVE, NULL, 0.0f, 1.0f,
                                       0.0f);
    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    ftColl_CreateAbsorbHit(fighter_gobj, &ness_attr->x98_PSI_MAGNET_ABSORPTION);
}

// 0x8011A6A8 //
// https://decomp.me/scratch/TjjOK //
void ftNess_SpecialAirLwHold_Action(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Hold Action State handler //
{
    Fighter* fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[8];
#endif

    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_HOLD,
                                       FIGHTER_GFX_PRESERVE, NULL, 0.0f, 1.0f,
                                       0.0f);
    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    ftColl_CreateAbsorbHit(fighter_gobj, &ness_attr->x98_PSI_MAGNET_ABSORPTION);
}

static inline f32 returnStateVar(s32 stateVar)
{
    return (f32) stateVar;
}

static inline void GetAttrStuff(HSD_GObj* arg0)
{
    Fighter* temp_r30 = arg0->user_data;
    ftNessAttributes* temp_r31 = temp_r30->x2D4_specialAttributes;
    temp_r30->x2344_stateVar2 += -1;

    if (((u32) temp_r30->x2200_ftcmd_var0 == 0U) &&
        ((returnStateVar(temp_r30->x2344_stateVar2)) <=
         temp_r31->x78_PSI_MAGNET_UNK1))
    {
        temp_r30->x2200_ftcmd_var0 = 1;
        temp_r30->facing_dir = -temp_r30->facing_dir;
    }
    func_80075AF0(
        temp_r30, 0,
        -((0.01745329238474369f * (180.0f / temp_r31->x78_PSI_MAGNET_UNK1)) -
          func_80075F48(temp_r30, 0)));
}

// 0x8011A700 //
// https://decomp.me/scratch/ZgMh8 //
void ftNess_SpecialLwTurn_Anim(
    HSD_GObj* arg0) // Ness's grounded PSI Magnet Turnaround Animation callback
                    // - unused //
{
    ftNessAttributes* temp_r31;
    Fighter* temp_r30;
    Fighter* temp_r29 = temp_r30 = arg0->user_data;

    if ((temp_r29->input.x65C_heldInputs & HSD_BUTTON_B) == false) {
        temp_r29->nessVars.SpecialLw.isRelease = 1;
    }
    if (temp_r29->nessVars.SpecialLw.releaseLag > 0) {
        temp_r29->nessVars.SpecialLw.releaseLag =
            temp_r29->nessVars.SpecialLw.releaseLag - 1;
    }

    GetAttrStuff(arg0);

    if ((s32) temp_r29->x2344_stateVar2 <= 0) {
        ftNess_SpecialLwHold_GroundOrAir(arg0);
    }
}

// 0x8011A810 //
// https://decomp.me/scratch/UQm4N // Ness's aerial PSI Magnet Turnaround
// Animation callback - unused //
void ftNess_SpecialAirLwTurn_Anim(HSD_GObj* arg0)
{
    s32 filler[2];
    ftNessAttributes* temp_r31;
    Fighter* temp_r30;
    Fighter* temp_r29 = temp_r30 = arg0->user_data;

    if ((temp_r29->input.x65C_heldInputs & HSD_BUTTON_B) == false) {
        temp_r29->nessVars.SpecialLw.isRelease = 1;
    }

    if (temp_r29->nessVars.SpecialLw.releaseLag > 0) {
        temp_r29->nessVars.SpecialLw.releaseLag =
            temp_r29->nessVars.SpecialLw.releaseLag - 1;
    }

    GetAttrStuff(arg0);

    if ((s32) temp_r29->x2344_stateVar2 <= 0) {
        ftNess_SpecialLwHold_GroundOrAir(arg0);
    }
}

// 0x8011A920 //
void ftNess_SpecialLwTurn_IASA(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet Turnaround IASA
                            // callback - unused //
{
    return;
}

// 0x8011A924 //
void ftNess_SpecialAirLwTurn_IASA(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Turnaround IASA callback
                            // - unused //
{
    return;
}

// 0x8011A928 //
void ftNess_SpecialLwTurn_Phys(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet Turnaround Physics
                            // callback - unused //
{
    func_80084F3C(fighter_gobj);
    func_8007AF10(fighter_gobj);
}

// 0x8011A95C //
void ftNess_SpecialAirLwTurn_Phys(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Turnaround Physics
                            // callback - unused //
{
    Fighter* fp;
    s32 unused[6];
    s32 magnetTimer;
    ftNessAttributes* ness_attr;
    attr* attr;

    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    attr = &fp->x110_attr;

    magnetTimer = fp->nessVars.SpecialLw.gravityDelay;
    if (magnetTimer != 0) {
        fp->nessVars.SpecialLw.gravityDelay = magnetTimer - 1;
        goto block_end;
    }
    func_8007D494(fp, ness_attr->x8C_PSI_MAGNET_FALL_ACCEL,
                  attr->x170_TerminalVelocity);
block_end:
    func_8007CF58(fp);
    func_8007AF10(fighter_gobj);
}

// 0x8011A9D0 //
// https://decomp.me/scratch/E3jqW //
void ftNess_SpecialLwTurn_Coll(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet Turnaround Collision
                            // callback - unused //
{
    if (func_80082708(fighter_gobj) == false) {
        ftNess_SpecialLwTurn_GroundToAir(fighter_gobj);
    }
}

// 0x8011AA0C //
// https://decomp.me/scratch/qkeXm //
void ftNess_SpecialAirLwTurn_Coll(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Turnaround Collision
                            // callback - unused //
{
    if (func_80081D0C(fighter_gobj) != false) {
        ftNess_SpecialAirLwTurn_AirToGround(fighter_gobj);
    }
}

// 0x8011AA48 //
// https://decomp.me/scratch/Myul8 //
void ftNess_SpecialLwTurn_GroundToAir(
    HSD_GObj* fighter_gobj) // Ness's ground->air PSI Magnet Turnaround Action
                            // State handler - unused //
{
    Fighter* fp;

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D5D4(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_TURN,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
}

// 0x8011AAA8 //
// https://decomp.me/scratch/rfuLo //
void ftNess_SpecialAirLwTurn_AirToGround(
    HSD_GObj* fighter_gobj) // Ness's air->ground PSI Magnet Turnaround Action
                            // State handler - unused //
{
    Fighter* fp;

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D7FC(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_TURN,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
    func_8007D468(fp);
}

// 0x8011AB10 //
// https://decomp.me/scratch/HzpAw //
bool ftNess_SpecialLwHold_GroundOrAir(
    HSD_GObj* arg0) // Decide grounded or aerial PSI Magnet Hold //
{
    s32 unused[10];
    Fighter* temp_r3;
    ftNessAttributes* attrs;

    temp_r3 = arg0->user_data;
    if (((s32) temp_r3->nessVars.SpecialLw.releaseLag <= 0) &&
        ((s32) temp_r3->nessVars.SpecialLw.isRelease != false))
    {
        if ((s32) temp_r3->xE0_ground_or_air == GA_Ground) {
            ftNess_SpecialLwEnd_Action(arg0);
        } else {
            ftNess_SpecialAirLwEnd_Action(arg0);
        }
        return false;
    }
    if ((s32) temp_r3->xE0_ground_or_air == GA_Ground) {
        Fighter_ActionStateChange_800693AC(arg0, AS_NESS_SPECIALLW_HOLD,
                                           FIGHTER_GFX_PRESERVE, NULL, 0.0f,
                                           1.0f, 0.0f);
        temp_r3 = arg0->user_data;
        attrs = temp_r3->x2D4_specialAttributes;
        ftColl_CreateAbsorbHit(arg0, &attrs->x98_PSI_MAGNET_ABSORPTION);
    } else {
        Fighter_ActionStateChange_800693AC(arg0, AS_NESS_SPECIALAIRLW_HOLD,
                                           FIGHTER_GFX_PRESERVE, NULL, 0.0f,
                                           1.0f, 0.0f);
        temp_r3 = arg0->user_data;
        attrs = temp_r3->x2D4_specialAttributes;
        ftColl_CreateAbsorbHit(arg0, &attrs->x98_PSI_MAGNET_ABSORPTION);
    }
    return true;
}

inline void MagnetStateVarCalc(HSD_GObj* fighter_gobj)
{
    Fighter* fp = GET_FIGHTER(fighter_gobj);
    fp->x2350_stateVar5 = fp->x2350_stateVar5 - 1;
    if ((s32) fp->x2350_stateVar5 <= 0) {
        func_80088478(fp, 0x334A1, 0x7F, 0x40);
        fp->x2350_stateVar5 = 0x28;
    }
}

// 0x8011ABF8 //
// https://decomp.me/scratch/jbsQw //
void ftNess_SpecialLwHit_Anim(
    HSD_GObj* arg0) // Ness's grounded PSI Magnet Absorb Animation callback //
{
    Fighter* temp_r30;
    Fighter* temp_r3_2;
    Fighter* temp_r4;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[44];
#endif

    s32 phi_r0;

    Fighter* temp_e1;
    Fighter* temp_e2;
    ftNessAttributes* attr;

    temp_r4 = arg0->user_data;
    if (FLAGS_NONE(temp_r4->input.x65C_heldInputs, HSD_BUTTON_B)) {
        temp_r4->nessVars.SpecialLw.isRelease = 1;
    }

    if (temp_r4->nessVars.SpecialLw.releaseLag > 0) {
        temp_r4->nessVars.SpecialLw.releaseLag =
            (s32) (temp_r4->nessVars.SpecialLw.releaseLag - 1);
    }
    if (!ftAnim_IsFramesRemaining(arg0)) {
        temp_r3_2 = arg0->user_data;
        if (((s32) temp_r3_2->nessVars.SpecialLw.releaseLag <= 0) &&
            ((s32) temp_r3_2->nessVars.SpecialLw.isRelease != 0))
        {
            if ((s32) temp_r3_2->xE0_ground_or_air == GA_Ground) {
                ftNess_SpecialLwEnd_Action(arg0);
            } else {
                ftNess_SpecialAirLwEnd_Action(arg0);
            }
            phi_r0 = 0;
        } else {
            if ((s32) temp_r3_2->xE0_ground_or_air == GA_Ground) {
                Fighter_ActionStateChange_800693AC(arg0, AS_NESS_SPECIALLW_HOLD,
                                                   FIGHTER_GFX_PRESERVE, NULL,
                                                   0.0f, 1.0f, 0.0f);

                temp_e1 = arg0->user_data;
                attr = temp_e1->x2D4_specialAttributes;
                ftColl_CreateAbsorbHit(arg0, &attr->x98_PSI_MAGNET_ABSORPTION);
            } else {
                Fighter_ActionStateChange_800693AC(
                    arg0, AS_NESS_SPECIALAIRLW_HOLD, FIGHTER_GFX_PRESERVE, NULL,
                    0.0f, 1.0f, 0.0f);
                temp_e2 = arg0->user_data;
                attr = temp_e2->x2D4_specialAttributes;
                ftColl_CreateAbsorbHit(arg0, &attr->x98_PSI_MAGNET_ABSORPTION);
            }
            phi_r0 = 1;
        }
        if (phi_r0 != 0) {
            func_8007DB24(arg0);
            temp_r30 = arg0->user_data;
            if (temp_r30->x2219_flag.bits.b0 == 0) {
                efAsync_Spawn(arg0, &temp_r30->x60C, 0U, 0x4F0U,
                              temp_r30->x5E8_fighterBones[23].x0_jobj);
                temp_r30->x2219_flag.bits.b0 = 1;
            }
            temp_r30->cb.x21D4_callback_EnterHitlag = efLib_PauseAll;
            temp_r30->cb.x21D8_callback_ExitHitlag = efLib_ResumeAll;
        }
    }

    MagnetStateVarCalc(arg0);
}

// 0x8011ADC8 //
// https://decomp.me/scratch/VSNzs //
void ftNess_SpecialAirLwHit_Anim(
    HSD_GObj* arg0) // Ness's aerial PSI Magnet Absorb Animation callback //
{
    s32 unused[6];
    Fighter* temp_r31;
    Fighter* temp_r31_2;
    Fighter* temp_r4;

    temp_r4 = arg0->user_data;
    if ((temp_r4->input.x65C_heldInputs & HSD_BUTTON_B) == false) {
        temp_r4->nessVars.SpecialLw.isRelease = 1;
    }

    if (temp_r4->nessVars.SpecialLw.releaseLag > 0) {
        temp_r4->nessVars.SpecialLw.releaseLag =
            (s32) (temp_r4->nessVars.SpecialLw.releaseLag - 1);
    }
    if ((!ftAnim_IsFramesRemaining(arg0)) &&
        (ftNess_SpecialLwHold_GroundOrAir(arg0) != false))
    {
        func_8007DB24(arg0);
        temp_r31 = arg0->user_data;
        if (temp_r31->x2219_flag.bits.b0 == 0) {
            efAsync_Spawn(arg0, &temp_r31->x60C, 0U, 0x4F0U,
                          temp_r31->x5E8_fighterBones[23].x0_jobj);
            temp_r31->x2219_flag.bits.b0 = 1;
        }
        temp_r31->cb.x21D4_callback_EnterHitlag = efLib_PauseAll;
        temp_r31->cb.x21D8_callback_ExitHitlag = efLib_ResumeAll;
    }
    temp_r31_2 = arg0->user_data;
    temp_r31_2->x2350_stateVar5 = (s32) (temp_r31_2->x2350_stateVar5 - 1);
    if ((s32) temp_r31_2->x2350_stateVar5 <= 0) {
        func_80088478(temp_r31_2, 0x334A1, 0x7F, 0x40);
        temp_r31_2->x2350_stateVar5 = 0x28;
    }
}

// 0x8011AEE0 //
void ftNess_SpecialLwHit_IASA(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Absorb IASA callback //
{
    return;
}

// 0x8011AEE4 //
void ftNess_SpecialAirLwHit_IASA(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet Absorb IASA callback //
{
    return;
}

// 0x8011AEE8 //
// https://decomp.me/scratch/wKRWI //
void ftNess_SpecialLwHit_Phys(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Absorb Physics callback //
{
    func_80084F3C(fighter_gobj);
    func_8007AF10(fighter_gobj);
}

// 0x8011AF1C //
// https://decomp.me/scratch/SfvyO //
void ftNess_SpecialAirLwHit_Phys(
    HSD_GObj* arg0) // Ness's aerial PSI Magnet Absorb Physics callback //
{
    s32 unused[6];
    Fighter* temp_r31;
    s32 temp_r3;
    ftNessAttributes* attrs;
    attr* attributes;

    temp_r31 = arg0->user_data;
    attrs = temp_r31->x2D4_specialAttributes;
    attributes = &temp_r31->x110_attr;

    temp_r3 = temp_r31->nessVars.SpecialLw.gravityDelay;
    if (temp_r3 != 0) {
        temp_r31->nessVars.SpecialLw.gravityDelay = temp_r3 - 1;
        goto end_part;
    }
    func_8007D494(temp_r31, attrs->x8C_PSI_MAGNET_FALL_ACCEL,
                  attributes->x170_TerminalVelocity);
end_part:
    func_8007CF58(temp_r31);
    func_8007AF10(arg0);
}

// 0x8011AF90 //
// https://decomp.me/scratch/XSIRp //
void ftNess_SpecialLwHit_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet Absorb Collision callback //
{
    if (func_80082708(fighter_gobj) == false) {
        ftNess_SpecialLwHit_GroundToAir(fighter_gobj);
    }
}

// 0x8011AFCC //
// https://decomp.me/scratch/Lq3zN //
void ftNess_SpecialAirLwHit_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet Absorb Collision callback //
{
    if (func_80081D0C(fighter_gobj) != false) {
        ftNess_SpecialAirLwHit_AirToGround(fighter_gobj);
    }
}

// 0x8011B008 //
// https://decomp.me/scratch/LkmRj //
void ftNess_SpecialLwHit_GroundToAir(
    HSD_GObj* fighter_gobj) // Ness's ground->air PSI Magnet Absorb Action State
                            // handler //
{
    Fighter* fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[16];
#endif

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D5D4(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_HIT,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    ftColl_CreateAbsorbHit(fighter_gobj, &ness_attr->x98_PSI_MAGNET_ABSORPTION);
}

// 0x8011B07C //
// https://decomp.me/scratch/uVIed //
void ftNess_SpecialAirLwHit_AirToGround(
    HSD_GObj* fighter_gobj) // Ness's air->ground PSI Magnet Absorb Action State
                            // handler //
{
    Fighter* fp;
    ftNessAttributes* ness_attr;

    /// @todo Unused stack.
#ifdef MUST_MATCH
    u8 unused[16];
#endif

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D7FC(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_HIT,
                                       FTNESS_SPECIALLW_COLL_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
    func_8007D468(fp);
    fp = GET_FIGHTER(fighter_gobj);
    ness_attr = fp->x2D4_specialAttributes;
    ftColl_CreateAbsorbHit(fighter_gobj, &ness_attr->x98_PSI_MAGNET_ABSORPTION);
}

// 0x8011B0F8 //
// https://decomp.me/scratch/H0osU //
void ftNess_AbsorbThink_DecideAction(
    HSD_GObj* gobj) // Ness's PSI Magnet OnAbsorb hook //
{
    s32 filler[2];
    f32 temp_f1;
    f32 temp_f31;
    s32 temp_r0;
    Fighter* temp_r31;
    ftNessAttributes* temp_r30;
    s32 phi_r4;

    temp_r31 = gobj->user_data;
    temp_r30 = temp_r31->x2D4_specialAttributes;
    temp_f31 = (f32) (s32) ((f32) temp_r31->AbsorbAttr.x1A44_damageTaken *
                            ftNess_GetAbsorbHeal(temp_r31));
    temp_r31->dmg.x1830_percent -= temp_f31;
    temp_f1 = temp_r31->dmg.x1830_percent;

    if (temp_r31->dmg.x1830_percent < 0.0f) {
        temp_f31 = temp_f1 + temp_f31;
        temp_r31->dmg.x1830_percent = 0.0f;
    }
    Player_SetHPByIndex((s32) temp_r31->xC_playerID,
                        temp_r31->x221F_flag.bits.b4,
                        (s32) temp_r31->dmg.x1830_percent);
    func_80040B8C(temp_r31->xC_playerID, temp_r31->x221F_flag.bits.b4,
                  (s32) temp_f31);
    temp_r31->facing_dir = (f32) temp_r31->AbsorbAttr.x1A40_absorbHitDirection;
    temp_r0 = temp_r31->action_id;

    if (((temp_r0 != AS_NESS_SPECIALLW_HIT) &&
         (temp_r0 != AS_NESS_SPECIALAIRLW_HIT)) ||
        !(temp_r31->x894_currentAnimFrame <= temp_r30->x7C_PSI_MAGNET_UNK2))
    {
        if ((s32) temp_r31->xE0_ground_or_air == GA_Ground) {
            phi_r4 = AS_NESS_SPECIALLW_HIT;
        } else {
            phi_r4 = AS_NESS_SPECIALAIRLW_HIT;
        }
        Fighter_ActionStateChange_800693AC(gobj, phi_r4, 2, NULL, 0.0f, 1.0f,
                                           0.0f);
        ftColl_CreateAbsorbHit(gobj, &temp_r30->x98_PSI_MAGNET_ABSORPTION);
    }
}

// 0x8011B25C //
// https://decomp.me/scratch/H32Eg //
void ftNess_SpecialLwEnd_Anim(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet End Animation callback //
{
    s32 unused[2];

    if (!ftAnim_IsFramesRemaining(fighter_gobj)) {
        func_8007DB24(fighter_gobj);
        func_8007D92C(fighter_gobj);
    }
}

// 0x8011B2A0 //
// https://decomp.me/scratch/uLilE //
void ftNess_SpecialAirLwEnd_Anim(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet End Animation callback //
{
    s32 unused[2];
    if (!ftAnim_IsFramesRemaining(fighter_gobj)) {
        func_8007DB24(fighter_gobj);
        func_8007D92C(fighter_gobj);
    }
}

// 0x8011B2E4 //
void ftNess_SpecialLwEnd_IASA(
    HSD_GObj* fighter_gobj) // Ness's grounded PSI Magnet End IASA callback //
{
    return;
}

// 0x8011B2E8 //
void ftNess_SpecialAirLwEnd_IASA(
    HSD_GObj* fighter_gobj) // Ness's aerial PSI Magnet End IASA callback //
{
    return;
}

// 0x8011B2EC //
// https://decomp.me/scratch/ccQxd //
void ftNess_SpecialLwEnd_Phys(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet End Physics callback //
{
    func_80084F3C(fighter_gobj);
}

// 0x8011B30C //
// https://decomp.me/scratch/EkqUN //
void ftNess_SpecialAirLwEnd_Phys(
    HSD_GObj* arg0) // Ness's aerial PSI Magnet End Physics callback //
{
    s32 unused[6];
    Fighter* temp_r31;
    s32 temp_r3;
    ftNessAttributes* attrs;
    attr* attributes;

    temp_r31 = arg0->user_data;
    attrs = temp_r31->x2D4_specialAttributes;
    attributes = &temp_r31->x110_attr;

    temp_r3 = temp_r31->nessVars.SpecialLw.gravityDelay;
    if (temp_r3 != 0) {
        temp_r31->nessVars.SpecialLw.gravityDelay = temp_r3 - 1;
        goto end_part;
    }
    func_8007D494(temp_r31, attrs->x8C_PSI_MAGNET_FALL_ACCEL,
                  attributes->x170_TerminalVelocity);
end_part:
    func_8007CF58(temp_r31);
}

// 0x8011B36C //
// https://decomp.me/scratch/5ROxm //
void ftNess_SpecialLwEnd_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet End Collision callback //
{
    if (func_80082708(fighter_gobj) == false) {
        ftNess_SpecialLwEnd_GroundToAir(fighter_gobj);
    }
}

// 0x8011B3A8 //
// https://decomp.me/scratch/phduU //
void ftNess_SpecialAirLwEnd_Coll(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet End Collision callback //
{
    if (func_80081D0C(fighter_gobj) != false) {
        ftNess_SpecialAirLwEnd_AirToGround(fighter_gobj);
    }
}

// 0x8011B3E4 //
// https://decomp.me/scratch/9ihkO //
void ftNess_SpecialLwEnd_GroundToAir(
    HSD_GObj* fighter_gobj) // Ness's ground->air PSI Magnet End Action State
                            // handler //
{
    Fighter* fp;

    fp = GET_FIGHTER(fighter_gobj);
    func_8007D5D4(fp);
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_END,
                                       FTNESS_SPECIALLW_END_FLAG, NULL,
                                       fp->x894_currentAnimFrame, 1.0f, 0.0f);
}

// 0x8011B444 //
// https://decomp.me/scratch/uJw37 //
void ftNess_SpecialAirLwEnd_AirToGround(
    HSD_GObj* fighter_gobj) // Ness's air->ground PSI Magnet End Action State
                            // handler //
{
    Fighter* temp_r31;

    temp_r31 = GET_FIGHTER(fighter_gobj);
    func_8007D7FC(temp_r31);
    Fighter_ActionStateChange_800693AC(
        fighter_gobj, AS_NESS_SPECIALLW_END, FTNESS_SPECIALLW_END_FLAG, NULL,
        temp_r31->x894_currentAnimFrame, 1.0f, 0.0f);
    func_8007D468(temp_r31);
}

// 0x8011B4AC //
// https://decomp.me/scratch/6wpwg //
void ftNess_SpecialLwEnd_Action(
    HSD_GObj*
        fighter_gobj) // Ness's grounded PSI Magnet End Action State handler //
{
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALLW_END, 0,
                                       NULL, 0.0f, 1.0f, 0.0f);
}

// 0x8011B4E4 //
// https://decomp.me/scratch/sbrLt //
void ftNess_SpecialAirLwEnd_Action(
    HSD_GObj*
        fighter_gobj) // Ness's aerial PSI Magnet End Action State handler //
{
    Fighter_ActionStateChange_800693AC(fighter_gobj, AS_NESS_SPECIALAIRLW_END,
                                       0, NULL, 0.0f, 1.0f, 0.0f);
}
