#include "g_local.h"

#include "imq2.h"

imq2_rect CutLeft(imq2_rect *Rectangle, float Value)
{
    float MinX = Rectangle->MinX;
    Rectangle->MinX = min(Rectangle->MaxX, Rectangle->MinX + Value);

    imq2_rect Result = { MinX, Rectangle->MinY, Rectangle->MinX, Rectangle->MaxY };
    return (Result);
}

imq2_rect CutRight(imq2_rect *Rectangle, float Value)
{
    float MaxX = Rectangle->MaxX;
    Rectangle->MaxX = max(Rectangle->MinX, Rectangle->MaxX - Value);

    imq2_rect Result = { Rectangle->MaxX, Rectangle->MinY, MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect CutTop(imq2_rect *Rectangle, float Value)
{
    float MinY = Rectangle->MinY;
    Rectangle->MinY = min(Rectangle->MaxY, Rectangle->MinY + Value);

    imq2_rect Result = { Rectangle->MinX, MinY, Rectangle->MaxX, Rectangle->MinY };
    return (Result);
}

imq2_rect CutBottom(imq2_rect *Rectangle, float Value)
{
    float MaxY  = Rectangle->MaxY;
    Rectangle->MaxY = min(Rectangle->MinY, Rectangle->MaxY - Value);

    imq2_rect Result = { Rectangle->MinX, Rectangle->MaxY, Rectangle->MaxX, MaxY };
    return (Result);
}

std::string FormatRect(imq2_rect Rect, int Color)
{
	float x = Rect.MinX;
	float y = Rect.MinY;
	float w = Rect.MaxX - Rect.MinX;
	float h = Rect.MaxY - Rect.MinY;

	return fmt::format("xl {} yt {} w {} h {} picc {}", x, y, w, h, Color);
}