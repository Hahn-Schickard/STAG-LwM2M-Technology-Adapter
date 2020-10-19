[![pipeline status](https://git.hahn-schickard.de/hahn-schickard/software-sollutions/application-engineering/internal/opc_ua_dev_group/gateway-project/lwm2m-technology-adapter/badges/master/pipeline.svg)](https://git.hahn-schickard.de/hahn-schickard/software-sollutions/application-engineering/internal/opc_ua_dev_group/gateway-project/lwm2m-technology-adapter/-/commits/master)
[![coverage report](https://git.hahn-schickard.de/hahn-schickard/software-sollutions/application-engineering/internal/opc_ua_dev_group/gateway-project/lwm2m-technology-adapter/badges/master/coverage.svg)](https://git.hahn-schickard.de/hahn-schickard/software-sollutions/application-engineering/internal/opc_ua_dev_group/gateway-project/lwm2m-technology-adapter/-/commits/master)

<img src="docs/code_documentation/vendor-logo.png" alt="" width="200"/>

# LwM2M Technology Adapter
## Brief description

Implements a Technology Adapter for Lighweight Machine To Machine (LwM2M) Communications protocol. The adapter functions as a LwM2M Server and uses [Hahn-Schickar LwM2M Server for C++17](https://git.hahn-schickard.de/hahn-schickard/software-sollutions/application-engineering/internal/cpp-projects/lwm2m/lwm2m-server) implementation.

## Required dependencies
* [Python 3.7](https://www.python.org/downloads/release/python-370/)
* [conan](https://docs.conan.io/en/latest/installation.html)

## Visual Studio Code Support

### Recomended Plugins:
* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)  - provides linking to inellisense and code debuggers
* [C++ Intellisense](https://marketplace.visualstudio.com/items?itemName=austin.code-gnu-global) - provides code highlithing and quick navigation
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) - provides CMake highlithing, configruing, building
* [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) - provides code formating
* [Test Explorer UI](https://marketplace.visualstudio.com/items?itemName=hbenl.vscode-test-explorer) - provides test runner integration
* [C++ TestMate](https://marketplace.visualstudio.com/items?itemName=matepek.vscode-catch2-test-adapter) - provides google-test framework adapter for Test Explorer UI

### Cmake Integration
A CMake variant file is provided with this repository for easy cmake configuration setup. This functionallity requires [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) plugin to be installed. After Visual Code has been started Open Controll Panel with *Cntrl + Shift + P* and type in CMake: Select Variant to start configuring your cmake project configuration. 
