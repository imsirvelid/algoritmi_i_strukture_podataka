#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
/*
U strukturi čvor dodan je atributo balans, te je dodan konstruktor bez parametara, bez ovoga se moglo i prolaze testovi samo sam
dodao kako bi se znalo da kada kreiram čvor da imam atribute podešene na odg vrijednosti
*/

template <typename TipKljuc, typename TipVrijednost>
struct Cvor {
	TipKljuc kljuc;
	TipVrijednost vrijednost;
	Cvor *lps, *dps, *r;
	int balans;
	public: Cvor(): kljuc(TipKljuc{}), vrijednost(TipVrijednost{}), lps(NULL), dps(NULL), r(NULL), balans(0) {}
};

template <typename TipKljuc, typename TipVrijednost>
class Mapa {
public:
	Mapa<TipKljuc, TipVrijednost>(){}
	virtual ~Mapa() {}
	virtual TipVrijednost &operator[](TipKljuc kljuc) = 0;
	virtual TipVrijednost operator[](TipKljuc kljuc) const = 0;
	virtual int brojElemenata() const = 0;
	virtual void obrisi() = 0;
	virtual void obrisi(TipKljuc kljuc) = 0;

};


template <typename TipKljuc, typename TipVrijednost>
class AVLStabloMapa : public Mapa<TipKljuc, TipVrijednost> {
	Cvor<TipKljuc, TipVrijednost> *korjen;
	int velicina;
	void Prekopiraj(Cvor<TipKljuc, TipVrijednost> *cvor);
	void Unisti();
	void balansiraj(Cvor<TipKljuc, TipVrijednost> *tekuci);
	void desna_rotacija(Cvor<TipKljuc, TipVrijednost> *tekuci);
	void lijeva_rotacija(Cvor<TipKljuc, TipVrijednost> *tekuci);
public:
	AVLStabloMapa<TipKljuc, TipVrijednost>() { korjen = NULL; velicina = 0; }
	AVLStabloMapa<TipKljuc, TipVrijednost>(const AVLStabloMapa<TipKljuc, TipVrijednost> &bsp) { korjen = NULL; velicina = 0; Prekopiraj(bsp.korjen); }
	~AVLStabloMapa<TipKljuc, TipVrijednost>(){ obrisi(); }
	TipVrijednost &operator[](TipKljuc kljuc);
	TipVrijednost operator[](TipKljuc kljuc) const;
	AVLStabloMapa<TipKljuc, TipVrijednost> &operator =(const AVLStabloMapa<TipKljuc, TipVrijednost> &m);
	int brojElemenata() const { return velicina; }
	void obrisi();
	void obrisi(TipKljuc kljuc);
	Cvor <TipKljuc, TipVrijednost> *pretrazi(Cvor<TipKljuc, TipVrijednost> *k, TipKljuc kluc) const;
};

template <typename TipKljuc, typename TipVrijednost>
void AVLStabloMapa<TipKljuc, TipVrijednost>::Prekopiraj(Cvor<TipKljuc, TipVrijednost> *cvor){
	if (cvor != NULL){
		(*this)[cvor->kljuc] = cvor->vrijednost;
		Prekopiraj(cvor->lps);
		Prekopiraj(cvor->dps);
	}
}

template <typename TipKljuc, typename TipVrijednost>
void AVLStabloMapa<TipKljuc, TipVrijednost>::desna_rotacija(Cvor<TipKljuc, TipVrijednost> *tekuci){
	auto roditelj = tekuci->r;
	roditelj->lps = tekuci->dps;
	tekuci->r = roditelj->r;
	roditelj->r = tekuci;
	if (tekuci->dps != NULL)
		tekuci->dps->r = roditelj;
	tekuci->dps = roditelj;
	if (tekuci->r != NULL and tekuci->r->dps == roditelj)
		tekuci->r->dps = tekuci;
	else if (tekuci->r != NULL) 
		tekuci->r->lps = tekuci;
	else 
		korjen = tekuci;
}

template <typename TipKljuc, typename TipVrijednost>
void AVLStabloMapa<TipKljuc, TipVrijednost>::lijeva_rotacija(Cvor<TipKljuc, TipVrijednost> *tekuci){
	auto roditelj = tekuci->r;
	roditelj->dps = tekuci->lps;
	tekuci->r = roditelj->r;
	roditelj->r = tekuci;
	if (tekuci->lps != NULL)
		tekuci->lps->r = roditelj;
	tekuci->lps = roditelj;
	if (tekuci->r != NULL and tekuci->r->dps == roditelj)
		tekuci->r->dps = tekuci;
	else if (tekuci->r != NULL) 
		tekuci->r->lps = tekuci;
	else 
		korjen = tekuci;
}

