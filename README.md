# [O]pen [I]mage [V]iewer

OIV is an hardware accelerated blazingly fast open source c++17 compliant cross platform 'C' library and application for viewing and manipulating images.  
It is a tool for both home users and professionals and it's designed for flexibility, user experience and performance.

## Features
* Supports many image formats.
* Fast intiial image load.
* OpenGL and Direct3D11 as a back bone for displaying images.
* Handles large files, up to 256 mega pixels (16 mega pixels per dimension).
* **Infinite panning**, image keeps panning even when the mouse cursor is at the edge of the desktop working area.
* **custom pan limits** - pan the image outside/inside the display area, this allows to center on the monitor any area of the image on any zoom level.
* custom zoom limits. 
* Pixel grid.
* **Multi full screen** - image spans across all monitors.
* **Fine panning** - image panning is bound only to screen space - image space panning could be less than 1 pixel.  
* Exif support.
* Full Image information (pixel format, transperency, size in memory, etc.)

## Todo
* Complete cmake and compatiblity with G++, linux and MacOS.
* Implement metal, vulkan and Direct3D12 renderers.
* Support for images larger than 256 mega pixels.
* Add image color transformations for adjusting brightness, contrast, saturation, hue, gamma, exposure.
* Remove freeimage as a fallback Codec and implement specialized codecs.
* Show and preview sub images (DDS mipmaps, PSD layers, etc').
* Play animated images.
* Extract various type of meta data.

--------------------------

## Build
### Windows
#### Requirements
* Windows Vista/7/8/8.1/10
* Microsoft build tools 2015

#### Instructions
1. Clone the repository  
2. Codec dependencies: till Cmake will be integrated, there's some manual labor involed.  
  a. Choose which codec to build by modifying the build configuration file: \ImageCodec\ImageLoader\Source\BuildConfig.h  
  b. Get the desired codec dependencies and add it to the relevant project  
     CodecJPG - libjpeg-turbo https://sourceforge.net/projects/libjpeg-turbo/  
     CodecPng - http://www.libpng.org/pub/png/libpng.html  
     CodecDDS - https://github.com/paroj/nv_dds  
     CodecFreeImage - http://freeimage.sourceforge.net/  

   
3. run the command "C:\Program Files (x86)\Microsoft Visual Studio\VS15Preview\MSBuild\15.0\Bin\MSBuild.exe" oiv.sln

### Linux
coming soon...

### MacOS
coming soon...


-----------------------------

## License
OIV is licensed under the OpenImageViewer License.
