# imq2

imq2 is a wrapper library for the built-in layout language in Quake 2, designed to make user interface development more accessible for programmers. This library provides a structured and flexible approach of creating user interfaces with the hopes of increased iteration speed and productivity. This library is intended for developers and modders working with Quake 2.

## Usage

The library files are mainly [imq2.h](src/rerelease/imq2.h) and [imq2.cpp](src/rerelease/imq2.h) that contains the library code. The library has introduced some new custom tokens to the Quake 2 layout language, in order for them to be properly parsed the changes this repository has inside of [cg_screen.cpp](src/rerelease/cg_screen.cpp) also need to be included in your project (only the modified version of `CG_ExecuteLayoutString`).

This repository also contains changes within some other files in order to provide layout examples, these examples are intended for users to view while reading the source code in order to get a better idea of how to build layouts.

### Available examples

The available examples are located within [g_cmds.cpp](src/rerelease/g_cmds.cpp), they are registered as custom commands which you can access by typing the command within the in-game console. For them to be viewed properly you need to be inside a level. While testing I have just booted up the first level of the campaign.

The commands along with what they display are as follows:

* customUI - Mainly used as my scratch space for the project. Currently displays three buttons which you can navigate between when pressing the movement keys.
* imq2example1 - The purpose of this example is to showcase the different combinations slice direction and alignment, the rectangles are color coded in order to aid with finding them in the code.
* imq2example2 - This example showcases the usage of relative alignments, the rectangles within the big black rectangle is positioned relative to the black rectangle.
* imq2example3 - This example achieves the exact same result as imq2example2 except that it does not use relative alignment and instead builds the UI through the intended slicing API.

### Getting started

```cpp
// Define a starting rectangle, a layout is defined as Start -> End rather than Position, Size but in this
// example since we are centering the rectangle it acts like Position and Size.
imq2_rect Layout = { 0, 0, 100, 50 };

// The imq2 object is created to hold all the state, the interface is intended to be defined
// between IMQ2Begin and IMQ2End, putting braces between these is personal preference. 
imq2 UI;
IMQ2Begin(&UI, Layout);
{
    // Here set set the alignment to be centered.
    IMQ2PushVerticalAlignment(&UI, imq2_vertical_align::Center);
    IMQ2PushHorizontalAlignment(&UI, imq2_horizontal_align::Center);
    {
        // Create our UI element, we give it the property to draw text and give it the string to display.
        IMQ2ElementCreate(&UI, Element_Flag_DrawText, "Hello imq2!", NULL, Layout);
    }
    // Clean up our alignments
    IMQ2PopHorizontalAlignment(&UI);
    IMQ2PopVerticalAlignment(&UI);
}
IMQ2End(&UI);

// After IMQ2End the interface is internally built but in order to display it we need it to construct a
// Quake 2 layout string which we later will use to render the user interface. We call this function 
// to get a layout string that we can use directly in cg_screen and call CG_ExecuteLayoutString with or
// send to cg_screen from the server as a network message using the svc_layout tag.
std::string UIString = IMQ2BuildUIString(&UI);
```

![Helloimq2](/docs/helloimq2.png)

###  Building layouts

The IMQ2 library is based on a layouting method called "[RectCut](https://halt.software/p/rectcut-for-dead-simple-ui-layouts)". The name stems from the idea of cutting a big rectangle into smaller pieces. In IMQ2 cutting is referred to as slicing.

Every layout starts with its initial rectangle using the type `imq2_rect`. This rectangle defined the start and end coordinates of your layout.

```cpp
// Defines a rectangle with top left corner at coordinate (0,0) and its bottom right corner at (100, 50).
imq2_rect Layout = { 0, 0, 100, 50 };
```

This layout is then sliced up into smaller parts by using the different slicing functions:
```c++
imq2_rect       IMQ2SliceLeft(imq2_rect *Rectangle,             float Amount);
imq2_rect       IMQ2SliceRight(imq2_rect *Rectangle,            float Amount);
imq2_rect       IMQ2SliceTop(imq2_rect *Rectangle,              float Amount);
imq2_rect       IMQ2SliceBottom(imq2_rect *Rectangle,           float Amount);
```

These functions accepts a rectangle, slices the rectangle up by specified amount and then return the slice. This works by these functions modifying the given rectangle. For example:

```cpp
imq2_rect Layout = { 0, 0, 100, 50 };

// A new rectangle is created with the bounds: { 0, 0, 50, 50}
// The input rectangle (Layout) is now modified to have the bounds { 50, 0, 50 50 }
imq2_rect LeftHalf = IMQ2SliceLeft(&Layout, 50);
```

Using this technique you can create very complex interfaces, for example if you were to create a 2x2 grid:

```cpp
imq2_rect Layout = { 0, 0, 100, 100 };

// Remember that Layout is sliced hence modified and therefore what is left here is our "BottomGrid", we do not need to slice again.
imq2_rect TopGrid = IMQ2SliceTop(&Layout, 50);
imq2_rect BottomGrid = Layout;

imq2_rect TopLeft = IMQ2SliceLeft(&TopGrid, 50);
imq2_rect TopRight = TopGrid;

imq2_rect BottomLeft = IMQ2SliceLeft(&BottomGrid, 50);
imq2_rect BottomRight = BottomGrid;
```

