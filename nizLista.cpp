#include <iostream>
#include <string>

using namespace std;
template <typename InfoTip>
class Iterator;

template <typename InfoTip>
class Lista {
    public:
        Lista() {}
        virtual ~Lista() {}
        virtual int brojElemenata() const = 0;
        virtual InfoTip &trenutni() = 0;
        virtual InfoTip trenutni() const = 0;
        virtual bool prethodni() = 0;
        virtual bool sljedeci() = 0;
        virtual void pocetak() = 0;
        virtual void kraj() = 0;
        virtual InfoTip obrisi()  = 0;
        virtual void dodajIspred(const InfoTip &el) = 0;
        virtual void dodajIza(const InfoTip &el) = 0;
        virtual InfoTip &operator [](int i) = 0;
        friend class Iterator<InfoTip>;
};

template <typename InfoTip>
class NizLista : public Lista<InfoTip> {
    protected: 
        int kapacitet, velicina, tekuci;
        InfoTip **L;
    public: 
        NizLista() { L = new InfoTip*[10]{}; kapacitet = 10; velicina = 0; tekuci = 0; }
        NizLista(const NizLista<InfoTip> &lista);
        ~NizLista() { 
            for (int i = 0; i < velicina; i++) 
                delete L[i]; 
            delete[] L;
         }
        int brojElemenata() const { return velicina; }
        InfoTip &trenutni();
        InfoTip trenutni() const;
        void Realociraj();
        bool prethodni();
        bool sljedeci();
        void pocetak() { tekuci = 0; }
        void kraj() { tekuci = velicina - 1; }
        InfoTip obrisi();
        void dodajIspred(const InfoTip &el);
        void dodajIza(const InfoTip &el);
        InfoTip &operator [](int i);
        InfoTip operator [](int i) const ;
        NizLista<InfoTip> &operator =(const NizLista<InfoTip> &lista);
        void IspisiElemente() const ;
        friend class Iterator<InfoTip>;
};

template <typename InfoTip>
InfoTip &NizLista<InfoTip>::trenutni() {
    if (velicina == 0) 
        throw "Nema elemenata";
    return *L[tekuci];
}

template <typename InfoTip>
InfoTip NizLista<InfoTip>::trenutni() const {
    if (velicina == 0) 
        throw "Nema elemenata";
    return *L[tekuci];
}

template <typename InfoTip>
NizLista<InfoTip>::NizLista(const NizLista<InfoTip> &lista): kapacitet(lista.kapacitet), velicina(lista.velicina), tekuci(lista.tekuci){
    L = new InfoTip*[kapacitet];
    for (int i = 0; i < velicina; i++) 
        L[i] = new InfoTip(*lista.L[i]);
}

template <typename InfoTip>
InfoTip &NizLista<InfoTip>::operator [](int i){
    if (i < 0 or i >= velicina)
        throw "Indeks nije validan";
    return *L[i];
}

template <typename InfoTip>
InfoTip NizLista<InfoTip>::operator [](int i) const {
    if (i < 0 or i >= velicina)
        throw "Indeks nije validan";
    return *L[i];
}

template <typename InfoTip>
NizLista<InfoTip> &NizLista<InfoTip>::operator =(const NizLista<InfoTip> &lista){
    delete[] L;
    kapacitet = lista.kapacitet; velicina = lista.velicina; tekuci = lista.tekuci;
    L = new InfoTip*[kapacitet];
    for (int i = 0; i < velicina; i++) 
        L[i] = new InfoTip(*lista.L[i]);

    return *this;
}

template <typename InfoTip>
void NizLista<InfoTip>::Realociraj(){
    //Ovo je pomocna funkcija koja sluzi da ponovo realocira prostor sa odgovarajucom velicinom u slucaju da probamo dodati novi elemenat u pun niz
    InfoTip **Novo = new InfoTip*[2 * kapacitet]{};
    for (int i = 0; i < kapacitet; i++) 
        Novo[i] = L[i];
    delete[] L;
    L = Novo;
    kapacitet *= 2;
}