template <typename TipKljuc, typename TipVrijednost>
void AVLStabloMapa<TipKljuc, TipVrijednost>::balansiraj(Cvor<TipKljuc, TipVrijednost> *tekuci){
	if (tekuci->r == NULL)
		return;
	if (tekuci->r->lps == tekuci)
		tekuci->r->balans++;
	else if (tekuci->r->dps == tekuci)
		tekuci->r->balans--;
	if (tekuci->r->balans == 0)
		return;
	if (tekuci->r->balans < -1 and tekuci->balans < 0){
		auto tek = tekuci;
		lijeva_rotacija(tekuci);
		tek->balans = 0;
		if (tek->lps != NULL)
			tek->lps->balans = 0;
		return;
	} else if (tekuci->r->balans > 1 and tekuci->balans > 0){
		auto tek = tekuci;
		desna_rotacija(tekuci);
		tek->balans = 0;
		if (tek->dps != NULL)
			tek->dps->balans = 0;
		return;
	} else if (tekuci->r->balans > 1 and tekuci->balans < 0){
		int nv1[]{0, 0, -1};
		int nv2[]{1, 0, 0};
		auto drugo_dijete = tekuci->dps;
		auto prvo_dijete = tekuci;
		auto roditelj = tekuci->r;
		lijeva_rotacija(tekuci->dps);
		desna_rotacija(drugo_dijete);
		roditelj->balans = nv1[drugo_dijete->balans + 1];
		prvo_dijete->balans = nv2[drugo_dijete->balans + 1];
		drugo_dijete->balans = 0;
		return;
	} else if (tekuci->r->balans < -1 and tekuci->balans > 0){
		int nv1[]{1, 0, 0};
		int nv2[]{0, 0, -1};
		auto drugo_dijete = tekuci->lps;
		auto prvo_dijete = tekuci;
		auto roditelj = tekuci->r;
		desna_rotacija(tekuci->lps);
		lijeva_rotacija(drugo_dijete);
		roditelj->balans = nv1[drugo_dijete->balans + 1];
		prvo_dijete->balans = nv2[drugo_dijete->balans + 1];
		drugo_dijete->balans = 0;
		return;
	}
	balansiraj(tekuci->r);
}

template <typename TipKljuc, typename TipVrijednost>
TipVrijednost &AVLStabloMapa<TipKljuc, TipVrijednost>::operator [](TipKljuc kljuc){
	if (velicina == 0){
		korjen = new Cvor<TipKljuc, TipVrijednost>();
		korjen->kljuc = kljuc;
		korjen->lps = korjen->dps = NULL;
		velicina++;
		return korjen->vrijednost;
	}
	Cvor<TipKljuc, TipVrijednost> *pom = pretrazi(korjen, kljuc);
	if (pom->kljuc == kljuc)
		return pom->vrijednost;
	Cvor<TipKljuc, TipVrijednost> *novi = new Cvor<TipKljuc, TipVrijednost>();
	if (pom->kljuc > kljuc and pom->lps == NULL){
		novi->vrijednost = TipVrijednost{};
		novi->r = pom;
		pom->lps = novi;
		novi->lps = novi->dps = NULL,
		novi->kljuc = kljuc;
	} else if (pom->kljuc < kljuc and pom->dps == NULL){
		novi->vrijednost = TipVrijednost{};
		novi->r = pom;
		pom->dps = novi;
		novi->lps = novi->dps = NULL,
		novi->kljuc = kljuc;
	}
	if (pom->lps != NULL && pom->dps != NULL)
		pom->balans = 0;
	else if (novi->r != NULL)
		balansiraj(novi);
	velicina++;
	return novi->vrijednost;
}

template <typename TipKljuc, typename TipVrijednost>
TipVrijednost AVLStabloMapa<TipKljuc, TipVrijednost>::operator [](TipKljuc kljuc) const {
	Cvor<TipKljuc, TipVrijednost> *pom = pretrazi(korjen, kljuc);
	if (pom == NULL or pom->kljuc != kljuc)
		return TipVrijednost{};
	return pom->vrijednost;
}

