#include <string>

// TODO(Oskar): use vrect_t ?
struct imq2_rect
{
    float MinX;
    float MinY;
    float MaxX;
    float MaxY;
};

struct imq2
{

};

imq2_rect CutLeft(imq2_rect *Rectangle, float Value);
imq2_rect CutRight(imq2_rect *Rectangle, float Value);
imq2_rect CutTop(imq2_rect *Rectangle, float Value);
imq2_rect CutBottom(imq2_rect *Rectangle, float Value);
std::string FormatRect(imq2_rect Rect, int Color);