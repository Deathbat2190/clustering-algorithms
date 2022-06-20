\chapter{Technologie, narzędzia i algorytmy}
# Użyte technologie
W tej pracy do implementacji algorytmów zostały użyte następujące technologie:

* C++ - język programowania użyty do implementacji algorytmów.
* Pyhon - język programowania użyty do stworzenia API pozwalającego na wywoływanie odpowiednich funkcji zaimplementowanych w C++.
* Raylib - biblioteka napisana w języku C wykorzystujaca OpenGL umożliwiająca tworzenia wizualizacji \cite{raylib}.

\noindent Jako baza do porównania wydajności zaimplementowanych algorytmów posłużył pakiet języka Python scikit-learn

# Środowisko oraz narzędzia
W trakcie implementacji algorytmów wykorzystane zostały następujące narzędzia:

* MSVC - Kompilator C++ firmy Microsoft w wersji 19.31.31105. Algorytmy były kompilowane z flagą -O2 włączającą optymalizację kodu.
* Neovide - edytor tekstu.
* LibreOffice Calc - arkusz kalkulacyjny wykorzystany do wygenerowania wykresów.

\noindent Wydajność algorytmów była testowana na komputerze o następującej specyfikacji:

* Płyta główna - Gigabyte B550 Aorus Pro V2.
* Procesor - AMD Ryzen 7 5800X, 8 rdzeni z bazowym taktowaniem 3,80 GHz (4,70 GHz w trybie turbo).
* RAM - ADATA XPG Spectrix D41 DDR4, 2x8GB podkręcone do 3200 MHz (bazowe taktowanie 2666 MHz).
* GPU - NVIDIA GeForce RTX 3060 Ti.
* SSD - Samsung 970 EVO 500GB.

# Działanie wybranych algorytmów
W tej pracy zaimplementowane zostały trzy algorytmy: K-Means, grupowanie hierarchiczne oraz DBSCAN. Są to jedne z 
najpopularniejszych algorytmów grupowania. W tej sekcji opisane zostało ich działanie.

## Metryki dystansu
W algorytmach grupowania istotną rolę odgrywają metryki pozwalające obliczyć dystans między dwiema obserwacjami. Poniżej w tabeli 
\ref{tabela_metryki} przedstawione zostały często wykorzystywane metryki dystansu \cite{metryki} gdzie $x$ i $y$ to $n$ wymiarowe wektory między 
którymi liczony jest dystans:

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, columns = {halign=c}}%, rows = {ht=1.5cm},  } 
        **Nazwa metryki** & **Wartość metryki** \\ 
        
        Odległość euklidesowa & $d(x, y) = \sqrt{(y_1 - x_1)^2 + ... + (y_n - x_n)^2}$ \\ 
        
        Odległość miejska & $d(x, y) = |y_1 - x_1| + ... |y_n - x_n|$ \\ 
        
        P-norma & $d(x, y) = (|y_1 - x_1|^p + ... |y_n - x_n|^p)^{1/p}$ \\
    \end{tblr}

    \caption{Wybrane metryki dystansu}
    \label{tabela_metryki}
    \end{center}
\end{table}

## K-Means
Parametry i pojęcia algorytmu K-Means:

* $k$ - ilość docelowych klastrów,
* $n$ - maksymalna ilość iteracji algorytmu (parametr opcjonalny),
* $d(x, y)$ - metryka do obliczenia dystansu,
* centroid - punkt powstający w wyniku uśrednienia wszystkich punktów w klastrze.

\noindent Poniżej przedstawione zostały kroki algorytmu K-Means:

*& Wybierz docelową liczbę klastrów $k$.
*& Z dostępnych punktów wylosuj $k$ punktów i ustaw je jako początkowe centroidy.
*& Dla każdego punktu oblicz dystansy do wszystkich centroidów za pomocą wybranej metryki $d(x, y)$ i przypisz ten punkt do 
   klastra, którego centroid znajduje się najbliżej.
*& Dla każdego klastra oblicz jego centroid.
*& Powtarzaj krok 3 i 4 do momentu aż klastry nie będą się zmieniać lub osiągnięta zostanie maksymalna liczba iteracji algorytmu $n$.

\noindent Na rysunku @kmeans przedstawiono 10000 punktów podzielonych na 5 klastrów za pomocą algorytmu K-Means oraz metryki dystansu 
euklidesowego.

@kmeans Dane pogrupowane algorytmem K-Means

