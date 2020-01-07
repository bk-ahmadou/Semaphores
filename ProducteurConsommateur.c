#include<stdio.h>
#include<sys/sem.h>
#include<unistd.h>
int tampon;
int mutex;
int plein;
int vide;
int msg;
int timeConsommation; int timeDepot;
void P(int semid)
{
     struct sembuf op;
     op.sem_num=0; op.sem_op=-1; op.sem_flg=0;
     semop(semid, &op, 1);
}
void V(int semid)
{
     struct sembuf op;
     op.sem_num=0; op.sem_op=1; op.sem_flg=0;
     semop(semid, &op, 1);
}
void Z(int semid)
{
     struct sembuf op;
     op.sem_num=0; op.sem_op=0; op.sem_flg=0;
     semop(semid, &op, 1);
}
void init(int semid,int E)
{
    semctl(semid, 0, SETVAL, E);
}
void produire_objet(int objet)
{
     objet=1;
}
void deposer_objet(int objet)
{
    //tampon=objet;
    printf("\n\t\t\t\t\t\t\t\t Phase de Production\n\n\n");
    printf ( "\t\t\t\tEspace_vide_Tampon = %d            \t\t\t\tEspace_occupe_Tampon = %d\n\n",semctl(vide,0,GETVAL),tampon-semctl(vide,0,GETVAL));
}
void Producteur()
{
    int objet;
    while(1)
    {
        produire_objet(objet);
        P(msg);
        sleep(timeDepot);
               P(vide);
               P(mutex);
               //Deposer objet
               deposer_objet(objet);
               V(mutex);
               V(plein);
    }
}
void consommer_objet(int objet)
{
    int consommation=objet;
    printf("\n\t\t\t\t\t\t\t\t Phase de Consommation\n\n\n");
     printf ( "\t\t\t\tEspace_vide_Tampon = %d            \t\t\t\tEspace_occupe_Tampon = %d\n\n",semctl(vide,0,GETVAL),tampon-semctl(vide,0,GETVAL));
}    
void retirer_objet(int objet)
{
    //objet=tampon;
}
void Consommateur()
{
    int objet;
    while(1)
    {
        V(msg);
        sleep(timeConsommation);
            P(plein ) ;
            P(mutex ) ;
            // consommer
            retirer_objet(objet);
            V(mutex ) ;
            V(vide ) ;
            consommer_objet(objet);
    }
}
int main()
{
    //Creation des semaphores
    mutex=semget(42, 1, IPC_CREAT|0666);
    plein=semget(43, 1, IPC_CREAT|0666);
    vide=semget(44, 1, IPC_CREAT|0666);
    msg=semget(45, 1, IPC_CREAT|0666);
    //Initialisation des semaphores
    /*Simulatin*/
    printf("Donnez la taille initiale du tampon: ");
    scanf("%d",&tampon);
    printf("Donnez le temps de production:");
    scanf("%d",&timeDepot);
    printf("Donnez le temps de consommation:");
    scanf("%d",&timeConsommation);
    
    init(mutex,1);
    init(plein,0);
    init(vide,tampon);
    init(msg,tampon-1);
    int f=fork();
    switch(f){
        case -1:printf("Erreur au niveau de la creation du consommateur\n");
        return -1;
        case 0: Consommateur();
        break;
        default: Producteur();
        break;
    }
    return 0;
}