#include <iostream>


template <typename Tip>
class DvostraniRed {
	struct Cvor {
		Tip element;
		Cvor *sljedeci;
		Cvor *prethodni;
		Cvor (const Tip &element, Cvor *prethodni, Cvor *sljedeci): element(element), prethodni(prethodni), sljedeci(sljedeci){}
		Cvor (Cvor *prethodni = nullptr, Cvor *sljedeci = nullptr): prethodni(prethodni), sljedeci(sljedeci) {}
	};
	Cvor *pocetak, *kraj;
	int broj_elemenata;
public:
	DvostraniRed();
	DvostraniRed(const DvostraniRed<Tip> &dr);
	~DvostraniRed();
	DvostraniRed &operator =(const DvostraniRed<Tip> &dr);
	void brisi();
	int brojElemenata(){ return broj_elemenata; }
	void staviNaVrh(const Tip &el);
	Tip skiniSaVrha();
	void staviNaCelo(const Tip &el);
	Tip skiniSaCela();
	Tip &vrh();
	Tip &celo();
	Tip vrh() const;
	Tip celo() const;
};

template <typename Tip>
void DvostraniRed<Tip>::brisi(){
	Cvor *p = pocetak;
	if (broj_elemenata != 0){
		while (p != kraj){
			p = p->sljedeci;
			delete p->prethodni;
		}
		delete p;
	}
	pocetak = kraj = nullptr;
	broj_elemenata = 0;
}

template <typename Tip>
DvostraniRed<Tip>::DvostraniRed(){
	pocetak = kraj = nullptr;
	broj_elemenata = 0;
}

template <typename Tip>
DvostraniRed<Tip>::DvostraniRed(const DvostraniRed<Tip> &dr){
	Cvor *p = dr.pocetak;
	broj_elemenata = 0;
	pocetak = kraj = nullptr;
	while (p != dr.kraj){
		staviNaVrh(p->element);
		p = p->sljedeci;			
	}
	staviNaVrh(p->element);
}

template <typename Tip>
DvostraniRed<Tip> &DvostraniRed<Tip>::operator =(const DvostraniRed<Tip> &dr){
	if (this == &dr)
		return *this;
	brisi();
	broj_elemenata = 0;
	pocetak = kraj = nullptr;
	Cvor *p = dr.pocetak;
	pocetak = kraj = nullptr;
	while (p != dr.kraj){
		staviNaVrh(p->element);
		p = p->sljedeci;			
	}
	staviNaVrh(p->element);
	return *this;
}

template <typename Tip>
DvostraniRed<Tip>::~DvostraniRed(){
	brisi();
}

template <typename Tip>
void DvostraniRed<Tip>::staviNaVrh(const Tip &el){
	if (pocetak == nullptr and kraj == nullptr)
		pocetak = kraj = new Cvor(el, nullptr, nullptr);
	else {
		kraj->sljedeci = new Cvor(el, kraj, nullptr);
		kraj = kraj->sljedeci;
	}
	broj_elemenata++;
}

template <typename Tip>
void DvostraniRed<Tip>::staviNaCelo(const Tip &el){
	if (pocetak == nullptr and kraj == nullptr)
		pocetak = kraj = new Cvor(el, nullptr, nullptr);
	else {
		pocetak->prethodni = new Cvor(el, nullptr, pocetak);
		pocetak = pocetak->prethodni;
	}
	broj_elemenata++;
}

template <typename Tip>
Tip DvostraniRed<Tip>::skiniSaCela(){
	if (broj_elemenata == 0)
		throw "Nema elemenata u redu";
	Tip x = pocetak->element;
	Cvor *p = pocetak;
	pocetak = pocetak->sljedeci;
	if (pocetak != nullptr)
		pocetak->prethodni = nullptr;
	broj_elemenata--;
	if (broj_elemenata == 0)
		pocetak = kraj = nullptr;
	delete p;
	return x;
}

template <typename Tip>
Tip DvostraniRed<Tip>::skiniSaVrha(){
	if (broj_elemenata == 0)
		throw "Nema elemenata u redu";
	Tip x = kraj->element;
	Cvor *p = kraj;
	kraj = kraj->prethodni;
	if (kraj != nullptr)
		kraj->sljedeci = nullptr;
	broj_elemenata--;
	if (broj_elemenata == 0)
		pocetak = kraj = nullptr;
	delete p;
	return x;
}