\clearpage
## Grupowanie Hierarchiczne
Parametry i pojęcia algorytmu grupowania hierarchicznego:
* odległość między klastrami - odległość obliczona za pomocą jednego ze sposobów opisanych w tabeli \ref{linkage_table}. 

\noindent Poniżej przedstawione zostały kroki algorytmu grupowania hierarchicznego:

*& Do każdego punktu przypisz unikalny klaster.
*& Znajdź dwa klastry znajdujące się najbliżej siebie i złącz je w jeden.
*& Powtarzaj krok 2 do momentu aż wszystkie punkty będą znajdować się w jednym klastrze.

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[1,c] X[3,l]}, row{1} = {halign=c}}
        **Sposób wyliczania odległości** & **Opis** \\ 
        
        Kompletne & Odległość jest największą ze wszystkich odległości między parami punktów z klastrów $A$ i $B$ \\

        Pojedyńcze & Odległość jest najmniejszą ze wszystkich odległości między parami punktów z klastrów $A$ i $B$ \\
        
        Średnie & Odległość jest średnią wszystkich odległości między parami punktów z klastrów $A$ i $B$ \\
        
        Centroidalne & Odległość między centroidami klastrów $A$ i $B$ \\
    \end{tblr}

    \caption{Sposoby obliczania odległości między klastrami}
    \label{linkage_table}
    \end{center}
\end{table}

\noindent Na rysunku @hierarchical przedstawiono 10000 punktów podzielonych na 5 klastrów za pomocą algorytmu grupowania hierarchicznego 
oraz średnim sposobie wyliczania odległości. Dodatkowo na rysunku @dendrogram przedstawiono przykładowy dendrogram powstały w 
wyniku pogrupowania 50 punktów. Oś $y$ reprezentuje dystans między klastrami, natomiast na osi $x$ znajdują się indeksy poszczególnych 
punktów.

@hierarchical Dane pogrupowane algorytmem grupowania hierarchicznego
\clearpage
@dendrogram Dendrogram
\clearpage

## DBSCAN
Parametry i pojęcia algorytmu DBSCAN:

* $\varepsilon$ - promień obszaru wokół danego punktu - obszar ten definiuje sąsiedztwo punktu,
* $m$ - minimalna ilość punktów znajdująca się w sąsiedztwie danego punktu wymagana aby uznać punkt za rdzeń,
* $d(x, y)$ - metryka do obliczenia dystansu.

\noindent Poniżej przedstawione zostały kroki algorytmu DBSCAN:

*& Ustaw status wszystkich punktów na nie przetworzone.
*& Dla każdego punktu $a$:
    *& Jeżeli $a$ ma status inny niż nie przetworzony pomiń ten punkt.
    *& Znajdź sąsiadów $a$ w promieniu $\varepsilon$.
    *& Jeżeli ilość sąsiadów $a$ jest mniejsza niż $m$ ustaw status $a$ na szum i pomiń ten punkt.
    *& Jeżeli ilość sąsiadów $a$ jest większa lub równa $m$ zwiększ licznik klastrów o 1 i ustaw status $a$ na aktualną wartość licznika.
    *& Dla każdego punktu $b$ w sąsiedztwie $a$:
        *& Jeżeli $b$ ma status ustawiony na szum, zmień status $b$ na aktualną wartość licznika klastrów i pomiń ten punkt.
        *& Jeżeli $b$ ma status inny niż nie przetworzony pomiń ten punkt.
        *& Ustaw status $b$ na aktualną wartość licznika klastrów.
        *& Znajdź sąsiadów $b$ w promieniu $\varepsilon$.
        *& Jeżeli ilość sąsiadów $b$ jest większa lub równa $m$ dodaj sąsiadów $b$ do zbioru zawierającego sąsiadów $a$.

\noindent Znajdowanie sąsiadów można zaimplementować na wiele różnych sposobów. W tej pracy sąsiedzi punktu znajdowani są poprzez 
obliczenie dystansu do wszystkich innych punktów i sprawdzenie czy obliczony dystans jest mniejszy lub równy \varepsilon.

\noindent Na rysunku @dbscan przedstawiono 10000 punktów pogrupowanych algorytmem DBSCAN z parametrami: $\varepsilon = 0.5$, 
$m = 5$ oraz metryką dystansu euklidesowego. Kolorem białym oznaczone zostały wartości odstające (punkty ze statusem szum).

@dbscan Dane pogrupowane algorytmem DBSCAN
