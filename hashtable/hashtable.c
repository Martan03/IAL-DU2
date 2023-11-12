/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    for (int i = 0; i < MAX_HT_SIZE; ++i) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    // Gets hash from the hash function based on key
    int hash = get_hash(key);

    // Iterates linked list on hash index until it finds key
    for (ht_item_t *temp = (*table)[hash]; temp; temp = temp->next) {
        if (temp->key == key)
            return temp;
    }

    // Item wasn't found
    return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    // Searches for key in table, changes the value to new value if exists
    ht_item_t *temp = ht_search(table, key);
    if (temp) {
        temp->value = value;
        return;
    }

    // Creates new item
    int hash = get_hash(key);
    temp = malloc(sizeof(ht_item_t));
    temp->key = key;
    temp->value = value;
    // Sets next item to the item that was previously first in the linked list
    // on given index
    temp->next = (*table)[hash];

    // Adds created item to the linked list
    (*table)[hash] = temp;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    // Searches for item with key in table, returns its value if exists
    ht_item_t *found = ht_search(table, key);
    if (found)
        return &found->value;
    return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    // Gets hash of the key
    int hash = get_hash(key);

    // Iterates linked list on hash index until it finds item with key
    ht_item_t *prev = NULL;
    for (ht_item_t *temp = (*table)[hash]; temp; prev = temp, temp = temp->next) {
        // Continues iterating when current item doesn't have given key
        if (temp->key != key)
            continue;

        // When previous item exists, links next item to the previous
        // Skips current item, which is item to be deleted
        if (prev)
            prev->next = temp->next;
        // Frees item to be deleted
        free(temp);
        return;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    // Iterates all linked lists in the table
    for (int i = 0; i < MAX_HT_SIZE; ++i) {
        // Iterates all items in linked list and frees them
        for (ht_item_t *temp = (*table)[i]; temp;) {
            ht_item_t *next = temp->next;
            free(temp);
            temp = next;
        }
        // Sets current linked list to NULL
        (*table)[i] = NULL;
    }
}
