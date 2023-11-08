/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu)
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 *
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/// @brief Checks if given char is alphanumeric
/// @param c char to be checked
/// @return true when alphanumeric, else false
bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/// @brief Convert upper case alphanumeric char to lower case
/// @param c character to be converted
/// @return converted char, original when not upper case alphanumeric
char to_lower(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

/// @brief Checks if char is digit
/// @param digit char to check if it's digit
/// @return true when digit, else false
bool is_digit(char digit) {
    return digit >= '0' && digit <= '9';
}

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v
 * uložen ve stromu.
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);
    int value;
    char current = input[0];
    for (int i = 0; current; current = input[++i]) {
        current = to_lower(input[i]);
        if (!is_alpha(current) && current != ' ')
            current = '_';
        value = 0;
        bst_search(*tree, current, &value);
        value += 1;
        bst_insert(tree, current, value);
    }
}

bool is_balanced(bst_node_t *tree, int *height) {
    if (!tree) {
        *height = 0;
        return true;
    }

    int left = 0;
    int right = 0;
    bool l = is_balanced(tree->left, &left);
    bool r = is_balanced(tree->right, &right);

    *height = left > right ? left + 1 : right + 1;
    return (left - right <= 1) && (right - left <= 1) && l && r;
}

void balance(bst_node_t **tree, bst_items_t *items, int start, int end) {
    if (start >= end)
        return;

    int center = (start + end) / 2;
    bst_insert(tree, items->nodes[center]->key, items->nodes[center]->value);
    balance(tree, items, start, center);
    balance(tree, items, center + 1, end);
}

/**
 * Vyvážení stromu.
 *
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 *
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    bst_items_t items = {
        .capacity = 0,
        .size = 0,
        .nodes = NULL,
    };
    bst_inorder(*tree, &items);

    bst_node_t *balanced;
    bst_init(&balanced);

    balance(&balanced, &items, 0, items.size);

    bst_dispose(tree);
    *tree = balanced;

    free(items.nodes);
}