template <typename TipKljuc, typename TipVrijednost>
Cvor <TipKljuc, TipVrijednost> *AVLStabloMapa<TipKljuc, TipVrijednost>::pretrazi(Cvor<TipKljuc, TipVrijednost> *k, TipKljuc kljuc) const {
	if (k == NULL)
		return k->r;
	if (k->kljuc == kljuc)
		return k;
	if (k->kljuc > kljuc){
		if (k->lps == NULL)
			return k;
		return pretrazi(k->lps, kljuc);
	}
	else{ 
		if (k->dps == NULL)
			return k;
		return pretrazi(k->dps, kljuc);
	}
}

template <typename TipKljuc, typename TipVrijednost>
void AVLStabloMapa<TipKljuc, TipVrijednost>::obrisi(TipKljuc kljuc){
	Cvor<TipKljuc, TipVrijednost> *brisanje = pretrazi(korjen, kljuc);
	if (brisanje->kljuc != kljuc)
		return;
	if (brisanje->dps == NULL and brisanje->lps == NULL){
		if (brisanje->r != NULL)
			if (brisanje->r->lps == brisanje)
				brisanje->r->lps = NULL;
			else 
				brisanje->r->dps = NULL;
		else 
			korjen = NULL;
		delete brisanje;
		brisanje = NULL;
		velicina--;
		if (velicina == 0)
			korjen = NULL;
		return;
	}
	Cvor<TipKljuc, TipVrijednost> *pom;
	if (brisanje->dps == NULL){
		pom = brisanje->lps;
		pom->r = brisanje->r;
		if (brisanje == korjen){
			korjen = pom;
		}
		else if (brisanje->r->lps == brisanje)
			brisanje->r->lps = pom;
		else 
			brisanje->r->dps = pom;
	}
	else if (brisanje->lps == NULL){
		pom = brisanje->dps;
		pom->r = brisanje->r;
		if (brisanje == korjen){
			korjen = pom;
		}
		else if (brisanje->r->lps == brisanje)
			brisanje->r->lps = pom;
		else 
			brisanje->r->dps = pom;
	}
	else {
		pom = brisanje->lps;
		while (pom->dps != NULL)
			pom = pom->dps;
		if (brisanje == korjen)
			korjen = pom;
		else if (brisanje->r->lps == brisanje)
			brisanje->r->lps = pom;
		else 
			brisanje->r->dps = pom;
		if (pom != brisanje->lps) {
			pom->r->dps = pom->lps;
			pom->lps = brisanje->lps;
			brisanje->lps->r = pom;
		}
		brisanje->dps->r = pom;
		pom->r = brisanje->r;
		pom->dps = brisanje->dps;
	}
	delete brisanje;
	brisanje = NULL;
	velicina--;
}

template <typename TipKljuc, typename TipVrijednost>
void AVLStabloMapa<TipKljuc, TipVrijednost>::obrisi(){
	if (korjen != NULL){
		Cvor<TipKljuc, TipVrijednost> *ds = korjen->dps;
		Cvor<TipKljuc, TipVrijednost> *ls = korjen->lps;
		delete korjen;
		korjen = ds;
		obrisi();
		korjen = ls;
		obrisi();
	}
	korjen = NULL;
	velicina = 0;
}

template <typename TipKljuc, typename TipVrijednost>
AVLStabloMapa<TipKljuc, TipVrijednost> &AVLStabloMapa<TipKljuc, TipVrijednost>::operator =(const AVLStabloMapa<TipKljuc, TipVrijednost> &m){
	if (this == &m)
		return *this;
	this->obrisi();
	korjen = NULL; velicina = 0;
	Prekopiraj(m.korjen);
	return *this;
}

