RegTlb is a command line application for registering and unregistering type libraries.

v1.0.0
It supports registration per-system or per-user
It also supports querying a type library for info about its contents, and querying the registry for registration information for specific libraries.
The reason for building this tool is that there is no built-in tool shipped with Windows for registering a type library.

Servers usually register their own type libraries, but this is not always the case and it may be convenient to register just the library without installing the actual COM servers.
For example, a developer may want to target the contained types for a build, without wanting to install or run the code locally.

USAGE:
RegTlb [/i | /i_user] /tlb <library path>
/i                      Register the type library
/i_user                 Register the type library for the current user
/tlb <path>             the full path of the tlb file. Use double quotes if the path has spaces.
Relative paths are allowed for the tlb file. The relative path will be converted.
to absolute paths when registering the tlb file in the registry.


RegTlb [/u | /u_user] /tlb <library path>
Unregister the type library, based on the information found in the tlb file.
/u                      Unregister the type library.
/u_user                 Unregister the type library for the current user.
/tlb <path>             the full path of the tlb file. Use double quotes if the path has spaces.


RegTlb /q [/tlb <library path> | /guid <guid>]
/q                      Query type library information
/tlb <library path>             Query type library information that is contained in the library (identifiers)
/guid <guid>            Query type library information that is contained in the registry for the specified GUID


RegTlb /u /guid <guid> /major <version> /minor <version> [/locale <lcid>] /syskind <kind>
/u                      Unregister the type library
/guid <guid>            the guid of the library.
/major <version>        Integer value. The major version number of the tlb.
/minor <version>        Integer value. The minor version number of the tlb.
/locale <lcid>          Integer value. The locale id. if not specified, 0 (LOCALE_NEUTRAL) is used.
/syskind <kind>         String value. The system kind. WIN32 or WIN64 are supported.
In case of doubt, use RegTlb /q /guid <guid> first to find the appropriate values.
