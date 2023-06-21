#include<stdio.h>
#include <stdlib.h>


#define MAX_SAT_UZ 100
#define MAX_YIGIN_KAP 50

char yolStringi[200];


typedef struct strcYiginYapisi
{
    int Alan[MAX_YIGIN_KAP];
    int ElemanSayisi;
    int AktifNokta;
} Yigin;




typedef struct strcDugum
{
    int DugumNo;
    int Malliyet;
    struct strcDugum* Komsu;
} Dugum;

int dugumSayisi=0;

Dugum** dugumler;	



Yigin* YiginOlustur()
{
    Yigin* ygn=(Yigin*)malloc(sizeof(Yigin));
    ygn->AktifNokta=0;      
    ygn->ElemanSayisi=0;    
}

void YiginiSifirla(Yigin* ygn)
{
    ygn->AktifNokta=0;
    ygn->ElemanSayisi=0;
}

int YiginaAt(Yigin* ygn,int elm)
{
    if(ygn->ElemanSayisi>=MAX_YIGIN_KAP)
    {
        printf("\n\n!------ Yigin dolu ------!\n\n");
        exit(1);
    }
    ygn->Alan[ygn->ElemanSayisi]=elm;
    ygn->ElemanSayisi++;
}

int YiginAktifElemaniVer(Yigin* ygn)
{
    int ae=-1;
    if(ygn->AktifNokta<ygn->ElemanSayisi)
    {
        ae=ygn->Alan[ygn->AktifNokta];
        ygn->AktifNokta++;              
    }
    return ae;
}
int YiginElamani(Yigin* ygn,int indx)
{

    int r=-1;
    if(indx<ygn->ElemanSayisi)r=ygn->Alan[indx];
    return r;
}
int YigindaAra(Yigin* ygn,int arananDugum)
{
    int bulundu=0;
    for(int i=0; i<ygn->ElemanSayisi; i++)
    {
        if(ygn->Alan[i]==arananDugum)
        {
            bulundu=1;
            break;
        }
    }
    return bulundu;
}

int CharSay(char* s,char c)
{
    int sy=0;
    int i=0;
    while(s[i]!='\0')
    {
        if(s[i]==c)sy++;
        i++;
    }
    return sy;
}

void DosyaOku()
{
    char satir[MAX_SAT_UZ];
    int satirSayisi=0;
    int maxSatirdakiVeriSayisi=0;
    int temp=0;
    FILE *fp;
    if ((fp=fopen("KomsulukMatrisi.txt", "r")) == NULL)
    {
        printf("\n\n!------ KomsulukMatrisi.txt Dosyasi algilanamadi ------!\n\n");
        exit(1);
    }


    while (fgets(satir, 80, fp)!=NULL)
    {
        satirSayisi++;
        temp=CharSay(satir,' ')+1; 
        if(temp>maxSatirdakiVeriSayisi)maxSatirdakiVeriSayisi=temp;
    }
    fclose (fp);

    if(satirSayisi>maxSatirdakiVeriSayisi)dugumSayisi=satirSayisi;
    else dugumSayisi=maxSatirdakiVeriSayisi;

    dugumler =(Dugum**)malloc(dugumSayisi * sizeof(Dugum*));


    if ((fp=fopen("KomsulukMatrisi.txt", "r")) == NULL)
    {
        printf("\n\n!------ KomsulukMatrisi.txt Dosyasi algilanamadi ------!\n\n");
        exit(1);
    }

    int maliyet=-1;
    Dugum * kms;
    printf("Komsuluk matrisi\n");
    printf("------------------------\n    ");
    for(int i=0; i<dugumSayisi; i++)printf("%d ",i);
    printf("\n  --------------------\n");

    for(int i=0; i<satirSayisi; i++)
    {
        fgets(satir, 80, fp);
        dugumler[i]=(Dugum*)malloc(sizeof(Dugum));
        dugumler[i]->DugumNo=i;
        dugumler[i]->Komsu=NULL;
        dugumler[i]->Malliyet=0;
        kms=dugumler[i];

        maxSatirdakiVeriSayisi=CharSay(satir,' ')+1;
        printf("%d.| ",i);


        for(int j=0; j<maxSatirdakiVeriSayisi; j++)
        {
            maliyet=satir[j*2]-'0'; 
            printf("%d ",maliyet); 
            if(maliyet>0)
            {

                kms->Komsu=(Dugum*)malloc(sizeof(Dugum));
                kms->Komsu->DugumNo=j;
                kms->Komsu->Komsu=NULL;
                kms->Komsu->Malliyet=maliyet;
                kms=kms->Komsu;

            }
        }
        printf("\n");
    }
}

void KomsulukListesiniGoster()
{
    Dugum * temp;
    printf("\nKomsuluk Listesi\n");
    printf("------------------------------\n");
    for(int i=0; i<dugumSayisi; i++)
    {
        printf("%d ->",i);
        temp=dugumler[i]->Komsu;
        while(temp!=NULL)
        {
            printf("%d(%d)->",temp->DugumNo,temp->Malliyet);
            temp=temp->Komsu;
        }
        printf("\n");
    }
    printf("------------------------------\n\n");
}


void BFSTara(int baslangicDugumNo,int bitisDugumNo)
{
    Yigin* ygnIncelenenDugumler=YiginOlustur();
    Yigin* ygnYol=YiginOlustur();  



    Dugum * temp=dugumler[baslangicDugumNo];

    YiginaAt(ygnIncelenenDugumler,temp->DugumNo); 

    int bakilanDugumNo=-1;
    Dugum * varMi=NULL;
    printf("Komsuluk Listesinde %d numarali dugumdan %d numarali dugume BFS Taramasi\n\n",baslangicDugumNo,bitisDugumNo);
    printf("Incelenen dugum no -> daha onceden incelenmemis komsular(baglanti agirligi) ->\n");

    do
    {
        bakilanDugumNo=YiginAktifElemaniVer(ygnIncelenenDugumler); 

        if(bakilanDugumNo==-1)
        {
            printf("\nBFS algoritmasiyla dolasilan dugumler ve doasim sirasi \n\n");
            for(int i=0; i<ygnYol->ElemanSayisi; i++)
            {
                printf("%d -> ", dugumler[YiginElamani(ygnYol,i)]->DugumNo);
            }
            printf("\n\n Komsuluk Listesinde BFS Taramasi tamamlandi\n\n");
            break; 
        }
        temp=dugumler[bakilanDugumNo]; 

        printf("\n%d->",temp->DugumNo);

        YiginaAt(ygnYol,temp->DugumNo); 

       if(temp->DugumNo== bitisDugumNo)
        {
            printf("Ulasildi\n");

        }
        else
        {
            while(temp!=NULL)
            {
                if(temp->Komsu!=NULL)
                {
                    if(YigindaAra(ygnIncelenenDugumler,temp->Komsu->DugumNo)==0)
                    {
                        YiginaAt(ygnIncelenenDugumler,temp->Komsu->DugumNo);
                        printf("%d(%d)->",temp->Komsu->DugumNo,temp->Komsu->Malliyet);
                    }
                }

                temp=temp->Komsu;
            }
        }

    } while(1);

    free(ygnIncelenenDugumler);
    free(ygnYol);


}

int main()
{
    DosyaOku();
    KomsulukListesiniGoster();
    BFSTara(8,0);
    getchar();
}