template <typename TipKljuc, typename TipVrijednost>
class BinStabloMapa : public Mapa<TipKljuc, TipVrijednost> {
	Cvor<TipKljuc, TipVrijednost> *korjen;
	int velicina;
	void Prekopiraj(Cvor<TipKljuc, TipVrijednost> *cvor);
	void Unisti();
public:
	BinStabloMapa<TipKljuc, TipVrijednost>() { korjen = NULL; velicina = 0; }
	BinStabloMapa<TipKljuc, TipVrijednost>(const BinStabloMapa<TipKljuc, TipVrijednost> &bsp) { korjen = NULL; velicina = 0; Prekopiraj(bsp.korjen); }
	~BinStabloMapa<TipKljuc, TipVrijednost>(){ obrisi(); }
	TipVrijednost &operator[](TipKljuc kljuc);
	TipVrijednost operator[](TipKljuc kljuc) const;
	BinStabloMapa<TipKljuc, TipVrijednost> &operator =(const BinStabloMapa<TipKljuc, TipVrijednost> &m);
	int brojElemenata() const { return velicina; }
	void obrisi();
	void obrisi(TipKljuc kljuc);
	Cvor <TipKljuc, TipVrijednost> *pretrazi(Cvor<TipKljuc, TipVrijednost> *k, TipKljuc kluc) const;
	void inorder(Cvor<TipKljuc, TipVrijednost> *cvor){
		if (cvor != NULL){
			inorder(cvor->lps);
			std::cout<<cvor->kljuc<<std::endl;
			inorder(cvor->dps);
		}
	}
	Cvor <TipKljuc, TipVrijednost> *getKorjen(){ return korjen; }
};

template <typename TipKljuc, typename TipVrijednost>
void BinStabloMapa<TipKljuc, TipVrijednost>::Prekopiraj(Cvor<TipKljuc, TipVrijednost> *cvor){
	if (cvor != NULL){
		(*this)[cvor->kljuc] = cvor->vrijednost;
		Prekopiraj(cvor->lps);
		Prekopiraj(cvor->dps);
	}
}

template <typename TipKljuc, typename TipVrijednost>
TipVrijednost &BinStabloMapa<TipKljuc, TipVrijednost>::operator [](TipKljuc kljuc){
	if (velicina == 0){
		korjen = new Cvor<TipKljuc, TipVrijednost>;
		korjen->kljuc = kljuc;
		korjen->lps = korjen->dps = NULL;
		velicina++;
		return korjen->vrijednost;
	}
	Cvor<TipKljuc, TipVrijednost> *pom = pretrazi(korjen, kljuc);
	if (pom->kljuc == kljuc)
		return pom->vrijednost;
	Cvor<TipKljuc, TipVrijednost> *novi = new Cvor<TipKljuc, TipVrijednost>;
	if (pom->kljuc > kljuc and pom->lps == NULL){
		novi->vrijednost = TipVrijednost{};
		novi->r = pom;
		pom->lps = novi;
		novi->lps = novi->dps = NULL,
		novi->kljuc = kljuc;
	} else if (pom->kljuc < kljuc and pom->dps == NULL){
		novi->vrijednost = TipVrijednost{};
		novi->r = pom;
		pom->dps = novi;
		novi->lps = novi->dps = NULL,
		novi->kljuc = kljuc;
	}
	velicina++;
	return novi->vrijednost;
}

template <typename TipKljuc, typename TipVrijednost>
TipVrijednost BinStabloMapa<TipKljuc, TipVrijednost>::operator [](TipKljuc kljuc) const {
	Cvor<TipKljuc, TipVrijednost> *pom = pretrazi(korjen, kljuc);
	if (pom == NULL or pom->kljuc != kljuc)
		return TipVrijednost{};
	return pom->vrijednost;
}

template <typename TipKljuc, typename TipVrijednost>
Cvor <TipKljuc, TipVrijednost> *BinStabloMapa<TipKljuc, TipVrijednost>::pretrazi(Cvor<TipKljuc, TipVrijednost> *k, TipKljuc kljuc) const {
	if (k == NULL)
		return k->r;
	if (k->kljuc == kljuc)
		return k;
	if (k->kljuc > kljuc){
		if (k->lps == NULL)
			return k;
		return pretrazi(k->lps, kljuc);
	}
	else 
		if (k->dps == NULL)
			return k;
		return pretrazi(k->dps, kljuc);
}