template <typename InfoTip>
bool NizLista<InfoTip>::prethodni(){
    if (velicina == 0)
        throw "Lista je prazna";
    if (tekuci == 0) return false;
    tekuci--;
    return true;
}

template <typename InfoTip>
bool NizLista<InfoTip>::sljedeci(){
    if (velicina == 0)
        throw "Lista je prazna";
    if (tekuci == velicina - 1) return false;
    tekuci++;
    return true;
}

template <typename InfoTip>
InfoTip NizLista<InfoTip>::obrisi(){
    if (velicina == 0) 
        throw "Nema sta za obrisati";
    InfoTip *obrisani = L[tekuci];
    for (int i = tekuci; i < velicina - 1; i++) 
        L[i] = L[i + 1];
    velicina--;
    if (tekuci == velicina and tekuci > 0) tekuci--;
    InfoTip vrijednost = *obrisani;
    delete obrisani;
    return vrijednost;
}

template <typename InfoTip>
void NizLista<InfoTip>::dodajIspred(const InfoTip &el){
    if (velicina == kapacitet - 1) Realociraj();
    for (int i = velicina; i > tekuci; i--)
        L[i] = L[i - 1];
    velicina++;
    L[tekuci] = new InfoTip(el);
    if (velicina != 1) tekuci++;
}

template <typename InfoTip>
void NizLista<InfoTip>::dodajIza(const InfoTip &el){
    if (velicina == 0) dodajIspred(el);
    else {
        if (velicina == kapacitet - 1) Realociraj();
        for (int i = velicina; i > tekuci + 1; i--)
            L[i] = L[i - 1];
        velicina++;
        L[tekuci + 1] = new InfoTip(el);
    }
}


template <typename InfoTip>
void NizLista<InfoTip>::IspisiElemente() const {
    // Ovo je pomocna funkcija koja samo ispisuje elemente radi lakse kontrole, trenutni elemenat je upakovan u []
    //std::cout<<"Tekuci je: "<<tekuci<<std::endl;
    for (int i = 0; i < brojElemenata(); i++){
        if (i == tekuci) std::cout << "["<< *L[i] << "] ";
        else std::cout << *L[i] << " ";
    }
    std::cout << std::endl;
}


template <typename InfoTip>
class JednostrukaLista : public Lista<InfoTip> {
    private:
        struct Cvor {
            InfoTip element;
            Cvor *sljedeci;
            Cvor (const InfoTip &element, Cvor *sljedeci):
            element(element), sljedeci(sljedeci){}
            Cvor(Cvor *sljedeci=0): sljedeci(sljedeci) {}
        };
        int lduzina, dduzina;
        Cvor *prvi, *zadnji, *tekuci;
        void Unisti();
        void Iniciraj();
    public:
        JednostrukaLista(): dduzina(0), lduzina(0) { prvi = zadnji = tekuci = new Cvor; }
        JednostrukaLista(const JednostrukaLista<InfoTip> &lista);
        ~JednostrukaLista() { Unisti(); }
        int brojElemenata() const { return lduzina + dduzina; }
        InfoTip &trenutni();
        InfoTip trenutni() const;
        bool prethodni();
        bool sljedeci();
        void pocetak();
        void kraj();
        InfoTip obrisi();
        void dodajIspred(const InfoTip &el);
        void dodajIza(const InfoTip &el);
        InfoTip &operator [](int i);
        InfoTip operator [](int i) const ;
        JednostrukaLista<InfoTip> &operator =(const JednostrukaLista<InfoTip> &lista);
        void IspisiElemente() const ;
        friend class Iterator<InfoTip>;
};

template <typename InfoTip>
JednostrukaLista<InfoTip>::JednostrukaLista(const JednostrukaLista<InfoTip> &lista): lduzina(0), dduzina(0){
    prvi = zadnji = tekuci = new Cvor();
    Cvor *p(lista.prvi->sljedeci);
    Cvor *pravi_tekuci = 0;
    while (p != NULL){
        dodajIza(p->element);
        sljedeci();
        if (p == lista.tekuci) pravi_tekuci = tekuci->sljedeci;
        p = p->sljedeci;
        sljedeci();
    }
    tekuci = pravi_tekuci;
}

