#include <iostream>
#include <chrono>



using namespace std;

struct Licznik {
	int ileX;
	int ileO;
};

int Najmniejszy(int a,int b){
    if(a<b){
        return a;
    }
    else{
        return b;
    }
}

int Najwiekszy(int a,int b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

class Plansza{
    int **Tablica;        //plansza gry
    Licznik *Sum_kol;		//tablica przechowująca ilosc wszystkich kółek i krzyżyków w kolumnach
    Licznik *Sum_rz;		//tablica przechowująca ilosc wszystkich kółek i krzyżyków w rzędach
    Licznik sum_sl;			// ilosc kółek i krzyżyków w przekątnej '/'
    Licznik sum_bck;		// ilosc kółek i krzyżyków w przekątnej '\'
    int rozm_pl;			//rozmiar planszy
    int poz_ruchy;			//ilosc pozostałych ruchów do wykoniania


    public:
    
		Plansza(int rozm){
        Tablica=new int*[rozm];
        Sum_kol=new Licznik[rozm];
        Sum_rz=new Licznik[rozm];
        
        rozm_pl=rozm;
        poz_ruchy=rozm*rozm;

        for(int i=0;i<rozm;i++){
            Tablica[i]=new int[rozm];
        }

        for(int i=0;i<rozm;i++){
			Sum_kol[i].ileO = 0;
			Sum_kol[i].ileX = 0;
            Sum_rz[i].ileO = 0;
			Sum_rz[i].ileX = 0;

            for(int j=0;j<rozm;j++){
                Tablica[i][j]=0;
            }
        }
        sum_sl.ileO = 0;
		sum_sl.ileX = 0;
        sum_bck.ileX = 0;
		sum_bck.ileO = 0;
    }
    ~Plansza(){
        delete [] Sum_kol;
        delete [] Sum_rz;

        for(int i=0;i<rozm_pl;i++){
            delete [] Tablica[i];
        }

        delete [] Tablica;
    }

	//zwraca liczbe wszystkich miejsc na planszy
    int Zwroc_li_msc(){
        return rozm_pl*rozm_pl;
    }
	
	//zwraca rozmiar planszy
	int Zwroc_rozm() {
		return rozm_pl;
	}
	
	//zwraca true jeśli pozycja jest pusta lub niedostępna
    bool Puste(int poz){
        int rz=poz/rozm_pl;
        int kol=poz%rozm_pl;
        
        if(poz>=0 && poz<rozm_pl*rozm_pl){
            if(Tablica[rz][kol]==0){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            cout << "wyjscie poza skale" <<endl;
            return false;
        }
    }
    
	// Wykonuje ruch znakiem O, numeracja od 0
    void Ruch_komp(int poz){
        
        int rz=poz/rozm_pl;
        int kol=poz%rozm_pl;
        
        if(poz>=0 && poz<rozm_pl*rozm_pl){
            const int kolko=1;

            Tablica[rz][kol]=kolko;
            Sum_rz[rz].ileO=Sum_rz[rz].ileO+1;
            Sum_kol[kol].ileO=Sum_kol[kol].ileO+1;
            if(rz==kol){
                sum_bck.ileO=sum_bck.ileO+1;
            }
            if(rz==rozm_pl-1-kol){
                sum_sl.ileO=sum_sl.ileO+1;
            }
            poz_ruchy--;

        }
        else{
            cout<<"wyjscie poza skale. Blad"<<endl;
        }
    }
    
	//Wykonuje ruch X, numeracja od 0
    void Ruch_gracza(int poz){
        
        int rz=poz/rozm_pl;
        int kol=poz%rozm_pl;

        
        if(poz>=0 && poz<rozm_pl*rozm_pl){
            const int krzyzyk=rozm_pl+1;
                
            Tablica[rz][kol]=krzyzyk;
            Sum_rz[rz].ileX=Sum_rz[rz].ileX+1;
            Sum_kol[kol].ileX=Sum_kol[kol].ileX+1;
            if(rz==kol){
                sum_bck.ileX=sum_bck.ileX+1;
            }
            if(rz==rozm_pl-1-kol){
                sum_sl.ileX=sum_sl.ileX+1;
            }
            poz_ruchy--;
        }
        else{
            cout<<"wyjscie poza skale. Blad"<<endl;
        }
    }

    //Cofa ruch który został wykonany na danej pozycji, 0-komputer || 1-gracz
    void Zeruj_msc(int poz,int gracz){
        int rz=poz/rozm_pl;
        int kol=poz%rozm_pl;
        
        if(poz>=0 && poz<rozm_pl*rozm_pl){
            if(gracz==0){

                Tablica[rz][kol]=0;
                Sum_rz[rz].ileO=Sum_rz[rz].ileO-1;
                Sum_kol[kol].ileO=Sum_kol[kol].ileO-1;
                if(rz==kol){
                    sum_bck.ileO=sum_bck.ileO-1;
                }
                if(rz==rozm_pl-1-kol){
                    sum_sl.ileO=sum_sl.ileO-1;
                }
                poz_ruchy++;
            }
            else{

                Tablica[rz][kol]=0;
                Sum_rz[rz].ileX=Sum_rz[rz].ileX-1;
                Sum_kol[kol].ileX=Sum_kol[kol].ileX-1;
                if(rz==kol){
                    sum_bck.ileX=sum_bck.ileX-1;
                }
                if(rz==rozm_pl-1-kol){
                    sum_sl.ileX=sum_sl.ileX-1;
                }
                poz_ruchy++;
            }
        }
        else{
            cout<<"wyjscie poza skale. Blad"<<endl;
        }
    
    };

	
	/*Sprawdza czy gra została zakończona. Zwraca: nic-1 | przegrana gracza--10 | remis-0 | wygrana-10
	Udoskonalona wersja*/
	int Czy_koniec2(int ile) {
		int WRzedzie = 0;

		if (sum_sl.ileO >= ile) { 
			if (ile == rozm_pl) {
				return -10;
			}
			else {
				for (int i = 0; i < rozm_pl; i++) {
					if (Tablica[i][rozm_pl - 1 - i] == 1) {
						WRzedzie++;
						if (WRzedzie == ile) {
							return -10;
						}
					}
					else {
						WRzedzie = 0;
					}
				}
			}
		}

		WRzedzie = 0;

		if (sum_bck.ileO >= ile) {
			if (ile == rozm_pl) {
				return -10;
			}
			else {
				for (int i = 0; i < rozm_pl; i++) {
					if (Tablica[i][i] == 1) {
						WRzedzie++;
						if (WRzedzie == ile) {
							return -10;
						}
					}
					else {
						WRzedzie = 0;
					}
				}
			}
		}

		WRzedzie = 0;

		if (sum_sl.ileX >= ile) {
			if (ile == rozm_pl) {
				return 10;
			}
			else {
				for (int i = 0; i < rozm_pl; i++) {
					if (Tablica[i][rozm_pl - 1 - i] == rozm_pl + 1) {
						WRzedzie++;
						if (WRzedzie == ile) {
							return 10;
						}
					}
					else {
						WRzedzie = 0;
					}
				}
			}
		}

		WRzedzie = 0;

		if (sum_bck.ileX >= ile) {
			if (ile == rozm_pl) {
				return 10;
			}
			else {
				for (int i = 0; i < rozm_pl; i++) {
					if (Tablica[i][i] == rozm_pl + 1) {
						WRzedzie++;
						if (WRzedzie == ile) {
							return 10;
						}
					}
					else {
						WRzedzie = 0;
					}
				}
			}
		}

		WRzedzie = 0;

		for (int i = 0; i < rozm_pl; i++) {
			if (Sum_kol[i].ileX >= ile) {
				if (ile == rozm_pl) {
					return 10;
				}
				else {
					for (int k = 0; k < rozm_pl; k++) {
						if (Tablica[k][i] == rozm_pl + 1) {
							WRzedzie++;
							if (WRzedzie == ile) {
								return 10;
							}
						}
						else {
							WRzedzie = 0;
						}
					}
				}
				
			}

			WRzedzie = 0;

			if (Sum_rz[i].ileX >= ile) {
				if (ile == rozm_pl) {
					return 10;
				}
				else {
					for (int k = 0; k < rozm_pl; k++) {
						if (Tablica[i][k] == rozm_pl + 1) {
							WRzedzie++;
							if (WRzedzie == ile) {
								return 10;
							}
						}
						else {
							WRzedzie = 0;
						}
					}
				}
			}

			WRzedzie = 0;

			if (Sum_kol[i].ileO >= ile) {
				if (ile == rozm_pl) {
					return -10;
				}
				else {
					for (int k = 0; k < rozm_pl; k++) {
						if (Tablica[k][i] == 1) {
							WRzedzie++;
							if (WRzedzie == ile) {
								return -10;
							}
						}
						else {
							WRzedzie = 0;
						}
					}
				}
			}
			
			WRzedzie = 0;
			
			if (Sum_rz[i].ileO >= ile) {
				if (ile == rozm_pl) {
					return -10;
				}
				else {
					for (int k = 0; k < rozm_pl; k++) {
						if (Tablica[i][k] == 1) {
							WRzedzie++;
							if (WRzedzie == ile) {
								return -10;
							}
						}
						else {
							WRzedzie = 0;
						}
					}
				}
			}
			WRzedzie = 0;
		}

		if (poz_ruchy == 0) { return 0; }

		return 1;
	}

	//Wypisuje plansze na konsoli
    void Wyswietl_plansze(){
        cout <<"*******************************************"<<endl;
        for(int i=0;i<rozm_pl;i++){
			cout << "    ";
			for (int j = 0; j < rozm_pl; j++) {
				cout << "------";
			}
			cout << endl;
			cout << "	| ";
			for (int j = 0; j < rozm_pl; j++){
				if (Tablica[i][j] == 1) {
					cout << 'O' << " | ";
				}
				else if (Tablica[i][j] == rozm_pl+1) {
					cout << 'X' << " | ";
				}
				else if (Tablica[i][j] == 0) {
					cout << " " << " | ";
				}

            }
            cout<<endl;
        }
		cout << "    ";
		for (int j = 0; j < rozm_pl; j++) {
			cout << "------";
		}
		cout << endl;
        cout <<"*******************************************"<<endl;
        cout<<endl;
    }
};


/*Algorytm wyznaczający wartość ruchów, wersja z cięciami alfa-beta, Zwraca 10 jesli wygrywa gracz lub -10 jeśli komputer
Remis jest sygnalizowany jako zero, dojscie do końca głębokości jest zwracane jako remis*/
int MiniMax2(Plansza& pl, int glebokosc, bool czy_max, int a, int b, int ile) {
	
	int Wartosc = pl.Czy_koniec2(ile);

	if (Wartosc != 1) {
		return Wartosc;
	}

	if (glebokosc == 10) {
		return 0;
	}

	if (czy_max) {
		int Najlepszy = -100000;

		for (int i = 0; i < pl.Zwroc_li_msc(); i++) {
			if (pl.Puste(i)) {
				pl.Ruch_gracza(i);

				Najlepszy = Najwiekszy(Najlepszy, MiniMax2(pl, glebokosc + 1, !czy_max,a,b,ile));

				pl.Zeruj_msc(i, 1);

				a = Najwiekszy(a, Najlepszy);
				if (a >= b) {
					break;
				}
			}
		}

		return Najlepszy;
	}
	else {
		int Najlepszy = 100000;

		for (int i = 0; i < pl.Zwroc_li_msc(); i++) {
			if (pl.Puste(i)) {
				pl.Ruch_komp(i);

				Najlepszy = Najmniejszy(Najlepszy, MiniMax2(pl, glebokosc + 1, !czy_max,a,b,ile));

				pl.Zeruj_msc(i, 0);
				b = Najmniejszy(b, Najlepszy);
				if (a >= b) {
					break;
				}
			}
		}

		return Najlepszy;
	}
}

//Funkcja wyliczająca najbardziej optymalny ruch przy pomocy algorytmu minimaks, zwraca pozycje na planszy
int Oblicz_ruch(Plansza &pl, int ile){
    int Wartosc=100000;
    int Ruch=-1;

	//ruchy predefiniowane
	if (pl.Zwroc_rozm()%2 == 1 && pl.Puste((pl.Zwroc_li_msc()-1)/2) == true) {
		cout << "p" << endl;
		return (pl.Zwroc_li_msc() - 1) / 2;
	}

	for (int i = 0; i < pl.Zwroc_li_msc(); i++) {

		if (pl.Puste(i)) {
			pl.Ruch_komp(i);

			int War_ru = MiniMax2(pl, 0, true,-100000,100000,ile);

			pl.Zeruj_msc(i, 0);

			if (Wartosc > War_ru) {
				Ruch = i;
				Wartosc = War_ru;
			}
		}
	}

    return Ruch;
}

//Funckcja wyświetlająca zasady gry
void Pomoc() {
	cout << "**************************************************************************************************************" << endl;
	cout << "*                                          Pomoc i Zasady Gry                                                *" << endl;
	cout << "*                                                                                                            *" << endl;
	cout << "*  Gre zawsze zaczyna gracz, ktory stawia na planszy X, po nim ruch wykonuje komputer ze znakiem O           *" << endl;
	cout << "*  Gra sprawdza czy z ktorejs z kolumn, rzedow lub przekatnych planszy znajduje sie wymagana ilosc           *" << endl;
	cout << "*  znakow w rzedzie, jesli tak, gra sie konczy. Uwaga, wygrac poprzez ustawienie znakow na przekatnej        *" << endl;
	cout << "*  mozna tylko kiedy znaki znajduja sie na jednej z dwoch glownych przkatnych, zaczynajacych sie             *" << endl;
	cout << "*  w jednym z rogow planszy                                                                                  *" << endl;
	cout << "*                                       Przyklad dla planszy 3X3                                             *" << endl;
	cout << "*                                                                                                            *" << endl;
	cout << "*                                               |    | X                                                     *" << endl;
	cout << "*                                            ------------                                                    *" << endl;
	cout << "*                                               | X  |                                                       *" << endl;
	cout << "*                                            ------------                                                    *" << endl;
	cout << "*                                             X |    |                                                       *" << endl;
	cout << "*                                                                                                            *" << endl;
	cout << "*  Na poczatku gry, wybieramy wielkosc planszy, czyli wybieramy ile pol znajdowac sie bedzie na jednej       *" << endl;
	cout << "*  planszy (plansza jest kwadratem, najmniejsza mozliwa jest 3x3(wpisujemy 3) a najwieksza 7x7(wpisujemy 7)).*" << endl;
	cout << "*  Nastepnie nalezy wpisac ile znakow pod rzad bedzie oznaczac wygrana. W mojej implementacji najmniejsza    *" << endl;
	cout << "*  wartoscia jaka mozna wybrac jest 3, a najwieksza podana wczesniej wielkosc planszy.                       *" << endl;
	cout << "*  Zeby postawic swoj znak, miejsce na planszy musi byc puste                                                *" << endl;
	cout << "*  Zeby wybrac miejsce dla swojego znaku wpisujemy numer miejsca, pamietajac ze indeksujemy od 0             *" << endl;
	cout << "*                                       Przyklad dla planszy 3X3                                             *" << endl;
	cout << "*                                                                                                            *" << endl;
	cout << "*                                             0 | 1  | 2                                                     *" << endl;
	cout << "*                                            ------------                                                    *" << endl;
	cout << "*                                             3 | 4  | 5                                                     *" << endl;
	cout << "*                                            ------------                                                    *" << endl;
	cout << "*                                             6 | 7  | 8                                                     *" << endl;
	cout << "*                                                                                                            *" << endl;
	cout << "*  Zeby w tym wypadku wybrac miejsce na srodku planszy trzeba w takim razie wpisac cyfre 4                   *" << endl;
	cout << "*                                                                                                            *" << endl;
	cout << "*                                           Dobrej Zabawy!!                                                  *" << endl;
	cout << "**************************************************************************************************************" << endl;
	cout << endl;
	cout << endl;
}

int main(){
    
	int ile_ruch;
	int rozmiar_pl;
	int poz;
    int koniec;
	double pomiar = 0.0000000;
    
	Pomoc();
	
	cout << "Wpisz rozmiar planszy (wielkosc krawedzi)" << endl;										//kod odczytujący wielkosc planszy wraz z obslugą błędu
	do {
		do {
			if (!cin.good()) {
				cin.clear();
				cin.ignore(1000, '\n');
			}

			cin >> rozmiar_pl;

		} while (!cin.good());

		if (rozmiar_pl<=2 || rozmiar_pl>7) {
			cout << "Wpisz poprawny rozmiar planszy" << endl;
		}
	} while (rozmiar_pl <= 1 || rozmiar_pl > 7);
	
	cout << "Wpisz ile tych samych znakow w rzedzie bedzie potrzebnych do wygranej" << endl;				//kod odczytujący warunek wygranej wraz z obsługą błędu
	do {
		do {
			if (!cin.good()) {
				cin.clear();
				cin.ignore(1000, '\n');
			}

			cin >> ile_ruch;
		} while (!cin.good());

		if (ile_ruch<=2 || ile_ruch>rozmiar_pl) {
			cout << "Wpisz poprawny warunek wygranej" << endl;
		}
	} while (ile_ruch <= 2 || ile_ruch > rozmiar_pl);

	Plansza pl(rozmiar_pl);
    
	pl.Wyswietl_plansze();
    
    do{
        //gracz																		
        do{																						//gracz rozpoczyna. Sprawdzanie ruchu wraz z obsługą błędu
            do{
                if(!cin.good()){
                    cin.clear();
                    cin.ignore(1000,'\n');
                }
                
				cin>>poz;
            }
            while(!cin.good());

			if (!pl.Puste(poz)) {
				cout << "Pozycja zajeta" << endl;
			}
        }
        while(!pl.Puste(poz));
        

        pl.Ruch_gracza(poz);
        koniec = pl.Czy_koniec2(ile_ruch);

        pl.Wyswietl_plansze();
        if(koniec!=1){
            break;
        }

        //komputer

		cout << "Myslenie ";																			//wykonywanie ruchu przez komputer
		auto pocz = chrono::steady_clock::now();
        poz=Oblicz_ruch(pl,ile_ruch);
		auto kon = chrono::steady_clock::now();
		chrono::duration<double> czas = kon - pocz;
		pomiar = czas.count();
		cout << '(' << pomiar << 's' <<')'<< endl;

		
        
		pl.Ruch_komp(poz);
        koniec = pl.Czy_koniec2(ile_ruch);
        pl.Wyswietl_plansze();
		
		
    }
    while(koniec==1);

    if(koniec==-10){
        cout<<"przegrana"<<endl;
    }
    else if(koniec==0)
    {
        cout<<"remis"<<endl;
    }
    else if(koniec==10){
        cout<<"wygrana"<<endl;
    }
    else{
        cout<<"blad"<<endl;
    }
    
    return 0;
}