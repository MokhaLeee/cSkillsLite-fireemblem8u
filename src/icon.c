#include "global.h"
#include "icon.h"
#include "skillsystem.h"

 /**
  * StanH hold a good idea on asm-hack, 
  * but if directly compile decomp, 
  * just enlarge this as 0x100 * sheet-length
  */
#define MAX_ICON_COUNT 0x300
#define MAX_ICON_GFX_COUNT 32
#define IDR_GETTER_SHEET_MAX 0x3

EWRAM_DATA static struct IconStruct DrawnIconLookupTable[MAX_ICON_COUNT] = {0};
EWRAM_DATA static u16 IconGFXIDLookupTable[MAX_ICON_GFX_COUNT] = {0};
extern void CopyToPaletteBuffer(const void *Palette, u32 Dest, u32 Size);
extern void RegisterTileGraphics(const void *GFX, void *Dest, u32 size);
extern void RegisterFillTile(const void *GFX, void *Dest, u32 size);

typedef const void* (*IDRGetIconFunc) (u8 index);

// IDR-getter functions
static const void *GetVanillaIconGfx(u8 index)
{
    return (void *)item_icon_tiles + index * 0x80;
}

static IDRGetIconFunc CONST_DATA IDRIconGetters[IDR_GETTER_SHEET_MAX] =
{
    GetVanillaIconGfx,
    get_skill_icon_gfx,
};

static const void *GetIconGfx(u16 icon)
{
    IDRGetIconFunc func = IDRIconGetters[ICON_SHEET(icon)];

    if (0 != func)
        func(ICON_INDEX(icon));
}

void ResetIconGraphics_()
{
    ResetIconGraphics(); //Probably was a debug wrapper.
}

void ResetIconGraphics() 
{
    CpuFill16(0, &DrawnIconLookupTable, sizeof(DrawnIconLookupTable));
    CpuFill16(0, &IconGFXIDLookupTable, sizeof(IconGFXIDLookupTable));
}

void LoadIconPalettes(u32 Dest)
{
    CopyToPaletteBuffer(&item_icon_palette, Dest << 5, sizeof(item_icon_palette));
}

void LoadIconPalette(u32 Index, u32 Dest)
{
    CopyToPaletteBuffer(item_icon_palette[Index], Dest << 5, 0x20);
}

int GetNextFreeIcon() // Unused
{ 
    int retVal = 0;
    int i;
    for(i = MAX_ICON_GFX_COUNT-1; i >= 0; i--)
    {
        if(IconGFXIDLookupTable[i] != 0) retVal++;
    }
    return retVal;
}

u16 GetIconGfxTileIndex(u32 Index)
{
    return 0x300 - ICON_INDEX(Index) * 4;
}

int GetIconGfxIndex(int Index)
{
    int i;
    for(i = 0; i <= MAX_ICON_GFX_COUNT-1; i++)
    {
        if (IconGFXIDLookupTable[i] == 0)
        {
            IconGFXIDLookupTable[i] = Index + 1;
            return i;
        }
    }
    return -1;
}

u16 GetIconTileIndex(int Index)
{
    if (DrawnIconLookupTable[Index].Index != 0)
    {   
        return GetIconGfxTileIndex(DrawnIconLookupTable[Index].Index);
    }
    else 
    {
        DrawnIconLookupTable[Index].Index = GetIconGfxIndex(Index) + 1;

        RegisterTileGraphics(
            GetIconGfx(Index),
            (void*)(VRAM + 
                (0x1FFE0 & 
                    (VRAM + 0x20 * GetIconGfxTileIndex(
                                   (DrawnIconLookupTable[Index].Index))))),
            0x80
        );

        return GetIconGfxTileIndex(DrawnIconLookupTable[Index].Index);
    }
}

void DrawIcon(u16* BgOut, int IconIndex, int OamPalBase) 
{
    if (IconIndex < 0) {
        BgOut[0]  = 0;
        BgOut[1]  = 0;
        BgOut[32] = 0;
        BgOut[33] = 0;
    } else {
        u16 Tile = GetIconTileIndex(IconIndex) + OamPalBase;

        BgOut[0]  = Tile++;
        BgOut[1]  = Tile++;
        BgOut[32] = Tile++;
        BgOut[33] = Tile;
    }
}

void ClearIconGfx(u32 Index) 
{
    IconGFXIDLookupTable[DrawnIconLookupTable[Index].Index - 1] = 0;
    DrawnIconLookupTable[Index].Index = 0;
}

void LoadIconObjectGraphics(int Index, int b)
{
    void* pTarget;

    pTarget = OBJ_VRAM0;
    pTarget += ((b & 0x3FF) * 0x20);

    if (Index < 0) { // Clear Obj VRAM
        RegisterFillTile(0, pTarget,         0x40);
        RegisterFillTile(0, pTarget + 0x400, 0x40);
    } else {
        const void* pSource = GetIconGfx(Index);

        RegisterTileGraphics(pSource,        pTarget,         0x40);
        RegisterTileGraphics(pSource + 0x40, pTarget + 0x400, 0x40);
    }
}