#ifndef GUARD_HARDWARE_H
#define GUARD_HARDWARE_H

// Utility macros and constants

#define TILEMAP_INDEX(aX, aY) (0x20 * (aY) + (aX))
#define TILEMAP_LOCATED(aMap, aX, aY) (TILEMAP_INDEX((aX), (aY)) + (aMap))

#define TILEREF(aChar, aPal) ((aChar) + ((aPal) << 12))

#define OAM2_PRIORITY(aValue) (((aValue) & 0x3) << 10)

#define BG_SYNC_BIT(aBg) (1 << (aBg))

enum
{
    BG0_SYNC_BIT = BG_SYNC_BIT(0),
    BG1_SYNC_BIT = BG_SYNC_BIT(1),
    BG2_SYNC_BIT = BG_SYNC_BIT(2),
    BG3_SYNC_BIT = BG_SYNC_BIT(3),
};

#define ApplyPalettes(aSrc, aPalId, aPalCount) CopyToPaletteBuffer((aSrc), 0x20 * (aPalId), 0x20 * (aPalCount))
#define ApplyPalette(aSrc, aPalId) ApplyPalettes((aSrc), (aPalId), 1)

// Functions

void CopyToPaletteBuffer(const void* src, int b, int size);
// ??? sub_8000E14(???);
void FlushLCDControl(void);
// ??? BG_GetControlBuffer(???);
int GetBackgroundTileDataOffset(int bg);
int GetTileIndex(int bg, int offset);
// ??? BG_GetTileMapOffset(???);
void SetBackgroundTileDataOffset(int bg, int offset);
void SetBackgroundMapDataOffset(int bg, int offset);
void SetBackgroundScreenSize(int bg, int size);
// ??? BG_SetColorBpp(???);
// ??? ApplyColorAddition_ClampMax(???);
// ??? ApplyColorAddition_ClampMin(???);
// ??? FlushBackgrounds(???);
void BG_Fill(void *dest, int b);
void RegisterBlankTile(int a);
void SetInterrupt_LCDVBlank(InterruptHandler handler);
void SetInterrupt_LCDVCountMatch(InterruptHandler handler);
// ??? sub_8001308(???);
// ??? SetLCDVCountSetting(???);
void SetMainUpdateRoutine(void(*)(void));
void ExecMainUpdate();
// ??? _UpdateKeyStatus(???);
void UpdateKeyStatus(struct KeyStatusBuffer *keyStatus);
// ??? sub_8001414(???);
void ResetKeyStatus(struct KeyStatusBuffer *keyStatus);
void SetKeyStatus_IgnoreMask(int keys);
int GetKeyStatus_IgnoreMask(void);
// ??? KeyStatusSetter_Set(???);
void NewKeyStatusSetter(int a);
void BG_SetPosition(u16 a, u16 b, u16 c);
// ??? sub_80014E8(???);
// ??? sub_800151C(???);
// ??? sub_8001530(???);
void sub_800154C(void* outTm, void const* inData, u8 base, u8 linebits);
// ??? sub_800159C(???);
// ??? sub_80016C4(???);
void sub_8001710(void);
void sub_800172C(u16 *src, int b, int c, int d);
void sub_80017B4(int a, int b, int c, int d);
// ??? sub_800183C(???);
// ??? sub_8001860(???);
// ??? sub_80018E4(???);
// ??? sub_8001964(???);
// ??? sub_80019E8(???);
// ??? sub_8001A6C(???);
void SetupBackgrounds(u16 *bgConfig);
u16* BG_GetMapBuffer(int bg);
void sub_8001C5C(u8);
// ??? ShouldSkipHSScreen(???);
void SoftResetIfKeyComboPressed();
// ??? sub_8001CB0(???);
// ??? ExecBothHBlankHandlers(???);
// ??? UpdateHBlankHandlerState(???);
void SetPrimaryHBlankHandler(void (*hblankHandler)(void));
void SetSecondaryHBlankHandler(void(*)(void));
// ??? GetBackgroundFromBufferPointer(???);
// ??? BG_SetPriority(???);
// ??? BG_GetPriority(???);
void SetSpecialColorEffectsParameters(u16 effect, u8 coeffA, u8 coeffB, u8 blendY);
void sub_8001ED0(int, int, int, int, int); // SetColorEffectFirstTarget
void sub_8001F0C(int, int, int, int, int);
void sub_8001F48(int); // SetColorEffectBackdropFirstTarget
void sub_8001F64(int a);
void SetDefaultColorEffects(void);
void EnablePaletteSync(void);
void DisablePaletteSync(void);
void BG_EnableSyncByMask(int bg);
void BG_EnableSync(int bg);
// ??? sub_8001FD0(???);
// ??? ClearTileRigistry(???);
void RegisterTileGraphics(void *a, void *b, int c);
void RegisterFillTile(void *a, void *b, int c);
// ??? FlushTiles(???);
void SetupOAMBufferSplice(int a);
// ??? FlushSecondaryOAM(???);
// ??? FlushPrimaryOAM(???);
void WriteOAMRotScaleData(int index, s16 pa, s16 pb, s16 pc, s16 pd);
// ??? sub_80021E4(???);
int GetPrimaryOAMSize(void);

#endif  // GUARD_HARDWARE_H
