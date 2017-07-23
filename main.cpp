#include "CC_Serveur.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#include <mysql.h>
#define host "172.16.202.41"
#define username "root"
#define password "test"
#define database "reservation"

/******************************************************************************************
Nom :
Constructeur de la classe CC_Serveur
Rôle :
Crée une socket, l'initialise, et la relie au port indiqué.
Permet la réception à partir de n'importe qu'elle adresse IP d'émetteur.
Paramètres passés :
P_port : numéro de port auquel relier la socket créée
Valeur renvoyée :
-
******************************************************************************************/

using namespace std;
CC_Serveur::CC_Serveur(u_short P_port)
{
    int erreur;
    int etat;
    WSADATA A_Descriptif;
    erreur=WSAStartup(VERSION, &A_Descriptif);
    if (erreur != 0)
    {
        cout << "Erreur d'initialisation de la DLL Winsock - " << endl;
        exit(1);
    }
    else
    {
        cout << "Initialisation de la DLL Winsock OK - " << endl;
        A_Prise=socket(AF_INET, SOCK_DGRAM, 17);
        A_Connection.sin_addr.s_addr= htonl(INADDR_ANY);
        A_Connection.sin_family=AF_INET;
        A_Connection.sin_port = htons(P_port);
        if(A_Prise==-1)
        {
            cout<<"invalid socket"<<endl;
            exit (0);
        }
        else
        {
            cout << "Initialisation de socket OK - " << endl;
        }

        etat=bind( A_Prise, (LPSOCKADDR) &A_Connection, sizeof(A_Connection) );
        cout<<"Etat du bind: "<<etat<<endl;
    }
}
/*****************************taille*************************************************************
Nom :
Destructeur de la classe CC_Serveur
Rôle :
Ferme la socket créée par le constructeur.
Paramètres passés :
-
Valeur renvoyée :
-
******************************************************************************************/
CC_Serveur::~CC_Serveur()
{
    closesocket(A_Prise);
    WSACleanup();
}
/******************************************************************************************
Nom :
O_Recoit
Rôle :
Attend de recevoir des données (bloquant)
Paramètres passés :
P_nbr_oct : nombre d'octets attendus
P_adr_oct : adresse où mettre les octets reçus
Valeur renvoyée :
int : nombre d'octets effectivement reçus
******************************************************************************************/
int CC_Serveur::O_Recoit(int P_nbr_oct, char FAR * const P_adr_oct) const
{

    int octrecus=0;
    int taille=sizeof(SOCKADDR);
    while(octrecus == 0)
    {
        octrecus = recvfrom(A_Prise, P_adr_oct, P_nbr_oct, 0, (LPSOCKADDR) &A_Connection, &taille);
    }

    return(octrecus);
}

/******************************************************************************************
Nom :
O_Emet
Rôle :
Envoit des données
Paramètres passés :
P_nbr_oct : nombre d'octets à envoyer
P_adr_oct : adresse où sont stockés les octets à envoyer
Valeur renvoyée :
int : nombre d'octets effectivement envoyés
******************************************************************************************/
int CC_Serveur::O_Emet(int P_nbr_oct, char FAR const *P_adr_oct) const
{
    int octemis;
    int taille = sizeof(SOCKADDR);

    octemis = sendto(A_Prise, P_adr_oct, P_nbr_oct, 0, (LPSOCKADDR) &A_Connection, taille);

    return (octemis);
}



