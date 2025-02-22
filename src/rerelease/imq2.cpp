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
    Rectangle->MaxY = max(Rectangle->MinY, Rectangle->MaxY - Value);

    imq2_rect Result = { Rectangle->MinX, Rectangle->MaxY, Rectangle->MaxX, MaxY };
    return (Result);
}

imq2_rect_cut RectCut(imq2_rect *Rect, imq2_cut_side Side)
{
    imq2_rect_cut Result = { Rect, Side };
    return (Result);
}

imq2_rect RectCutCut(imq2_rect_cut RectCut, float Value)
{
    switch (RectCut.Side)
    {
        case Cut_Side_Left:     return CutLeft(RectCut.Rectangle, Value);
        case Cut_Side_Right:    return CutRight(RectCut.Rectangle, Value);
        case Cut_Side_Top:      return CutTop(RectCut.Rectangle, Value);
        case Cut_Side_Bottom:   return CutBottom(RectCut.Rectangle, Value);
        default: abort();
    }
}

std::string FormatRect(imq2_rect Rect, int Color)
{
	float x = Rect.MinX;
	float y = Rect.MinY;
	float w = Rect.MaxX - Rect.MinX;
	float h = Rect.MaxY - Rect.MinY;

	return fmt::format("xl {} yt {} w {} h {} picc {}", x, y, w, h, Color);
}

void IMQ2Begin(imq2 *UI, imq2_rect Layout, bool Center)
{
    UI->ElementCount = 0;
    UI->Centered = Center;
}

void IMQ2ElementCreate(imq2 *UI, imq2_element_flags Flags, const char *String, const char *PicName, imq2_rect Rectangle)
{
    // imq2_ui_element *Element = (imq2_ui_element *)malloc(sizeof(imq2_ui_element));

    imq2_ui_element *Element = UI->Elements + UI->ElementCount++;
    Element->Index = UI->ElementCount - 1;
    Element->Initialized = true;
    Element->Rectangle = Rectangle;
    Element->String = String;
    Element->Flags = Flags;
    Element->ColorValue = 0; // TODO(Oskar)
    Element->PicName = PicName; // TODO(Oskar)

    if (UI->ElementCount >= 1 && UI->ActiveElement == NULL)
    {
        if (Element->Flags & Element_Flag_Clickable)
        {
            UI->ActiveElement = Element;
        }
    }
}

void IMQ2ElementCreate(imq2 *UI, imq2_element_flags Flags, const char *String, const char *PicName, imq2_rect Rectangle, int Color)
{
    // imq2_ui_element *Element = (imq2_ui_element *)malloc(sizeof(imq2_ui_element));

    imq2_ui_element *Element = UI->Elements + UI->ElementCount++;
    Element->Index = UI->ElementCount - 1;
    Element->Initialized = true;
    Element->Rectangle = Rectangle;
    Element->String = String;
    Element->Flags = Flags;
    Element->ColorValue = Color; // TODO(Oskar)
    Element->PicName = PicName; // TODO(Oskar)

    if (UI->ElementCount >= 1 && UI->ActiveElement == NULL)
    {
        if (Element->Flags & Element_Flag_Clickable)
        {
            UI->ActiveElement = Element;
        }
    }
}

void IMQ2Button(imq2 *UI, imq2_rect_cut Layout, float Value, const char *Label)
{
    imq2_rect Rectangle = RectCutCut(Layout, Value);

    IMQ2ElementCreate(UI, (Element_Flag_Clickable | Element_Flag_DrawBackground | Element_Flag_DrawText), Label, NULL, Rectangle);
}

void IMQ2UpgradeSelectionButton(imq2 *UI, imq2_rect_cut Layout, float Value, const char *Label, const char *Pic, const char *Text)
{
    imq2_rect Rectangle = RectCutCut(Layout, Value);

    IMQ2ElementCreate(UI, (Element_Flag_Clickable | Element_Flag_DrawBackgroundPic), NULL, "backtile", Rectangle);
    
    imq2_rect LabelRectangle = RectCutCut(RectCut(&Rectangle, Cut_Side_Top), 30);
    IMQ2ElementCreate(UI, (Element_Flag_DrawText), Label, NULL, LabelRectangle);

    imq2_rect PicRectangle = RectCutCut(RectCut(&Rectangle, Cut_Side_Top), 50);
    IMQ2ElementCreate(UI, (Element_Flag_DrawPic), NULL, Pic, PicRectangle);

    imq2_rect TextRectangle = RectCutCut(RectCut(&Rectangle, Cut_Side_Top), 60);
    IMQ2ElementCreate(UI, (Element_Flag_DrawText), Text, NULL, TextRectangle);
}

void IMQ2ProgressBar(imq2 *UI, imq2_rect_cut Layout, float Value, float Min, float Max, float Progress, const char *Label, const char *Pic)
{
    imq2_rect Rectangle = RectCutCut(Layout, Value);

    float TotalWidth = Rectangle.MaxX - Rectangle.MinX;;
    float ProgressWidth = TotalWidth * (Progress - Min) / (Max - Min);

    imq2_rect RectangleCopy = { Rectangle.MinX, Rectangle.MinY, Rectangle.MaxX, Rectangle.MaxY };
    IMQ2ElementCreate(UI, Element_Flag_DrawBackground, NULL, NULL, RectangleCopy, 0);
    
    imq2_rect ProgressRectangle = RectCutCut(RectCut(&Rectangle, Cut_Side_Left), ProgressWidth);
    IMQ2ElementCreate(UI, (Element_Flag_DrawBackground), NULL, NULL, ProgressRectangle, 100);
    
    IMQ2ElementCreate(UI, Element_Flag_DrawBackgroundPic, NULL, Pic, RectangleCopy);

    IMQ2ElementCreate(UI, Element_Flag_DrawText, Label, NULL, RectangleCopy);
}

void IMQ2End(imq2 *UI)
{

}

std::string IMQ2BuildUIString(imq2 *UI)
{
    std::string helpString = "";
    if (UI->Centered)
    {
        helpString += "imuic ";
    }

	for(int I = 0; I < UI->ElementCount; ++I)
	{
		imq2_ui_element *Element = UI->Elements + I;
		float x = Element->Rectangle.MinX;
		float y = Element->Rectangle.MinY;
		float w = Element->Rectangle.MaxX - Element->Rectangle.MinX;
		float h = Element->Rectangle.MaxY - Element->Rectangle.MinY;
        float centerX = x + (w / 2);
        float centerX2 = x + (w / 4);
        float centerY = y + (h / 4);
        float Color = Element->ColorValue;

        if (UI->ActiveElement == Element)
        {
            helpString += fmt::format("xl {} yt {} w {} h {} picc {} ", x-1, y-1, w+2, h+2, 255);
        }

        if (Element->Flags & Element_Flag_DrawBackgroundPic)
        {
            helpString += fmt::format("xl {} yt {} w {} h {} picb {} ", x, y, w, h, Element->PicName);
        }
        if (Element->Flags & Element_Flag_DrawBackground)
        {
            helpString += fmt::format("xl {} yt {} w {} h {} picc {} ", x, y, w, h, Color);
        }
        if (Element->Flags & Element_Flag_DrawText)
        {
            helpString += fmt::format("xl {} yt {} stringc \"{}\" ", centerX, centerY, Element->String);
        }
        if (Element->Flags & Element_Flag_DrawPic)
        {
            helpString += fmt::format("xl {} yt {} picnc {} ", centerX, centerY, Element->PicName);
        }
	}
    helpString.pop_back();

    return helpString;
}