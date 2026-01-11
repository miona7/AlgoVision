# 🧠 AlgoVision

AlgoVision je desktop aplikacija za **vizuelizaciju grafovskih algoritama**, namenjena učenju, demonstraciji i analizi rada algoritama nad grafovima.
Aplikacija omogućava interaktivno praćenje izvršavanja algoritama korak po korak, uz jasan vizuelni prikaz promena stanja čvorova i grana.
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
- 🌲 Prim 
- 🧩 Kahn (Topološko sortiranje)
- 🔗 Tarjan za jake komponente povezanosti

---

## 🎮 Kontrola izvršavanja algoritama

Aplikacija omogućava potpunu kontrolu toka izvršavanja algoritma:

- ▶️ Play – automatsko izvršavanje algoritma
- ⏸️ Pause – pauziranje izvršavanja
- ⏭️ Step forward – prelazak na sledeći korak
- ⏮️ Step back – povratak na prethodni korak
- 🔄 Undo / Redo – poništavanje i ponovno izvršavanje koraka

Ovakav način rada omogućava detaljno razumevanje svakog algoritma i lak povratak na prethodna stanja.

---

## 🎨 Vizuelne i interaktivne mogućnosti

Korisnik može da prilagodi izgled i ponašanje aplikacije:

- 🔘 Podešavanje veličine čvorova
- 🎨 Vizuelno razlikovanje stanja čvorova i grana (posećen, aktivan, deo puta, relaksiran, itd.)
- 🌗 Promena izgleda i teme aplikacije
- 👁️ Jasna i pregledna vizuelna reprezentacija algoritamskih stanja

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
- 💡 Opcionalno: clang-format i clang-tidy za proveru stila i automatsko formatiranje koda

---

#### 🐧 Linux

```bash
# 1️⃣ Kloniranje repozitorijuma (SSH)
git clone git@gitlab.com:matf-bg-ac-rs/course-rs/projects-2025-2026/AlgoVision.git
cd /putanja/do/projekta

# 2️⃣ Kreiranje build direktorijuma
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 3️⃣ Kompajliranje
cmake --build build

# 4️⃣ (Opcionalno) Formatiranje koda
# Omogućiti globbing u bash-u
shopt -s globstar
clang-format -i Source/**/*.cpp Headers/**/*.h Tests/**/*.cpp

# 5️⃣ (Opcionalno) Analiza sa clang-tidy
clang-tidy Source/**/*.cpp Tests/**/*.cpp -p build
# Automatska primena popravki
clang-tidy Source/**/*.cpp Tests/**/*.cpp -p build --fix
```

---

#### 🖥️ Windows (PowerShell)

```powershell
# 1️⃣ Kloniranje repozitorijuma (SSH)
git clone git@gitlab.com:matf-bg-ac-rs/course-rs/projects-2025-2026/AlgoVision.git
cd C:\putanja\do\projekta

# 2️⃣ Kreiranje build direktorijuma
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 3️⃣ Kompajliranje
cmake --build build

# 4️⃣ (Opcionalno) Formatiranje koda
clang-format -i Source\**\*.cpp Headers\**\*.h Tests\**\*.cpp

# 5️⃣ (Opcionalno) Analiza sa clang-tidy
clang-tidy Source\**\*.cpp Tests\**\*.cpp -p build
# Automatska primena popravki
clang-tidy Source\**\*.cpp Tests\**\*.cpp -p build --fix
```

---

### Napomena o Qt i compile_commands.json
- Kada build-ujete projekat **iz Qt Creator-a**, `compile_commands.json` se NE generiše automatski.
- Potrebno je ili:
  1. Pokrenuti build iz terminala sa CMake-om kao gore navedeno (`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`)
  2. Ili u Qt Creator-u omogućiti **CMake opciju za export compile commands** u postavkama projekta.

- clang-tidy koristi taj fajl da bi znao:
  - Include foldere (`-I`)
  - C++ standard (`-std=c++20`)
  - Sve flags koje projekat koristi

- Bez `compile_commands.json`, clang-tidy neće moći da analizira kod ispravno.

---

# 👩‍💻 Autori:
 - <a href="https://gitlab.com/miona7">Miona Sretenović 133/2022</a>
 - <a href="https://gitlab.com/ostojic.nina99">Nina Ostojić 46/2022</a>
 - <a href="https://gitlab.com/sofijajan.official">Sofija Janevska 35/2022</a>
 - <a href="https://gitlab.com/ognjen_mark">Ognjen Marković 41/2022</a>
 - <a href="https://gitlab.com/Boce03">Bogdan Micić 32/2022</a>

 ---