int main()
{
    u_short port;
    cout<<"SERVEURS:Entrez un numero de port compris entre 1024 et 65535 :";
    cin>>port;
    while(1)
    {
        MYSQL *conn;
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        conn = mysql_init(NULL);
        unsigned char message[6];
        int longueur=200;
        CC_Serveur recoi(port);
        short octemis;
        short octrecus;
        octrecus = recoi.O_Recoit(6, (char*)message);
        short nom=(short)message[0];
        /**************/
        if(nom==14)
        {
            short numPlace=(short)message[2];
            short numPlace2=(short)message[3];
            short stage=(short)message[4];
            short state=(short)message[5];
            /*********/
            char select[255]="SELECT Numero_place,Etage FROM etatparking";
            char update[255];
            sprintf (update, "UPDATE etatparking SET Etat_place=%d WHERE Numero_place=%d%d AND Etage=%d",state, numPlace, numPlace2, stage);
            /********/
            if(mysql_real_connect(conn,host,username,password,database,0,NULL,0))
            {
                mysql_query(conn,select);
                res_set = mysql_store_result(conn);
                mysql_query(conn,update);
                mysql_free_result(res_set);
                mysql_close(conn);
            }
            else
            {
                cout<<"Echec de connexion au serveur de base de donnees. ";
            }
            cout<<endl;
        }
        /****************/
        else if(nom==11)
        {
            int BADGE;
            short rfid=(short)message[2];
            short rfid2=(short)message[3];
            short rfid3=(short)message[4];
            short rfid4=(short)message[5];
            short cmptr2=0;
            short taille;
            short taille2;
            short taille3;
            short taille4;
            short i;
            short valeur;
            short valeur2;
            short valeur3;
            /**********/
            SYSTEMTIME t;
            GetLocalTime(&t);
            /*********/
            short heure=t.wHour;
            short minute=t.wMinute;
            short annee=t.wYear;
            short mois=t.wMonth;
            short jours=t.wDay;
            /*********/
            char badge[8];
            char hour[5];
            char minut[5];
            char year[11];
            char mounth[11];
            char day[11];
            char point[11]= {':'};
            char separe[11]= {'-'};
            char separe2[11]= {'-'};
            char zeros[11]= {'0'};
            char zeros2[11]= {'0'};
            char zeros3[11]= {'0'};
            char zeros4[11]= {'0'};
            char ctr[5];
            ctr[0]=0x0B;
            ctr[1]=0x0B;
            ctr[2]=0x00;
            ctr[4]='\0';
            char ctr2[2];
            ctr2[1]=0x00;
            /*********/
            char select[255]="SELECT HeureDebut,HeureFin,Date FROM reservation WHERE RFID=";
            sprintf (badge,"%x%x%x%x",rfid,rfid2,rfid3,rfid4);
            sscanf (badge,"%x",&BADGE);
            //sprintf (select,"SELECT HeureDebut,HeureFin,Date FROM reservation WHERE RFID=%d%d%d%d",rfid, rfid2, rfid3,rfid4);
            /**********/
            itoa(heure,hour,10);
            itoa(minute,minut,10);
            itoa(annee,year,10);
            itoa(mois,mounth,10);
            itoa(jours,day,10);
            /**********/
            taille=strlen(minut);
            taille2=strlen(hour);
            taille3=strlen(day);
            taille4=strlen(mounth);
            /**********/
            if(taille==1)
            {
                strcat(zeros,minut);
                strcat(point,zeros);
            }
            else
            {
                strcat(point,minut);

            }
            if(taille2==1)
            {
                strcat(hour,point);
                strcat(zeros,hour);
            }
            else
            {
                strcat(hour,point);

            }
            if(taille3==1)
            {
                strcat(zeros2,day);
                strcat(separe,zeros2);
            }
            else
            {
                strcat(separe,day);
            }
            if(taille4==1)
            {
                strcat(mounth,separe);
                strcat(zeros3,mounth);
                strcat(separe2,zeros3);
            }
            else
            {
                strcat(mounth,separe);
                strcat(separe2,mounth);
            }
            strcat(year,separe2);
            itoa(BADGE,badge,10);
            strcat(select,badge);
            /**********/
            if(mysql_real_connect(conn,host,username,password,database,0,NULL,0))
            {
                mysql_query(conn,select);
                res_set = mysql_store_result(conn);
                while ((row = mysql_fetch_row(res_set)) != NULL)
                {
                    char ligne[255];
                    char ligne2[255];
                    char ligne3[255];
                    strcpy(ligne,row[0]);
                    strcpy(ligne2,row[1]);
                    strcpy(ligne3,row[2]);
                    if(taille2==1)
                    {
                        valeur = strcmp (ligne,zeros);
                        valeur2 = strcmp (ligne2,zeros);
                    }
                    else
                    {
                        valeur = strcmp (ligne,hour);
                        valeur2 = strcmp (ligne2,hour);
                    }

                    valeur3 = strcmp (ligne3,year);
                    if(valeur3==0  && valeur2>=0 && valeur<=0)
                    {
                        cout<<"Ouvert";
                        cmptr2=1;
                    }
                }
                if(cmptr2==0)
                {
                    cout<<"Fermer";
                }
                *ctr2=cmptr2;
                strcat(ctr,ctr2);
                octemis = recoi.O_Emet(4,ctr );
                mysql_free_result(res_set);
                mysql_close(conn);
            }
            else
            {
                cout<<"Echec de connexion au serveur de base de donnees. ";
            }
            cout<<endl;
        }
        /******************/
        else if(nom==10)
        {
            short stage2=(short)message[2];
            short cmptr=0;
            char ctr[5];
            ctr[0]=0x0A;
            ctr[1]=0x0A;
            ctr[2]=0x00;
            ctr[4]='\0';

            char ctr2[2];
            ctr2[1]=0x00;
            /**********/
            char select[255];
            sprintf (select,"SELECT Numero_place FROM etatparking WHERE Etat_place=0 AND Etage=%d",stage2);
            /********/
            if(mysql_real_connect(conn,host,username,password,database,0,NULL,0))
            {
                mysql_query(conn,select);
                res_set = mysql_store_result(conn);
                while ((row = mysql_fetch_row(res_set)))
                {
                    cmptr++;
                }
                *ctr2=cmptr;
                strcat(ctr,ctr2);
                octemis = recoi.O_Emet(4,ctr );
                cout<<"Nombre de place disponible: "<<cmptr;
                mysql_free_result(res_set);
                mysql_close(conn);
            }
            else
            {
                cout<<"Echec de connexion au serveur de base de donnees. ";
            }
            cout<<endl;
        }
        else
        {
            cout<<"ERROR";
        }
    }
}
