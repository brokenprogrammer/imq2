#pragma once

#define IMQ2_STACK_MAX 16

struct imq2_rect
{
    float MinX;
    float MinY;
    float MaxX;
    float MaxY;
};

enum imq2_slice_side
{
    Slice_Side_Left,
    Slice_Side_Right,
    Slice_Side_Top,
    Slice_Side_Bottom,
};

struct imq2_rect_slice
{
    imq2_rect *Rectangle;
    imq2_slice_side Side;
};

enum class imq2_horizontal_align
{
    Left,    // xl: MinX is absolute left edge
    Right,   // xr: MinX is offset from right edge
    Center   // xc: MinX is offset from center
};

enum class imq2_vertical_align
{
    Top,    // yt: MinX is absolute left edge
    Bottom, // yb: MinX is offset from right edge
    Center  // xc: MinX is offset from center
};

struct imq2_color
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};

typedef uint64_t imq2_element_flags;
enum
{
    Element_Flag_Clickable         = (1<<0),
    Element_Flag_DrawBackground    = (1<<1),
    Element_Flag_DrawPic           = (1<<2),
    Element_Flag_DrawText          = (1<<3),
    Element_Flag_DrawBackgroundPic = (1<<4),
};

struct imq2_ui_element
{
    uint64_t Index;
    bool Initialized;

    imq2_rect Rectangle;
    const char *String;
    imq2_element_flags Flags;
    const char *PicName;
    
    imq2_color BackgroundColor;
    imq2_horizontal_align HorizontalAlign;
    imq2_vertical_align VerticalAlign;
};

struct imq2_default_style
{
    imq2_color BackgroundColor;
    imq2_horizontal_align HorizontalAlignment;
    imq2_vertical_align VerticalAlignment;
};

struct imq2
{
    imq2_ui_element Elements[256];
    uint64_t ElementCount;

    imq2_ui_element *ActiveElement;

    imq2_default_style DefaultStyle;

    // NOTE(Oskar): Stacks
    uint64_t BackgroundColorStackCount;
    imq2_color BackgroundColorStack[IMQ2_STACK_MAX];

    uint64_t HorizontalAlignmentStackCount;
    imq2_horizontal_align HorizontalAlignmentStack[IMQ2_STACK_MAX];

    uint64_t VerticalAlignmentStackCount;
    imq2_vertical_align VerticalAlignmentStack[IMQ2_STACK_MAX];
};

// NOTE(Oskar): Layouting
imq2_rect       IMQ2SliceLeft(imq2_rect *Rectangle,             float Value);
imq2_rect       IMQ2SliceRight(imq2_rect *Rectangle,            float Value);
imq2_rect       IMQ2SliceTop(imq2_rect *Rectangle,              float Value);
imq2_rect       IMQ2SliceBottom(imq2_rect *Rectangle,           float Value);
imq2_rect_slice IMQ2PrepareSlice(imq2_rect *Rectangle,          imq2_slice_side Side);
imq2_rect       IMQ2ApplySlice(imq2_rect_slice RectangleSlice,  float Value);

// NOTE(Oskar): Stacks
void IMQ2PushBackgroundColor(imq2 *UI, imq2_color Color);
void IMQ2PopBackgroundColor(imq2 *UI);
imq2_color IMQ2PeekBackgroundColor(imq2 *UI);

void IMQ2PushHorizontalAlignment(imq2 *UI, imq2_horizontal_align Alignment);
void IMQ2PopHorizontalAlignment(imq2 *UI);
imq2_horizontal_align IMQ2PeekHorizontalAlignment(imq2 *UI);

void IMQ2PushVerticalAlignment(imq2 *UI, imq2_vertical_align Alignment);
void IMQ2PopVerticalAlignment(imq2 *UI);
imq2_vertical_align IMQ2PeekVerticalAlignment(imq2 *UI);

// NOTE(Oskar): UI stuff
void IMQ2Begin(imq2 *UI, imq2_rect Layout);
void IMQ2End(imq2 *UI);
void IMQ2ElementCreate(imq2 *UI, imq2_element_flags Flags, const char *String, const char *PicName, imq2_rect Rectangle);
void IMQ2Button(imq2 *UI, imq2_rect_slice Layout, float Value, const char *Label);
void IMQ2UpgradeSelectionButton(imq2 *UI, imq2_rect_slice Layout, float Value, const char *Label, const char *Pic, const char *Text);
void IMQ2ProgressBar(imq2 *UI, imq2_rect_slice Layout, float Value, float Min, float Max, float Progress, const char *Label, const char *Pic);
std::string IMQ2BuildUIString(imq2 *UI);

// TODO(Oskar): Imlpement
// Same as cut, except they keep the input rect intact.
// Useful for decorations (9-patch-much?).
imq2_rect get_left(const imq2_rect* rect, float a);
imq2_rect get_right(const imq2_rect* rect, float a);
imq2_rect get_top(const imq2_rect* rect, float a);
imq2_rect get_bottom(const imq2_rect* rect, float a);

// TODO(Oskar): Implement
// These will add a rectangle outside of the input rectangle.
// Useful for tooltips and other overlay elements.
imq2_rect add_left(const imq2_rect* rect, float a);
imq2_rect add_right(const imq2_rect* rect, float a);
imq2_rect add_top(const imq2_rect* rect, float a);
imq2_rect add_bottom(const imq2_rect* rect, float a);