template <typename Tip>
Tip &DvostraniRed<Tip>::vrh(){
	if (broj_elemenata == 0)
		throw "Nema elemenata u redu";
	return kraj->element;
}

template <typename Tip>
Tip &DvostraniRed<Tip>::celo(){
	if (broj_elemenata == 0)
		throw "Nema elemenata u redu";
	return pocetak->element;
}

template <typename Tip>
Tip DvostraniRed<Tip>::vrh() const {
	if (broj_elemenata == 0)
		throw "Nema elemenata u redu";
	return kraj->element;
}

template <typename Tip>
Tip DvostraniRed<Tip>::celo() const {
	if (broj_elemenata == 0)
		throw "Nema elemenata u redu";
	return pocetak->element;
}


void test_staviNaCelo(){	
	std::cout<<"------Test staviNaCelo------"<<std::endl;
	DvostraniRed<int> dr;
	std::cout<<"Prije dodavanja : "<<dr.brojElemenata()<<std::endl;
	for (int i = 0; i < 12; i++)
		dr.staviNaCelo(i);
	// Prvi dodani elemenat ce biti na kraju reda, jer stalno dodajemo na celo reda
	std::cout<<"Element na kraju "<<dr.skiniSaVrha()<<std::endl;
	// Element na celu je onaj zadnji dodani elemeant
	std::cout<<"Element na celu "<<dr.skiniSaCela()<<std::endl;
}

void test_staviNaVrh(){
	std::cout<<"------Test staviNaVrh------"<<std::endl;
	DvostraniRed<double> dr;
	for (double i = 0; i < 125; i+=0.1)
		dr.staviNaVrh(i);
	// Sada elemenat na celu je onaj prvi dodani, jer sad stalno dodajemo elemente samo na vrh ...
	std::cout<<"Element na celu je "<<dr.skiniSaCela()<<std::endl;
	std::cout<<"Element na vrhu je "<<dr.skiniSaVrha()<<std::endl;
	std::cout<<"Element na vrhu nakon jednog skidanja sa vrha je "<<dr.skiniSaVrha()<<std::endl;
	std::cout<<"Broj elemenata je "<<dr.brojElemenata()<<std::endl;
}

void test_skiniSaVrha(){
	std::cout<<"------Test skiniSaVrha------"<<std::endl;
	std::cout<<"\tPrazan red"<<std::endl;
	DvostraniRed<int> dr;
	try{
		dr.skiniSaVrha();
	}catch(...){
		std::cout<<"\t\tIzuzetak"<<std::endl;
	}
	for (int i = 0; i < 10; i++)
		dr.staviNaVrh(i);
	// Zadnji stavljen je 9
	std::cout<<"\t"<<dr.skiniSaVrha()<<std::endl;
	for (int i = 0; i < 12; i++)
		dr.staviNaCelo(i);
	// Skinuta je 9 pa bi trebala sad biti 8
	std::cout<<"\t"<<dr.skiniSaVrha()<<std::endl;
	std::cout<<"\tBroj elemenata nakon dva skidanja (i 22 stavljanja) sa vrha je "<<dr.brojElemenata()<<std::endl;
}

void test_skiniSaCela(){
	std::cout<<"------Test skiniSaCela------"<<std::endl;
	std::cout<<"Kad je red prazan"<<std::endl;
	DvostraniRed<double> dr;
	try {
		dr.skiniSaCela();
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < 5; i++)
		dr.staviNaVrh(i);
	// Prvi stavljen element na vrh mora biti na celu, jer stalno dodajemo na vrh
	std::cout<<dr.skiniSaCela()<<std::endl;
	for (int i = 0; i < 10; i++)
		dr.staviNaCelo(i);
	std::cout<<dr.skiniSaCela()<<std::endl;
	std::cout<<"Broj elemenata nakon dva skidanja (i 15 stavljanja) sa cela je "<<dr.brojElemenata()<<std::endl;
}

