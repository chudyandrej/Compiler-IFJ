# Compiler

Vysoké učení technické v Brne, fakulta informačných technológii.

Intepred jazyka IFJ15.
Projekt pre predmety Formálne jazyky a prekladače a Algoritmy.

Andrej Chudý, Martin Kopec, Maroś Kopec, Václav Kondula, Honza Bajer

Kazdodenni prace s gitem:
1) vytvorit si pracovni vetev
```bash
$ git checkout -b parser
```
2) **work**
3) pridat vsechny zmeny do commitu
```bash
$ git add .
```
4) commit (pokud nemate radi vim)
```bash
$ git commit -m 'Zprava, co jste pridali'
```
5) pushnout zmeny na github na svoji vetev
```bash
$ git push origin parser:parser
# git push origin LOKALNI:VZDALENA
```

Stahnout si neci zmeny:
1) zkontrolovat, na ktere vetvi jsme
```bash
$ git branch
```
2) zmenit vetev (vsechny zmeny musi byt commitnute)
```bash
$ git checkout scanner
# git checkout LOKALNI
```
3) stahnout zmeny
```bash
$ git pull origin scanner
# git pull origin VZDALENA
```

Specialitky:
1) Chci se kouknout na jinou vetev ale nechci jeste commitnout
```bash
$ git stash
# ulozi vsechny zmeny na zasobnik
$ git checkout VETEV
# take a look
$ git checkout TVOJE_VETEV
$ git stash pop 
# vrati zmeny ze zasobniku
```
