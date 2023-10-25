/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree) {
        if (tree->key == key) {
            *value = tree->value;
            return true;
        }

        if (tree->key < key)
            tree = tree->right;
        else
            tree = tree->left;
    }
    return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t **node = &(*tree);
    while (*node) {
        if ((*node)->key == key) {
            (*node)->value = value;
            return;
        }

        if ((*node)->key < key)
            node = &(*node)->right;
        else
            node = &(*node)->left;
    }

    *node = malloc(sizeof(bst_node_t));
    if (!(*node))
        return;
    (*node)->key = key;
    (*node)->value = value;
    (*node)->right = NULL;
    (*node)->left = NULL;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t **node = &(*tree);
    for (; (*node)->right; node = &(*node)->right)
        ;
    target->key = (*node)->key;
    target->value = (*node)->value;
    free(*node);
    *node = NULL;
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t **node = &(*tree);
    while (*node) {
        if ((*node)->key == key) {
            if (!(*node)->right && !(*node)->left) {
                free(*node);
                *node = NULL;
            } else if ((*node)->right && (*node)->left) {
                bst_replace_by_rightmost(*node, &(*node)->left);
            } else if ((*node)->right) {
                bst_node_t *rem = *node;
                *node = (*node)->right;
                free(rem);
            } else {
                bst_node_t *rem = *node;
                *node = (*node)->left;
                free(rem);
            }
        }
        else if ((*node)->key < key)
            node = &(*node)->right;
        else
            node = &(*node)->left;
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if (!(*tree))
        return;

    stack_bst_t stack;
    stack_bst_init(&stack);

    stack_bst_push(&stack, *tree);
    while (!stack_bst_empty(&stack)) {
        *tree = stack_bst_pop(&stack);
        if ((*tree)->right)
            stack_bst_push(&stack, (*tree)->right);
        if ((*tree)->left)
            stack_bst_push(&stack, (*tree)->left);
        free(*tree);
    }

    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    // Iterates to the leftmost node
    for (; tree; tree = tree->left) {
        stack_bst_push(to_visit, tree);
        bst_add_node_to_items(tree, items);
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    bst_leftmost_preorder(tree, &to_visit, items);
    bst_node_t *node = NULL;
    while (!stack_bst_empty(&to_visit)) {
        node = stack_bst_pop(&to_visit);
        if (node->right)
            bst_leftmost_preorder(node->right, &to_visit, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    // Iterates to the leftmost node
    for (; tree; tree = tree->left) {
        stack_bst_push(to_visit, tree);
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);

    bst_leftmost_inorder(tree, &to_visit);
    bst_node_t *node = NULL;
    while (!stack_bst_empty(&to_visit)) {
        node = stack_bst_pop(&to_visit);
        bst_add_node_to_items(node, items);
        if (node->right)
            bst_leftmost_inorder(node->right, &to_visit);
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    // Iterates to the leftmost node
    for (; tree; tree = tree->left) {
        stack_bool_push(first_visit, true);
        stack_bst_push(to_visit, tree);
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);
    stack_bool_t first_visit;
    stack_bool_init(&first_visit);

    bst_leftmost_postorder(tree, &to_visit, &first_visit);
    bst_node_t *node = NULL;
    while (!stack_bst_empty(&to_visit)) {
        node = stack_bst_top(&to_visit);

        if (stack_bool_pop(&first_visit)) {
            stack_bool_push(&first_visit, false);
            bst_leftmost_postorder(node->right, &to_visit, &first_visit);
        } else {
            bst_add_node_to_items(node, items);
            stack_bst_pop(&to_visit);
        }
    }
}
