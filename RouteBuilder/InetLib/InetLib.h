
#ifdef INETLIB_EXPORTS
#define INETLIB_API __declspec(dllexport)
#else
#define INETLIB_API __declspec(dllimport)
#endif
#define COSMCTRL_NOD2D
bool INETLIB_API InetLib_Initialize();
