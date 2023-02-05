#include <iostream>
#include <string>

using namespace std;

template <typename Tip>
class Stek {
    private:
        struct Cvor {
            Tip info;
            Cvor *sljedeci;
            Cvor (const Tip &info, Cvor *sljedeci): info(info), sljedeci(sljedeci){}
        };
        Cvor *top;
        int velicina;
        void Kopiraj(const Stek &s);
    public:
        Stek(): top(0), velicina(0) {}
        Stek(const Stek &s) { Kopiraj(s); }
        ~Stek();
        Stek &operator =(const Stek &s);
        void stavi(const Tip &info);
        Tip skini();
        Tip &vrh();
        void brisi();
        bool prazan() const;
        int brojElemenata() const;
        void IspisiElemente() const;
};

template <typename Tip>
Stek<Tip>::~Stek(){
    while (!prazan()) skini();
}

template <typename Tip>
void Stek<Tip>::Kopiraj(const Stek &s){
    Cvor *q, *p(s.top);
    top = 0;
    velicina = s.velicina;
    while (p != 0){
        Cvor *n(new Cvor(p->info, 0));
        if (top == 0)
            top = n;
        else
            q->sljedeci = n;
        p = p->sljedeci;
        q = n;
    }
}

template <typename Tip>
Stek<Tip> &Stek<Tip>::operator =(const Stek &s){
    if (&s == this)
        return *this;
    brisi();
    Kopiraj(s);
    return *this;
}

template <typename Tip>
Tip Stek<Tip>::skini(){
    if (top == 0)
        throw "Stek je prazan!";
    Tip info = top->info;
    Cvor *p = top->sljedeci;
    delete top;
    top = p;
    velicina--;
    return info;
}

template <typename Tip>
void Stek<Tip>::stavi(const Tip &el){
    Cvor *p = new Cvor(el, top);
    top = p;
    velicina++;
}

template <typename Tip>
void Stek<Tip>::brisi(){
    while (!prazan()){
        skini();
    }
}

template <typename Tip>
Tip &Stek<Tip>::vrh() {
    if (top == 0)
        throw "Stek je prazan";
    return top->info;
}

template <typename Tip>
bool Stek<Tip>::prazan() const {
    return velicina == 0;
}

template <typename Tip>
int Stek<Tip>::brojElemenata() const {
    return velicina;
}

template <typename Tip>
void Stek<Tip>::IspisiElemente() const{
    // Pomocna funkcija koja samo ispisuje elmente (s vrha ka dnu)
    Stek<Tip> pom(*this);
    while (!pom.prazan())
        std::cout<<pom.skini()<<" ";
    std::cout<<std::endl; 
}

/*
Sljedeće klase treba fino testirati
operator =
stavi 
skini
vrh
brisi
brojElemenata
*/


// TESTNE FUNKCIJE

void test_operatori(){
    std::cout<<"Probavanje nekih operatpra"<<std::endl;
    Stek<std::string> stek;
    std::string neki[]{"Danas", "Jucer", "Utorak", "ASP"};
    for (int i = 0; i < 20; i++)
        stek.stavi(neki[i % 4]);
    Stek<std::string> s1;
    s1 = stek;
    Stek<std::string> s3(s1);
    s1.IspisiElemente();
    s3.IspisiElemente();
    stek.IspisiElemente();
}

void test_stavi(){
    Stek<int> stek;
    bool valja = true;
    valja = !(stek.brojElemenata());
    for (int i = 0; i < 10; i++)
        stek.stavi(i * i);
    valja = (stek.skini() == 81) && stek.brojElemenata();
    if (valja)
        std::cout<<"Metoda stavi uredu"<<std::endl;
    else
        std::cout<<"Metoda stavi nije uredu"<<std::endl;
}

void test_skini(){
    Stek<int> stek;
    try{
        stek.skini();
        std::cout<<"Nema izuzetka a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    bool valja = true;
    for (int i = 0; i < 110; i++)
        stek.stavi(i * i);
    valja = (stek.skini() == 109 * 109) && (stek.brojElemenata() != 0);
    for (int i = 0; i < 109; i++)
        stek.skini();
    valja = valja && !stek.brojElemenata();
    if (valja)
        std::cout<<"Metoda skini uredu"<<std::endl;
    else 
        std::cout<<"Metoda skini nije uredu"<<std::endl;
}

void test_vrh(){
    Stek<int> stek;
    //Provjera izuzetka kada je stek prazan
    bool valja = true;
    try {
        stek.vrh();
        valja = false;
    }catch(...){
        valja = true;
    }
    for (int i = 0; i < 1111; i++)
        stek.stavi(i);
    //Zadnji dodani elemenat tj vrh je 1110, metoda vrh ne skida elemeant nego samo ga ocita
    valja = (stek.vrh() == 1110) && (stek.vrh() == 1110);
    stek.stavi(22);
    valja = valja && (stek.vrh() == 22);
    if (valja)
        std::cout<<"Metoda vrh je uredu"<<std::endl;
    else 
        std::cout<<"Metoda vrh nije uredu"<<std::endl;
}

void test_brisi(){
    Stek<int> stek;
    stek.brisi();
    bool valja = true;
    for (int i = 0; i < 111; i++)
        stek.stavi(i);
    stek.brisi();
    valja = valja && !(stek.brojElemenata());
    try{
        stek.vrh();
        valja = false;
    }catch(...){
        0;
    }
    if (valja)
        std::cout<<"Metoda brisi uredu"<<std::endl;
    else 
        std::cout<<"Metoda brisi nije uredu"<<std::endl;
}

void test_brojElemenata(){
    Stek<int> stek;
    bool valja = (stek.brojElemenata() == 0);
    for (int i = 0; i < 100; i++)
        stek.stavi(i);
    valja = valja && (stek.brojElemenata() == 100);
    stek.skini();
    valja = valja && (stek.brojElemenata() == 99);
    if (valja)
        std::cout<<"Metoda brojElemenata uredu"<<std::endl;
    else 
        std::cout<<"Metoda brojElemenata nije uredu"<<std::endl;
}

int main(){
    test_stavi();
    test_skini();
    test_vrh();
    test_brisi();
    test_brojElemenata();
    test_operatori();
    return 0;
}