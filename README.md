🏰Castel Crush – Proiect Modern C++ & Qt

Echipa SiegeLines

Joc de strategie 2D pe benzi (lane defense) în mod Player vs. Computer, dezvoltat în Modern C++ (C++20) și Qt 6. 
Aplicația simulează un joc de tip Tower Defense / Castle Crush în consolă, bazat pe benzi de luptă, generare de resurse și confruntări între trupe și turnuri.

👥 Echipa de dezvoltare

 Serban Andreea Viorica – Ierarhia claselor de unități (`Soldat`, `Arcas`, `Mag`, 'Cercetas' 'Tanc'), gestionarea trupelor active, energie, selectie si lupta
 Comsea Andreea – Partea de board, tabla, deplasare, adversar si afisare

🎮 Descrierea Jocului & Mecanici

* **Benzi de luptă (Lanes):** Harta este divizată în 2 culoare principale (Top, Mid, Bottom).
* **Turnuri (Towers):** Fiecare jucător dispune de turnuri de apărare și un turn principal (King Tower). Când turnul principal este distrus, jocul se încheie.
* **Resurse (Elixir/Mană):** Se generează automat în timp și permite invocarea unităților pe benzi.
* **Unități (Troops):** 
  * Unități Melee (ex: *Knight* - puncte mari de viață, atac de aproape).
  * Unități Ranged (ex: *Archer* - puncte mici de viață, atac la distanță).
* **Ciclu de luptă:** Detectare coliziuni, avansare pe bandă și prioritizare a țintelor (unitate vs. unitate / unitate vs. turn).

🏗️ Structura Proiectului

* **`Game`**: Coordonează bucla jocului (`update()`, `render()`), stările și input-ul jucătorilor.
* **`Board`**: Reprezintă grila jocului și conține structura benzilor.
* **`Tower`**: Gestionează viața, raza și punctele de atac ale apărărilor.
* **`Unit`** *(clasă de bază abstractă)*: Definește atributele comune (HP, damage, range, viteza de deplasare).
  * `Knight` *(clasă derivată)*
  * `Archer` *(clasă derivată)*
* **`Lane`**: Enumerare (`enum class`) pentru gestionarea simplificată a benzilor.


⚔️Regulile Jocului

Puncte de energie: Jucătorul acumulează energie pasiv în timp (regenerare pe tick). 
Plasarea unităților consumă energie; butoanele devin active doar dacă energia curentă >= costul unității.
Plasare: Jucătorul alege tipul de unitate și banda pe care o urmeaza.
Unități în mod automat/randomizat conform unui interval de decizie și resurselor disponibile.
Mișcare și Luptă:Trupele avansează automat de-a lungul benzii.La 
întâlnirea unui inamic sau la atingerea castelului advers, unitatea se oprește și declanșează atacul periodic în funcție de raza de acțiune.
Când HP-ul unei unități ajunge la 0, aceasta este eliminată din simulare și de pe ecran.
Unități Disponibile (5 tipuri)

⚙️Arhitectură și Organizare

├── .gitignore
├── README.md
├── CMakeLists.txt (sau CastelCrush.sln)
├── Core/                              
│   ├── Enums.h                       
│   ├── UnitStats.h                   
│   ├── Unit.h / Unit.cpp            
│   ├── Castle.h / Castle.cpp         
│   ├── EnergyManager.h / .cpp        
│   ├── Board.h / Board.cpp           
│   └── Game.h / Game.cpp              
└── GUI/                               
    ├── main.cpp                       
    ├── GameWindow.h / .cpp            
    ├── BoardWidget.h / .cpp           
    ├── EnergyWidget.h / .cpp          
    └── ControlPanelWidget.h / .cpp    


Împărțirea Responsabilităților în Echipă

Proiectul este împărțit pe fluxuri funcționale complete (ambele componente includ atât C++ Core, cât și Qt):

👩‍💻Membra A/ Serban Viorica Andreea: Unități, Energie, Selecție și Luptă

Core C++: Ierarhia de clase Unit și derivatele specifice, UnitStats, logica de energie (EnergyManager), sistemul de atac și aplicarea daunelor.

Integrare: Validarea acțiunilor de spawn și scăderea resurselor la plasare.


👩‍💻Membra B/ Comsea Andreea: Tablă, Mișcare, AI Adversar și Randare

Core C++: Clasa Board, Castle, translația pozițiilor și deplasarea pe tick (Game::update), logica de decizie automată a computerului.

Integrare: Detectarea condiției de final (victorie/înfrângere) și mecanismul de restart


📅Workflow Git

Dezvoltarea se face prin 2 branch-uri dedicate fiecarui membru.

Modificările sunt integrate în branch-ul main exclusiv prin Pull Request-uri verificate reciproc.


🔨 Configurare si Compilare

Compilator: MSVC (Visual Studio 2022)

Extensie VS: Qt Visual Studio Tools

