#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <windows.h>

#define L 20

using namespace std;

void gotoxy(int x, int y) // Lepiej zrobic metode prywatna w klasie towar
{
    COORD c;
    c.X = x-1;
    c.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class towar
{
protected:
    char nazwa[20];
    float cena;
    int sztuk;
public:
         towar();
    void czysc();
    void setNazwa(char *naz);
    void setCena(float cen);
    void setSztuk(int szt);
    void pobierz(char *naz, float cen, int szt);
    char* getNazwa();
    float getCena();
    int getSztuk();
    void drukuj(); //!!!!!!
    float wartosc();
    void piszXY(int x, int y);

};

class bazatowar:public towar
{
protected:
    towar *tab;
    towar *found;
    int rozmiar;
    int licznosc;
    int licznoscFound;
    int current; //indeks elementu aktualnego, dziedzina = liczby naturlna, wartosc -1 oznacza brak el. aktualnego
    int currentFound;
private:
    int i; //bardziej uniwersalnym rozwi¹zaniem jest deklarowanie lokalnego "i" w metodach
public:
        bazatowar(int rozmiar);
        bazatowar();
    void dodajkolejny(char *naz,float cena, int szt);
    void czysccalosc();
    void drukujcalosc();
    void wartosci();
    float sumawartosc();
    void saveFile();
    void readFile();
        ~bazatowar(); //destruktor - nie moze miec typu (nawet void)
    void setCurrent(int index);
    void next(); //zamiast void lepiej bool lub int w 4 metodach
    void previous();
    void nextFound();
    void previousFound();
    bool searchCena(float min,float max); //<min,max>
    void sortCena();
    void sortNazwa();
    void showCurrent(int x, int y);
    void showCurrentFound(int x, int y);
    void deleteCurrent();
    void deleteCurrent2();
};

towar::towar()
{
    strcpy(nazwa," "); //powinien czyscic
    cena=0;
    sztuk=0;
}

void towar::czysc()
{
    strcpy(nazwa," "); //powinien czyscic
    cena=0;
    sztuk=0;
}

void towar::setNazwa(char *naz)
{
    strcpy(nazwa,naz);
}

void towar::setCena(float cena)
{
    this->cena=cena;
}

void towar::setSztuk(int szt)
{
    sztuk=szt;
}

void towar::pobierz(char *naz, float cena, int szt)
{
    strcpy(nazwa,naz);
    this->cena=cena; //zmieniono nazwe z cen na cena
    sztuk=szt;
}

char* towar::getNazwa()
{
    return nazwa;
}

float towar::getCena()
{
    return cena;
}

int towar::getSztuk()
{
    return sztuk;
}

void towar::drukuj()
{
    cout<<nazwa<<"  "<<cena<<"  "<<sztuk<<endl;
}

float towar::wartosc()
{
    return cena*sztuk;
}
//*********************metody bazatowar***********************8
bazatowar::bazatowar(int rozmiar)
{
    this->rozmiar=rozmiar;
    tab=new towar[rozmiar];
    found=new towar[rozmiar];
    for(i=0;i<rozmiar;i++)
        tab[i].czysc();
    licznosc=0;
    licznoscFound=0;
    current=-1; // ustawienie currenta
    currentFound=-1;
}

void bazatowar::czysccalosc()
{
      for(i=0;i<rozmiar;i++)
        tab[i].czysc();
    licznosc=0;
    current=-1;
}
void bazatowar::dodajkolejny(char *naz, float cena, int szt) //a moze lepiej bool albo int zamiast void
{
    if(licznosc<rozmiar)
    {
        tab[licznosc].pobierz(naz,cena,szt);
        licznosc++;
        current=-1;
    }
}  //return vo nie wolno metodzie "brudzic" na erkanie, ale nie wolno zwróciæ booolowski lub int'owy b³¹d
void bazatowar::drukujcalosc()//tymczasowo
{
    for(i=0;i<rozmiar;i++)
        tab[i].drukuj();
}
void bazatowar::wartosci()
{
    for(i=0;i<rozmiar;i++)
        cout<<tab[i].wartosc()<<endl;
    //kit! nie ma ekran tylko np. zwrocenie talbicy wartosci..
}
float bazatowar::sumawartosc()
{
    float suma_wart=0;
    for(i=0;i<rozmiar;i++) //lepiej... <licznosc
        suma_wart+=tab[i].wartosc();
    return suma_wart;
}
void bazatowar::saveFile()
{
    FILE *fp;
    fp=fopen("pl.dat","wb");
    fwrite(&rozmiar, sizeof(rozmiar),1,fp);
    fwrite(&licznosc,sizeof(licznosc),1,fp);
    fwrite(tab, sizeof(*tab)*rozmiar,1,fp); //lepiej... *licznosc
    fclose(fp);
}
void bazatowar::readFile()
{
        FILE *fp;
    fp=fopen("pl.dat","rb");
    fread(&rozmiar, sizeof(rozmiar),1,fp);
    fread(&licznosc,sizeof(licznosc),1,fp);
    fread(tab, sizeof(*tab)*rozmiar,1,fp); //lepiej... *licznosc
    fclose(fp);
    if (licznosc > 0)
        current = 0;
    else
        current = -1;
}

bazatowar::~bazatowar()
{
    cout << "Halo! Tu destruktor" << endl; //to tylko potwierdzenia
    delete [] tab; // to jedyna istotna instrukcja
    getchar(); // to tulko potwieerdzenie
}

void towar::piszXY(int x, int y)
{ // lepiej 2 metody (etykiety i wartosci - oddzielnie)
    gotoxy(x, y);
    cout << "Nazwa:                      ";
    gotoxy(x, y+1);
    cout << "Cena:                     ";
    gotoxy(x, y+2);
    cout << "Sztuk:                    ";
    gotoxy(x+8, y);
    cout << nazwa;
    gotoxy(x+8, y+1);
    cout << cena;
    gotoxy(x+8, y+2);
    cout << sztuk;
}

void bazatowar::showCurrent(int x, int y)
{
    if (current>=0 && current < licznosc)
        tab[current].piszXY(x, y);
}
void bazatowar::showCurrentFound(int x, int y)
{
    if (current>=0 && current < licznosc)
        found[currentFound].piszXY(x, y);
}
void bazatowar::next()
{
    if (current<licznosc-1)
        current++;
}

void bazatowar::previous()
{
    if (current>0)
        current--;
}
void bazatowar::nextFound()
{
    if(currentFound<licznoscFound-1)
        currentFound++;
}
void bazatowar::previousFound()
{
    if(currentFound>0)
        currentFound--;
}

void bazatowar::setCurrent(int index)
{
    if (index>0 && index < licznosc)
        {
        current = index - 1;
        cout << "Podano element: " << index << endl;
        }
    else
        {
        cout << "Podaj current w dziedzine licznosci";
        cout << "Podano element: " << index << endl;
        }
}
void bazatowar::searchCena(float min,float max)
{
    //zalecany bezpiecznik wtedy metoda nie bool lecz nie int
    licznoscFound=0;
    currentFound=-1;
    for(i=0;i<licznosc;i++)
        if(tab[i].getCena()>=min && tab[i].getCena()<=max)
    {
        found[licznoscFound]=tab[i];
        licznoscFound++;
    }
    if(licznoscFound)
    {
        currentFound=0;
        return 1
    }

    return 0; //niepotrzebny
}
void bazatowar::sortCena()
{
    if(licznosc>1)
    {
        towar pom;
        for(int nr=1;nr<licznosc;nr++)
            for(i=0;i<licznosc-nr;i++)
                if(tab[i].getCena().tab[i+1].getCena())
        {
            pom=tab[i];
            tab[i]=tab[i+1];
            tab[i+1]=pom;
        }
    }
}
void bazatowar::sortNazwa()
{
    if(licznosc>1)
    {
        towar pom;
        for(int nr=1;nr<licznosc;nr++)
            for(i=0;i<licznosc-nr;i++)
                if(strcmp(tab[i].getNazwa(), tab[i+1].getNazwa())>0) //jak zwraca 0 to znaczy że oba elemnety są identyczne, jak wiecej niz 0 to wiekszy 1 niz 2, a jak mniejszy to 1 mniejszy niz 2;
        {
            pom=tab[i];
            tab[i]=tab[i+1];
            tab[i+1]=pom;
        }
    }
}
void bazatowar::deleteCurrent() //lepiej bool
{
    if (licznosc>=1)
    {
        for(i=current; i<licznosc-1; i++)
            tab[i]=tab[i+1];
        if(current==licznosc-1)
            current--; // po to żeby gdy usuwamy z ostatniego elementu to element =licznosc-1, wiec po skasowaniu current byłby poza tablicą.przez co trzeba current przesunąc na jeden wcześniej.
        licznosc--;
        //return 1;
    }
    //return 0;
}

void bazatowar::deleteCurrent2()
{
    if (licznosc>=1)
    {
        if(current<licznosc-1)
          tab[current]=tab[licznosc-1];
        if(current==licznosc-1)
            current--;
        licznosc--;
        //return 1;
    }
    //return 0;
}

int main()
{
    //towar t1,t2, tablica[5];
    char n[20],zn;
    float c,min,max;
    int sz,i, rozmiar;
    cout<<"Ile elementow w tablicy?";
    cin>>rozmiar;
    cin.ignore();
    bazatowar baza(rozmiar), baza2;

    do
    {
        cout<<endl<<"1.czyszczenie"<<endl;
        cout<<"2.czytanie z dodaniem kolejnego elementu"<<endl;
        cout<<"3.drukowanie ca³ej tablicy"<<endl;
        cout<<"4.wartosc i ich suma"<<endl;
        cout<<"5.koniec"<<endl;
        cout<<"6.Save to file"<<endl;
        cout<<"7.Read to file"<<endl<<endl;
        cout<<"8.Delete current"<<endl;
        cout<<"u. Delete current ver.2"<<endl;
        cout<<"9.Display Current"<<endl;
        cout<<"a.Next"<<endl;
        cout<<"b.Previous"<<endl;
        cout<<"c.Set Current"<<endl;
        cout<<"s.Search Cena"<<endl;
        cout<<"A.Next Found"<<endl;
        cout<<"B.Previous Found"<<endl;

        zn=getch();
        system("CLS");
        switch(zn)
        {
            case '1':   baza.czysccalosc();
                        break;
            case '2': {
                        fflush(stdin);
                        cout<<"nazwa: ";gets(n);
                        fflush(stdin);
                        cout<<"cena: "; cin>>c;
                        fflush(stdin);
                        cout<<"sztuk: "; cin>>sz;
                        fflush(stdin);

                       // cin.ignore();
                        baza.dodajkolejny(n,c,sz);
                        break;
                        }
            case '3':   baza.drukujcalosc();
                        break;
            case '4':   baza.wartosci();
                        cout<<"suma wszysstkich wartosci= "<<baza.sumawartosc()<<endl;
                        break;
            case '5':   cout<<"do widzenia"<<endl;
                        getchar();
            case '6':   baza.saveFile();
                        cout<<"Zapisano"<<endl;
                        break;
            case '7':   baza.readFile();
                        cout<<"Odczytano"<<endl;
                        break;
            case '8':   baza.deleteCurrent();
                        baza.showCurrent(5,10);
                        break;
            case 'u':   baza.deleteCurrent2();
                        baza.showCurrent(5,10);
                        break;
            case '9':   baza.showCurrent(5,10);
                        break;
            case 'a':   baza.next();
                        baza.showCurrent(5,10);
                        break;
            case 'b':   baza.previous();
                        baza.showCurrent(5,10);
                        break;
            case 'A':   baza.nextFound();
                        baza.showCurrentFound(5,10);
                        break;
            case 'B':   baza.previousFound();
                        baza.showCurrentFound(5,10);
                        break;
            case 'c':   cout<<"Podaj indeks elekementu: ";
                        cin>>i;
                        baza.setCurrent(i);
                        baza.showCurrent(5,10);
                        break;
            case 's':   cout<<"Podaj cene minimalna: ";
                        cin>>min;
                        cout<<"Podaj cene maksymalna: "; //dopisać sprawdzenie czy max>podana>min
                        cin>>max;
                        if(!baza.searchCena(min,max))
                            cout<<"Niczego nie znalazlem.";
                        else
                            baza.showCurrentFound(5,10);
                        break;
        }
    }while(zn!='5');
}
