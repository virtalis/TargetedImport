# Targeted Import Plugin

## Introduction

The Targeted Import Plugin allows users of Visionary Render's Lua scripting system to perform import operations with additional options to specify the scenes node and the libraries node into which the imported model structure and its resources will be stored. This allows additional control when importing which might be useful in certain situations such as importing on to a scenes assembly node with a pre-defined position and rotation in the scene.

A side-effect of using this plugin is that imports happen as soon as the function is called, as opposed to using [vrPostCommand](https://virtalis.github.io/developer/advanced_appcommands.html) which simply creates an import request for VisRen to process at a later time. Execution will not be returned to VisRen during this process so the UI will lock. The import progress bar will not be displayed as it usually is when import commands are processed.

For this reason this is not recommended for use in background scripts that will be executing while VisRen is in active use. You should show a progress dialogue when using it for importing large files. You may also want to set [`vrSetScriptTimeout`](https://virtalis.github.io/developer/group__api__lua.html#1ga571a167ad57f0c34c4e85e493a5455f0) to set the timeout to 0 (infinite) and disable the warning about long-running scripts.

## Build / Install

To build this plugin unzip all the files to a single folder and setup the project by downloading [CMake](https://cmake.org/) and running the command "`cmake .`" in the folder.

A post-build event will create a "plugins" folder in the solution directory, this will contain a single subfolder with the plugin name, which in turn contains the plugin DLL and a plugin.txt file containing the name of the DLL file. To install the plugin simply copy the folder inside the plugins directory into your VisRen plugins directory.

## Lua Functions

### Import

Description:

Imports the specified file onto the specified scenes and libraries nodes using a specified importer. This function is essentially a wrapper for the VRXImport function which is part of the [VR Exchange C API](https://virtalis.github.io/developer/group__api__exchange.html).

If importerName is not specified (null), VisRen attempts to determine a suitable importer automatically using the file extension.

Usage:

```lua
success = TargetedImport.Import(filepath, scenesNode, librariesNode, importerName)
```

Arguments:

| Name | Type | Description |
| ---- | ---- | ----------- |
| filepath | String | The full path to the file to be imported |
| scenesNode | Node | The node to create imported assemblies on (e.g. scenes root) |
| librariesNode | Node | The node to create imported library assets on (e.g. libraries root) |
| importerName | String | The name of the importer plugin to use. Optional, defaults to nil |

Returns:
| Name | Type | Description |
| ---- | ---- | ----------- |
| success | Boolean | Whether the import was successful |

### ImportAndMerge

Description:

Imports the specified file onto the specified scenes and libraries nodes using a specified importer, utilising the merge options to append the imported data to existing data. This function is essentially a wrapper for the VRXImportAndMerge function which is part of the [VR Exchange C API](https://virtalis.github.io/developer/group__api__exchange.html).

If importerName is not specified (null), VisRen attempts to determine a suitable importer automatically using the file extension.

Usage:

```lua
success = TargetedImport.ImportAndMerge(filepath, scenesNode, librariesNode, mergeOptions, importerName)
```

Arguments:

| Name | Type | Description |
| ---- | ---- | ----------- |
| filepath | String | The full path to the file to be imported |
| scenesNode | Node | The node to create imported assemblies on (e.g. scenes root) |
| librariesNode | Node | The node to create imported library assets on (e.g. libraries root) |
| mergeOptions | Node | The node containing the merge settings |
| importerName | String | The name of the importer plugin to use. Optional, defaults to nil |

Returns:
| Name | Type | Description |
| ---- | ---- | ----------- |
| success | Boolean | Whether the import was successful |
