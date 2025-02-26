# imq2

imq2 is a wrapper library for the built-in layout language in Quake 2, designed to make user interface development more accessible for programmers. This library provides a structured and flexible approach of creating user interfaces with the hopes of increased iteration speed and productivity. This library is intended for developers and modders working with Quake 2.

## Usage

The library files are mainly [imq2.h](rerelease/imq2.h) and [imq2.cpp](rerelease/imq2.h) that contains the library code. The library has introduced some new custom tokens to the Quake 2 layout language, in order for them to be properly parsed the changes this repository has inside of [cg_screen.cpp](rerelease/cg_screen.cpp) also need to be included in your project (only the modified version of `CG_ExecuteLayoutString`).

This repository also contains changes within some other files in order to provide layout examples, these examples are inteded for users to view while reading the source code in order to get a better idea of how to build layouts.

### Available examples

The available examples are located within [g_cmds.cpp](rerelease/g_cmds.cpp), they are registered as custom commands which you can access by typing the command within the in-game console. For them to be viewed properly you need to be inside a level. While testing I have just booted up the first level of the campaign.

The commands along with what they display are as follows:

* customUI - Mainly used as my scratch space for the project. Currently displays three buttons which you can navigate between when pressing the movement keys.
* imq2example1 - The purpose of this example is to showcase the different combinations slice direction and alignment, the rectangles are color coded in order to aid with finding them in the code.
* imq2example2 - This example showcases the usage of relative alignments, the rectangles within the big black rectangle is positioned relative to the black rectangle.
* imq2example3 - This example achieves the exact same result as imq2example2 except that it does not use relative alignment and instead builds the UI through the intended slice API.

### Getting started

```c++
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

