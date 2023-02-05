#include <iostream>

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
    if (velicina == 0) throw "Nema elemenata";
    return *L[tekuci];
}

template <typename InfoTip>
InfoTip NizLista<InfoTip>::trenutni() const {
    if (velicina == 0) throw "Nema elemenata";
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
    return *L[i];
}

template <typename InfoTip>
InfoTip NizLista<InfoTip>::operator [](int i) const {
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
    InfoTip **Novo = new InfoTip*[2 * kapacitet]{};
    for (int i = 0; i < kapacitet; i++) 
        Novo[i] = L[i];
    delete[] L;
    L = Novo;
    kapacitet *= 2;
}

template <typename InfoTip>
bool NizLista<InfoTip>::prethodni(){
    if (tekuci == 0) return false;
    tekuci--;
    return true;
}

template <typename InfoTip>
bool NizLista<InfoTip>::sljedeci(){
    if (tekuci == velicina - 1) return false;
    tekuci++;
    return true;
}

template <typename InfoTip>
InfoTip NizLista<InfoTip>::obrisi(){
    if (velicina == 0) throw "Nema sta za obrisati";
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
    int t = 0;
    Cvor *trazeni = prvi;
    while (t++ < i+1){
        trazeni = trazeni->sljedeci;
    }
    return trazeni->element;
}

