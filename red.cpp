#include <iostream>
#include <string>

using namespace std;

template <typename Tip>
class Red {
	int velicina;
	struct Cvor {
		Tip element;
		Cvor *sljedeci;
		Cvor (const Tip &element, Cvor *sljedeci): element(element), sljedeci(sljedeci){}
		Cvor (Cvor *sljedeci = nullptr): sljedeci(sljedeci) {}
	} *pocetak, *kraj;
	void Kopiraj(const Red<Tip> &r);
	void Dealociraj();
public:
	Red();
	Red(const Red<Tip> &r) { Kopiraj(r); }
	~Red();
	void brisi() { Dealociraj(); }
	void stavi(const Tip &el);
	Tip skini(); // Baci izuzetak ako je red prazan
	Tip &celo(); // Baci izuzetak ako je red prazan
	int brojElemenata() { return velicina; }
	Red &operator =(const Red<Tip> &r) { if (this == &r) return *this; Dealociraj(); Kopiraj(r); return *this; }
	void IspisiElemente() const;
};

template <typename Tip>
void Red<Tip>::Dealociraj(){
	Cvor *p = pocetak;
	while(pocetak != nullptr){
		pocetak = p->sljedeci;
		delete p;
		p = pocetak;
	}
	pocetak = kraj = nullptr;
	velicina = 0;
}

template <typename Tip>
void Red<Tip>::Kopiraj(const Red<Tip> &r){
	velicina = 0;
	pocetak = kraj = nullptr;
	Cvor *p = r.pocetak;
	while (p != nullptr){
		stavi(p->element);
		p = p->sljedeci;
	}
}


template <typename Tip>
Red<Tip>::Red(): velicina(0), pocetak(0), kraj(0){}

template <typename Tip>
Red<Tip>::~Red() {
	Dealociraj();
}

template <typename Tip>
Tip Red<Tip>::skini(){
	if (velicina == 0)
		throw "Red je prazan";
	Cvor *p = pocetak;
	pocetak = pocetak->sljedeci;
	Tip element = p->element;
	delete p;
	velicina--;
	return element;
}

template <typename Tip>
void Red<Tip>::stavi(const Tip &el){
	if (velicina == 0){
		Cvor *p = new Cvor(el, nullptr);
		pocetak = kraj = p;
	}else {
		Cvor *p = new Cvor(el, nullptr);
		kraj->sljedeci = p;
		kraj = p;
	}
	velicina++;
}

template <typename Tip>
Tip &Red<Tip>::celo(){
	if (velicina == 0)
		throw "Red je prazan";
	return pocetak->element;
}

template <typename Tip>
void Red<Tip>::IspisiElemente() const {
	// Ovo je samo pomocna funkcija koja služi da ispiše sve elemente od prvog ka zadnjem
	Cvor *p = pocetak;
	while (p != nullptr){
		std::cout<<p->element<<" ";
		p = p->sljedeci;
	}
	std::cout<<std::endl;
}


void test_stavi(){
	Red<std::string> red;
	std::string tekst[]{"Ovo", " ", "je ", "neki ", "random ", "tekst ", "da ", "isprobam ", " stavi ", " sa stringovima"};
	for (int i = 0; i < 10; i++)
		red.stavi(tekst[i]);
	red.IspisiElemente();
}

void test_skini(){
	Red<int> red;
	// Red je na pocetku prazan, treba baciti izuzetak
	try{
		red.skini();
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	red.stavi(1);
	std::cout<<red.skini()<<std::endl;
	std::cout<<"Sad bi red opet trebo biti prazan, pa opet treba izuzetak"<<std::endl;
	try{
		red.skini();
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < 12; i++)
		red.stavi(i*i);
	std::cout<<"Celo -> "<<red.skini()<<std::endl;
}

void test_celo(){
	Red<double> red;
	std::cout<<"Prazan red"<<std::endl;
	try{
		red.celo();
		std::cout<<"Nema izuzetka, a trebalo je biti"<<std::endl;
	}catch(...){
		std::cout<<"Izuzetak"<<std::endl;
	}
	for (int i = 0; i < 10; i++)
		red.stavi(3.1415968 * i);
	red.IspisiElemente();
	red.celo() = 3.1415968 * 2;
	red.IspisiElemente();
}

void test_konstr(){
	Red<std::string> red;
	std::string tekst[]{"Ovo", " ", "je ", "neki ", "random ", "tekst ", "da ", "isprobam ", " stavi ", " sa stringovima"};
	for (int i = 0; i < 10; i++)
		red.stavi(tekst[i]);
	red.IspisiElemente();
	Red<std::string> red2(red);
	red.skini(); red.skini();
	red2.IspisiElemente();
}

void test_operator(){
	Red<int> red;
	for (int i = 0; i < 5; i++)
		red.stavi(i*i);
	Red<int> red2;
	red2 = red;
	red.IspisiElemente();
	red.skini();
	red2.IspisiElemente();
}

void test_brisi(){
    Red<int> red;
    for (int i = 0; i < 15; i++)
        red.stavi(i);
    red.brisi();
    try{
        red.celo();
        std::cout<<"Izuzetak je trebao biti"<<std::endl;
    }catch(...){
        std::cout<<"Izuzetak"<<std::endl;
    }
    std::cout<<"Velicina nakon brisanja je "<<red.brojElemenata()<<std::endl;
}

void test_brojElemenata(){
    Red<int> red;
    for (int i = 0; i < 10; i++)
        red.stavi(i);
    std::cout<<"Nakon dodavanja 10 elemenata velicina "<<red.brojElemenata()<<std::endl;
    for (int i = 0; i < 4; i++)
        red.skini();
    red.celo();
    std::cout<<"Nakon skidanja 4 elementa velicina "<<red.brojElemenata()<<std::endl;
}

int main(){
	Red<int> red;
	//red.stavi(12);red.stavi(122);red.stavi(111);
	//red.IspisiElemente();
	test_stavi();
	test_skini();
	test_celo();
	test_konstr();
	test_operator();
    test_brisi();
    test_brojElemenata();
	return 0;
}