template <typename InfoTip>
JednostrukaLista<InfoTip> &JednostrukaLista<InfoTip>::operator =(const JednostrukaLista<InfoTip> &lista){
    Unisti();
    prvi = zadnji = tekuci = new Cvor();
    Cvor *p(lista.prvi->sljedeci);
    Cvor *pravi_tekuci = 0;
    while (p != NULL){
        dodajIza(p->element);
        sljedeci();
        if (p == lista.tekuci) pravi_tekuci = tekuci->sljedeci;
        p = p->sljedeci;
        sljedeci();
    }
    tekuci = pravi_tekuci;
    return *this;   
}

template <typename InfoTip>
InfoTip &JednostrukaLista<InfoTip>::trenutni(){
    if (lduzina + dduzina == 0)
        throw "Lista je prazna";
    return tekuci->sljedeci->element;
}

template <typename InfoTip>
InfoTip JednostrukaLista<InfoTip>::trenutni() const {
    if (lduzina + dduzina == 0)
        throw "Lista je prazna";
    return tekuci->sljedeci->element;
}

template <typename InfoTip>
InfoTip &JednostrukaLista<InfoTip>::operator [](int i){
    if (i < 0 or i >= brojElemenata())
        throw "Indeks izvan opsega";
    int t = 0;
    Cvor *trazeni = prvi;
    while (t++ < i+1){
        trazeni = trazeni->sljedeci;
    }
    return trazeni->element;
}

template <typename InfoTip>
InfoTip JednostrukaLista<InfoTip>::operator [](int i) const {
    if (i < 0 or i >= brojElemenata())
        throw "Indeks izvan opsega";
    int t = 0;
    Cvor *trazeni = prvi;
    while (t++ < i+1){
        trazeni = trazeni->sljedeci;
    }
    return trazeni->element;
}

template <typename InfoTip>
void JednostrukaLista<InfoTip>::Unisti(){
    while (prvi != NULL){
        tekuci = prvi;
        prvi = prvi->sljedeci;
        delete tekuci;
    }
    prvi = zadnji = tekuci = 0;
    lduzina = dduzina = 0;
}

template <typename InfoTip>
bool JednostrukaLista<InfoTip>::prethodni(){
    if (lduzina + dduzina == 0)
        throw "Lista je prazna";
    if (lduzina == 0) return false;
    Cvor *priv = prvi;
    while (priv->sljedeci != tekuci)
        priv = priv->sljedeci;
    tekuci = priv;
    lduzina--;
    dduzina++;
    return true;
}

template <typename InfoTip>
bool JednostrukaLista<InfoTip>::sljedeci(){
    if (lduzina + dduzina == 0)
        throw "Lista je prazna";
    if (dduzina == 1) return false;
    tekuci = tekuci->sljedeci;
    dduzina--;
    lduzina++;
    return true;
}

template <typename InfoTip>
void JednostrukaLista<InfoTip>::pocetak(){
    if (lduzina + dduzina == 0)
        throw "Lista je prazna";
    dduzina += lduzina;
    lduzina = 0;
    tekuci = prvi;
}

template <typename InfoTip>
void JednostrukaLista<InfoTip>::kraj(){
    if (lduzina + dduzina == 0)
        throw "Lista je prazna";
    tekuci = zadnji;
    dduzina = lduzina + dduzina - 1;
    lduzina = 1;
}