void test_vrh(){
	std::cout<<"------Test vrh------"<<std::endl;
	std::cout<<"Kada je prazno"<<std::endl;
	DvostraniRed<int> dr;
	try{
		dr.vrh();
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < 12; i++)
		dr.staviNaVrh(i);
	std::cout<<"Prije poziva vrh broj elemenata je "<<dr.brojElemenata()<<std::endl;
	std::cout<<dr.vrh()<<std::endl;
	std::cout<<"Nakon poziva vrh broj elemenata je "<<dr.brojElemenata()<<std::endl;
	dr.vrh() = 120;
	std::cout<<dr.vrh()<<std::endl;
	const DvostraniRed<int> constdr(dr);
	std::cout<<"Poziv vrh nad const DvostranimRedom "<<constdr.vrh()<<std::endl;
}

void test_celo(){
	std::cout<<"------Test celo------"<<std::endl;
	std::cout<<"Prazan dvostrani red"<<std::endl;
	DvostraniRed<double> dr;
	try {
		dr.celo();
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < 10; i++)
		dr.staviNaCelo(i);
	std::cout<<"Prije poziva celo broj elemenata je "<<dr.brojElemenata()<<std::endl;
	std::cout<<dr.celo()<<std::endl;
	std::cout<<"Nakon poziva celo broj elemenata je "<<dr.brojElemenata()<<std::endl;
	dr.celo() = 0;
	std::cout<<dr.celo()<<std::endl;
	const DvostraniRed<double> cdr = dr;
	std::cout<<"Poziv celo nad const DvostranimRedom "<<cdr.celo()<<std::endl;
}

void test_brisi(){
	std::cout<<"------Test brisi------"<<std::endl;
	DvostraniRed<int> dr;
	dr.brisi();
	std::cout<<dr.brojElemenata()<<std::endl;
	for (int i = 0; i < 12550; i++){
		if (i % 2 == 0)
			dr.staviNaVrh(i*i);
		else
			dr.staviNaCelo(i);
	}
	std::cout<<dr.brojElemenata()<<std::endl;
	dr.brisi();
	std::cout<<"Nakon brisanja"<<std::endl;
	std::cout<<dr.brojElemenata()<<std::endl;
}


void test_operatori(){
	// U istoj funkciji cu testirati i konstruktor kopije i operator dodjele
	std::cout<<"------Test operatore dodjele i konstruktora kopije------"<<std::endl;
	DvostraniRed<int> dr;
	for (int i = 0; i < 12; i++)
		dr.staviNaVrh(i*i);
	DvostraniRed<int> kopija, konstr(dr);
	kopija = konstr;
	konstr = konstr;
	std::cout<<dr.brojElemenata()<<" - "<<kopija.brojElemenata()<<" - "<<konstr.brojElemenata()<<std::endl;
	// Svi moraju imati isti elemenat na celu i na vrhu
	std::cout<<dr.skiniSaCela()<<" - "<<dr.skiniSaVrha()<<std::endl;
	std::cout<<kopija.skiniSaCela()<<" - "<<kopija.skiniSaVrha()<<std::endl;
	std::cout<<konstr.skiniSaCela()<<" - "<<konstr.skiniSaVrha()<<std::endl;
	// Izmjena elemenata u jednom nebi trebala da utice na druge
	dr.skiniSaVrha(); dr.skiniSaCela();
	kopija.vrh() = 11111;
	std::cout<<dr.skiniSaCela()<<" - "<<dr.skiniSaVrha()<<std::endl;
	std::cout<<kopija.skiniSaCela()<<" - "<<kopija.skiniSaVrha()<<std::endl;
	std::cout<<konstr.skiniSaCela()<<" - "<<konstr.skiniSaVrha()<<std::endl;
}

void test_brojElemenata() {
	std::cout<<"-----Test brojElemenata------"<<std::endl;
	DvostraniRed<int> dr;
	std::cout<<dr.brojElemenata()<<std::endl;
	for (int i = 0; i < 12; i++)
		dr.staviNaVrh(i);
	std::cout<<dr.brojElemenata()<<std::endl;
	dr.brisi();
	std::cout<<dr.brojElemenata()<<std::endl;
	// Ova je metoda svakako trivijalna, a i mnoštvo puta se pozvala iz ostalih funkcija koje su testirale prošle metode tako da
}

int main(){
	test_staviNaCelo();
	test_staviNaVrh();
	test_skiniSaVrha();
	test_skiniSaCela();
	test_vrh();
	test_celo();
	test_brisi();
	test_operatori();
	test_brojElemenata();
	return 0;
}