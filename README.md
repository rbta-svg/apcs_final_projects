# apcs_final_projects
ed/edX editor, final project for the 'Algorithms and Principles of Computer Science' course. 


edU, o ed con Undo multipli Progetto di algoritmi e strutture dati
Descrizione
Il progetto consiste nell’implementare un semplice editor di testi. L’editor con- sidera un documento come una sequenza di righe, di dimensione arbitraria, numerate a partire da uno.
L’interfaccia dell’editor `e costituita da comandi testuali, terminati da un carattere di “a capo”. I comandi possono essere seguiti da una porzione di testo, costituita da una o piu` righe, terminata da un carattere . (punto) che compare come unico carattere sulla riga successiva. I comandi sono costituiti da un’unica lettera, preceduta opzionalmente da uno o due numeri interi.
L’interfaccia dell’editor `e liberamente ispirata a quella del tradizionale editor ed.
In alcuni comandi, gli interi che compaiono in essi rappresentano degli speci- ficatori di indirizzo. Piu` precisamente, uno specificatore di indirizzo `e un numero n, espresso in decimale, che indica l’indirizzo della n-esima riga; la prima riga del testo ha indirizzo 1.
I comandi supportati sono i seguenti, con la convenzione che ind1,ind2 indicano due specificatori di indirizzo tali che ind1 ≤ ind2 e le parentesi tonde sono introdotte per semplicit`a di lettura di questo testo, ma non incluse nel comando:
• (ind1,ind2)c
Cambia il testo presente alle righe comprese tra ind1 ed ind2 (estremi inclusi). Il testo che segue il comando deve essere fatto di un numero di righe pari a ind2-ind1+1. ind1 deve essere o un indirizzo effettivamente presente nel testo, oppure il primo indirizzo dopo l’ultima riga presente nel testo (o 1 se il testo `e ancora vuoto).
• (ind1,ind2)d
Cancella le righe comprese tra ind1 ed ind2 (estremi inclusi), spostando verso l’alto le righe successive a quella di indirizzo ind2 (se ce ne sono). La cancellazione di una riga che non esiste nel testo non ha alcun effetto.
• (ind1,ind2)p
Stampa le righe tra ind1 ed ind2, inclusi. Laddove non sia presente una riga nel testo nella posizione da stampare, viene stampata una riga contenente il solo carattere ’.’ seguito da un “a capo”.
• (numero)u


Effettua l’annullamento (undo) di un numero di comandi (c o d) pari a quello specificato tra parentesi tonde (laddove numero `e un intero stret- tamente maggiore di zero). Una sequenza di comandi di undo consecutivi annulla un numero di passi pari alla somma dei passi specificati in ognuno di essi. Se il numero di comandi di cui effettuare l’annullamento `e superiore a quello dei comandi eseguiti, vengono annullati tutti i passi. L’esecuzio- ne di un comando di modifica del testo (c, d) dopo un undo cancella gli effetti dei comandi annullati definitivamente. Si noti che nel numero dei comandi da annullare si contano anche i comandi che non hanno effetto (per esempio la cancellazione di un blocco di righe che non esistono).
• (numero)r
Annulla l’effetto di undo per un numero di comandi pari a numero a partire dalla versione corrente (funzione di redo). Si noti che numero deve essere un intero strettamente maggiore di zero. Si ha quindi che una sequenza di comandi del tipo
10u 5r
`e a tutti gli effetti equivalente al solo comando 5u. Similmente, la sequenza
12u 3r 2u 7r
equivale al comando 4u. Nel caso in cui il numero di comandi di cui effet- tuare il redo `e superiore a quelli annullati correntemente, viene effettuato il numero massimo di redo possibili.
•q
Termina l’esecuzione dell’editor
Una riga di testo fornita in ingresso all’editor pu`o contenere al massimo 1024 caratteri.
Si assuma che all’editor vengano dati solo comandi corretti (non `e quindi necessario verificare la loro correttezza). Per esempio, non viene mai dato un comando c in cui `e ind1 > ind2, o ind1 < 1, o ind1 non `e n ́e l’indirizzo di una riga presente nel testo, n ́e l’indirizzo della prima riga dopo il testo. Similmente per gli altri comandi. Si faccia attenzione per`o al fatto che, in alcuni casi, il comando `e ammesso, ma semplicemente non ha effetto; per esempio, `e possibile dare un comando d in cui ind1 non `e l’indirizzo di una riga del testo, ma in questo caso il comando non ha alcun effetto sul testo.
Esempio di input
Esempio di sequenza di comandi:
2

1,2c
prima riga
seconda riga
.
2,3c
nuova seconda riga terza riga
.
1,3p
1,1c
nuova prima riga
.
1,2p
2,2d
4,5p
1,3p
4,5d
1,4p
3u
1,6p
1r
1,3p
q
La precedente sequenza di comandi d`a origine al seguente output da parte dell’editor:
prima riga
nuova seconda riga terza riga
nuova prima riga nuova seconda riga .
.
nuova prima riga terza riga
.
nuova prima riga terza riga
.
.
prima riga
nuova seconda riga terza riga
.
.
.
nuova prima riga nuova seconda riga terza riga
3