template <typename InfoTip>
InfoTip JednostrukaLista<InfoTip>::obrisi(){
    if(dduzina + lduzina == 0)
        throw "Nista za obrisati!\n";
    InfoTip x;
    if (dduzina > 1){
        x = tekuci->sljedeci->element;
        Cvor* lpriv = tekuci->sljedeci;
        tekuci->sljedeci = lpriv->sljedeci;
        if (zadnji == lpriv) zadnji = tekuci;
        delete lpriv;
        dduzina--;
    }else if (lduzina != 0){
        prethodni();
        x = tekuci->sljedeci->sljedeci->element;
        delete tekuci->sljedeci->sljedeci;
        tekuci->sljedeci->sljedeci = 0;
        zadnji = tekuci->sljedeci;
        dduzina--;
    }else {
        x = tekuci->sljedeci->element;
        delete tekuci->sljedeci;
        tekuci->sljedeci = 0;
        prvi = zadnji = tekuci;
        dduzina--;
    }
    return x; 
}

template <typename InfoTip>
void JednostrukaLista<InfoTip>::dodajIspred(const InfoTip &el){
    Cvor *p = new Cvor(el, tekuci->sljedeci);
    tekuci->sljedeci = p;
    if (dduzina != 0) tekuci = p;
    if (zadnji == tekuci)
        zadnji = tekuci->sljedeci;
    if (dduzina == 0) dduzina++;
    else lduzina++;
}

template <typename InfoTip>
void JednostrukaLista<InfoTip>::dodajIza(const InfoTip &el){
    if (dduzina + lduzina == 0) {
        dodajIspred(el);
    }
    else {
        Cvor *p = new Cvor(el, tekuci->sljedeci->sljedeci);
        tekuci->sljedeci->sljedeci = p;
        if (dduzina == 1) zadnji = tekuci->sljedeci;
        else if (dduzina == 2) zadnji = zadnji->sljedeci;
        dduzina++;
    }
}

template <typename InfoTip>
void JednostrukaLista<InfoTip>::IspisiElemente() const {
    if (brojElemenata() == 0) return;
    Cvor *priv = prvi;
    // Element koji je zamisljen kao trenutni je u []
    while(priv != zadnji->sljedeci and priv->sljedeci != nullptr){
        if (tekuci == priv) cout<<"["<<priv->sljedeci->element<<"] ";
        else cout<<priv->sljedeci->element<<" ";
        priv = priv->sljedeci;
    }
    cout<<endl;
}


// ------------------- OVDJE IDU TESTNE FUNKCIJE ------------------- //

void jlista_dodajIspred(){
    cout<<endl<<"JednostrukaLista dodajIspred"<<endl;
    JednostrukaLista<int> lista;
    lista.dodajIspred(12); lista.dodajIspred(14); lista.dodajIspred(111);
    if (lista.trenutni() == 12) cout<<"-JPLista Trenutni OK"<<endl; // Ako je lista prazna prvi dodani elemenat postaje trenutni
    if (lista.brojElemenata() == 3) cout<<"-JPLista Brelemenata OK"<<endl;
}

void jlista_dodajIza(){
    cout<<endl<<"JednostrukaLista dodajIza"<<endl;
    JednostrukaLista<int> lista;
    for (int i = 1; i <= 5; i++)
        lista.dodajIza(i * i + 2 * i - 1);
    if (lista.trenutni() == 2) cout<<"-Trenutni OK"<<endl; // Ako je lista prazna prvi dodani elemenat postaje trenutni
    if (lista.brojElemenata() == 5) cout<<"-Brelemenata OK"<<endl;
}

void jlista_trenutni(){
    cout<<endl<<"JednostrukaLista trenutni"<<endl;
    JednostrukaLista<int> lista;
    try{
        lista.sljedeci();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int i = 1; i <= 10; i++){
        if (i % 2 == 0) lista.dodajIspred(4 * i * i - 3);
        else lista.dodajIza(5 * i - 1);
    }
    const JednostrukaLista<int> clista(lista);
    if (clista.trenutni() == 4) cout<<"-Konstantna lista OK"<<endl;
    lista.trenutni() = 44;
    if (lista.trenutni() == 44) cout<<"-Izmjena preko trenutnog OK"<<endl;
}

