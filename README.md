# 🧠 AlgoVision

AlgoVision je desktop aplikacija za **vizualizaciju grafovskih algoritama**, namenjena učenju, demonstraciji i analizi rada algoritama nad grafovima.
Aplikacija omogućava interaktivno praćenje izvršavanja algoritama korak po korak, uz jasan vizualni prikaz promena stanja čvorova i grana.
Cilj projekta je da apstraktne koncepte iz teorije grafova učini **intuitivnijim i pristupačnijim** kroz animacije,
kontrolu toka izvršavanja i prilagodljiv korisnički interfejs.

---

## ✨ Podržani algoritmi

AlgoVision trenutno podržava sledeće grafovske algoritme:

- 🔍 DFS 
- 🌊 BFS 
- ⭐ A* 
- 📏 Dijkstra
- 🔁 Bellman–Ford
- 🔄 Floyd–Warshall
- 🧩 Kahn (Topološko sortiranje)
- 🌲 Prim (Minimalno razapinjuće stablo)
- ⛓️ Tarjan za jake komponente povezanosti

---

## 🎮 Kontrola izvršavanja algoritama

Aplikacija omogućava potpunu kontrolu toka izvršavanja algoritma:

- ▶️ Play – automatsko izvršavanje algoritma
- ⏸️ Pause – pauziranje izvršavanja
- ⏭️ Step forward – prelazak na sledeći korak
- ⏮️ Step back – povratak na prethodni korak
- ⏹️ Reset - povratak na početak

Ovakav način rada omogućava detaljno razumevanje svakog algoritma i lak povratak na prethodna stanja.

---

## 🎹 Prečice

Da bi rad sa aplikacijom bio brži i efikasniji, podržane su sledeće prečice na tastaturi:

| Akcija           | Prečica        |
|------------------|----------------|
| Create graph     | Ctrl + N       |
| Open graph       | Ctrl + O       |
| Save graph       | Ctrl + S       |
| Save image       | Ctrl + I       |
| Change theme     | Ctrl + T       |
| Help             | Ctrl + H       |
| Undo             | Ctrl + Z       |
| Redo             | Ctrl + Y       |
| Pan              | Ctrl + P       |
| Zoom in          | Ctrl + +       |
| Zoom out         | Ctrl + -       |

---

## 🎨 Vizualne i interaktivne mogućnosti

Korisnik može da prilagodi izgled i ponašanje aplikacije:

- ✏️ Interaktivno dodavanje i brisanje čvorova i grana
- 🔘 Podešavanje veličine čvorova
- 🎨 Vizualno razlikovanje stanja čvorova i grana (posećen, aktivan, deo puta, relaksirana, itd.)
- 🌗 Promena izgleda i teme aplikacije
- 👁️ Jasna i pregledna vizualna reprezentacija algoritamskih stanja

---

## 📂 Rad sa fajlovima

Aplikacija podržava rad sa grafovima putem fajlova:

- 📥 Učitavanje grafa iz JSON fajla
- 📤 Čuvanje grafa u JSON format
- 🖼️ Čuvanje trenutnog stanja grafa kao slike

Ove funkcionalnosti omogućavaju lako deljenje grafova, ponovno korišćenje test primera i dokumentovanje rezultata.

---

## ⚙️ Build & Run

### 🔧 Preduslovi

Za izgradnju i pokretanje projekta potrebno je:

- 🧩 C++17 ili noviji
- 🏗️ Qt 6
- 🛠️ CMake (verzija 3.16 ili novija)
- 🐧 Linux ili 🖥️ Windows

---

### 🔹 Pokretanje projekta u Qt Creator-u

1. Otvoriti **Qt Creator**.  
2. Izabrati **File → Open File or Project**.  
3. Navigirati do direktorijuma gde je kloniran projekat i izabrati `CMakeLists.txt`.  
4. Qt Creator će automatski prepoznati CMake projekat i tražiti build folder (možete kreirati novi, npr. `build`).  
5. Konfigurisati build tip (Debug ili Release).  
6. Kliknuti **Configure Project** i sačekati da se projekat učita.  
7. Nakon toga možete koristiti **Run** dugme za pokretanje aplikacije iz Qt Creator-a.  

---

#### 🐧 Linux

```bash
# 1️⃣ Kloniranje repozitorijuma (SSH)
git clone git@gitlab.com:matf-bg-ac-rs/course-rs/projects-2025-2026/AlgoVision.git
cd /putanja/do/projekta

# 2️⃣ Kreiranje build direktorijuma
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 

# 3️⃣ Kompajliranje
cmake --build build

# 4️⃣ Pokretanje aplikacije
./build/AlgoVision

```

---

#### 🖥️ Windows (PowerShell)

```powershell
# 1️⃣ Kloniranje repozitorijuma (SSH)
git clone git@gitlab.com:matf-bg-ac-rs/course-rs/projects-2025-2026/AlgoVision.git
cd C:\putanja\do\projekta

# 2️⃣ Kreiranje build direktorijuma
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 3️⃣ Kompajliranje
cmake --build build

# 4️⃣ Pokretanje aplikacije
.\build\Release\AlgoVision.exe

```
---

## 🎥 Demo snimak

[!demo]()

---

# 👩‍💻 Autori:
 - <a href="https://gitlab.com/miona7">Miona Sretenović 133/2022</a>
 - <a href="https://gitlab.com/ostojic.nina99">Nina Ostojić 46/2022</a>
 - <a href="https://gitlab.com/sofijajan.official">Sofija Janevska 35/2022</a>
 - <a href="https://gitlab.com/ognjen_mark">Ognjen Marković 41/2022</a>
 - <a href="https://gitlab.com/Boce03">Bogdan Micić 32/2022</a>

 ---