template <typename TipKljuc, typename TipVrijednost>
void BinStabloMapa<TipKljuc, TipVrijednost>::obrisi(TipKljuc kljuc){
	Cvor<TipKljuc, TipVrijednost> *brisanje = pretrazi(korjen, kljuc);
	if (brisanje->kljuc != kljuc)
		return;
	if (brisanje->dps == NULL and brisanje->lps == NULL){
		if (brisanje->r != NULL)
			if (brisanje->r->lps == brisanje)
				brisanje->r->lps = NULL;
			else 
				brisanje->r->dps = NULL;
		else 
			korjen = NULL;
		delete brisanje;
		brisanje = NULL;
		velicina--;
		if (velicina == 0)
			korjen = NULL;
		return;
	}
	Cvor<TipKljuc, TipVrijednost> *pom;
	if (brisanje->dps == NULL){
		pom = brisanje->lps;
		pom->r = brisanje->r;
		if (brisanje == korjen){
			korjen->lps->r = pom;
			korjen = pom;
		}
		else if (brisanje->r->lps == brisanje)
			brisanje->r->lps = pom;
		else 
			brisanje->r->dps = pom;
	}
	else if (brisanje->lps == NULL){
		pom = brisanje->dps;
		pom->r = brisanje->r;
		if (brisanje == korjen){
			korjen->dps->r = pom;
			korjen = pom;
		}
		else if (brisanje->r->lps == brisanje)
			brisanje->r->lps = pom;
		else 
			brisanje->r->dps = pom;
	}
	else {
		pom = brisanje->lps;
		while (pom->dps != NULL)
			pom = pom->dps;
		if (brisanje == korjen)
			korjen = pom;
		else if (brisanje->r->lps == brisanje)
			brisanje->r->lps = pom;
		else 
			brisanje->r->dps = pom;
		if (pom != brisanje->lps) {
			pom->r->dps = pom->lps;
			pom->lps = brisanje->lps;
			brisanje->lps->r = pom;
		}
		brisanje->dps->r = pom;
		pom->r = brisanje->r;
		pom->dps = brisanje->dps;
	}
	delete brisanje;
	brisanje = NULL;
	velicina--;
}

template <typename TipKljuc, typename TipVrijednost>
void BinStabloMapa<TipKljuc, TipVrijednost>::obrisi(){
	if (korjen != NULL){
		Cvor<TipKljuc, TipVrijednost> *ds = korjen->dps;
		Cvor<TipKljuc, TipVrijednost> *ls = korjen->lps;
		delete korjen;
		korjen = ds;
		obrisi();
		korjen = ls;
		obrisi();
	}
	korjen = NULL;
	velicina = 0;
}

template <typename TipKljuc, typename TipVrijednost>
BinStabloMapa<TipKljuc, TipVrijednost> &BinStabloMapa<TipKljuc, TipVrijednost>::operator =(const BinStabloMapa<TipKljuc, TipVrijednost> &m){
	if (this == &m)
		return *this;
	this->obrisi();
	korjen = NULL; velicina = 0;
	Prekopiraj(m.korjen);
	return *this;
}

int main(){
    std::cout<<"******Poredjenje BinStabloMape i AVLStabloMape******"<<std::endl;
    BinStabloMapa<int, int> bsm;
    clock_t v1 = clock();
    for (int i = 0; i < 90000; i++){
        int dodani = i;
        bsm[dodani] = dodani;
    }
    clock_t v2 = clock();
    std::cout<<"Dodavanje (90000) elemenata u BinStabloMapa: "<<(double) (v2 - v1) / (CLOCKS_PER_SEC / 1000)<<"ms"<<std::endl;
    v1 = clock();
    bsm[1000000000] = 2289;
    v2 = clock();
    std::cout<<"Dodavanje novog elementa u BinStabloMapa: "<<(double) (v2 - v1) / (CLOCKS_PER_SEC / 1000)<<std::endl;
    AVLStabloMapa<int, int> avl;
    v1 = clock();
    for (int i = 0; i < 190000; i++){
        int dodani = i;
        avl[dodani] = dodani;
    }
    v2 = clock();
    std::cout<<"Dodavanje (190000) elemenata u AVLStabloMapa: "<<(double) (v2 - v1) / (CLOCKS_PER_SEC / 1000)<<"ms"<<std::endl;
    v1 = clock();
    avl[1000000000] = 2289;
    v2 = clock();
    std::cout<<"Dodavanje novog elementa u AVLStabloMapa: "<<(double) (v2 - v1) / (CLOCKS_PER_SEC / 1000)<<std::endl;
	return 0;
}

/*
Kod BinStabloMape dodavali smo malo elemenata (90000) ali budući da su u rastucem redosljedu mapa se svodi na listu, dok
kod AVL stabla smo dodavali 190000 elemenata i opet smo dobili znatno bolja vremena pristupa i dodavanja elemenata.
*/