template <typename InfoTip>
InfoTip JednostrukaLista<InfoTip>::operator [](int i) const {
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
template <typename Tip>
class DvostrukaLista : public Lista<Tip> {
	struct Cvor {
		Tip element;
		Cvor *sljedeci, *prethodni;
		Cvor (const Tip &element, Cvor *prethodni, Cvor *sljedeci):
		element(element), prethodni(prethodni), sljedeci(sljedeci) {}
		Cvor (Cvor *prethodni = 0, Cvor *sljedeci = 0): prethodni(0), sljedeci(0) {}
	};
	Cvor *tekuci, *prvi, *zadnji;
	int lduzina, dduzina;
	void Dealociraj();
public:
	DvostrukaLista();
    DvostrukaLista(const DvostrukaLista<Tip> &lista);
    ~DvostrukaLista();
    int brojElemenata() const { return lduzina + dduzina; }
    Tip &trenutni();
    void dodajIspred(const Tip &el);
    void dodajIza(const Tip &el);
    Tip trenutni() const;
    bool prethodni();
    bool sljedeci();
    void pocetak();
    void kraj();
    Tip obrisi();
    Tip &operator [](int i);
    Tip operator [](int i) const ;
    DvostrukaLista<Tip> &operator =(const DvostrukaLista<Tip> &lista);
    void IspisiElemente() const ;
    friend class Iterator<Tip>;
};

template <typename Tip>
void DvostrukaLista<Tip>::Dealociraj(){
	Cvor *p = prvi;
	while (p->sljedeci != zadnji){
		p = p->sljedeci;
		delete p->prethodni;
	}
	delete zadnji;
	delete p;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(){
	lduzina = dduzina = 0;
	tekuci = prvi = new Cvor;
	zadnji = new Cvor;
	zadnji->prethodni = prvi;
	zadnji->sljedeci = nullptr;
	prvi->prethodni = nullptr;
	prvi->sljedeci = zadnji;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista<Tip> &l){
	lduzina = dduzina = 0;
	tekuci = prvi = new Cvor;
	zadnji = new Cvor;
	zadnji->prethodni = prvi;
	zadnji->sljedeci = nullptr;
	prvi->prethodni = nullptr;
	prvi->sljedeci = zadnji;
	Cvor *pl = l.prvi, *t = nullptr;
	while (pl->sljedeci != l.zadnji){
		if (pl == l.tekuci)
			t = tekuci;
		dodajIza(pl->sljedeci->element);
		pl = pl->sljedeci;
		if (brojElemenata() > 1)
			tekuci = tekuci->sljedeci;
	}
	tekuci = t->sljedeci;
	lduzina = l.lduzina; dduzina = l.dduzina;
}

template <typename Tip>
DvostrukaLista<Tip>::~DvostrukaLista(){
	Cvor *p = prvi;
	while (p->sljedeci != zadnji){
		p = p->sljedeci;
		delete p->prethodni;
	}
	delete zadnji;
	delete p;
}

template <typename Tip>
Tip &DvostrukaLista<Tip>::trenutni(){
	if (lduzina + dduzina == 0)
		throw "Lista je prazna";
	return tekuci->sljedeci->element;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::trenutni() const {
	if (lduzina + dduzina == 0)
		throw "Lista je prazna";
	return tekuci->sljedeci->element;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &el){
	if (lduzina + dduzina == 0){
		Cvor *p = new Cvor(el, prvi, zadnji);
		prvi->sljedeci = p;
		zadnji->prethodni = p;
		dduzina++;
	}
	else {
		Cvor *p = new Cvor(el, tekuci, tekuci->sljedeci);
		tekuci->sljedeci->prethodni = p;
		tekuci->sljedeci = p;
		tekuci = p;
		lduzina++;
	}
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip &el){
	//std::cout<<"Dodajemo element: "<<el<<std::endl;
	if (lduzina + dduzina == 0)
		dodajIspred(el);
	else {
		Cvor *p = new Cvor(el, tekuci->sljedeci, tekuci->sljedeci->sljedeci);
		tekuci->sljedeci->sljedeci->prethodni = p;
		tekuci->sljedeci->sljedeci = p;
		dduzina++;
	}
}

template <typename Tip>
bool DvostrukaLista<Tip>::prethodni(){
	if (lduzina + dduzina == 0)
		throw "Lista je prazna";
	if (lduzina == 0)
		return false;
	tekuci = tekuci->prethodni;
	lduzina--;
	dduzina++;
	return true;
}

template <typename Tip>
bool DvostrukaLista<Tip>::sljedeci(){
	if (lduzina + dduzina == 0)
		throw "Lista je prazna";
	if (dduzina == 1)
		return false;
	tekuci = tekuci->sljedeci;
	dduzina--;
	lduzina++;
	return true;
}

template <typename Tip>
void DvostrukaLista<Tip>::IspisiElemente() const {
	Cvor *p = prvi;
	while (p->sljedeci != zadnji){
		if (p == tekuci)
			std::cout<<"["<<p->sljedeci->element<<"] ";
		else 
			std::cout<<p->sljedeci->element<<" ";
		p = p->sljedeci;
	}
	std::cout<<std::endl;
}

template <typename Tip>
void DvostrukaLista<Tip>::pocetak(){
	if (lduzina + dduzina == 0)
		throw "Lista je prazna";
	dduzina += lduzina;
	lduzina = 0;
	tekuci = prvi;
}

template <typename Tip>
void DvostrukaLista<Tip>::kraj(){
	if (lduzina + dduzina == 0)
		throw "Lista je prazna";
	lduzina += dduzina - 1;
	dduzina = 1;
	tekuci = zadnji->prethodni->prethodni;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::obrisi(){
	if (lduzina + dduzina == 0)
		throw "Nema elemenata za obrisati";
	Cvor *p = tekuci->sljedeci;
	tekuci->sljedeci = p->sljedeci;
	p->sljedeci->prethodni = tekuci;
	Tip ret = p->element;
	delete p;
	dduzina--;
	if (lduzina + dduzina == 0)
		tekuci = prvi;
	if (dduzina == 0 and lduzina != 0){
		tekuci = tekuci->prethodni;
		dduzina++;
		lduzina--;
	}
	return ret;
}

template <typename Tip>
Tip &DvostrukaLista<Tip>::operator [](int i){
	if (i < 0 || i >= lduzina + dduzina)
		throw "Indeks izvan opsega";
	Cvor *p;
	if (lduzina - i > 0 and lduzina - i < i){
		//IDEMO OD TRENUTNE POZICIJE DO TE POZICIJE JER JE NAJBLIŽE
		//std::cout<<"Krecemo od tekuceg nazad"<<std::endl;
		p = tekuci;
		int dist = lduzina - i - 1;
		while(dist != 0){
			p = p->prethodni;
			dist--;
		}
	}else if (lduzina - i > i){
		//IDEMO OD 0 DO TRENUTNE POZICIJE
		//std::cout<<"Krecemo od 0"<<std::endl;
		p = prvi->sljedeci;
		while(i != 0){
			p = p->sljedeci;
			i--;
		}
	}else if (lduzina < i && i - lduzina < lduzina + dduzina - i){
		//IDEMO OD LDUZINE DO TOG ELEMENTA
		//std::cout<<"Krecemo od tekuceg naprijed"<<std::endl;
		p = tekuci->sljedeci;
		int dist = i - lduzina;
		while(dist != 0){
			p = p->sljedeci;
			dist--;
		}
	}else {
		p = zadnji;
		int dist = lduzina + dduzina - i;
		while(dist != 0){
			p = p->prethodni;
			dist--;
		}
	}
	return p->element;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator [](int i) const {
	if (i < 0 || i >= lduzina + dduzina)
		throw "Indeks izvan opsega";
	Cvor *p;
	if (lduzina - i > 0 and lduzina - i < i){
		p = tekuci;
		int dist = lduzina - i;
		while(dist != 0){
			p = p->prethodni;
			dist--;
		}
	}else if (lduzina - i > i){
		p = prvi->sljedeci;
		while(i != 0){
			p = p->sljedeci;
			i--;
		}
	}else if (lduzina < i && i - lduzina < lduzina + dduzina - i){
		p = tekuci->sljedeci;
		int dist = i - lduzina;
		while(dist != 0){
			p = p->sljedeci;
			dist--;
		}
	}else {
		p = zadnji;
		int dist = lduzina + dduzina - i;
		while(dist != 0){
			p = p->prethodni;
			dist--;
		}
	}
	return p->element;
}

template <typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator =(const DvostrukaLista<Tip> &l){
	if (this == &l)
		return *this;
	Dealociraj();
	lduzina = dduzina = 0;
	tekuci = prvi = new Cvor;
	zadnji = new Cvor;
	zadnji->prethodni = prvi;
	zadnji->sljedeci = nullptr;	
	prvi->prethodni = nullptr;
	prvi->sljedeci = zadnji;
	Cvor *pl = l.prvi, *t = nullptr;
	while (pl->sljedeci != l.zadnji){
		if (pl == l.tekuci)
			t = tekuci;
		dodajIza(pl->sljedeci->element);
		pl = pl->sljedeci;
		if (brojElemenata() > 1)
			tekuci = tekuci->sljedeci;
	}
	tekuci = t->sljedeci;
	lduzina = l.lduzina; dduzina = l.dduzina;
	return *this;
}

template <typename Tip>
class Iterator {
	const NizLista<Tip> *niz;
	const JednostrukaLista<Tip> *jlista;
	const DvostrukaLista<Tip> *dlista;
	typename JednostrukaLista<Tip>::Cvor *trenutniJlista, *pocetakJlista;
	typename DvostrukaLista<Tip>::Cvor *trenutniDlista, *pocetakDlista;
	int trenutniNiz, brojElemenata, dduzina, lduzina;
	public:
		Iterator(const NizLista<Tip> &niz);
		Iterator(const JednostrukaLista<Tip> &jlista);
		Iterator(const DvostrukaLista<Tip> &dlista);
		//Tip &trenutni();
		Tip trenutni() const;
		bool sljedeci();
		void pocetak();
};

template <typename Tip>
Iterator<Tip>::Iterator(const NizLista<Tip> &niz){
	jlista = nullptr; dlista = nullptr;
	trenutniNiz = niz.tekuci;
	brojElemenata = niz.velicina;
	this->niz = &niz;
}

template <typename Tip>
Iterator<Tip>::Iterator(const JednostrukaLista<Tip> &jlista){
	this->jlista = &jlista;
	trenutniJlista = jlista.tekuci;
	lduzina = jlista.lduzina; dduzina = jlista.dduzina;
	pocetakJlista = jlista.prvi;
	niz = nullptr; dlista = nullptr;
}

template <typename Tip>
Iterator<Tip>::Iterator(const DvostrukaLista<Tip> &dlista){
	this->dlista = &dlista;
	trenutniDlista = dlista.tekuci;
	dduzina = dlista.dduzina; lduzina = dlista.lduzina;
	pocetakDlista = dlista.prvi;
	niz = nullptr; jlista = nullptr;
}

template <typename Tip>
bool Iterator<Tip>::sljedeci(){
	if (niz == nullptr and (jlista != nullptr or dlista != nullptr)){
		if (dduzina == 1)
			return false;
		if (jlista != nullptr)
			trenutniJlista = trenutniJlista->sljedeci;		
		else 
			trenutniDlista = trenutniDlista->sljedeci;
		dduzina--;
		lduzina++;
	}else {
		if (trenutniNiz == brojElemenata - 1)
			return false;
		trenutniNiz++;
	}
	return true;
}

template <typename Tip>
Tip Iterator<Tip>::trenutni() const {
	if (niz == nullptr and (jlista != nullptr or dlista != nullptr)){
		if (jlista != nullptr)
			return trenutniJlista->sljedeci->element;
		else{ 
			return trenutniDlista->sljedeci->element;
		}
	}
	return *(niz->L[trenutniNiz]);
}

template <typename Tip>
void Iterator<Tip>::pocetak(){
	if (niz != nullptr){
		trenutniNiz = 0;
		return;
	}
	else if (jlista != nullptr)
		trenutniJlista = pocetakJlista;
	else 
		trenutniDlista = pocetakDlista;
	dduzina = lduzina + dduzina;
	lduzina = 0;
}

void test_operatorpristupa(){
	DvostrukaLista<int> lista;
	try{
		lista[0];
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < 100; i++)
		lista.dodajIspred(i);
	for (int i = 100; i < 150; i++)
		lista.dodajIza(i);
	std::cout<<"Probavanje pristupa van opsega"<<std::endl;
	try{
		lista[lista.brojElemenata() + 1];
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < lista.brojElemenata(); i++)
		std::cout<<lista[i]<<" ";
	std::cout<<std::endl;
}

void test_operatoradodjele(){
	DvostrukaLista<int> lista;
	for (int i = 0; i < 10; i++)
		lista.dodajIspred(i);
	for (int i = 10; i < 15; i++)
		lista.dodajIza(i);
	std::cout<<"U obe liste bi trebao biti isti trenutni (oznacen je sa [])"<<std::endl;
	lista.IspisiElemente();
	DvostrukaLista<int> l;
	l = lista;
	l.IspisiElemente();
	std::cout<<"Obe liste trebaju imati iste elemente"<<std::endl;
	for (int i = 0; i < lista.brojElemenata(); i++)
		std::cout<<lista[i]<<" ";
	std::cout<<std::endl;
	for (int i = 0; i < l.brojElemenata(); i++)
		std::cout<<l[i]<<" ";
	std::cout<<std::endl;
	std::cout<<"Obe liste trebaju imati isti broj elemenata"<<std::endl;
	std::cout<<lista.brojElemenata()<<" - "<<l.brojElemenata()<<std::endl;
}

void test_kopirajuci(){
	DvostrukaLista<int> lista;
	for (int i = 0; i < 10; i++)
		lista.dodajIspred(i);
	for (int i = 10; i < 15; i++)
		lista.dodajIza(i);
	const DvostrukaLista<int> l(lista);
	l.IspisiElemente();
	lista.IspisiElemente();
}


template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &lista){
	Iterator<Tip> *iterator;
	if (const DvostrukaLista<Tip> *dlista = dynamic_cast<const DvostrukaLista<Tip> *>(&lista))
		iterator = new Iterator<Tip>(*dlista);
	else if (const JednostrukaLista<Tip> *jlista = dynamic_cast<const JednostrukaLista<Tip> *>(&lista))
		iterator = new Iterator<Tip>(*jlista);
	else {
		const NizLista<Tip> *nlista = dynamic_cast<const NizLista<Tip>*>(&lista);
		iterator = new Iterator<Tip>(*nlista);
	}
	iterator->pocetak();
	Tip max = iterator->trenutni();
	while(iterator->sljedeci()){
		if (iterator->trenutni() > max)
			max = iterator->trenutni();
	}
	delete iterator;
	return max;
}

void test_dajMaksimum(){
	JednostrukaLista<int> jlista;
	DvostrukaLista<int> dlista;
	NizLista<int> nlista;
	for (int i = 0; i < 23; i++){
		jlista.dodajIspred(i*i);
		dlista.dodajIspred(i*i);
		nlista.dodajIspred(i*i);
	}
	for (int i = 0; i < 23; i++){
		jlista.dodajIza(i*i);
		dlista.dodajIza(i*i);
		nlista.dodajIza(i*i);
	}
	//Svaka lista treba imati iste elemente, te isti trenutni elemenat (koji je oznacen sa [])
	jlista.IspisiElemente();
	dlista.IspisiElemente();
	nlista.IspisiElemente();
	// Funkcija dajMaksimum mora za svaku listu vratiti isti elemenat jer su svi isti
	std::cout<<dajMaksimum(jlista)<<" - "<<dajMaksimum(dlista)<<" - "<<dajMaksimum(nlista)<<std::endl;
	// Nakon traženog maksimuma elementi i trenutni moraju ostati isti
	jlista.IspisiElemente();
	dlista.IspisiElemente();
	nlista.IspisiElemente();
}

int main(){
	test_dajMaksimum();
	return 0;
}