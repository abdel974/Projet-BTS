#include <winsock2.h>
#ifndef __CC_Serveur
#define __CC_Serveur
#define VERSION 0x0101
class CC_Serveur
{
private :
SOCKET A_Prise;
SOCKADDR_IN A_Connection;
public :
CC_Serveur(u_short);
~CC_Serveur();
int O_Recoit(int, char FAR * const) const;
int O_Emet(int, char FAR const *) const;
};
#endif