void jlista_prethodni(){
    cout<<endl<<"JednostrukaLista prethodni"<<endl;
    JednostrukaLista<double> lista;
    try{
        lista.sljedeci();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    lista.dodajIspred(22);
    cout<<"-Nema clanova prije ";
    if (!lista.prethodni())
        cout<<"OK"<<endl;
    else
        cout<<"NOK"<<endl;
    lista.dodajIspred(23);
    lista.dodajIspred(22.3225);
    lista.prethodni();
    if (lista.trenutni() == 22.3225) cout<<"-Prethodni OK"<<endl;
}

void jlista_sljedeci(){
    cout<<endl<<"JednostrukaLista sljedeci"<<endl;
    JednostrukaLista<double> lista;
    try{
        lista.sljedeci();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    lista.dodajIspred(22);
    lista.dodajIspred(22);
    cout<<"-Nema clanova poslje ";
    if (!lista.sljedeci())
        cout<<"OK"<<endl;
    else
        cout<<"NOK"<<endl;
    lista.dodajIza(23);
    lista.dodajIza(22.3225);
    lista.sljedeci();
    if (lista.trenutni() == 22.3225) cout<<"-Sljedeci OK"<<endl;
}

void jlista_obrisi(){
    cout<<endl<<"JednostrukaLista obrisi"<<endl;
    JednostrukaLista<double> lista;
    try{
        lista.obrisi();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int i = 0; i < 10000; i++) 
        lista.dodajIza(22.22*i);
    for (int i = 0; i < 20; i++)
        lista.obrisi();
    // Provjerava da li je broj elemenata i trenutni elemenat nakon brisanja dobar
    if (lista.brojElemenata() == 10000 - 20 && lista.trenutni() == (22.22 * 9980))
        cout<<"OK"<<endl;
}

void jlista_operator1(){
    cout<<endl<<"JednostrukaLista test operatora"<<endl;
    JednostrukaLista<double> lista;
    for (int i = 0; i < 10000; i++) 
        lista.dodajIza(22.22*i);
    for (int i = 0; i < 20; i++)
        lista.obrisi();
    lista[0] = 22121;
    if (lista[0] == 22121) 
        cout<<"OK"<<endl;
    const JednostrukaLista<double> l(lista);
    if (l[0] == lista[0])
        cout<<"OK"<<endl;
}

void jlista_kraj(){
    cout<<endl<<"JednostrukaLista kraj"<<endl;
    JednostrukaLista<double> lista;
    try{
        lista.kraj();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int i = 0; i < 123122; i++){
        if (i % 3 == 0) 
            lista.dodajIspred(i * i - 12);
        else 
            lista.dodajIza(i);
    }
    lista.kraj();
    if (lista.trenutni() == 1)
        cout<<"OK"<<endl;
    // Sada cemo puno puta ici na pocetak pa na kraj da vidimo da li je dovojlno brzo
    for (int i = 0; i < 12121111; i++){
        lista.pocetak();
        lista.kraj();
    }
    if (lista.trenutni() == 1)
        cout<<"OK"<<endl;
}

void jlista_brElemenata() {
    cout<<endl<<"JednostrukaLista brojElemenata"<<endl;
    JednostrukaLista<int> lista;
    for (int i = 0; i < 21; i++) 
        lista.dodajIspred(i);
    lista.obrisi();
    lista.obrisi();
    for (int i = 0; i < 10; i++)
        lista.dodajIza(11);
    const JednostrukaLista<int> l(lista);
    if (lista.brojElemenata() == 29)
        cout<<"OK"<<endl;
}


void nizlista_dodajIspred(){
    cout<<endl<<"NizLista dodajIspred"<<endl;
    NizLista<double> lista;
    lista.dodajIspred(12);
    // Prvi elemenat kad se doda postaje trenutni
    if (lista.trenutni() == 12) 
        cout<<"OK"<<endl;
    else
        cout<<"Nije OK"<<endl;
    for (int i = 0; i < 12; i++)
        lista.dodajIspred(i*i);
    if (lista.trenutni() == 12) 
        cout<<"OK"<<endl;
    else 
        cout<<"Nije OK"<<endl;
}

void nizlista_dodajIza(){
    cout<<endl<<"NizLista dodajIza"<<endl;
    NizLista<double> lista;
    lista.dodajIza(102.221);
    // Prvi elemenat kad se doda postaje trenutni
    if (lista.trenutni() == 102.221) 
        cout<<"OK"<<endl;
    else
        cout<<"Nije OK"<<endl;
    // Dodajemo puno elemenata da vidimo da li realokacija radi dobro
    for (int i = 0; i < 1212; i++){
        //cout<<"VAMO"<<endl;
        lista.dodajIza(i*i);
        //cout<<"TAMO"<<endl;
    }
    // Kada dodamo dalje elemente trenutni ostaje isti
    if (lista.trenutni() == 102.221) 
        cout<<"OK"<<endl;
    else 
        cout<<"Nije OK"<<endl;
}

void nizlista_trenutni(){
    cout<<endl<<"NizLista trenutni"<<endl;
    NizLista<int> l;
    try{
        l.trenutni();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    l.dodajIspred(0);
    l.dodajIza(1);
    // Prvi dodani elemeant postaje trenutni
    if (l.trenutni() == 0)
        cout<<"OK"<<endl;
    else
        cout<<"Nije OK"<<endl;
    for (int i = 0; i < 12221; i++){
        if (i % 2 == 0) 
            l.dodajIspred(2*i);
        else 
            l.dodajIza(3*i);
    }
    // Sada kad dodamo puno elemenata trenutni treba ostati isti
    if (l.trenutni() == 0)
        cout<<"OK"<<endl;
    else 
        cout<<"Nije OK"<<endl;
    l.kraj();
    auto pom = l.trenutni();
    l.pocetak();
    while (l.sljedeci());
    if (pom == l.trenutni())
        cout<<"OK"<<endl;
    else 
        cout<<"Nije OK"<<endl;
}

void nizlista_prethodni(){
    cout<<"NizLista prethodni"<<endl;
    NizLista<int> nlista;
    try{
        nlista.prethodni();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int i = 0; i < 20; i++)
        nlista.dodajIza(20);
    //Ako smo na prvom elementu i probamo ici na prethodni
    if (!nlista.prethodni())
        cout<<"OK"<<endl;
    nlista.kraj();
    if (nlista.prethodni() and nlista.trenutni() == 20)
        cout<<"OK"<<endl;
}

void nizlista_sljedeci(){
    cout<<"NizLista sljedeci"<<endl;
    NizLista<int> nlista;
    try{
        nlista.sljedeci();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int i = 0; i < 120; i++)
        nlista.dodajIspred(10);
    if (!nlista.sljedeci())
        cout<<"OK"<<endl;
    else 
        cout<<"Nije"<<endl;
    nlista.pocetak();
    if (nlista.sljedeci() and nlista.trenutni() == 10)
        cout<<"OK"<<endl;
    else 
        cout<<"Nije"<<endl;
}

void nizlista_obrisi(){
    cout<<"NizLista obrisi"<<endl;
    NizLista<int> nlista;
    try{
        nlista.obrisi();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    try {
        nlista.obrisi();
        cout<<"Nije"<<endl;
    }catch(...){
        cout<<"OK"<<endl;
    }
    for (int i = 0; i < 10; i++)
        nlista.dodajIspred(10);
    for (int i = 0; i < 10; i++)
        nlista.dodajIza(i + 1);
    int t_velicina = nlista.brojElemenata();
    for (int i = 0; i < 3; i++)
        nlista.obrisi();
    if (t_velicina == 20 and t_velicina == nlista.brojElemenata() + 3)
        cout<<"OK"<<endl;
    else 
        cout<<"Nije"<<endl;
}

void nizlista_operator1(){
    cout<<"NizLista operatori"<<endl;
    NizLista<double> nlista;
    for (int i = 0; i < 12313; i++){
        if (i % 2 == 0)
            nlista.dodajIspred(i*i - 2*i);
        else 
            nlista.dodajIza(i*i - 4*i + 2);
    }
    if (nlista.trenutni() == 0 and (nlista.trenutni() = 2) and nlista.trenutni() == 2)
        cout<<"Dodjela uredu"<<endl;
    else 
        cout<<"Dodjela nije uredu"<<endl;
    nlista[0] = 21; nlista[1] = 13;
    if (nlista[0] == 21 and nlista[1] == 13)
        cout<<"Operator [] i dodjela"<<endl;
    else 
        cout<<"Problem sa operatorom []"<<endl;
    const NizLista<double> konstantna(nlista);
    if (konstantna[21] == nlista[21])
        cout<<"Konstantna lista"<<endl;
    else 
        cout<<"Problem u operatoru"<<endl;
    nlista.obrisi();
}

void nizlista_kraj(){
    cout<<"NizLista kraj"<<endl;
    NizLista<int> nlista;
    try{
        nlista.kraj();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int j = 0; j < 21; j++)
        nlista.dodajIspred(j * j);
    //Trenutni elemenat treba biti onaj zadnji jer je dodavano stalno ispred
    if (nlista.trenutni() == (nlista.kraj(), nlista).trenutni())
        cout<<"OK"<<endl;
    nlista.pocetak();
    if (nlista.trenutni() == 1 and (nlista.kraj(), nlista).trenutni() == 0)
        cout<<"ok"<<endl;
    else 
        cout<<"Nije uredu"<<endl;
}

void nizlista_pocetak(){
    cout<<"NizLista pocetak"<<endl;
    NizLista<int> nlista;
    try{
        nlista.pocetak();
        std::cout<<"Nema izuzetka, a trebalo bi ga biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    for (int j = 0; j < 21; j++)
        nlista.dodajIza(j * j);
    //Trenutni elemenat treba biti onaj zadnji jer je dodavano stalno ispred
    if (nlista.trenutni() == (nlista.pocetak(), nlista).trenutni())
        cout<<"OK"<<endl;
    nlista.kraj();
    if (nlista.trenutni() == 1 and (nlista.pocetak(), nlista).trenutni() == 0)
        cout<<"ok"<<endl;
    else 
        cout<<"Nije uredu"<<endl;
}

void nizlista_brElemenata(){
    cout<<"NizLista brojElemenata"<<endl;
    NizLista<int> nlista;
    int ukupno_el = 40, obrisanih = 0;
    for (int i = 0; i < ukupno_el; i++)
        nlista.dodajIspred(i * i + 20 * i + 11);
    if (ukupno_el == nlista.brojElemenata())
        cout<<"Ima dovoljno elemenata"<<endl;
    else
        cout<<"Nema, problem sa Brelemenata"<<endl;
    obrisanih = 12;
    for (int i = 0; i < obrisanih; i++)
        nlista.obrisi();
    if (nlista.brojElemenata() + obrisanih == ukupno_el)
        cout<<"Radi i nakon brisanja elemenata"<<endl;
    else 
        cout<<"Ne radi nakon brisanja elemenata kako treba"<<endl;
}

int main(){   
    cout<<"U funkcijama koje testiraju metode nakon pozivanja samih metoda if-ovima sam provjeravao da li vracaju vrijednosti koje treba"<<endl;
    cout<<"umjesto ispisa samih vrijednosti, kako bi bilo lakše upratiti ako negdje ima da nešto nije uredu"<<endl;
    jlista_dodajIspred();
    jlista_dodajIza();
    jlista_trenutni();
    jlista_prethodni();
    jlista_sljedeci();
    jlista_obrisi();
    jlista_operator1();
    jlista_kraj();
    jlista_brElemenata();
    nizlista_dodajIspred();
    nizlista_dodajIza();
    nizlista_trenutni();
    nizlista_prethodni();
    nizlista_sljedeci();
    nizlista_obrisi();
    nizlista_operator1();
    nizlista_kraj();
    nizlista_pocetak();
    nizlista_brElemenata();
    return 0;
}