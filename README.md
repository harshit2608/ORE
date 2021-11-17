<h1 align="center">ORE (OPENGL RENDERING ENGINE)</h1>

<br/>

## What is ORE?

ORE(OPENGL RENDERING ENGINE) is a simple erndering engine that will allow the user to load and view their 3D Models in realtime.

PS: I am developing this project for my college minor assessment

## Getting Started

Start by cloning the repository with

```cmd
git clone --recursive https://github.com/org-Stupefy/Victoria.git
```

If the repository was cloned non-recursively previously, use
`git submodule update --init`
to clone the necessary submodules.

## How to build ORE?

To build ORE you need to have [CMAKE](https://cmake.org/download/)

**Building ORE on windows**

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
