# [O]pen [I]mage [V]iewer

OIV is an hardware accelerated open source c++17 compliant cross platform 'C' library and application for viewing and manipulating images.  
It is a tool for both home users and professionals and it's designed for flexibility, user experience and performance, both for loading and displaying images.

## OIV features
* supports many image formats.
* fast first image loading - loading the first image is at the highest priority when the OIV is being load, so iterations are cheap.
* Uses OpenGL and Direct3D11 as a back bone for displaying images.
* Handles HUGE files up to 256 mega pixels (16MP per dimension)
* Infinite panning, image keeps pan even when the mouse cursor is at the edge of the working area
* custom panning limits - pan the image outside/inside the client rect - this is useful for example when you want to center on the screen a zoomed area of the image that is relativly close to the edge.
* custom zoom in/out limits. 
* Pixel grid.
* Multi full screen - image spans across all monitors.
* Fine panning - image panning is smooth with no gaps between pan steps.
* Exif support.
* Full Image information (pixel format, transperency, size in memory, etc.)

## Known limitation
* 256Mega pixel image no more than 16M pixel per dimention

## TODO
* Complete cmake and compatiblity with G++, linux and MacOS.
* Implement metal, vulkan and Direct3D12 renderers.
* Add image color transformations for adjusting brightness, contrast, saturation, hue, gamma, exposure.
* Remove freeiamge as a fallback Codec and implement specialized codecs.
* show sub images (DDS mipmaps, PSD layers, etc').
* Play animated images.
* extract various typesof meta data

--------------------------

## Build
### Windows
#### Requirements
* Windows Vista/7/8/8.1/10
* Microsoft build tools 2015

#### Instructions
1. Clone the repository
2. The deafult source uses the embedded codec, if you do not want to exlcude the embedded codec 
3. run the command "C:\Program Files (x86)\Microsoft Visual Studio\VS15Preview\MSBuild\15.0\Bin\MSBuild.exe" oiv.sln

### Linux
coming soon...

### MacOS
coming soon...


-----------------------------

##License
OIV is licensed under the OpenImageViewer License.
