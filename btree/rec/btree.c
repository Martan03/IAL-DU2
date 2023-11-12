/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // Checks if tree is NULL
    if (!tree)
        return false;

    // Searches in right subtree when key is greater then current node key
    if (tree->key < key)
        return bst_search(tree->right, key, value);
    // Searches in left subtree when key is less then current node key
    else if (tree->key > key)
        return bst_search(tree->left, key, value);

    // Value was found
    *value = tree->value;
    return true;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // Checks if tree is NULL
    if (!tree || !(*tree)) {
        // Creates new tree item with given key and value
        *tree = malloc(sizeof(bst_node_t));
        if (!(*tree))
            return;
        (*tree)->key = key;
        (*tree)->value = value;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
    }
    // When key equals key of the current item, sets the value to given value
    else if ((*tree)->key == key)
        (*tree)->value = value;
    // Inserts to right subtree when key is greater then current item key
    else if ((*tree)->key < key)
        bst_insert(&(*tree)->right, key, value);
    // Inserts to left subtree when key is less then current item key
    else
        bst_insert(&(*tree)->left, key, value);
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    // Checks if right subtree exists
    if ((*tree)->right) {
        // Recursively calls this function for the right subtree
        bst_replace_by_rightmost(target, &(*tree)->right);
        return;
    }
    // Replaces target item by the current (rightmost) item
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    // Frees current item
    free(*tree);
    *tree = NULL;
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    // Checks if tree is null
    if (!tree || !(*tree))
        return;

    // Checks if key of current item equals to given key
    if ((*tree)->key == key) {
        // Removes current item, which doesn't contain any subtree
        if (!(*tree)->right && !(*tree)->left) {
            free(*tree);
            *tree = NULL;
        }
        // Removes current item, which contains both subtrees
        else if ((*tree)->right && (*tree)->left)
            bst_replace_by_rightmost(*tree, &(*tree)->left);
        // Removes current item, which includes right subtree only
        else if ((*tree)->right) {
            bst_node_t *rem = *tree;
            *tree = (*tree)->right;
            free(rem);
        // Removes current item, which includes left subtree only
        } else {
            bst_node_t *rem = *tree;
            *tree = (*tree)->left;
            free(rem);
        }
    // Recursively calls for right subtree when key is greater then current key
    } else if ((*tree)->key < key) {
        bst_delete(&(*tree)->right, key);
    // Recursively calls for left subtree when key is less then current key
    } else {
        bst_delete(&(*tree)->left, key);
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    // Checks if tree is null
    if (!(*tree))
        return;

    // Recursivelly goes to left and right nodes
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    // Frees current node
    free(*tree);
    *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    // Checks if tree is null
    if (!tree)
        return;

    // Preorder gets node, left and then right
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    // Checks if tree is null
    if (!tree)
        return;

    // Inorder gets left, node and then right
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    // Checks if tree is null
    if (!tree)
        return;

    // Postorder gets left, right and then node
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
}
