# IAL-DU2

V druhé domácí úloze na Vás čekají následující čtyři příklady:

- hashtable: tabulka s rozptýlenými položkami *(3b)*,
- btree/rec: binární vyhledávací strom a operace nad ním implementované rekurzivně *(2.5b)*
- btree/iter: binární vyhledávací strom a operace nad ním implementované iterativně *(2.5b)*
- btree/exa: jednoduché použití a vybalancování binárního vyhledávacího stromu *(2b)*

Vaší úlohou je v souborech

- hashtable/hashtable.c,
- btree/rec/btree.c,
- btree/iter/btree.c, a
- tree/exa/exa.c

doplnit těla funkcí. Tyto soubory, **se zachováním adresářové struktury**, odevzdejte 
prostřednictvím STUDISu jako `tar` achiv *(bez komprimace, kódování UTF-8)*. Do jiných 
souborů nezasahujte *(neodevzdávají se = nebude hodnoceno)* a neupravujte předpisy 
funkcí. Pomocné funkce by něměly být potřeba, pokud nejsou explicitně zmíněny.

Obsah archivu tedy bude vypadat následovně:

- xlogin00.tar
  - hashtable
    - hashtable.c
  - btree
    - rec
      - btree.c
    - iter
      - btree.c
    - exa
      - exa.c
     
Správnost implementace si můžete částečně ověřit pomocí přiložené testovací aplikace, 
kterou přeložíte pomocí připraveného `Makefile` *(`make test`, `make clean`)*. Poskytnuté 
testy nepokrývají vše co bude testováno a je doporučeno napsat si vlastní testy.

Vaše řešení musí být přeložitelné a spustitelné na serveru `eva.fit.vutbr.cz.` 
Nedodržení zadání *(zásahy do kostry, přejmenovaní souborů, nezachování adresářové* 
*struktury apod.)* **bude hodnoceno 0 body**. Stejně tak bude 0 body hodnoceno i řešení 
nepřeložitelné či řešení, které se po spuštění zacyklí — testy budou časově limitované. 
Bodovou ztrátou budou dále postiženy projekty bez vhodných komentářů.

Domácí úlohu vypracovávejte, prosím, samostatně a své řešení nikde volně nepublikujte! 
**V případě odhalení plagiátorství či nedovolené spolupráce nebude udělen zápočet a dále 
bude zváženo zahájení disciplinárního řízení.**
