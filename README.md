# Formatiranje koda i provera stila

Ovaj projekat koristi **clang-format** i **clang-tidy** za formatiranje i proveru stila koda.
**Važno:** Uputstvo je prilagođeno Linux-u i Windows-u.

---

## Linux
```bash
# Build projekat i generisanje `compile_commands.json`
cd /putanja/do/projekta     
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   

# Omogućiti globbing ako koristite bash
shopt -s globstar

# Formatiranje svih cpp i h fajlova
clang-format -i Source/**/*.cpp Headers/**/*.h Tests/**/*.cpp

# Analiza svih cpp fajlova sa clang-tidy
clang-tidy Source/**/*.cpp -p build
```
---

## Windows (Command Prompt ili PowerShell)
```cmd
# Build projekat i generisanje `compile_commands.json`
cd C:\putanja\do\projekta    
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON    

# Formatiranje svih cpp i h fajlova
clang-format -i Source\**\*.cpp Headers\**\*.h Tests\**\*.cpp

# Analiza svih cpp fajlova sa clang-tidy
clang-tidy Source\**\*.cpp -p build
```
---

## Napomena o Qt i compile_commands.json
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
