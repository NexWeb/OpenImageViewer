
---
---
---

# Moved!
## New Website: www.openimageviewer.com
## New Repository: https://github.com/OpenImageViewer/OpenImageViewer
---
---
---



# Open Image Viewer

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/86c11bc7e75e4677b8c2b5d50f9cd1c3)](https://app.codacy.com/app/TheNicker/OIV?utm_source=github.com&utm_medium=referral&utm_content=OpenImageViewer/OIV&utm_campaign=Badge_Grade_Settings)

**Open Image Viewer** is a hardware accelerated blazingly fast open code c++17 compliant cross-platform 'C' library and application for viewing and 
manipulating images.

The motivation for this project is to create an open code image viewer with great emphasis on ergonomics and performance for every type of user suitable for the starting novice or the hardcore power user.

**Open Image Viewer** in its current form is a collection of 3 projects:
1. Independent c++ Image codec library.
2. C API image viewing engine.
3. Image viewer - windows only, (linux, very soon)

## Highlights
* Supports potentially any image format, plugin based image codecs with FreeImage as fallback.
* Fast initial image load.
* Hardware accelerated by using OpenGL and Direct3D11.
* Handles files up to 256 mega pixels (16 mega pixels per dimension).
* State of the art selection rect.
* Display sub images, such as DDS mipmaps.
* On screen display of image pixel position and value, works also with all floating point types. 
* Infinite panning, no need to ever lift the mouse by using low level API for capturing mouse events.
* Custom pan/zoom limits - place any pixel of an image anywhere on the physical monitor.
* Pixel grid.
* Multi full screen - image spans across all monitors.
* Support for image meta data, such as orientation.

## Todo
* Complete CMake and compatibility with g++, Linux and MacOS.
* Link the repositoty to continuous integration services. 
* Implement Metal, Vulkan and Direct3D12 renderers.
* Add GPU support for Lanczos re-sampling.
* Support for images larger than 256 mega pixels.
* Remove freeimage as a fallback codec and implement specialized codecs.
* Play animated images.
* Suppport more types of meta data.

--------------------------

## Build your copy from source / Start developing

External dependencies are not mandatory, the core project relies solely on the standard CRT and STL.

### Instructions

1. **Get The Source Code:**  
Clone the repository by running the command:  
`git clone https://github.com/OpenImageViewer/OIV`

1. **Third Party Dependencies**  
Modify "\oiv\Configuration.h" to choose which dependencies to use.
Get the desired [libraries](#libraries) and add it to the relevant project.

1. **Embedded Codec Dependencies:**  
Modify "ImageCodec\ImageLoader\Source\BuildConfig.h" to choose which codecs are statically embedded into the image loader.
Embedded codecs are optional and they can all be disabled.
Get the desired [codec libraries](#codec-libraries-optional) and add it to the relevant project.

### Build the project

#### Windows
##### Requirements
* Windows Vista/7/8/8.1/10
* Microsoft build tools 2017 or higher ([download](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=15#))
##### Build Command
* Run the command: "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin" oiv.sln  
<span style="background-color: yellow;">**NOTE:** The path for msbuild may vary depending on your setup</span>

### Linux
#### Requirements
coming soon ...
#### Build Process
coming soon ...

### MacOS
#### Requirements
coming soon ...
#### Build Process
coming soon ...

-----------------------------
## Libraries
Name         | Link
------------ | -------------
***libfreetype2*** | http://git.sv.nongnu.org/r/freetype/freetype2.git

## Codec Libraries (optional)
Name | Link
------------ | -------------
***CodecJPG - libjpeg-turbo*** | https://sourceforge.net/projects/libjpeg-turbo/  
***CodecPNG - libpng*** | http://www.libpng.org/pub/png/libpng.html  
***CodecDDS - NVIDIA dds loader fork*** | https://github.com/paroj/nv_dds  
***CodecPSD - libpsd fork*** | https://github.com/TheNicker/libpsd  
***CodecFreeImage*** | http://freeimage.sourceforge.net/
-----------------------------



## License
OIV uses the [OpenImageViewer License](LICENSE.md) license.
