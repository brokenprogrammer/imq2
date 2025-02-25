#include "g_local.h"
#include "imq2.h"
#include <assert.h>

imq2_rect IMQ2SliceLeft(imq2_rect *Rectangle, float Amount)
{
    float MinX = Rectangle->MinX;
    Rectangle->MinX = min(Rectangle->MaxX, Rectangle->MinX + Amount);

    imq2_rect Result = { MinX, Rectangle->MinY, Rectangle->MinX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2SliceRight(imq2_rect *Rectangle, float Amount)
{
    float MaxX = Rectangle->MaxX;
    Rectangle->MaxX = max(Rectangle->MinX, Rectangle->MaxX - Amount);

    imq2_rect Result = { Rectangle->MaxX, Rectangle->MinY, MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2SliceTop(imq2_rect *Rectangle, float Amount)
{
    float MinY = Rectangle->MinY;
    Rectangle->MinY = min(Rectangle->MaxY, Rectangle->MinY + Amount);

    imq2_rect Result = { Rectangle->MinX, MinY, Rectangle->MaxX, Rectangle->MinY };
    return (Result);
}

imq2_rect IMQ2SliceBottom(imq2_rect *Rectangle, float Amount)
{
    float MaxY  = Rectangle->MaxY;
    Rectangle->MaxY = max(Rectangle->MinY, Rectangle->MaxY - Amount);

    imq2_rect Result = { Rectangle->MinX, Rectangle->MaxY, Rectangle->MaxX, MaxY };
    return (Result);
}

imq2_rect_slice IMQ2PrepareSlice(imq2_rect *Rectangle, imq2_slice_side Side)
{
    imq2_rect_slice Result = { Rectangle, Side };
    return (Result);
}

imq2_rect IMQ2ApplySlice(imq2_rect_slice RectangleSlice, float Amount)
{
    switch (RectangleSlice.Side)
    {
        case Slice_Side_Left:     return IMQ2SliceLeft(RectangleSlice.Rectangle, Amount);
        case Slice_Side_Right:    return IMQ2SliceRight(RectangleSlice.Rectangle, Amount);
        case Slice_Side_Top:      return IMQ2SliceTop(RectangleSlice.Rectangle, Amount);
        case Slice_Side_Bottom:   return IMQ2SliceBottom(RectangleSlice.Rectangle, Amount);
        default: abort();
    }
}

imq2_rect IMQ2PeekLeft(const imq2_rect* Rectangle, float Amount)
{
    float MaxX = min(Rectangle->MaxX, Rectangle->MinX + Amount);

    imq2_rect Result = { Rectangle->MinX, Rectangle->MinY, MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2PeekRight(const imq2_rect* Rectangle, float Amount)
{
    float MinX = max(Rectangle->MinX, Rectangle->MaxX - Amount);
    
    imq2_rect Result = { MinX, Rectangle->MinY, Rectangle->MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2PeekTop(const imq2_rect* Rectangle, float Amount)
{
    float MaxY = min(Rectangle->MaxY, Rectangle->MinY + Amount);
    
    imq2_rect Result = { Rectangle->MinX, Rectangle->MinY, Rectangle->MaxX, MaxY };
    return (Result);
}

imq2_rect IMQ2PeekBottom(const imq2_rect* Rectangle, float Amount)
{
    float MinY = max(Rectangle->MinY, Rectangle->MaxY - Amount);
    
    imq2_rect Result = { Rectangle->MinX, MinY, Rectangle->MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2GrowLeft(const imq2_rect* Rectangle, float Amount)
{
    imq2_rect Result = { Rectangle->MinX - Amount, Rectangle->MinY, Rectangle->MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2GrowRight(const imq2_rect* Rectangle, float Amount)
{
    imq2_rect Result = { Rectangle->MinX, Rectangle->MinY, Rectangle->MaxX + Amount, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2GrowTop(const imq2_rect* Rectangle, float Amount)
{
    imq2_rect Result = { Rectangle->MinX, Rectangle->MinY - Amount, Rectangle->MaxX, Rectangle->MaxY };
    return (Result);
}

imq2_rect IMQ2GrowBottom(const imq2_rect* Rectangle, float Amount)
{
    imq2_rect Result = { Rectangle->MinX, Rectangle->MinY, Rectangle->MaxX, Rectangle->MaxY + Amount };
    return (Result);
}


void IMQ2PushBackgroundColor(imq2 *UI, imq2_color Color)
{
    assert(UI->BackgroundColorStackCount < IMQ2_STACK_MAX);
    UI->BackgroundColorStack[UI->BackgroundColorStackCount++] = Color;
}

void IMQ2PopBackgroundColor(imq2 *UI)
{
    if (UI->BackgroundColorStackCount)
    {
        --UI->BackgroundColorStackCount;
    }
}

imq2_color IMQ2PeekBackgroundColor(imq2 *UI)
{
    if (UI->BackgroundColorStackCount == 0)
    {
        return UI->DefaultStyle.BackgroundColor;
    }

    return UI->BackgroundColorStack[UI->BackgroundColorStackCount - 1];
}

void IMQ2PushHorizontalAlignment(imq2 *UI, imq2_horizontal_align Alignment)
{
    assert(UI->HorizontalAlignmentStackCount < IMQ2_STACK_MAX);
    UI->HorizontalAlignmentStack[UI->HorizontalAlignmentStackCount++] = Alignment;
}

void IMQ2PopHorizontalAlignment(imq2 *UI)
{
    if (UI->HorizontalAlignmentStackCount)
    {
        --UI->HorizontalAlignmentStackCount;
    }
}

imq2_horizontal_align IMQ2PeekHorizontalAlignment(imq2 *UI)
{
    if (UI->HorizontalAlignmentStackCount == 0)
    {
        return UI->DefaultStyle.HorizontalAlignment;
    }

    return UI->HorizontalAlignmentStack[UI->HorizontalAlignmentStackCount - 1];
}

void IMQ2PushVerticalAlignment(imq2 *UI, imq2_vertical_align Alignment)
{
    assert(UI->VerticalAlignmentStackCount < IMQ2_STACK_MAX);
    UI->VerticalAlignmentStack[UI->VerticalAlignmentStackCount++] = Alignment;
}

void IMQ2PopVerticalAlignment(imq2 *UI)
{
    if (UI->VerticalAlignmentStackCount)
    {
        --UI->VerticalAlignmentStackCount;
    }
}

imq2_vertical_align IMQ2PeekVerticalAlignment(imq2 *UI)
{
    if (UI->VerticalAlignmentStackCount == 0)
    {
        return UI->DefaultStyle.VerticalAlignment;
    }

    return UI->VerticalAlignmentStack[UI->VerticalAlignmentStackCount - 1];
}

void IMQ2PushParent(imq2 *UI, imq2_ui_element *Parent)
{
    assert(UI->ParentStackCount < IMQ2_STACK_MAX);

    Parent->IsParent = true;
    UI->ParentStack[UI->ParentStackCount++] = Parent;
}

void IMQ2PopParent(imq2 *UI)
{
    if (UI->ParentStackCount)
    {
        --UI->ParentStackCount;
    }
}

imq2_ui_element *IMQ2PeekParentStack(imq2 *UI)
{
    if (UI->ParentStackCount == 0)
    {
        return NULL;
    }

    return UI->ParentStack[UI->ParentStackCount - 1];
}

void IMQ2Begin(imq2 *UI, imq2_rect Layout)
{
    UI->ElementCount = 0;

    UI->ParentStackCount = 0;
    UI->BackgroundColorStackCount = 0;
    UI->HorizontalAlignmentStackCount = 0;
    UI->VerticalAlignmentStackCount = 0;

    UI->DefaultStyle = {};
    UI->DefaultStyle.BackgroundColor = { 0, 0, 0, 255 };
    UI->DefaultStyle.HorizontalAlignment = imq2_horizontal_align::Left;
    UI->DefaultStyle.VerticalAlignment = imq2_vertical_align::Top;
}


void IMQ2End(imq2 *UI)
{

}

bool IMQ2NavigateNext(imq2 *UI)
{
    bool Result = false;

    if (UI->ActiveElement->Index < UI->ElementCount - 1)
    {
        uint64_t StartingIndex = UI->ActiveElement->Index + 1;
        for (uint64_t Index = StartingIndex; Index < UI->ElementCount; Index++)
        {
            imq2_ui_element *Element = UI->Elements + Index;
            if (!Element->Initialized)
            {
                break;
            }

            if (Element->Flags & Element_Flag_Clickable)
            {
                UI->ActiveElement = Element;
                Result = true;
                break;
            }
        }
    }

    return (Result);
}

bool IMQ2NavigatePrevious(imq2 *UI)
{
    bool Result = false;
    if (UI->ActiveElement->Index > 0)
    {
        uint64_t StartingIndex = UI->ActiveElement->Index - 1;
        for (uint64_t Index = StartingIndex; Index >= 0; Index--)
        {
            imq2_ui_element *Element = UI->Elements + Index;
            if (!Element->Initialized)
            {
                break;
            }
            
            if (Element->Flags & Element_Flag_Clickable)
            {
                UI->ActiveElement = Element;
                Result = true;
                break;
            }
        }
    }

    return (Result);
}

std::string IMQ2BuildUIString(imq2 *UI)
{
    std::string UIString = "";
	for(int Index = 0; Index < UI->ElementCount; ++Index)
	{
		imq2_ui_element *Element = UI->Elements + Index;
		float X = Element->Rectangle.MinX;
		float Y = Element->Rectangle.MinY;
        float W = Element->Rectangle.MaxX - Element->Rectangle.MinX;
		float H = Element->Rectangle.MaxY - Element->Rectangle.MinY;

        // NOTE(Oskar): Handle alignment
        std::string XToken;
        switch (Element->HorizontalAlign)
        {
            case imq2_horizontal_align::Left:
            {
                XToken = "xl";
            } break;

            case imq2_horizontal_align::Right:
            {
                XToken = "xr";
                X = -Element->Rectangle.MinX - W;
            } break;

            case imq2_horizontal_align::Center:
            {
                XToken = "xc";
                X -= W / 2;
            } break;
        }

        float CenterX = X + (W / 2);
        float CenterX2 = X + (W / 4);
        float CenterY = Y + (H / 4);
        imq2_color Color = Element->BackgroundColor; // TODO(Oskar): Serialize propperly

        if (Element->IsParent)
        {
            UIString += "par ";
        }

        if (Element->Parent)
        {
            UIString += "rel ";
        }

        if (UI->ActiveElement == Element)
        {
            // TODO(Oskar): Make this color base of something else or allow user to specify it.
            imq2_color HighLightColor = { 57, 255, 20, 255 };
            UIString += fmt::format("{} {} yt {} w {} h {} col {} {} {} {} ", XToken, X-1, Y-1, W+2, H+2, HighLightColor.R, HighLightColor.G, HighLightColor.B, HighLightColor.A);
        }

        if (Element->Flags & Element_Flag_DrawBackgroundPic)
        {
            UIString += fmt::format("{} {} yt {} w {} h {} picb {} ", XToken, X, Y, W, H, Element->PicName);
        }
        if (Element->Flags & Element_Flag_DrawBackground)
        {
            UIString += fmt::format("{} {} yt {} w {} h {} col {} {} {} {} ", XToken, X, Y, W, H, Color.R, Color.G, Color.B, Color.A);
        }
        if (Element->Flags & Element_Flag_DrawText)
        {
            UIString += fmt::format("{} {} yt {} stringc \"{}\" ", XToken, CenterX, CenterY, Element->String);
        }
        if (Element->Flags & Element_Flag_DrawPic)
        {
            UIString += fmt::format("{} {} yt {} picnc {} ", XToken, CenterX, CenterY, Element->PicName);
        }

        // NOTE(Oskar): Used to mark end of UI element, this resets parent and relative flag.
        UIString += "; ";
	}
    UIString.pop_back();

    return (UIString);
}

imq2_ui_element *IMQ2ElementCreate(imq2 *UI, imq2_element_flags Flags, const char *String, const char *PicName, imq2_rect Rectangle)
{
    imq2_ui_element *Element = UI->Elements + UI->ElementCount++;
    Element->Index = UI->ElementCount - 1;
    Element->Initialized = true;
    Element->IsParent = false;
    Element->Rectangle = Rectangle;
    Element->String = String;
    Element->Flags = Flags;
    Element->PicName = PicName;

    Element->Parent = IMQ2PeekParentStack(UI);
    Element->BackgroundColor = IMQ2PeekBackgroundColor(UI);
    Element->HorizontalAlign = IMQ2PeekHorizontalAlignment(UI);
    Element->VerticalAlign = IMQ2PeekVerticalAlignment(UI);

    if (UI->ElementCount >= 1 && UI->ActiveElement == NULL)
    {
        if (Element->Flags & Element_Flag_Clickable)
        {
            UI->ActiveElement = Element;
        }
    }

    return (Element);
}

void IMQ2Button(imq2 *UI, imq2_rect_slice Layout, float Value, const char *Label)
{
    imq2_rect Rectangle = IMQ2ApplySlice(Layout, Value);

    IMQ2ElementCreate(UI, (Element_Flag_Clickable | Element_Flag_DrawBackground | Element_Flag_DrawText), Label, NULL, Rectangle);
}

void IMQ2UpgradeSelectionButton(imq2 *UI, imq2_rect_slice Layout, float Value, const char *Label, const char *Pic, const char *Text)
{
    imq2_rect Rectangle = IMQ2ApplySlice(Layout, Value);

    IMQ2ElementCreate(UI, (Element_Flag_Clickable | Element_Flag_DrawBackgroundPic), NULL, "backtile", Rectangle);
    
    imq2_rect LabelRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&Rectangle, Slice_Side_Top), 30);
    IMQ2ElementCreate(UI, (Element_Flag_DrawText), Label, NULL, LabelRectangle);

    imq2_rect PicRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&Rectangle, Slice_Side_Top), 50);
    IMQ2ElementCreate(UI, (Element_Flag_DrawPic), NULL, Pic, PicRectangle);

    imq2_rect TextRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&Rectangle, Slice_Side_Top), 60);
    IMQ2ElementCreate(UI, (Element_Flag_DrawText), Text, NULL, TextRectangle);
}

void IMQ2ProgressBar(imq2 *UI, imq2_rect_slice Layout, float Value, float Min, float Max, float Progress, const char *Label, const char *Pic)
{
    imq2_ui_element *Parent = NULL;
    imq2_rect Rectangle = IMQ2ApplySlice(Layout, Value);

    float TotalWidth = Rectangle.MaxX - Rectangle.MinX;;
    float ProgressWidth = TotalWidth * (Progress - Min) / (Max - Min);

    imq2_rect RectangleCopy = { Rectangle.MinX, Rectangle.MinY, Rectangle.MaxX, Rectangle.MaxY };
    IMQ2PushBackgroundColor(UI, {0, 0, 0, 255});
    {
        Parent = IMQ2ElementCreate(UI, Element_Flag_DrawBackground, NULL, NULL, RectangleCopy);
    }
    IMQ2PopBackgroundColor(UI);

    imq2_rect ProgressRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&Rectangle, Slice_Side_Left), ProgressWidth);
    IMQ2PushParent(UI, Parent);
    {
        IMQ2PushHorizontalAlignment(UI, imq2_horizontal_align::Left);
        {
            IMQ2PushBackgroundColor(UI, {57, 255, 20, 255});
            {
                IMQ2ElementCreate(UI, (Element_Flag_DrawBackground), NULL, NULL, ProgressRectangle);
            }
            IMQ2PopBackgroundColor(UI);
        }
        IMQ2PopHorizontalAlignment(UI);
    }
    IMQ2PopParent(UI);

    IMQ2ElementCreate(UI, Element_Flag_DrawBackgroundPic, NULL, Pic, RectangleCopy);

    IMQ2ElementCreate(UI, Element_Flag_DrawText, Label, NULL, RectangleCopy);
}

void IMQ2Speedometer(imq2 *UI, imq2_rect_slice Layout, float Value, float Progress, const char *Label, const char *Pic)
{
    imq2_ui_element *Parent = NULL;
    imq2_rect Rectangle = IMQ2ApplySlice(Layout, Value);

    float TotalWidth = Rectangle.MaxX - Rectangle.MinX;;
    float FirstProgressBarMin = 0;
    float FirstProgressBarMax = 500;
    float FirstProgressWidth = TotalWidth * (Progress - FirstProgressBarMin) / (FirstProgressBarMax - FirstProgressBarMin);

    float SecondProgressBarMin = 500;
    float SecondProgressBarMax = 1250;
    float SecondProgressWidth = TotalWidth * (Progress - SecondProgressBarMin) / (SecondProgressBarMax - SecondProgressBarMin);
    imq2_rect SecondProgressRectangleCopy = { Rectangle.MinX, Rectangle.MinY, Rectangle.MaxX, Rectangle.MaxY };

    imq2_rect RectangleCopy = { Rectangle.MinX, Rectangle.MinY, Rectangle.MaxX, Rectangle.MaxY };
    IMQ2PushBackgroundColor(UI, {0, 0, 0, 255});
    {
        Parent = IMQ2ElementCreate(UI, Element_Flag_DrawBackground, NULL, NULL, RectangleCopy);
    }
    IMQ2PopBackgroundColor(UI);

    imq2_rect ProgressRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&Rectangle, Slice_Side_Left), FirstProgressWidth);
    IMQ2PushParent(UI, Parent);
    {
        IMQ2PushHorizontalAlignment(UI, imq2_horizontal_align::Left);
        {

            IMQ2PushBackgroundColor(UI, {57, 255, 20, 255});
            {
                IMQ2ElementCreate(UI, (Element_Flag_DrawBackground), NULL, NULL, ProgressRectangle);
            }
            IMQ2PopBackgroundColor(UI);
            
            if (Progress >= 500)
            {
                imq2_rect SecondProgressRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&SecondProgressRectangleCopy, Slice_Side_Left), SecondProgressWidth);
                IMQ2PushBackgroundColor(UI, {255, 0, 0, 255});
                {
                    IMQ2ElementCreate(UI, (Element_Flag_DrawBackground), NULL, NULL, SecondProgressRectangle);
                }
                IMQ2PopBackgroundColor(UI);
            }
        }
        IMQ2PopHorizontalAlignment(UI);
    }
    IMQ2PopParent(UI);
    
    IMQ2ElementCreate(UI, Element_Flag_DrawBackgroundPic, NULL, Pic, RectangleCopy);

    IMQ2ElementCreate(UI, Element_Flag_DrawText, Label, NULL, RectangleCopy);
}