More advanced layouting can be seen within one of the UI examples that included in this repository.

### Creating UI elements

IMQ2 is designed in a way where it doesn't ship with default UI elements for buttons, labels etc. Every interface is different and instead IMQ2 aims to aid you in building your own customized UI elements that fits your project.

Every UI element is created with the `IMQ2ElementCreate` function. This function accepts one or more `imq2_element_flags`, An optional string value, a pic name as well as a `imq2_rect` for where the element is located.

Here are a few examples:

```cpp
// Renders a full colored rectangle
IMQ2ElementCreate(&UI, Element_Flag_DrawBackground, NULL, NULL, Rectangle);

// Renders the specified Label at given rectangle position
IMQ2ElementCreate(&UI, Element_Flag_DrawText, Label, NULL, Rectangle);

// Combines the clickable, background and text element in order to create a element which has a background and text.
// The clickable property will be explained later.
IMQ2ElementCreate(&UI, (Element_Flag_Clickable | Element_Flag_DrawBackground | Element_Flag_DrawText), Label, NULL, Rectangle);
```

UI elements are modified by underlying "stacks" that the library keeps track of. For example if you are interested in changing the background color of an element you would push your desired color onto the BackgroundColor stack. You can tihnk of it like a layer system in paint where each push would add a new layer and pop would remove that last applied layer.

```cpp
// Will make every element with the DrawBackground property be in the color red.
IMQ2PushBackgroundColor(&UI, { 255, 0, 0, 255 });
{
    IMQ2ElementCreate(&UI, Element_Flag_DrawBackground, NULL, NULL, Rectangle);
}
// We pop the last color we pushed onto the stack when we are done using it.
IMQ2PopBackgroundColor(&UI);
```

This creates a neat way of building things, combining these techniques gives us a lot of freedom to build our own UI elements and entire interfaces. Here is an example of how we could build a progressbar.

```cpp
void IMQ2ProgressBar(imq2 *UI, imq2_rect_slice Layout, float Value, float Min, float Max, float Progress, const char *Label, const char *Pic)
{
    imq2_ui_element *Parent = NULL;

    // Use ApplySlice helper function to retrieve a Rectangle slice to base our progressbar from.
    imq2_rect Rectangle = IMQ2ApplySlice(Layout, Value);

    // Calculate the total width available in our rectangle, remember that the rectangles are in Start, End rather than Position, Size. Use this width to calculate how wide the progress should be within the progressbar.
    float TotalWidth = Rectangle.MaxX - Rectangle.MinX;
    float ProgressWidth = TotalWidth * (Progress - Min) / (Max - Min);

    // Push a background color, mainly for style purposes, I decided that the background of my progressbar is black.
    IMQ2PushBackgroundColor(UI, {0, 0, 0, 255});
    {
        Parent = IMQ2ElementCreate(UI, Element_Flag_DrawBackground, NULL, NULL, Rectangle);
    }
    IMQ2PopBackgroundColor(UI);

    // Here we do something especial. We create a copy of the rectangle because we are about to slice it for the progress rectangle but we do not want to apply the slice to some of the elements we are about to create later.
    imq2_rect RectangleCopy = { Rectangle.MinX, Rectangle.MinY, Rectangle.MaxX, Rectangle.MaxY };
    imq2_rect ProgressRectangle = IMQ2ApplySlice(IMQ2PrepareSlice(&RectangleCopy, Slice_Side_Left), ProgressWidth);
    // We push the original back background element as our "parent". This in itself does not do anything other than
    // telling the internal layouting system of quake to store the x, y, w, h values of the parent on a stack for later use.
    IMQ2PushParent(UI, Parent);
    {
        // We now push something called an alignment, here we are telling that every coordinate from here on out
        // will be relative to the last parent. This modified what the MinX value within our imq2_rect stands for. 
        // Rather than being the start coordinate on the screen it is now an offset value from the parent's X value 
        // from the left. This is what imq2example2 is tryign to show and explain.
        IMQ2PushHorizontalAlignment(UI, imq2_horizontal_align::RelativeLeft);
        {
            IMQ2ElementCreate(UI, (Element_Flag_DrawBackground), NULL, NULL, ProgressRectangle);
        }
        IMQ2PopHorizontalAlignment(UI);
    }
    IMQ2PopParent(UI);

    // On top of the progressbar we draw an image as some sort of overlay, just for style purposes.
    IMQ2ElementCreate(UI, Element_Flag_DrawBackgroundPic, NULL, Pic, Rectangle);

    // Here we allow an optional label as text within the progressbar, can be percentage or some other text.
    if (Label != NULL)
    {
        IMQ2ElementCreate(UI, Element_Flag_DrawText, Label, NULL, Rectangle);
    }
}

```

## Thanks to

3rd party textures that I used for testing as well as in some of the example. These are not provided within this repo and has to be downloaded separately.

* samon - [Weapon hud icon used in UI example](https://gfx.quakeworld.nu/details/570/weapon-hud-icons-high-quality/).
* PrimeviL - [Face icon used in UI example](https://gfx.quakeworld.nu/details/140/face-icons-faithfull-/).
