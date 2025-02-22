#pragma once

// TODO(Oskar): use vrect_t ?
struct imq2_rect
{
    float MinX;
    float MinY;
    float MaxX;
    float MaxY;
};

enum imq2_cut_side
{
    Cut_Side_Left,
    Cut_Side_Right,
    Cut_Side_Top,
    Cut_Side_Bottom,
};

struct imq2_rect_cut
{
    imq2_rect *Rectangle;
    imq2_cut_side Side;
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
    int ColorValue;
    const char *PicName;
};

struct imq2
{
    imq2_ui_element Elements[256];
    uint64_t ElementCount;

    imq2_ui_element *ActiveElement;
    bool Centered;
};

imq2_rect CutLeft(imq2_rect *Rectangle, float Value);
imq2_rect CutRight(imq2_rect *Rectangle, float Value);
imq2_rect CutTop(imq2_rect *Rectangle, float Value);
imq2_rect CutBottom(imq2_rect *Rectangle, float Value);
imq2_rect_cut RectCut(imq2_rect *Rect, imq2_cut_side Side);
imq2_rect RectCutCut(imq2_rect_cut RectCut, float Value);
std::string FormatRect(imq2_rect Rect, int Color);

void IMQ2Begin(imq2 *UI, imq2_rect Layout, bool Center = false);
void IMQ2End(imq2 *UI);
void IMQ2ElementCreate(imq2 *UI, imq2_element_flags Flags, const char *String, imq2_rect Rectangle);
void IMQ2Button(imq2 *UI, imq2_rect_cut Layout, float Value, const char *Label);
void IMQ2UpgradeSelectionButton(imq2 *UI, imq2_rect_cut Layout, float Value, const char *Label, const char *Pic, const char *Text);
void IMQ2ProgressBar(imq2 *UI, imq2_rect_cut Layout, float Value, float Min, float Max, float Progress, const char *Label, const char *Pic);
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