# Changelog
## [0.2.4] - 2022.11.17
### Changed
 - conan packaging recipe
 - gtest dependency to fuzzy v1.11
 - Technology_Adapter_Interface dependency to fuzzy v0.1
 - Variant_Visitor dependency to fuzzy v0.1
 - LwM2M_Server dependency to fuzzy v0.5
 - HSCUL dependency to fuzzy v0.2

## [0.2.3] - 2022.10.19
### Changed
 - conan packaging layout
 - Contribution guide 
 - code formatting rules
 - Technology_Adapter_Interface dependency to v0.1.9
 - LwM2M_Server dependency to v0.5.0
 - DeviceEventHandler.cpp to work with LwM2M v0.5.0 Device Model
 - DeviceEventHandler.hpp to use HaSLL v0.3.1 implementation
 - main.cpp to use HaSLL v0.3.1 implementation
 - integration test to check for broken dependency links

### Fixed
 - Third Party license documentation in License file

### Removed
 - unused license mentions from Notice file
 - conan directory
 - conanfile.txt

## [0.2.2] - 2021.08.09
### Changed
 - LwM2M_Server to 0.3.0
 - Technology_Adapter_Interface to 0.1.6
 - device information printing in example runner
 - `DeviceBuilderInterfaceMock` to `DeviceMockBuilder` to build Mock Devices

### Fixed
 - `ContentFormat` encoding not being set for certain responses

### Added
 - TimeStamp to DateTime conversion

## [0.2.1] - 2021.04.01
### Changed
 - LwM2M Server to 0.2.2
 - Technology Adapter Interface to 0.1.5
 - install include path to resolve potential header name collisions

## [0.2.0] - 2021.03.05
### Added
 - config dir
 - `readWrapper()` function for LwM2M Resources

### Changed
 - LwM2M Server to 0.2.1
 - Conan package integration test CMakeLists.txt to use config directory
 - loggerConfig.json to be under config/ dir
 - serverConfig.json to be under config/ dir
 - model/ to be under config/ dir

## [0.1.1] - 2020.11.16
### Added
 - POSIX Time handling from Information_Model 0.1.4

### Changed
 - valgrind script to use better error detection

## [0.1.0] - 2020.10.07
### Added
 - logger config file
 - lwm2m server config file
 - lwm2m model files
 - Write callback wrapper

### Changed
 - LwM2M Server to 0.1.2
 - Technology Adapter Interface to 0.1.4
 - example.cpp to only load the minimum amount of symbols
 - LwM2M_TechnolgoyAdapter to confirm with 0.1.4 Technology Adapter Interface

## [Initial commit]
### Added
 - LwM2M_Adapter
 - LwM2M_RegistryEventHandler
 - DeviceEventHandler
 - conan packaging
 - conan package integration test
 - documentation
 - LICENSE and NOTICE files
 - Readme
 - Contributing guide
 - Authors
