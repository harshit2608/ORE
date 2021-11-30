<h1 align="center">ORE (OPENGL RENDERING ENGINE)</h1>

<br/>

<p align="center">
<img alt="C++" src="https://img.shields.io/badge/C++%20-%2300599C.svg?logo=c%2B%2B&logoColor=white">
 <img alt="OpenGL" src="https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?&logo=opengl"/>
 </p>

## What is ORE?

ORE(OpenGL Rendering Engine) is a simple erndering engine that will allow the user to load and view their 3D models in realtime.

PS: I am developing this project for my college minor assessment

## Getting Started

Start by cloning the repository with

```
git clone --recursive https://github.com/harshit2608/ORE.git
```

If the repository was cloned non-recursively previously, use
`git submodule update --init`
to clone the necessary submodules.

## How to build ORE?

<h5>Make sure you have python installed on your system</h5>

After cloning remove run `build.bat`
To build ORE you need to have [CMAKE](https://cmake.org/download/)

<h4>Building ORE on windows</h4>

**MSVC**
Download MSVC from [here](https://visualstudio.microsoft.com/vs/features/cplusplus/)

Open command prompt in root directory and type the following commands:

```
mkdir build && cd build
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release ..
```

**MINGW**
Download MinGW from [here](https://www.mingw-w64.org)

Open command prompt in root directory and type the following commands:

```
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
```

<h4>Building ORE on Linux</h4>

**PROJECT IS UNDERDEVELOPMENT AND HASN'T BEEN TESTED ON LINUX!!**
